#include "headers.h"

void kjob(char **argv, int argc)
{
    if (argc != 3)
    {
        fprintf(stderr, "format: kjob id signal\n");
        retval = -1;
        return;
    }
    int pd = atoi(argv[1]);
    int signal = atoi(argv[2]);
    if (pd > bg_cnt || bg_jb[pd].status == 0 || pd < 1)
    {
        fprintf(stderr, "No such job exists\n");
        retval = -1;
        return;
    }
    kill(bg_jb[pd].pid, signal);
}

void overkill()
{
    int i;
    for (i = 1; i <= bg_cnt; i++)
    {
        kill(bg_jb[i].pid, 9);
    }
}

void bg(char **argv, int argc)
{
    if (argc != 2)
    {
        fprintf(stderr, "format: bg <jobnumber>\n");
        retval = -1;
        return;
    }
    else
    {
        int pd = atoi(argv[1]);
        if (pd > bg_cnt || bg_jb[pd].status == 0 || pd < 1)
        {
            fprintf(stderr, "No such job\n");
            retval = -1;
            return;
        }
        pid_t pid = bg_jb[pd].pid;
        kill(pid, SIGTTIN);
        kill(pid, SIGCONT);
    }
}

void print_jobs()
{
    char status;
    for (int i = 1; i <= bg_cnt; i++)
    {
        if (bg_jb[i].status == 1)
        {
            sprintf(file, "/proc/%d/stat", bg_jb[i].pid);
            if ((fd = fopen(file, "r")) == NULL)
            {
                printf("[%d] %s %s [%d]\n", i, "Completed", bg_jb[i].command, bg_jb[i].pid);
                return;
            }
            fscanf(fd, "%*d %*s %c", &status);
            fclose(fd);
            printf("[%d] ", i);
            if (status == 'T')
                printf("%s ", "Stopped");
            else
                printf("%s ", "Running");
            printf("%s [%d]\n", bg_jb[i].command, bg_jb[i].pid);
        }
    }
}

void fg(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "format:fg pid\n");
        retval = -1;
        return;
    }
    int n = atoi(argv[1]);
    if (n > bg_cnt || n < 1)
    {
        fprintf(stderr, "No job exists with that number\n");
        retval = -1;
        return;
    }
    if (bg_jb[n].status != 1)
    {
        fprintf(stderr, "No such job exists\n");
        retval = -1;
        return;
    }
    int pid = bg_jb[n].pid;
    int pgid = getpgid(pid);
    tcsetpgrp(shell, pgid);
    fgpid = pid;
    int status;
    if (killpg(pgid, SIGCONT) < 0)
        retval = -1, perror("cant get to foreground\n");
    waitpid(pid, &status, WUNTRACED);
    if (!WIFSTOPPED(status))
    {
        bg_jb[n].status = 0;
        if (n == bg_cnt)
            bg_cnt -= 1;
        fg_jb.status = 0;
    }
    else
        retval = -1;
    tcsetpgrp(shell, shellpgid);
    tcgetattr(shell, &bg_jb[n].tmodes);
    tcsetattr(shell, TCSADRAIN, &shell_tmodes);
    return;
}
