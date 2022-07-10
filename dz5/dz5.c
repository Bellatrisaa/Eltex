#include <stdio.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#define FILENAME "file"

int main(void)
{
    int file;

    if ((file = open(FILENAME, O_CREAT | O_RDWR)) < 0) {
        perror("Failed to creat file");
        return -1;
    }

    const char *str = "Text for file";

    if ((write(file, str, strlen(str))) < 0) {
        perror("Failed to write file");
        return -1;
    }

    printf("Written: %s\n", str);

    char str_buf[16];

    memset(str_buf, 0, sizeof(str_buf));

    if (lseek(file, 0, SEEK_SET) == -1L || (read(file, str_buf, sizeof(str_buf))) < 0) {
        perror("Failed to read file");
        return -1;
    }

    printf("Read: %s\n", str_buf);

    if (close(file) != 0) {
        perror("Failed to close file");
        return -1;
    }

    return 0;
}
