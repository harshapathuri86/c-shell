#include "headers.h"

void kjob(char **argv)
{
    int pd = atoi(argv[1]);
    int signal = atoi(argv[2]);
    if (pd > bg_cnt || bg_jb[pd].status == 0 || pd < 1)
    {
        printf("No such job exists\n");
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
        printf("format: bg <jobnumber>\n");
        return;
    }
    else
    {
        int pd = atoi(argv[1]);
        if (pd > bg_cnt || bg_jb[pd].status == 0 || pd < 1)
        {
            printf("No such job\n");
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
            }
            else
            {
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
}

void fg(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("format:fg pid\n");
        return;
    }
    int n = atoi(argv[1]);
    if (n > bg_cnt || n < 1)
    {
        printf("No job exists with that number\n");
        return;
    }
    if (bg_jb[n].status != 1)
    {
        printf("No such job exists\n");
        return;
    }
    int pid = bg_jb[n].pid;
    int pgid = getpgid(pid);
    tcsetpgrp(shell, pgid);
    fgpid = pid;
    int status;
    if (killpg(pgid, SIGCONT) < 0)
        perror("cant get to foreground\n");
    waitpid(pid, &status, WUNTRACED);
    if (!WIFSTOPPED(status))
    {
        bg_jb[bg_cnt].status = 0;
        bg_cnt--;
        fg_jb.status = 0;
    }
    tcsetpgrp(shell, mypgid);
    return;
}
