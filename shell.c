#include "headers.h"

int main()
{
    initialize();
    while (1)
    {
        while (sigsetjmp(ctrlc_buf, 1) != 0)
            ;
        home = getenv("HOME");
        dup2(SIN, STDIN_FILENO);
        dup2(SOUT, STDOUT_FILENO);
        prompt();
        ptr = NULL;
        ptr = readline(PROMPT);
        if (ptr == NULL)
        {
            overkill();
            printf("quit\n");
            exit(0);
        }
        Dup = strdup(ptr);
        ptr = white_space(ptr);
        if (!(strcmp(ptr, "") && strcmp(ptr, "\n")))
            continue;
        add_history(Dup);
        write_history(NULL);
        free(Dup);
        retval = 1;
        tokenize_input(ptr);
    }
    free(ptr);
    return 0;
}
