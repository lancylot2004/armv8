///
/// branchTranslator.h
/// Transform a [IR] of a Branch instruction to a binary instruction.
///
/// Created by Lancelot Liu on 29/05/2024.
///

#ifndef ASSEMBLER_BRANCH_TRANSLATOR_H
#define ASSEMBLER_BRANCH_TRANSLATOR_H

#include <string.h>

#include "const.h"
#include "error.h"
#include "helpers.h"
#include "ir.h"
#include "state.h"

Instruction translateBranch(IR *irObject, AssemblerState *state);

#endif // ASSEMBLER_BRANCH_TRANSLATOR_H
