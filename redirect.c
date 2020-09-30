#include "headers.h"

void make_args(char *buf)
{
    argc = 1;
    argv[0] = strtok(buf, " ");
    while ((argv[argc] = strtok(NULL, " ")) != NULL)
        argc++;
    argv[argc] = NULL;
}

void breakup(char *cmd, int a, int b, int c)
{
    make_args(cmd);
    int v1 = -1, v2 = -1;
    var[0] = var[1] = var[2] = NULL;
    if (a == 1)
    {
        for (int i = 0; i < argc - 1; i++)
        {
            if (strcmp(argv[i], "<") == 0)
            {
                var[1] = strdup(argv[i + 1]);
                v1 = i;
                break;
            }
        }
    }
    if (b == 1)
    {
        for (int i = 0; i < argc - 1; i++)
        {
            if (strcmp(argv[i], ">") == 0)
            {
                var[2] = strdup(argv[i + 1]);
                v2 = i;
                break;
            }
        }
    }
    else if (c == 1)
    {
        for (int i = 0; i < argc - 1; i++)
        {
            if (strcmp(argv[i], ">>") == 0)
            {
                var[2] = strdup(argv[i + 1]);
                v2 = i;
                break;
            }
        }
    }
    if (var[1] != NULL)
        var[1] = white_space(var[1]);
    if (var[2] != NULL)
        var[2] = white_space(var[2]);
    int ans = 0;
    if (v1 == -1 && v2 == -1)
        ;
    else if (v1 == -1)
    {
        if (v2 == 0)
            ans = 2;
        else if (v2 == argc - 2)
        {
            argv[argc - 2] = NULL;
        }
    }
    else if (v2 == -1)
    {
        if (v1 == 0)
            ans = 2;
        else if (v1 == argc - 2)
        {
            argv[argc - 2] = NULL;
        }
    }
    else
    {
        if ((v1 == 0 && v2 == 2) || (v2 == 0 && v1 == 2))
            ans = 4;
        else if ((v1 == 0 && v2 == argc - 2) || (v2 == 0 && v1 == argc - 2))
        {
            argv[argc - 2] = NULL;
            ans = 2;
        }
        else
        {
            argv[argc - 4] = NULL;
        }
    }
    argc = 0;
    while (argv[ans] != NULL)
    {
        argv[argc++] = argv[ans++];
    }
    argv[argc] = NULL;
}

int redirect()
{
    if (ovr == 1)
    {
        if ((output_fd = open(var[2], O_CREAT | O_WRONLY | O_TRUNC, 0644)) < 0)
        {
            perror(var[2]);
            return -1;
        }
        dup2(output_fd, 1);
        close(output_fd);
        ovr = 0;
    }
    else if (apn == 1)
    {
        if ((output_fd = open(var[2], O_CREAT | O_APPEND | O_WRONLY, 0644)) < 0)
        {
            perror(var[2]);
            return -1;
        }
        dup2(output_fd, 1);
        close(output_fd);
        apn = 0;
    }
    if (inp == 1 && var[1] != NULL)
    {
        if ((input_fd = open(var[1], O_RDONLY, 0)) < 0)
        {
            perror(var[1]);
            return -1;
        }
        dup2(input_fd, 0);
        close(input_fd);
        inp = 0;
    }
    return 1;
}
