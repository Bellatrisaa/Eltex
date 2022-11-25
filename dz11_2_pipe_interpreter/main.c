#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <linux/limits.h>

#define MAX_ARGV_SIZE 64

#define exit_failure(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0) \

int main(void)
{
    pid_t pid;
    int fd = 1, status, pipes[2][2], k = 0, c = 0;
    bool ispipe = false;
    char  *line = NULL;
    size_t linelen = 0;

    char *argv[MAX_ARGV_SIZE],
          argv_buffer[MAX_ARGV_SIZE][PATH_MAX];

    while (1) {
	bool errflag = false;

        if (ispipe) printf(">> ");
        else {
	    printf("> ");
	    memset((void *)argv, 0, sizeof argv);
	    c = 0, k = 0;
	}

        int n = getline(&line, &linelen, stdin);

        if (n <= 1) continue;
        if (!strncmp("exit", line, n - 1)) break;

        for (int i = 0; i < n - 1; i++) {
	    if(errflag) break;
            switch (line[i]) {
            case '|' :
                if (!argv[c] && (!c || !argv[c - 1])) {
                    fprintf(stderr, "syntax error\n");
                    errflag = true;
		    continue;
                }

                if (!isspace(line[i - 1])) argv[c++][k] = '\0';
 
                argv[c++] = NULL;
                ispipe = true;
                k = 0;
                break;

            case '\t' :
            case '\n' :
            case '\r' :
            case ' ' :
                if (k) {
                    argv[c++][k] = '\0';
                    k = 0;
                }
                break;

            default :
                if (!argv[c]) argv[c] = argv_buffer[c];
                ispipe = false;
                argv[c][k++] = line[i];
            }
        }

	if(errflag || ispipe) continue;
        if (argv[c]) argv[c++][k] = '\0';

        for (int i = 0, k = 0; i <= c; i++) {
            if (argv[i]) continue;
            if (i != c && pipe(pipes[fd]) == -1)
		exit_failure("pipe");
          
            int ifd = (!k) ? STDIN_FILENO : pipes[!fd][0];
            int ofd = (i == c) ? STDOUT_FILENO : pipes[fd][1];
            
            if(!(pid = fork())) {
                if (ifd != STDIN_FILENO) {
                    if (dup2(ifd, STDIN_FILENO) == -1) 
			    exit_failure("child: dup2: stdin");
                    close(ifd);
                }
                if (ofd != STDOUT_FILENO) {
                    if (dup2(ofd, STDOUT_FILENO) == -1)
                        exit_failure("child: dup2: stdout");
                    close(ofd);
                }
                if (execvp(argv[k], argv + k) == -1)
		    exit_failure("child: execvp");
	    }
	    else{
                if (ofd != STDOUT_FILENO) close(ofd);
                if (ifd != STDIN_FILENO) close(ifd);
               
                waitpid(pid, &status, 0);
                if (!WIFEXITED(status)) exit(0);
	    }
	    k = i + 1;
            fd = !fd;
        }
    }
    free(line);
    exit(EXIT_SUCCESS);
}
