#include "headers.h"

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"

char *dirpath()
{
    char *dir = strdup(Pwd);
    if (strncmp(dir, home, strlen(home)) == 0)
    {
        rel = dir + strlen(home) - 1;
        rel[0] = '~';
        return rel;
    }
    return dir;
}

void prompt()
{
    gethostname(buf2, 1024);
    // printf(ANSI_COLOR_GREEN "%s@%s" ANSI_COLOR_RESET, getenv("USER"), buf2);
    // printf(":");
    // printf(ANSI_COLOR_BLUE "%s" ANSI_COLOR_RESET, dirpath());
    // printf("$ ");
    printf("<");
    printf(ANSI_COLOR_GREEN "%s@%s" ANSI_COLOR_RESET, getenv("USER"), buf2);
    printf(":");
    printf(ANSI_COLOR_BLUE "%s" ANSI_COLOR_RESET, dirpath());
    printf("> ");
}
