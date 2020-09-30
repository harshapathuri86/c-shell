#include "headers.h"

void command(int in, int out, char *cmd)
{
    if (in != 0)
    {
        dup2(in, 0);
        close(in);
    }
    if (out != 1)
    {
        dup2(out, 1);
        close(out);
    }
    built_in(cmd);
    dup2(SIN, 0);
    dup2(SOUT, 1);
    return;
}

void tokenize_pipe(char *buf)
{
    int i, in, fd[2];
    int n = 1;
    cmds[0] = strtok(buf, "|");
    while ((cmds[n] = strtok(NULL, "|")) != NULL)
        n++;
    cmds[n] = NULL;
    in = 0;
    for (i = 0; i < n - 1; ++i)
    {
        pipe(fd);
        command(in, fd[1], cmds[i]);
        close(fd[1]);
        in = fd[0];
    }
    if (in != 0)
        dup2(in, 0), close(in);
    built_in(cmds[n - 1]);
}

void tokenize_input(char *buf)
{
    if (buf == NULL)
        return;
    token[0] = strtok(buf, ";");
    int z = 1;
    while ((token[z] = strtok(NULL, ";")) != NULL)
        z += 1;
    token[z] = NULL;
    for (int i = 0; i < z; i++)
    {
        background = 0;
        int l = strlen(token[i]);
        if (token[i][l - 1] == '&')
        {
            background = 1;
            token[i][l - 1] = '\0';
        }
        tokenize_pipe(token[i]);
    }
    return;
}