#include "headers.h"

int sys_cmds(int argc, char **argv)
{

    child = fork();
    if (child < 0)
    {
        perror("forking");
    }
    else if (child == 0)
    {
        setpgid(child, child);
        if (background == 0)
            tcsetpgrp(shell, getpid());
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        signal(SIGTSTP, SIG_DFL);
        signal(SIGTTIN, SIG_DFL);
        signal(SIGTTOU, SIG_DFL);
        signal(SIGCHLD, SIG_DFL);
        if (execvp(argv[0], argv) < 0)
        {
            fprintf(stderr, "%s: command not found\n", argv[0]);
            exit(EXIT_FAILURE);
        }
        exit(EXIT_SUCCESS);
    }
    else
    {
        if (background == 0)
        {
            fg_jb.pid = child;
            fg_jb.status = 1;
            strcpy(fg_jb.command, "");
            for (int i = 0; i < argc - 1; i++)
            {
                strcat(fg_jb.command, argv[i]);
                strcat(fg_jb.command, " ");
            }
            strcat(fg_jb.command, argv[argc - 1]);
            tcsetpgrp(shell, child);
            waitpid(child, &status, WUNTRACED);
            if (!WIFEXITED(status))
            {
                bg_jb[++bg_cnt] = fg_jb;
                fg_jb.pid = 0;
                fg_jb.status = 0;
            }
            tcsetpgrp(shell, mypgid);
        }
        else
        {
            bg_cnt++;
            printf("[%d] %d\n", bg_cnt, child);
            strcpy(bg_jb[bg_cnt].command, "");
            for (int i = 0; i < argc - 1; i++)
            {
                strcat(bg_jb[bg_cnt].command, argv[i]);
                strcat(bg_jb[bg_cnt].command, " ");
            }
            if (argv[argc - 1] != NULL)
                strcat(bg_jb[bg_cnt].command, argv[argc - 1]);
            bg_jb[bg_cnt].pid = child;
            bg_jb[bg_cnt].status = 1;
        }
    }
}

void built_in(char *cmd)
{
    inp = ovr = apn = 0;
    if (strchr(cmd, '<') && strchr(cmd, '>'))
    {
        inp = 1;
        if (strstr(cmd, ">>"))
            apn = 1;
        else
            ovr = 1;
    }
    else if (strchr(cmd, '<'))
    {
        inp = 1;
    }
    else if (strstr(cmd, ">>"))
    {
        apn = 1;
    }
    else if (strchr(cmd, '>'))
        ovr = 1;
    breakup(cmd, inp, ovr, apn);
    if (redirect() < 0)
    {
        return;
    }
    if (background)
    {
        sys_cmds(argc, argv);
        return;
    }
    if (argv[0] == NULL || !strcmp(argv[0], ""))
        return;
    if (strcmp(argv[0], "cd") == 0)
    {
        Pwd = getenv("MYPWD");
        ch_dir(argv[0]);
        setenv("MYOLDPWD", Pwd, 1);
        getcwd(Pwd, 1024);
        setenv("MYPWD", Pwd, 1);
    }
    else if (strcmp(argv[0], "pwd") == 0)
    {
        printf("%s\n", getenv("MYPWD"));
    }
    else if (strcmp(argv[0], "echo") == 0)
    {
        echo_it(argv);
    }
    else if (strcmp(argv[0], "ls") == 0)
    {
        list_it(argv, argc);
    }
    else if (strcmp(argv[0], "pinfo") == 0)
    {
        if (argc == 1)
        {
            pd[0] = '\0';
            sprintf(pd, "%d", getpid());
            argv[1] = pd;
        }
        pinfo(argv[1]);
    }
    else if (strcmp(argv[0], "history") == 0)
    {
        print_hist(argc, argv);
    }
    else if (strcmp(argv[0], "nightswatch") == 0)
    {
        nights_watch(argc, argv);
    }
    else if (strcmp(argv[0], "getenv") == 0)
    {
        env(argc, argv, 0);
    }
    else if (strcmp(argv[0], "setenv") == 0)
    {
        env(argc, argv, 1);
    }
    else if (strcmp(argv[0], "unsetenv") == 0)
    {
        env(argc, argv, 2);
    }
    else if (strcmp(argv[0], "jobs") == 0)
    {
        print_jobs();
    }
    else if (strcmp(argv[0], "kjob") == 0)
    {
        kjob(argv);
    }
    else if (strcmp(argv[0], "fg") == 0)
    {
        fg(argc, argv);
    }
    else if (strcmp(argv[0], "bg") == 0)
    {
        bg(argv, argc);
    }
    else if (strcmp(argv[0], "overkill") == 0)
    {
        overkill();
    }
    else if (strcmp(argv[0], "quit") == 0)
    {
        exit(0);
    }
    else
    {
        sys_cmds(argc, argv);
    }
    return;
}
