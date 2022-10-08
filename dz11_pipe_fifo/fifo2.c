#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(void)
{
  int fd = open("myfifo", O_RDONLY);
  char m;

  while (read(fd, &m, 1) > 0)
        printf("%c", m);
  printf("\n");
  close(fd);
  return 0;
}
