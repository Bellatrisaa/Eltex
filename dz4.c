#include <stdio.h>
#include <stdlib.h>
#include "string.h"

struct subscriber
{
  char name [20];
  int age;
  char gender;
  char number[11];
};

void print (struct subscriber A[],int N)
{
  for (int i = 0; i < N; i++)
    if(strcmp(A[i].name, "NULL")){
    printf("%d)   %s   %d   %c   ", i, A[i].name, A[i].age, A[i].gender);

    for (int j = 0; j < 11; j++)
      printf("%c", A[i].number[j]);

    printf("\n");
  }
}

void sort1 (struct subscriber A[],int N)
{
struct subscriber temp;
for (int i = 0; i < N; i++)
	for (int j = 0; j < N - 1 - i; j++)
	 if (A[j].age > A[j+1].age){
     temp = A[j];
     A[j] = A[j+1];
     A[j+1] = temp;
   }
print (A,N);
}

void sort2 (struct subscriber A[],int N)
{
  struct subscriber temp;
  for (int i = 0; i < N; i++)
  	for (int j = 0; j < N - 1 - i; j++)
  	 if (strcmp(A[j].name, A[j+1].name) > 0){
       temp = A[j];
       A[j] = A[j+1];
       A[j+1] = temp;
     }
  print (A,N);
}

void DeleteSubscriber(struct subscriber A[], int N)
{
  int i;
  printf(" Enter the index of the subscriber to be deleted\n");
  scanf("%d ", &i);
  if(i < N && i > 0) strcpy (A[i].name , "NULL");
}

void AddSubscriber(struct subscriber *newSub, char name [20], int age, char gender , char number[11])
{
  strcpy (newSub->name , name);
  newSub->age = age;
  newSub->gender = gender;
  strcpy(newSub->number, number);
}

void CreateSubscriber(struct subscriber directory[], int N)
{
  int index = N;
  for(int i = 0; i < N; i++)
    if(!(strcmp(directory[i].name, "NULL"))){ index = i; break; }

  if(index == N) printf(" Directory is already filled\n");
  else{
    char scr [20];
    char name [20];
    int age;
    char gender;
    char number[11];
    printf(" Enter\n name:"); scanf("%s", name);
    getc(stdin);
    printf(" age:"); scanf("%d", &age);
    setbuf(stdin, NULL);
    printf(" gender:"); scanf("%c", &gender);
    fgets(scr, 10, stdin);
    printf(" number:"); fgets(number, 11, stdin);
    setbuf(stdin, NULL);
    AddSubscriber(&directory[index], name, age, gender, number);
  }
}

void SetStartDirectory(struct subscriber directory[], int N)
{
  AddSubscriber(&directory[0], "Nika", 19, 'W', "89132281337");
  AddSubscriber(&directory[1], "Serega", 21, 'M', "88005553535");
  AddSubscriber(&directory[2], "Stas", 34, 'M', "81232233221");
  AddSubscriber(&directory[3], "Tanya", 28, 'W', "84445552626");
  AddSubscriber(&directory[4], "Nastya", 28, 'W', "86969696969");
  for(int i = 5; i < 10; i++)
    strcpy (directory[i].name , "NULL");
}

int main()
{
  int N = 10;
  int c;
  struct subscriber directory[10];
  SetStartDirectory(directory,N);

  while (true){
    printf(" MENU\n 1 - Show directory\n 2 - Sort by age\n 3 - Sort by name\n 4 - Delete subscriber\n 5 - Add a subscriber\n 0 - Exit\n");
    scanf("%d", &c);
    setbuf(stdin, NULL);
    system("clear");
    switch (c) {
      case 1: print(directory, N); break;
      case 2: sort1(directory, N); break;
      case 3: sort2(directory, N); break;
      case 4: DeleteSubscriber(directory, N); break;
      case 5: CreateSubscriber(directory, N); break;
      case 0: return 0;
    }
  }
  return 0;
}
