///
/// assemble.c
/// The entrypoint to the assembler program.
///
/// Created by ??? on ???.
///

#include "assemble.h"

/// The entrypoint to the assembler program.
/// @param argc Number of arguments. Should be 3.
/// @param argv Arguments. Should contain program name, assembly input, and object code out.
/// @return Program exit code.
/// @example \code ./assemble code.s code.o \endcode
int main(int argc, char **argv) {
    // Check that [argv] is valid, i.e., has 3 args.
    if (argc != 3) return EXIT_FAILURE;

    FILE *fileIn = fopen(argv[1], "r");
    char line[256];
    AssemblerState state = createState();

    // TODO: Handle line too long... is it possible?
    while (fgets(line, sizeof(line), fileIn)) {
        char *trimmedLine = trim(line, " ");
        IR currentIR;
        // TODO: Remove this which silences warnings during debug of other files.
        printf("%d", currentIR.type);

        // Check if line is blank.
        if (strlen(line) == 0) continue;

        // Check if line is directive.
        if (line[0] == '.') {
            handleDirective(trimmedLine, &state);
        }

        // Check if line is a label.
        char *colon = strchr(line, ':');
        if (colon != NULL) {
            // Ensure all preceding chars are alphabet.
            for (char *p = line; p < colon; p++) {
                if (!isalpha((unsigned char) *p)) {
                    currentIR = handleInstruction(trimmedLine, &state);
                }
            }

            handleLabel(trimmedLine, &state);
        }

        // By default, return instruction.
        currentIR = handleInstruction(trimmedLine, &state);

        // TODO: Append current IR to some dynamic array of all IRs.
    }

    fclose(fileIn);
    destroyState(state);

    // TODO: Final process all IRs, and write to file.
    FILE *fileOut = fopen(argv[2], "wb");
    // Write resulting instruction (if any) to file.
    //    if (!inst) continue;
    //    fwrite(&inst, sizeof(BitInst), 1, fileOut);
    fclose(fileOut);

    return EXIT_SUCCESS;
}

