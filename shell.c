#include "headers.h"

int main()
{
    initialize();
    while (1)
    {
//        dup2(SIN, STDIN_FILENO);
//        dup2(SOUT, STDOUT_FILENO);
        home = getenv("MYHOME");
        prompt();
        retval = 1;
        ptr = NULL;
        int len = getline(&ptr, &n, stdin);
        if (len == -1)
        {
            overkill();
            printf("quit\n");
            exit(0);
        }
        ptr[len - 1] = '\0';
        Dup = strdup(ptr);
        ptr = white_space(ptr);
        if (!(strcmp(ptr, "") && strcmp(ptr, "\n")))
            continue;
        add_history(Dup);
        write_history(NULL);
        free(Dup);
        tokenize_input(ptr);
    }
    free(ptr);
    return 0;
}
