#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <time.h>

void * ShowThreadID(void* thread_data)
{
    pid_t tid = syscall(SYS_gettid);
    const char* msg = (const char*)thread_data;
    printf("%s: %d\n", msg, tid);
    pthread_exit(0);
}

void * IncrementInt(void* data)
{
  int increment = 0;
  int* N = (int*)data;
  while (increment < *N) increment++;
  pthread_exit(0);
}

int main()
{
  pthread_t thread1 = pthread_create(&thread1, NULL, ShowThreadID, "Thread №1 ID = ");
  pthread_t thread2 = pthread_create(&thread2, NULL, ShowThreadID, "Thread №2 ID = ");
  pthread_t thread3 = pthread_create(&thread3, NULL, ShowThreadID, "Thread №3 ID = ");

  pthread_join(thread3, NULL);
  pthread_join(thread2, NULL);
  pthread_join(thread1, NULL);


  sleep(2);

  int d = 1000000;
  int * data = &d;

  clock_t begin = clock();

  pthread_t thread4 = pthread_create(&thread4, NULL, IncrementInt, &data);
  pthread_t thread5 = pthread_create(&thread5, NULL, IncrementInt, &data);
  pthread_t thread6 = pthread_create(&thread6, NULL, IncrementInt, &data);
  pthread_t thread7 = pthread_create(&thread7, NULL, IncrementInt, &data);
  pthread_t thread8 = pthread_create(&thread8, NULL, IncrementInt, &data);

  pthread_join(thread4, NULL);
  pthread_join(thread5, NULL);
  pthread_join(thread6, NULL);
  pthread_join(thread7, NULL);
  pthread_join(thread8, NULL);

  clock_t end = clock();

  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

  printf("5 Threads time is %f seconds \n", time_spent);

  begin = clock();

  for(int i = 0; i < 5; i++)
  for(int j = 0; j < 1000000; ) j++;

  end = clock();

  time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

  printf("NO Threads time is %f seconds \n", time_spent);

  return 0;
}
