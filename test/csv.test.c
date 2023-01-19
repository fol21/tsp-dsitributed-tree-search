#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* getfield(char* line, int num)
{
    const char* tok;
    for (tok = strtok(line, ";");
            tok && *tok;
            tok = strtok(NULL, ";"))
    {
        if (!--num)
            return tok;
    }
    return NULL;
}

int main()
{
    FILE* stream = fopen("./datasets/P01/p01_d.txt", "r");

    char line[1024];
    fgets(line, 1024, stream);
    char* tmp = strdup(line);
    printf("Number of cities %d\n", atoi(getfield(tmp, 1)));
    while (fgets(line, 1024, stream))
    {
        tmp = strdup(line);
        printf("Field 1 would be %d\n", atoi(getfield(tmp, 1)));
        // NOTE strtok clobbers tmp
        free(tmp);
    }
}