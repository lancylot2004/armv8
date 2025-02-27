///
/// error.h
/// "For when you need to skidaddle."
///
/// Created by Lancelot Liu on 30/05/2024.
///

#ifndef COMMON_ERROR_H
#define COMMON_ERROR_H

#include <errno.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <string.h>

extern bool JUMP_ON_ERROR;
extern jmp_buf fatalBuffer;
extern char *fatalError;



/// Assert [__CONDITION__], pretty-printing an error and exiting if it is not met.
/// @param __CONDITION__ The condition to assert over.
/// @param __MESSAGE__ The failure message.
#define assertFatal(__CONDITION__, __MESSAGE__) \
    assertFatal_(__CONDITION__, __MESSAGE__, __FILE__, __LINE__, __func__)

/// Assert [__CONDITION__], pretty-printing an error and exiting if it is not met.
/// @param __CONDITION__ The condition to assert over.
/// @param __FORMAT__ The failure message.
/// @param ... The values to be substituted into the message, if any.
#define assertFatalWithArgs(__CONDITION__, __FORMAT__, ...) \
    assertFatal_(__CONDITION__, __FORMAT__, __FILE__, __LINE__, __func__, __VA_ARGS__)

void assertFatal_(bool condition, char format[], char *file, int line, const char *func, ...);

/// Assert that [__VALUE__] is not null, pretty-printing an error and exiting if it is.
/// @param __VALUE__ The value to assert not null over.
/// @param __MESSAGE__ The failure message.
#define assertFatalNotNull(__VALUE__, __MESSAGE__) \
    assertFatal_((__VALUE__) != NULL, __MESSAGE__, __FILE__, __LINE__, __func__)

/// Assert that [__VALUE__] is not null, pretty-printing an error and exiting if it is.
/// @param __VALUE__ The value to assert not null over.
/// @param __FORMAT__ The failure message.
/// @param ... The values to be substituted into the message, if any.
#define assertFatalNotNullWithArgs(__VALUE__, __FORMAT__, ...) \
    assertFatal_((__VALUE__) != NULL, __FORMAT__, __FILE__, __LINE__, __func__, __VA_ARGS__)

/// Pretty-prints the given error and exits the program.
/// @param __MESSAGE__ The failure message.
/// @warning Will definitely exit the program.
#define throwFatal(__MESSAGE__) \
    throwFatal_(__MESSAGE__, __FILE__, __LINE__, __func__)

/// Pretty-prints the given error and exits the program.
/// @param __MESSAGE__ The failure message.
/// @warning Will definitely exit the program.
/// @param ... The values to be substituted into the message, if any.
#define throwFatalWithArgs(__FORMAT__, ...) \
    throwFatal_(__FORMAT__, __FILE__, __LINE__, __func__, __VA_ARGS__)

noreturn void throwFatal_(char format[], char *file, int line, const char *func, ...);

#endif // COMMON_ERROR_H
