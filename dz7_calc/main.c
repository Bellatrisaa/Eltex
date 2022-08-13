#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

double plus(double x, double y)
{
  return x + y;
}

double minus(double x, double y)
{
  return x - y;
}

double mult(double x, double y)
{
  return x * y;
}

double delen(double x, double y)
{
  return x / y;
}


int main()
{
  while(1){
    double x, y;
    int operation;
    double result;
    bool err = false;

    printf("Enter x: "); scanf("%lf", &x);
    printf("Enter y: "); scanf("%lf", &y);
    printf("Enter operation:\n 1) +\n 2) -\n 3) *\n 4) /\n 0) Exit\n "); scanf("%d", &operation);

    switch (operation) {
      case 1: result = plus(x,y); break;
      case 2: result = minus(x,y); break;
      case 3: result = mult(x,y); break;
      case 4: if(y != 0) result = delen(x,y);
              else  err = true;
              break;
      case 0: return 0;
      default: err= true;
    }
    system("clear");
    if(err) printf("\nErr\n");
    else printf("\nresult = %lf\n", result);
  }
}
