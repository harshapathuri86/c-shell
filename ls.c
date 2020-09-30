#include "headers.h"

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
