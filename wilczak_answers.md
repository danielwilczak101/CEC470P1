1.
It will do
ADD ACC 0x0
when all bits are 0.

2.
There are 256 possible math opcodes implemented, 12 possible memory opcodes implemented, and 7 possible branch opcodes implemented, and 2 special opcodes. In total, there are 149 implemented OPCODEs, leaving room for 107 more possible opcodes.

3.
To compare ACC with a constant, a subtract opcode for the ACC and constant needs to be put into memory, followed by the constant, followed by a branch opcode (say, BRZ). This is "0xc6 (constant) 0x11".

To PUSH or PULL from a stack, the memory opcodes are needed. Pushing a value would be storing a value to the top of the stack and incrementing a stack pointer. Pulling a value would be loading a value from the top of the stack and decrementing the stack pointer (and possibly reseting the top of the stack to all 0's before decrementing).

To take the 2's complement of ACC, it needs to be XOR'd with all 1's and then incremented. This is "0xa6 0xff 0xd4".

4.
You will need additional registers to store intermediate information related to what was decoded for the execute function.
