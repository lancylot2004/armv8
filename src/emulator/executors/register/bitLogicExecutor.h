///
/// bitLogicExecute.h
/// Executes a bit-logic instruction
///
/// Created by Billy Highley on 30/05/2024.
///

#ifndef EMULATOR_BIT_LOGIC_EXECUTOR_H
#define EMULATOR_BIT_LOGIC_EXECUTOR_H

#include "../../../common/ir/register.h"
#include "../../system/registers.h"
#include "bitwiseShifts.h"

void bitLogicExecute(Register_IR *registerIR, Registers registers);

#endif // EMULATOR_BIT_LOGIC_EXECUTOR_H
