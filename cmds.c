#include "headers.h"

int max(int a, int b)
{
    if (a <= b)
        return b;
    else
        return a;
}

int min(int a, int b)
{
    if (a <= b)
        return a;
    else
        return b;
}

void echo_it(char **argv)
{
    int i = 1;
    while (argv[i] != NULL)
        printf("%s ", argv[i]), i += 1;
    printf("\n");
}

void ch_dir(char *buf)
{
    buf += 2;
    buf = white_space(buf);
    if (strcmp(buf, "-") == 0)
    {
        chdir(getenv("MYOLDPWD"));
        return;
    }
    if (strcmp(buf, "") == 0 || (!(strcmp(buf, "~") && strcmp(buf, "~/") && strcmp(buf, home))))
        chdir(home);
    else if (strncmp(buf, "~/", 2) == 0)
    {
        chdir(home);
        buf += 2;
        if (chdir(buf) < 0)
        {
            retval = -1;
            perror((buf - 2));
            chdir(Pwd);
        }
    }
    else
    {
        if (chdir(buf) < 0)
            retval = -1, perror(buf);
    }
}

void print_hist(int argc, char **argv)
{
    register HIST_ENTRY **list;
    register int i;
    int l1 = history_length;
    int l2;
    if (argc == 1)
        l2 = 10;
    else
        l2 = atoi(argv[1]);
    list = history_list();
    if (list)
        for (i = max(l1 - l2, 0); i < (max(l1 - l2, 0) + min(l1, l2)); i++)
            printf("%d: %s\n", i, list[i]->line);
    else
        retval = -1, fprintf(stderr, "no history list\n");
    return;
}

void env(int argc, char **argv, int a)
{
    if (a == 0)
    {
        //getenv
        if (argc != 2)
        {
            fprintf(stderr, "format:getenv <env>\n");
            retval = -1;
            return;
        }
        printf("%s %s\n", argv[1], getenv(argv[1]));
    }
    else if (a == 1)
    {
        //setenv
        if (argc > 3 || argc < 2)
        {
            fprintf(stderr, "format: setenv var [value]\n");
            retval = -1;
            return;
        }
        if (getenv(argv[1]) != NULL)
        {
            printf("%s has been overwritten\n", argv[1]);
        }
        else
        {
            printf("%s has been created\n", argv[1]);
        }
        if (argv[2] == NULL)
        {
            setenv(argv[1], "", 1);
        }
        else
        {
            setenv(argv[1], argv[2], 1);
        }
    }
    else if (a == 2)
    {
        //unsetenv
        if (argc != 2)
        {
            fprintf(stderr, "format: unsetenv var\n");
            retval = -1;
            return;
        }
        if (getenv(argv[1]) != NULL)
        {
            unsetenv(argv[1]);
            printf("%s", "The variable has been erased\n");
        }
        else
        {
            printf("%s", "The variable does not exist\n");
        }
    }
    return;
}

char *white_space(char *buf)
{
    if (buf == NULL)
        return NULL;
    int l = strlen(buf);
    end = buf + l - 1;
    while (isspace((unsigned char)*buf))
        buf += 1;
    while (end > buf && isspace((unsigned char)*end))
        end--;
    end[1] = '\0';
    return buf;
}

void pinfo(char *pid)
{
    char status;
    sprintf(file, "/proc/%s/stat", pid);
    if ((fd = fopen(file, "r")) == NULL)
    {
        retval = -1;
        fprintf(stderr, "No process with pid %s\n", pid);
        return;
    }
    fscanf(fd, "%*d %*s %c %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %lu %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d", &status, &memory);
    fclose(fd);
    printf("pid -- %s\n", pid);
    printf("Process Status -- %d\nmemory -- %ld\n", status, memory);
    snprintf(path, 30, "/proc/%s/exe", pid);
    l = readlink(path, BUF, sizeof(BUF) - 1);
    if (l != -1)
    {
        BUF[l] = '\0';
        if (strncmp(BUF, home, strlen(home)) == 0)
        {
            printf("Executable Path -- ~%s\n", (BUF + strlen(home)));
        }
        else
            printf("Executable Path -- %s\n", BUF);
    }
    return;
}

void nights_watch(int argc, char **argv)
{
    int cmd;
    if (argc <= 3 || strcmp(argv[1], "-n") != 0)
    {
        fprintf(stderr, "format: nightswatch -n <time> <command> \n");
        retval = -1;
        return;
    }
    int time = atoi(argv[2]);
    if (strcmp(argv[3], "newborn") == 0)
        cmd = 2;
    else if (strcmp(argv[3], "interrupt") == 0)
        cmd = 1;
    else
    {
        fprintf(stderr, "wrong <command>\n");
        retval = -1;
        return;
    }
    fd_set F;
    struct timeval t;
    int ret, n, lno, cnt, ft = 1, ct;
    char buff[1024];
    char *ptr, *ptr1;
    char ch[1024];
    while (1)
    {
        FD_ZERO(&F);
        FD_SET(0, &F);
        t.tv_sec = time;
        t.tv_usec = 0;
        ret = select(1, &F, NULL, NULL, &t);
        if (ret == -1)
        {
            perror("select()");
            retval = -1;
            exit(EXIT_FAILURE);
        }
        else if (ret)
        {
            scanf("%s", ch);
            for (int i = 0; i < strlen(ch); i++)
                if (ch[i] == 'q')
                    return;
            goto run;
        }
        else
        {
        run:
            if (cmd == 2)
            {
                int fd1 = open("/proc/loadavg", O_RDONLY);
                n = read(fd1, buff, 50);
                close(fd1);
                buff[n - 1] = '\0';
                int p;
                for (int i = 0; i < n; i++)
                    if (buff[i] == ' ')
                        p = i;
                printf("pid %s\n", buff + p);
            }
            else if (cmd == 1)
            {
                size_t len = 0;
                ptr = NULL;
                FILE *fd = fopen("/proc/interrupts", "r");
                if (fd == NULL)
                {
                    perror("");
                    return;
                }
                lno = 0;
                while ((n = getline(&ptr, &len, fd)) != -1)
                {
                    ptr[n - 1] = '\0';
                    lno += 1;
                    if (lno == 1 && ft == 1)
                    {
                        ft = 0;
                        ptr1 = strtok(ptr, "' ''\t'");
                        cnt = 1;
                        printf("%s", ptr1);
                        while ((ptr1 = strtok(NULL, "' ''\t'")) != NULL)
                            printf("\t%s", ptr1), cnt += 1;
                        printf("\n");
                    }
                    ct = cnt;
                    if (lno == 3)
                    {
                        ptr1 = strtok(ptr, "' ''\t'");
                        while ((ptr1 = strtok(NULL, "' ''\t'")) != NULL && ct >= 1)
                            printf("%s\t", ptr1), ct -= 1;
                        printf("\n");
                        break;
                    }
                }
                fclose(fd);
            }
        }
    }
    exit(EXIT_SUCCESS);
}
