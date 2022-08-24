#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <dlfcn.h>

int main()
{
  double (*fun)(double, double);
  void *mylib = NULL;
  mylib = dlopen("./libcalc.so",RTLD_NOW);

  if(mylib == NULL){
    printf("lib is NULL\n");
    dlerror();
    return 1;
  }

  while(1){
    double x, y;
    int operation;
    double result;
    bool err = false;

    printf("Enter x: "); scanf("%lf", &x);
    printf("Enter y: "); scanf("%lf", &y);
    printf("Enter operation:\n 1) +\n 2) -\n 3) *\n 4) /\n 0) Exit\n "); scanf("%d", &operation);

    switch (operation) {
      case 1:
        fun = (double(*)(double, double))dlsym(mylib, "plus");
        result = fun(x,y);
        break;
      case 2:
        fun = (double(*)(double, double))dlsym(mylib, "minus");
        result = fun(x,y);
        break;
      case 3:
        fun = (double(*)(double, double))dlsym(mylib, "mult");
        result = fun(x,y);
        break;
      case 4:
        if(y != 0){
          fun = (double(*)(double, double))dlsym(mylib, "delen");
          result = fun(x,y);
          break;
        }
        else  err = true;  break;
      case 0: dlclose(mylib); return 0;
      default: err= true;
    }
    system("clear");
    if(err) printf("\nErr\n");
    else printf("\nresult = %lf\n", result);
  }
  dlclose(mylib);
  return 1;
}
