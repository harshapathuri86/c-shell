#include "headers.h"

void sig_handler(int sig)
{
    if (sig == SIGINT)
    {
        printf("\n");
        siglongjmp(ctrlc_buf, 1);
        prompt();
        retval = -1;
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
    if (getenv("MYPWD") == NULL)
        setenv("MYPWD", getcwd(NULL, 0), 1), Pwd = getenv("MYPWD");
    char *dir = strdup(Pwd);
    if (getenv("MYHOME") != NULL)
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
    strcpy(PROMPT, "");
    if (retval == -1)
        strcat(PROMPT, ":'(");
    else
        strcat(PROMPT, ":')");
    strcat(PROMPT, "<");
    strcat(PROMPT, getenv("USER"));
    strcat(PROMPT, "@");
    strcat(PROMPT, buf2);
    strcat(PROMPT, ":");
    strcat(PROMPT, dirpath());
    strcat(PROMPT, "> ");
}

void initialize()
{
    shell = STDERR_FILENO;
    bg_cnt = 0;
    if (isatty(shell))
    {
        while (tcgetpgrp(shell) != (shellpgid = getpgrp()))
            kill(-shellpgid, SIGTTIN);
    }
    signal(SIGINT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    shellpid = shellpgid = getpid();
    if (setpgid(shellpid, shellpgid) < 0)
    {
        perror("Couldn't put the shell in its own process group");
        exit(1);
    }
    tcsetpgrp(shell, shellpgid);
    tcgetattr(shell, &shell_tmodes);
    SIN = dup(STDIN_FILENO);
    SOUT = dup(STDOUT_FILENO);
    Pwd = getenv("PWD");
    setenv("MYPWD", Pwd, 1);
    setenv("OLDPWD", Pwd, 1);
    setenv("MYHOME", Pwd, 1);
    home = getenv("MYHOME");
    using_history();
    stifle_history(20);
    read_history(NULL);
    retval = 0;
    signal(SIGCHLD, sig_handler);
    signal(SIGINT, sig_handler);
    signal(SIGTSTP, SIG_IGN);
}
