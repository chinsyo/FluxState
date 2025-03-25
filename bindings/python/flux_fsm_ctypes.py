import ctypes
import os
from typing import Optional, Dict, List, Any
from ctypes import CFUNCTYPE, POINTER, Structure, c_int, c_void_p, c_size_t

# 加载动态库
# 动态库扩展名适配不同平台
_lib_extension = '.so' if os.name == 'posix' else '.dll'
_lib_path = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..', 'build', 'lib', 'libflux_fsm_core.so'))
if not os.path.exists(_lib_path):
    raise FileNotFoundError(f"Shared library not found at {_lib_path}")
_lib = ctypes.CDLL(_lib_path)

# 定义C结构体
class flux_fsm_transition_t(Structure):
    _fields_ = [
        ("from_state", ctypes.c_int),
        ("event", ctypes.c_int),
        ("to_state", ctypes.c_int),
        ("guard", ctypes.CFUNCTYPE(ctypes.c_bool, ctypes.c_void_p)),
        ("action", ctypes.CFUNCTYPE(None, ctypes.c_void_p)),
    ]

class flux_fsm_t(Structure):
    _fields_ = [
        ('initial_state', c_int),
        ('current_state', c_int),
        ('context', c_void_p),
        ('transitions', POINTER(flux_fsm_transition_t)),
        ('transition_count', c_size_t),
        ('handlers', POINTER(CFUNCTYPE(None, c_void_p, c_int, c_int))),
        ('handler_count', c_size_t)
    ]

# 定义函数原型
_lib.flux_fsm_create.restype = POINTER(flux_fsm_t)
_lib.flux_fsm_create.argtypes = [c_int, c_void_p]

_lib.flux_fsm_destroy.argtypes = [POINTER(flux_fsm_t)]

# 回调适配器装饰器
class HandlerWrapper:
    def __init__(self) -> None:
        self._handlers: Dict[Any, Any] = {}

    def __call__(self, func: Any) -> Any:
        def wrapper(_, ctx: c_void_p, event: c_int) -> None:
            return func(ctx, event)
        self._handlers[func] = CFUNCTYPE(None, c_void_p, c_int, c_int)(wrapper)
        return func

handler_wrapper = HandlerWrapper()

# State类封装
class State:
    def __init__(self, state_id: int) -> None:
        self.id: int = state_id
        self._handlers: Dict[int, Any] = {}

    @handler_wrapper
    def handler(self, func: Any) -> Any:
        self._handlers[self.id] = func
        return func

# Transition类封装
class Transition:
    def __init__(
        self,
        from_state: int,
        event: int,
        to_state: int,
        guard: Optional[Any] = None,
        action: Optional[Any] = None
    ) -> None:
        self.from_state: int = from_state
        self.event: int = event
        self.to_state: int = to_state
        self.guard: Optional[Any] = guard
        self.action: Optional[Any] = action

# FSM封装类
class FiniteStateMachine:
    def __init__(self, initial_state: int, context: Optional[Any] = None) -> None:
        self._fsm: POINTER(flux_fsm_t) = _lib.flux_fsm_create(
            initial_state,
            ctypes.byref(context) if context else None
        )
        self.states: Dict[int, State] = {}
        self.transitions: List[flux_fsm_transition_t] = []

    def add_state(self, state: State) -> None:
        self.states[state.id] = state

    def add_transition(self, transition: Transition) -> None:
        c_trans = flux_fsm_transition_t()
        c_trans.from_state = transition.from_state
        c_trans.event = transition.event
        c_trans.to = transition.to_state
        c_trans.guard = transition.guard
        c_trans.action = transition.action
        self.transitions.append(c_trans)

    def process_event(self, event: int) -> None:
        _lib.flux_fsm_process_event(self._fsm, event)

    def __del__(self) -> None:
        _lib.flux_fsm_destroy(self._fsm)