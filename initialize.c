#include "headers.h"

void sig_handler(int sig)
{
    if (sig == SIGINT)
    {
        signal(SIGINT, SIG_IGN);
        fflush(stdout);
        fflush(stdin);
        signal(SIGINT, sig_handler);
    }
    if (sig == SIGCHLD)
    {
        int i, status, pd;
        while ((pd = waitpid(-1, &status, WNOHANG)) > 0)
        {
            for (i = 1; i <= bg_cnt; i++)
            {
                if (bg_jb[i].status == 1 && bg_jb[i].pid == pd)
                    break;
            }
            if (i != bg_cnt + 1)
            {
                if (WIFEXITED(status))
                    fprintf(stdout, "\n%s with pid %d exited normally\n", bg_jb[i].command, bg_jb[i].pid);
                else if (WIFSIGNALED(status))
                    fprintf(stdout, "\n%s with pid %d exited with signal %d\n", bg_jb[i].command, bg_jb[i].pid, status);
                bg_jb[i].status = 0;
            }
        }
    }
    return;
}

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
    printf("<");
    printf(ANSI_COLOR_GREEN "%s@%s" ANSI_COLOR_RESET, getenv("USER"), buf2);
    printf(":");
    printf(ANSI_COLOR_BLUE "%s" ANSI_COLOR_RESET, dirpath());
    printf("> ");
}

void initialize()
{
    shell = STDERR_FILENO;
    bg_cnt = 0;
    if (isatty(shell))
    {
        while (tcgetpgrp(shell) != (shellpgid = getpgrp()))
            kill(shellpgid, SIGTTIN);
    }
    signal(SIGINT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    mypid = mypgid = getpid();
    setpgid(mypid, mypgid);
    tcsetpgrp(shell, mypgid);
    SIN = dup(STDIN_FILENO);
    SOUT = dup(STDOUT_FILENO);
    Pwd = getenv("PWD");
    setenv("MYPWD", Pwd, 1);
    setenv("MYOLDPWD", Pwd, 1);
    setenv("MYHOME", Pwd, 1);
    home = getenv("MYHOME");
    using_history();
    stifle_history(20);
    read_history(NULL);
    signal(SIGCHLD, sig_handler);
    signal(SIGINT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
}
