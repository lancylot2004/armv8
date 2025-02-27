///
/// registerTranslator.h
/// Transform a [IR] of a Data Processing (Immediate) instruction to a binary instruction.
///
/// Created by Jack Wong on 01/06/2024.
///

#ifndef ASSEMBLER_REGISTER_TRANSLATOR_H
#define ASSEMBLER_REGISTER_TRANSLATOR_H

#include "const.h"
#include "error.h"
#include "helpers.h"
#include "ir.h"
#include "state.h"

Instruction translateRegister(IR *irObject, unused AssemblerState *state);

#endif // ASSEMBLER_REGISTER_TRANSLATOR_H
