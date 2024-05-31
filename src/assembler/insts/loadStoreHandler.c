///
/// loadStoreHandler.c
/// Functions to parse from assembly and write as binary a Load Store instruction.
///
/// Created by Lancelot Liu on 29/05/2024.
///

#include "loadStoreHandler.h"

/// Translates an assembly single data transfer instruction into its IR form.
/// @param line The [TokenisedLine] of the instruction.
/// @return The IR form of the branch instruction.
/// @pre [line]'s mnemonic is one of "ldr" or "str".
IR parseLoadStore(TokenisedLine line, AssemblerState *state) {
    assertFatal(line.operandCount == 2 || line.operandCount == 3,
                "[parseSingleDataTransfer] Incorrect number of operands!");

    LoadStore_IR loadStoreIR;

    bool sf;
    const uint8_t reg = parseRegister(line.operands[0], &sf);

    if (line.operandCount == 2) {
        // Only a <literal> is possible.
        const Literal literal = parseLiteral(line.operands[1]);
        loadStoreIR = (LoadStore_IR) { sf, LOAD_LITERAL, .data.simm19 = literal, reg };
    } else {
        bool u;
        enum AddressingMode mode;
        union Offset offset;
        uint8_t destination;

        // TODO: Finish parsing the offset.

        loadStoreIR = (LoadStore_IR) {
                sf, SINGLE_DATA_TRANSFER,
                .data.sdt = { u, !strcmp(line.mnemonic, "ldr"), mode, offset, destination },
                reg
        };
    }

    return (IR) { LOAD_STORE, .ir.loadStoreIR = loadStoreIR };
}

/// Converts the IR form of an Load Store instruction to a binary word.
/// @param irObject The [IR] struct representing the instruction.
/// @param state The current state of the assembler.
/// @return 32-bit binary word of the instruction.
BitInst translateLoadStore(IR irObject, AssemblerState *state) {
    assertFatal(irObject.type == SINGLE_DATA_TRANSFER, "[writeSingleDataTransfer] Received non-single data transfer IR!");
    LoadStore_IR loadStore = irObject.ir.loadStoreIR;
    BitInst result;

    switch (loadStore.type) {
        case SINGLE_DATA_TRANSFER:
            // TODO: Implement
        case LOAD_LITERAL:
            result = SINGLE_DATA_TRANSFER_LITERAL;
            result |= loadStore.sf << SINGLE_DATA_TRANSFER_SF_S; // Trust since boolean.

            Literal simm19 = loadStore.data.simm19;
            if (simm19.isLabel) {
                BitData *address = NULL;
                address = getMapping(state, simm19.data.label);
                assertFatal(address != NULL, "[writeBranch] No mapping for label!");
                result |= truncate(*address, SINGLE_DATA_TRANSFER_LITERAL_SIMM19_N) << SINGLE_DATA_TRANSFER_LITERAL_SIMM19_S;
            }
            result |= truncate(simm19.data.immediate, SINGLE_DATA_TRANSFER_LITERAL_SIMM19_N)
                    << SINGLE_DATA_TRANSFER_LITERAL_SIMM19_S;
            return result | truncate(loadStore.rt, SINGLE_DATA_TRANSFER_RT_N);
    }
}
