import sys
from pathlib import Path
sys.path.append(str(Path(__file__).resolve().parent.parent.parent))

from bindings.python.flux_fsm_ctypes import FiniteStateMachine, State, Transition
from bindings.python.flux_fsm_ctypes import handler_wrapper
import ctypes

# 定义状态和事件
STATE_A = 1
STATE_B = 2
EVENT_SWITCH = 100

# 创建状态机
fsm = FiniteStateMachine(initial_state=STATE_A)

# 创建并添加状态
@State(STATE_A)
class StateA:
    @handler_wrapper
    @State.handler
    def on_enter(self, ctx, event):
        print("Entering State A")

@State(STATE_B)
class StateB:
    @handler_wrapper
    @State.handler
    def on_enter(self, ctx, event):
        print("Entering State B")

# 添加状态转移
trans = Transition(
    from_state=STATE_A,
    event=EVENT_SWITCH,
    to_state=STATE_B,
    guard=lambda ctx: True,
    action=lambda ctx: print("Performing transition action")
)
fsm.add_transition(trans)

# 处理事件
fsm.process_event(EVENT_SWITCH)

# 异步回调示例
async_callback = ctypes.CFUNCTYPE(None, ctypes.c_void_p, ctypes.c_void_p)(
    lambda event, ctx: print(f"Async event received: {event}")
)