#include "../lib/global.h"

int GLOBAL_printLineInFile(char *filename, char *line)
{
    FILE *file = fopen(filename, "a");
    if (file == NULL)
    {
        printf("ERROR: Could not open file %s for writing.\n", filename);
        return -1;
    }

    fprintf(file, "%s\n", line);
    fclose(file);
    return 0;
}