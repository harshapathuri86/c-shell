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
            perror((buf - 2));
            chdir(Pwd);
        }
    }
    else
    {
        if (chdir(buf) < 0)
            perror(buf);
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
        printf("no history list\n");
    return;
}

void env(int argc, char **argv, int a)
{
    if (a == 0)
    {
        //getenv
        if (argc != 2)
        {
            printf("format:getenv <env>\n");
            return;
        }
        printf("%s %s\n", argv[1], getenv(argv[1]));
    }
    else if (a == 1)
    {
        //setenv
        if (argc > 3 || argc < 2)
        {
            printf("format: setenv var [value]\n");
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
            printf("invalid format\n");
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

void kjob(char **argv)
{
    int pd = atoi(argv[1]);
    int signal = atoi(argv[2]);
    if (pd > bg_cnt)
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
        if (pd > bg_cnt)
            printf("No such job\n");
        else
        {
            int i = 1;
            for (i = 0; i < bg_cnt; ++i)
            {
                if (pd == 0)
                    break;
                if (bg_jb[i].status == 1)
                    pd -= 1;
            }
            pd = i;
            pid_t pid = bg_jb[pd].pid;
            kill(pid, SIGTTIN);
            kill(pid, SIGCONT);
        }
    }
}

void print_jobs()
{
    int j = 1;
    char status;
    int p;
    long unsigned mem;
    for (int i = 1; i <= bg_cnt; i++)
    {
        if (bg_jb[i].status == 1)
        {
            char file[100];
            sprintf(file, "/proc/%d", bg_jb[i].pid);
            strcat(file, "/stat");
            if ((fd = fopen(file, "r")) == NULL)
            {
                printf("[%d] %s %s [%d]\n", j, "DONE", bg_jb[i].command, bg_jb[i].pid);
            }
            else
            {
                fscanf(fd, "%d %*s %c %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %lu %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d", &p, &status, &mem);
                fclose(fd);
                printf("[%d] ", j);
                if (status == 'T')
                    printf("%s ", "Stopped");
                else
                    printf("%s ", "Running");
                printf("%s [%d]\n", bg_jb[i].command, bg_jb[i].pid);
            }
            j++;
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
    if (n > bg_cnt)
    {
        printf("No job exists with that number\n");
        return;
    }
    int i = 1;
    for (i = 0; i < bg_cnt; ++i)
    {
        if (n == 0)
            break;
        if (bg_jb[i].status == 1)
            n -= 1;
    }
    n = i;
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
    snprintf(path, 30, "/proc/%s/stat", pid);
    FILE *fd = fopen(path, "r");
    if (!fd)
    {
        printf("%s  No such pid\n", pid);
        return;
    }
    printf("pid -- %s\n", pid);
    fgets(BUF, 1024, fd);
    b[0] = strtok(BUF, " ");
    int n = 1;
    while (((b[n] = strtok(NULL, "' ''\t'")) != NULL) && n < 25)
        n++;
    b[n] = NULL;
    fclose(fd);
    printf("Process Status -- %s\nmemory -- %s\n", b[2], b[22]);
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
        printf("format: nightswatch -n <time> <command> \n");
        return;
    }
    int time = atoi(argv[2]);
    if (strcmp(argv[3], "newborn") == 0)
        cmd = 2;
    else if (strcmp(argv[3], "interrupt") == 0)
        cmd = 1;
    else
    {
        printf("wrong <command>\n");
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

void print_details(char *name)
{
    //type
    lstat(name, &per);
    if ((per.st_mode & S_IFMT) == S_IFBLK)
        printf("b");
    else if ((per.st_mode & S_IFMT) == S_IFLNK)
        printf("l");
    else if ((per.st_mode & S_IFMT) == S_IFCHR)
        printf("c");
    else if ((per.st_mode & S_IFMT) == S_IFDIR)
        printf("d");
    else if ((per.st_mode & S_IFMT) == S_IFIFO)
        printf("f");
    else if ((per.st_mode & S_IFMT) == S_IFREG)
        printf("-");
    else if ((per.st_mode & S_IFMT) == S_IFSOCK)
        printf("s");
    //permissions
    printf("%c", (per.st_mode & S_IRUSR) ? 'r' : '-');
    printf("%c", (per.st_mode & S_IWUSR) ? 'w' : '-');
    printf("%c", (per.st_mode & S_IXUSR) ? 'x' : '-');
    printf("%c", (per.st_mode & S_IRGRP) ? 'r' : '-');
    printf("%c", (per.st_mode & S_IWGRP) ? 'w' : '-');
    printf("%c", (per.st_mode & S_IXGRP) ? 'x' : '-');
    printf("%c", (per.st_mode & S_IROTH) ? 'r' : '-');
    printf("%c", (per.st_mode & S_IWOTH) ? 'w' : '-');
    printf("%c", (per.st_mode & S_IXOTH) ? 'x' : '-');
    printf(" %4ld ", per.st_nlink);
    // uname and grp
    printf("%10s ", getpwuid(per.st_uid)->pw_name);
    printf("%10s ", getgrgid(per.st_gid)->gr_name);
    // size
    printf("%10ld ", per.st_size);
    // last access time
    t = localtime(&per.st_mtime);
    strftime(T, sizeof(T), "%b %e %Y %H:%M", t);
    printf("%s ", T);
    // read symlink
    if ((per.st_mode & S_IFMT) == S_IFLNK)
    {
        if ((cnt = readlink(name, buf, sizeof(buf))) != -1)
        {
            buf[cnt] = '\0';
            printf("%s -> %s\n", d->d_name, buf);
        }
    }
    else
        printf("%s\n", d->d_name);
}

void list_it(char **argv, int n)
{
    fl = fa = cnt = 0;
    for (int i = 1; i < n; i++)
    {
        if (fl == 0 && argv[i] != NULL && !strcmp(argv[i], "-l"))
            fl = 1, argv[i] = NULL, cnt += 1;
        if (fa == 0 && argv[i] != NULL && !strcmp(argv[i], "-a"))
            fa = 1, argv[i] = NULL, cnt += 1;
        if (fl == 0 && fa == 0 && argv[i] != NULL && !(strcmp(argv[i], "-la") && strcmp(argv[i], "-al")))
            fa = fl = 1, argv[i] = NULL, cnt += 1;
    }
    if (cnt == (n - 1))
    {
        D = opendir(".");
        if (D == NULL)
        {
            perror("");
            return;
        }
        while ((d = readdir(D)) != NULL)
        {
            name[0] = '\0';
            sprintf(name, "%s/%s", ".", d->d_name);
            if (strncmp(d->d_name, ".", 1) != 0 || fa)
            {
                if (fl == 1)
                    print_details(name);
                else
                    printf("%s\n", d->d_name);
            }
        }
    }
    for (int i = 1; i < n; i++)
    {
        if (argv[i] == NULL)
            continue;
        if (strncmp(argv[i], "~", 1) == 0)
        {
            abs_dir[0] = '\0';
            sprintf(abs_dir, "%s%s", home, argv[i] + 1);
            argv[i] = abs_dir;
        }
        argv[i] = realpath(argv[i], NULL);
        if (cnt != (n - 2))
        {
            printf("%s:\n", argv[i]);
        }
        D = opendir(argv[i]);
        if (D == NULL)
        {
            perror(argv[i]);
            return;
        }
        while ((d = readdir(D)) != NULL)
        {
            name[0] = '\0';
            sprintf(name, "%s/%s", argv[i], d->d_name);
            if (strncmp(d->d_name, ".", 1) != 0 || fa)
            {
                if (fl == 1)
                    print_details(name);
                else
                    printf("%s\n", d->d_name);
            }
        }
        closedir(D);
        free(argv[i]);
        printf("\n");
    }
    return;
}
