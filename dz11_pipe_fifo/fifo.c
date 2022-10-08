#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>


int main(void)
{
  if(mkfifo("myfifo", S_IROTH | S_IWOTH) == -1){
    fprintf(stderr, "Failed to create fifo\n");
    exit(EXIT_FAILURE);
  }

  char *msg  = "Good afternoon, fifo!";
  int fd = open("myfifo", O_WRONLY);
  write(fd, msg, strlen(msg));
  close(fd);
  return 0;
}
