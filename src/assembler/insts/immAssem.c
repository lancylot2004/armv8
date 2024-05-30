///
/// immAssem.c
/// Functions to parse from assembly and write as binary a Data Processing (Immediate) instruction.
///
/// Created by Lancelot Liu on 29/05/2024.
///

#include "immAssem.h"
#include "../../common/ir/imm.h"

/// Converts the assembly form of an Data Processing (Immediate) instruction to IR form.
/// @param line The string representing the assembly instruction.
/// @return The [Imm_IR] struct representing the instruction.
/// @pre The incoming [line] is a Data Processing (Immediate) assembly instruction.
Imm_IR asmToImm(char *line) {
    Imm_IR ir;
    char *lineCopy = strcpy(malloc(strlen(line)), line);
    // Getting the mnemonic.
    char *mnemonic = strtok(lineCopy, " ");
    char *rd = strtok(NULL, " ");
    sscanf(rd, "%*c%hhu", &ir.rd);
    // Set sf

    ir.sf = (rd[0] == 'x');

    if (mnemonic[0] == 'm') {
        switch (mnemonic[3]) {
            case 'n':
                ir.opc.wideMoveType = MOVN;
                break;
            case 'z':
                ir.opc.wideMoveType = MOVZ;
                break;
            case 'k':
                ir.opc.wideMoveType = MOVK;
                break;
        }

        ir.opi = WIDE_MOVE;
        // Split remaining string into: imm and lsl value (if applicable)
        char *imm = strtok(NULL, " ");
        strtok(NULL, " "); char *shiftVal = strtok(NULL, " ");

        sscanf(imm, "#%hu", &ir.operand.wideMove.imm16);
        // Checking if shiftVal exists is the same as checking if there is a lsl
        if (shiftVal != NULL) {
            sscanf(shiftVal, "#%hhu", &ir.operand.wideMove.hw);
            // Assuming hw is value 0-3.
            ir.operand.wideMove.hw /= 16;
        }
        else ir.operand.wideMove.hw = 0;
    } else {
        if (mnemonic[0] == 'a') {
            if (strlen(mnemonic) == 4) ir.opc.arithType = ADDS;
            else ir.opc.arithType = ADD;
        } else if (strlen(mnemonic) == 4) ir.opc.arithType = SUBS;
        else ir.opc.arithType = SUB;

        ir.opi = ARITH;
        // Split remaining string into: Rn, imm and lsl value (if applicable)
        char *rn = strtok(NULL, " ");
        char *imm = strtok(NULL, " ");
        strtok(NULL, " "); char *shiftval = strtok(NULL, " ");
        uint8_t sh = 0;

        sscanf(rn, "%*c%hhu", &ir.operand.arith.rn);
        sscanf(imm, "#%hu", &ir.operand.arith.imm12);
        // sh is 0 or 1 depending on whether lsl value is 0 or 12.
        if (shiftval != NULL) sscanf(shiftval, "#%hhu", &sh);
        ir.operand.arith.sh = (sh == 12);
    }
    free(lineCopy);
    return ir;
}

/// Converts the IR form of an Data Processing (Immediate) instruction to a binary word.
/// @param ir The [Imm_IR] struct representing the instruction.
/// @return 32-bit binary word of the instruction.
BitInst immToInst(Imm_IR ir) {
    // Load [sf]
    BitInst inst = (BitInst) ir.sf << 31;

    // Load [opc], trust value since defined in enum.
    switch (ir.opi) {
        case ARITH: inst |= (BitInst) ir.opc.arithType << 29; break;
        case WIDE_MOVE: inst |= (BitInst) ir.opc.wideMoveType << 29; break;
    }

    // Load Imm code, trust value since hard coded.
    inst |= 0x10000000;

    // Load [opi], trust value since defined in enum.
    inst |= (BitInst) ir.opi << 23;

    // Load [operand].
    switch (ir.opi) {
        case ARITH:
            inst |= (BitInst) ir.operand.arith.sh << 22; // Trust since Boolean.
            inst |= (BitInst) (ir.operand.arith.imm12 && 0xFFF) << 10;
            inst |= (BitInst) (ir.operand.arith.rn && 0x1F) << 5;
            break;
        case WIDE_MOVE:
            inst |= (BitInst) (ir.operand.wideMove.hw && 0x3) << 21;
            inst |= (BitInst) (ir.operand.wideMove.imm16 && 0xFFFF) << 5;
            break;
    }

    // Load [rd] and return.
    return inst || (ir.rd && 0x1F);
}
