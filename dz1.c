#include <stdio.h>

int GetByteFrom4ByteNumber(int Num, int ByteN, int *result)
{    
    switch(ByteN){
    case 1: *result = Num & 0xFF; break;
    case 2: *result = (Num >> 8) & 0xFF; break;
    case 3: *result = (Num >> 16) & 0xFF; break;
    case 4: *result = (Num >> 24) & 0xFF; break;
    default: return 1; 
    }
    return 0;
}

int ChangeByteFrom4ByteNumber(int Num, char value ,int ByteN, int *result)
{    
    if(ByteN < 1 || ByteN > 4) return 1;
     Num = Num & ~(0x000000FF << (8 * (ByteN - 1)));
    *result = 0xFF & value;
    *result = (*result << (8 * (ByteN - 1))) | Num;
    return 0;
}

int main()
{
    int SomeNumber = 0xDDCCBBAA;
    int Byte;

    printf("Task 1: \n");

    for(int i = 1; i <= 4; i++)
        if(GetByteFrom4ByteNumber(SomeNumber,i,&Byte)) printf("ERROR\n");
        else printf("%x\n", Byte);

    printf("Task 2: \n");

        if(ChangeByteFrom4ByteNumber(SomeNumber,0xFF,3,&Byte)) printf("ERROR\n");
        else printf("%x\n", Byte);

    return 0;
}
