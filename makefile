CC=gcc
DEPS = headers.h
LFLAGS=-lreadline
OBJ = initialize.o shell.o tokenize.o cmds.o run_cmds.o prompt.o

%.o: %.c $(DEPS)
		$(CC) -c -o $@ $< $(CFLAGS)

shell: $(OBJ)
		gcc -o $@ $^ $(CFLAGS) ${LFLAGS}
