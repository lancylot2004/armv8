///
/// instructionHandler.c
/// Functions to categorise assembly instructions.
///
/// Created by Lancelot Liu on 30/05/2024.
///

#include "instructionHandler.h"

static const IREntry instructionHandlers[] = {
        { "add", parseDataProcessing },
        { "adds", parseDataProcessing },
        { "sub", parseDataProcessing },
        { "subs", parseDataProcessing },
        { "cmp", parseDataProcessing },
        { "cmn", parseDataProcessing },
        { "neg", parseDataProcessing },
        { "negs", parseDataProcessing },
        { "and", parseRegister },
        { "ands", parseRegister },
        { "bic", parseRegister },
        { "bics", parseRegister },
        { "eor", parseRegister },
        { "eon", parseRegister },
        { "orr", parseRegister },
        { "orn", parseRegister },
        { "tst", parseDataProcessing },
        { "mvn", parseRegister },
        { "mov", parseDataProcessing },
        { "movn", parseImmediate },
        { "movk", parseImmediate },
        { "movz", parseImmediate },
        { "madd", parseRegister },
        { "msub", parseRegister },
        { "mul", parseDataProcessing },
        { "mneg", parseDataProcessing },
        { "b", parseBranch },
        { "br", parseBranch },
        { "ldr", parseLoadStore },
        { "str", parseLoadStore },
};

static const InstructionEntry translationHandlers[] = {
        {IMMEDIATE,  translateImmediate},
        {REGISTER,   translateRegister},
        {LOAD_STORE, translateLoadStore},
        {BRANCH,     translateBranch},
        {CONSTANT,   translateConstant},
};

/// Gets the corresponding [IRGenerator].
/// @param mnemonic The mnemonic to search for.
/// @return Either the corresponding [IRGenerator], or NULL if none was found.
IRGenerator getParseFunction(const char *mnemonic) {
    for (int i = 0; i < (int) (sizeof(instructionHandlers) / sizeof(IREntry)); i++) {
        if (!strcmp(instructionHandlers[i].mnemonic, mnemonic)) {
            return instructionHandlers[i].handler;
        }
    }

    return NULL;
}

/// Gets the corresponding [InstructionGenerator].
/// @param type The type to search for.
/// @return Either the corresponding [InstructionGenerator], or NULL if none was found.
InstructionGenerator getTranslateFunction(IRType type) {
    for (int i = 0; i < (int) (sizeof(translationHandlers) / sizeof(InstructionEntry)); i++) {
        if (translationHandlers[i].type == type) {
            return translationHandlers[i].handler;
        }
    }

    return NULL;
}

/// Returns the immediate value from the IR form of a constant.
/// @param irObject The [IR] struct representing the immediate value.
/// @param state The current state of the assembler.
/// @return 32-bit binary word of the constant.
Instruction translateConstant(IR *irObject, unused AssemblerState *state) {
    assertFatal(irObject->type == CONSTANT, "[translateConstant] Received non-constant IR!");
    return irObject->ir.memoryData;
}
