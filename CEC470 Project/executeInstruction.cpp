void executeInstruction(void);

void executeInstruction(void) {

  if ((IR & 0x80) == 0x80) { // Math OPCODE
    int dest;
    int source;

    switch (IR & 0x0c) { // Destination

      case 0: // Indirect (MAR as pointer)
        dest = memory[MAR];
        break;

      case 4: // Accumulator ACC
        dest = ACC;
        break;

      case 8: // Address register MAR
        dest = MAR;
        break;

      case 12: // Memory
        if ((IR & 0x02) == 0)
          dest = memory[((memory[PC-2] << 8) + memory[PC-1])];
        else
          dest = memory[((memory[PC-4] << 8) + memory[PC-3])];
        break;
    }

    switch (IR & 0x03) { // Source

      case 0: // Indirect (MAR used as a pointer)
        source = memory[MAR];
        break;

      case 1: // Accumulator ACC
        source = ACC;
        break;

      case 2: // Constant
        source = memory[PC-1];
        if ((IR & 0x0c) == 0x8)
          source += memory[PC-2] << 8;
        break;

      case 3: // Memory
        source = memory[((memory[PC-2] << 8) + memory[PC-1])];
        if ((IR & 0x0c) == 0x8) {
          source <<= 8;
          source += memory[(memory[PC-2] << 8) + memory[PC-1]+1];
        }
        break;
    }

    switch (IR & 0x70){ // Checking the function
      case 0x00: // AND
        dest &= source;
        break;
      case 0x10: // OR
        dest |= source;
        break;
      case 0x20: // XOR
        dest ^= source;
        break;
      case 0x30: // ADD
        dest += source;
        break;
      case 0x40: // SUB
        dest -= source;
        break;
      case 0x50: // INC
        dest++;
        break;
      case 0x60: // DEC
        dest--;
        break;
      case 0x70: // NOT
        dest = !dest;
        break;
    }

    switch (IR & 0x0c) { // Store destination

      case 0x0: // Indirect (MAR used as pointer)
        memory[MAR] = dest & 0xff;
        break;

      case 0x4: // Accumulator ACC
        ACC = dest & 0xff;
        break;

      case 0x8: // Address register MAR
        MAR = dest & 0xffff;
        break;

      case 0xc: // Memory
        memory[((memory[PC-2] << 8) + memory[PC-1])] = (dest >> 8) & 0xff;
        memory[((memory[PC-2] << 8) + memory[PC-1]) + 1] = dest & 0xff;
        break;
    }
  } // End if Math OPCODE

  else if ((IR & 0xf0) == 0) { // Memory OPCODE

    if ((IR & 0x08) == 0) { // Store

      if ((IR & 0x04) == 0) { // Storing from Register = Accumulator ACC

        switch (IR & 0x03) {

          case 0: // Storing with Method = Operand is used as address
            memory[memory[((memory[PC-2] << 8) + memory[PC-1])]] = ACC;
            break;

          case 1: // Operand is used as a Constant
            memory[((memory[PC-2] << 8) + memory[PC-1])] = ACC;
            break;

          case 2: // Storing with Method = Indirect (MAR used as a pointer)
            memory[MAR] = ACC;
            break;
        }
      }

      else { // Storing from Register = Index register MAR

        switch (IR & 0x03) {

          case 0: // Storing with Method = Operand is used as address
            memory[memory[((memory[PC-2] << 8) + memory[PC-1])]] = (MAR >> 8) & 0xff;
            memory[memory[((memory[PC-2] << 8) + memory[PC-1])] + 1] = MAR & 0xff;
            break;

          case 1: // Operand is used as a Constant
            memory[((memory[PC-2] << 8) + memory[PC-1])] = (MAR >> 8) & 0xff; 
            memory[((memory[PC-2] << 8) + memory[PC-1]) + 1] = MAR & 0xff;
            break;

          case 2: // Storing with Method = Indirect (MAR used as a pointer)
            memory[MAR] = (MAR >> 8) & 0xff;
            memory[MAR+1] = MAR & 0xff;
          break;
        }
      }
    }
    else { // Load

      if ((IR & 0x04) == 0) { // Loading into register Accumulator ACC

        switch (IR & 0x03) {

          case 0: // Method = Operand is used as address
            ACC = memory[((memory[PC-2] << 8) + memory[PC-1])];
            break;

          case 1: // Method = Operand is used as a constant
            ACC = memory[PC-1];
            break;

          case 2: // Method = Indirect (MAR used as a pointer)
            ACC = memory[MAR];
            break;
        }
      }

      else { // Loading into MAR

        switch (IR & 0x03) {

          case 0: // Method = Operand is used as address
            MAR = (memory[PC-2] << 8) + memory[PC-1];
            MAR = (memory[MAR] << 8) + memory[MAR+1];
            break;

          case 1: // Method = Operand is used as a constant
            MAR = (memory[PC-2] << 8) + memory[PC-1];
            break;

          case 2: // Method = Indirect (MAR used as pointer)
            MAR = (memory[MAR] << 8) + memory[MAR+1];
            break;
        }
      }
    }
  } // End if memory OPCODE

  else if ((IR & 0xF8) == 0x10) { // Branches

    switch (IR & 0x07) { // Type of branch

      case 0: // BRA (Unconditional branch/branch always)
        PC = (memory[PC-2] << 8) + memory[PC-1];
        break;

      case 1: // BRZ (Branch if ACC = 0)
        if (ACC == 0)
          PC = (memory[PC-2] << 8) + memory[PC-1];
        break;

      case 2: // BNE (Branch if ACC != 0)
        if (ACC != 0)
          PC = (memory[PC-2] << 8) + memory[PC-1];
        break;

      case 3: // BLT (Branch if ACC < 0)
        if ((ACC & 0x80) != 0)
          PC = (memory[PC-2] << 8) + memory[PC-1];
        break;

      case 4: // BLE (Branch if ACC <= 0)
        if (((ACC & 0x80) != 0) || (ACC == 0))
          PC = (memory[PC-2] << 8) + memory[PC-1];
        break;

      case 5: // BGT (Branch if ACC > 0)
        if (((ACC & 0x80) == 0) && (ACC != 0))
          PC = (memory[PC-2] << 8) + memory[PC-1];
        break;

      case 6: // BGE (Branch if ACC >= 0)
        if ((ACC & 0x80) == 0)
          PC = (memory[PC-2] << 8) + memory[PC-1];
        break;
    }
  }

  // All else is either a "No Operation", "Halt", or illegal opcode,
  // in which case do nothing.
}
