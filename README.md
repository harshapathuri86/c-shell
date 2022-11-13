# C-Shell

---

A Shell written in C. This is a project for my Operating Systems class. The goal is to create a user-defined interactive shell program that can create and manage new processes. It will also be able to handle input/output redirection and piping, and will be able to run in the background.

It has the following features:

- Shell Prompt
   `user@host:~/dir$` contains the username, hostname, and the invoking directory as the home directory.
   Supports full path names that are outside of the home directory.

- Built-in Commands (without using `execvp`)
  - `cd <dir>`: Changes the current directory to `<dir>`
    - `cd` changes to the home directory
    - `cd -` changes to the previous directory

  - `pwd` prints the current directory
  - `echo` prints the arguments
  - `ls [-al] <dir>` lists the contents of the directory
        - argument `dir` prints the contents of the directory
        - `-l` long listing format with file size, permissions, owner, group, date modified, file name and symbolic link
        - `-a` shows hidden files

- External Commands (using `execvp`) with background execution ( `&` ) and show when a background process is finished similar to bash.

- Input/Output Redirection
  - `>` redirects the output of a command to a file
  - `<` redirects the input of a command from a file
  - `>>` appends the output of a command to a file
  - Shows an error if the file does not exist

- Piping
  - `|` pipes the output of one command to the input of another command
  - Supports multiple pipes
  - Supports redirection with pipes

- User defined commands
  - `pinfo` prints the process id, process status, memory, and executable path of the shell or the process id given as an argument

  - `nightswatch [-n] <command>` Modified version of `watch` that prints the output of the command every `n` seconds. `n` is 2 seconds by default.
    - `-n` specifies the number of seconds to wait between updates
    - `command` can be `interrupt` or `newborn`
      - `interrupt` prints the number of interrupts CPU(s) have been interrupted by the keyboard since boot
      - `newborn` prints the PID of the most recently created process

  - `history <n>` prints the last 10 or `n` commands entered by the user

  - `setenv <var> <value>` sets the environment variable `var` to `value`

  - `unsetenv <var>` unsets the environment variable `var`

  - `jobs` lists all the background processes spawned by the shell and their status (running or stopped) in the order they were spawned

  - `kjob <job number> <signal number>` sends the signal `signal number` to the process with the job number `job number`

  - `fg <job number>` brings the process with the job number `job number` to the foreground

  - `bg <job number>` brings the process with the job number `job number` to the background

  - `overkill` kills all the background processes spawned by the shell

  - `quit` exits the shell

- Signal Handling

  - `Ctrl+Z` stops the foreground process  
  - `Ctrl+C` kills the foreground process
    
- Exit Codes in the prompt
