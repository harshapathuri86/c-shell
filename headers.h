#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"
#include "string.h"
#include "readline/readline.h"
#include "readline/history.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "sys/wait.h"
#include "signal.h"
#include "fcntl.h"
#include "dirent.h"
#include "pwd.h"
#include "grp.h"
#include "time.h"
#include "fcntl.h"
#include "sys/time.h"
#include "ctype.h"

void echo_it(char **argv);
void ch_dir(char *buf);
void list_it(char **argv, int n);
void pinfo(char *pid);
void print_hist(int argc, char **argv);
void nights_watch(int argc, char **argv);
void env(int argc, char **args, int a);
void print_jobs();
void fg(int argc, char **argv);
void overkill();
void kjob(char **argv);
void bg(char **argv, int argc);
void prompt();
void nights_watch(int argc, char **argv);
void tokenize_input(char *buf);
void initialize();
void built_in(char *cmd);
char *white_space(char *buf);

typedef struct job
{
    char command[200];
    pid_t pid;
    int status;
} job;

job bg_jb[200], fg_jb;
FILE *fd;
struct dirent *d;
struct tm *t;
struct stat per;
struct passwd pswd;
time_t cur_time;
DIR *D;

pid_t fgpid, mypid, mypgid;
pid_t child, grandchild;

int shell, shellpgid, len, background, SIN, SOUT, terminal, bg_cnt, st, info, status, N, argc, ret, input_fd, output_fd, inp, apn, ovr, fl, fa, cnt, l;
long unsigned int n;

char *lol, *home, *Pwd, *ptr, *Dup, *end, *rel, *dir, *arg;
char pd[1024], *var[3], *argv[1024], pwd[1024], *cmds[1024], *token[1024], buf1[1024], buf2[1024], PROMPT[1024], buf[1024], T[1024], name[1024], abs_dir[1024], path[100], *b[100], BUF[1024], PROMPT[1024];
