///
/// process.c
/// Midway point to get [IRExecutor]s and [BinaryParser]s.
///
/// Created by Billy Highley and Alexander Biraben-Renard on 29/05/2024.
///

#include "emulatorDelegate.h"

/// Get the corresponding [IRExecutor] for this [irObject].
/// @param instruction The binary representation of the instruction.
/// @returns The corresponding [IRExecutor].
Executor getExecuteFunction(IR *irObject) {
    switch (irObject->type) {
        case IMMEDIATE:
            return executeImmediate;

        case REGISTER:
            return executeRegister;

        case LOAD_STORE:
            return executeLoadStore;

        case BRANCH:
            return executeBranch;

        default:
            throwFatal("Invalid IR!");
    }
}

/// Get the corresponding [BinaryParser] for this [instruction].
/// @param instruction The binary representation of the instruction.
/// @returns The corresponding [BinaryParser].
Decoder getDecodeFunction(const Instruction instruction) {
    Component op0 = decompose(instruction, OP0_M);
    if ((op0 & OP0_IMMEDIATE_M) == OP0_IMMEDIATE_C) {
        return decodeImmediate;
    } else if ((op0 & OP0_REGISTER_M) == OP0_REGISTER_C) {
        return decodeRegister;
    } else if ((op0 & OP0_LOAD_STORE_M) == OP0_LOAD_STORE_C) {
        return decodeLoadStore;
    } else if ((op0 & OP0_BRANCH_M) == OP0_BRANCH_C) {
        return decodeBranch;
    }

    throwFatal("Invalid binary instruction!");
}

/// Executes [instruction] given context.
/// @param instruction The binary instruction to execute.
/// @param registers The current virtual registers.
/// @param memory The address of the virtual memory.
void execute(Instruction *instruction, Registers registers, Memory memory) {
    // Store the address in the PC before execution.
    BitData pcVal = getRegPC(registers);

    // Decode and execute.
    IR ir = getDecodeFunction(*instruction)(*instruction);
    getExecuteFunction(&ir)(&ir, registers, memory);

    // Increment PC only when no branch or jump instructions applied.
    if (pcVal == getRegPC(registers)) incRegPC(registers);

    // Fetch next instruction
    *instruction = readMem(memory, false, getRegPC(registers));
}
