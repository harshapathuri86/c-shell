CC=gcc
DEPS = headers.h
LFLAGS=-lreadline
OBJ = initialize.o shell.o tokenize.o redirect.o cmds.o ls.o jobs.o run_cmds.o

%.o: %.c $(DEPS)
		$(CC) -c -o $@ $< $(CFLAGS)

shell: $(OBJ)
		gcc -o $@ $^ $(CFLAGS) ${LFLAGS}
