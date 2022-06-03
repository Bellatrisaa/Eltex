#include <stdio.h>

void ShowBytes(int Number)
{
    const unsigned char *Bytes = (const unsigned char *)(&Number);
    for (int i = 0; i < sizeof(Number); i++)
        printf("%x\n", Bytes[i]);
}

void ChangeByte(int Number, int ByteNumber)
{
    unsigned char *Bytes = (unsigned char *)(&Number);
    Bytes[ByteNumber] = 0xEE;
    for (int i = 0; i < sizeof(Number); i++)
        printf("%x", Bytes[i]);
    printf("\n");
}

int main(void)
{
    int Number = 0xAABBCCDD;
    ShowBytes(Number);
    ChangeByte(Number, 2);
    return 0;
}
