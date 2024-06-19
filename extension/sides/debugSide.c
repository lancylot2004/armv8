///
/// debugSide.c
/// The side panel for the binary representation of the assembly code.
///
/// Created by Lancelot Liu on 19/06/2024.
///

#include "debugSide.h"

static Registers_s lastRegs;

static void rerenderLineWrapper(Line *line, int index) {
    rerenderLine(line, index, false);
}

static void printMaybeSelected(WINDOW *win, bool selected, int row, int col, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    wattron(win, selected ? COLOR_PAIR(SELECTED_SCHEME) : A_NORMAL);
    wmove(win, row, col);
    vw_printw(win, fmt, args);
    wattroff(win, selected ? COLOR_PAIR(SELECTED_SCHEME) : A_NORMAL);
    va_end(args);
}

void updateDebug(Registers regs) {
    // Print out all lines in current window.
    iterateLinesInWindow(file, &rerenderLineWrapper);

    werase(side);
    wmove(side, 0, 0);
    int width = getmaxx(side);

    int currLine = 0;
    for (; currLine < NO_GPRS / 2; currLine++) {
        printMaybeSelected(side, getReg(regs, currLine) != getReg(&lastRegs, currLine), currLine, 0,
                           "X%02d = %016" PRIx64, currLine, getReg(regs, currLine));

        int otherLine = currLine + NO_GPRS / 2;
        printMaybeSelected(side, getReg(regs, otherLine) != getReg(&lastRegs, otherLine), 
                           currLine, width / 2, "X%02d = %016" PRIx64, otherLine, getReg(regs, otherLine));
    }

    currLine++;
    printMaybeSelected(side, getRegPC(regs) != getRegPC(&lastRegs), currLine++, 0, 
                       "PC = %016" PRIx64 "\n", getRegPC(regs));

    wprintw(side, "PSTATE : ");

    printMaybeSelected(side, getRegState(regs, N) != getRegState(&lastRegs, N), currLine, 9, 
                       "%c", getRegState(regs, N) ? 'N' : '-');
    printMaybeSelected(side, getRegState(regs, Z) != getRegState(&lastRegs, Z), currLine, 10, 
                       "%c", getRegState(regs, Z) ? 'Z' : '-');
    printMaybeSelected(side, getRegState(regs, C) != getRegState(&lastRegs, C), currLine, 11, 
                       "%c", getRegState(regs, C) ? 'C' : '-');
    printMaybeSelected(side, getRegState(regs, V) != getRegState(&lastRegs, V), currLine, 12, 
                       "%c", getRegState(regs, V) ? 'V' : '-');

    currLine += 2;
    if (fatalError[0] != '\0') {
        wattron(side, COLOR_PAIR(ERROR_SCHEME));
        mvwprintw(side, currLine, 0, "FATAL ERROR: %s", fatalError);
        wattroff(side, COLOR_PAIR(ERROR_SCHEME));
    }

    wrefresh(side);
    lastRegs = *regs;
}
