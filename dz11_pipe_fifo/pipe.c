#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
  int fd[2];
  char c;
  if(pipe(fd)){
    fprintf(stderr, "Failed to create pipe\n");
    exit(EXIT_FAILURE);
  }
  if( fork() > 0) {
      char *msg  = "Good afternoon, pipe!";
      close(fd[0]);

      write(fd[1],msg,strlen(msg));

      close(fd[1]);
      exit(EXIT_SUCCESS);
  }
  else {
      char m;
      close(fd[1]);

      while (read(fd[0], &m, 1) > 0)
            printf("%c", m);
      printf("\n");

      close(fd[0]);
      exit(EXIT_SUCCESS);
  }
  return 0;
}
