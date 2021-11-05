#define NOP_OPCODE 0x18
#define HALT_OPCODE 0x19

void fetchNextInstruction(void);

// Declare variables
unsigned char memory[65536];
unsigned char ACC = 0;    // Accumulator
unsigned char IR = 0;    // Instruction Register
unsigned int MAR = 0;    // Memory Address Register
unsigned int PC = 0;    // Program Counter

void fetchNextInstruction(void) {
  IR = memory[PC++];

  if (IR & 0x80) { // Math OPCODE

    if ((IR & 0x0c) == 0x0c) // Destination: memory
      PC += 2;

    switch (IR & 0x03) { // Source

      case 3: // Constant
        if ((IR & 0x08) == 0x08) // 16-bit destination
          PC++;
        PC++;
        break;

      case 4: // Memory
        PC += 2;
        break;
    }
  } // End if math OPCODE

  else if ((IR & 0xf0) == 0) { // Memory
    if ((IR & 2) == 0) { // Operand used
      PC++;
      if (((IR & 1) == 0) || ((IR & 4) == 4)) // Address is operand or register is MAR
        PC++;
    }
  } // End if memory OPCODE

  else if ((IR & 0xF8) == 0x10) // Branches
    PC += 2;

  // All else is either a "No Operation", "Halt", or illegal opcode,
  // in which case do nothing.
}
