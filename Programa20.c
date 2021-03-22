#include<pthread.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>

int P1=8;
int P2=7;
int P3=4;
int P4=3;
int P5=4;

void *Codigo_Hilo1(void *arg)
{
   int i;
   for(i=0;i<P1;i++)
   {
      printf("\nHilo: 1, ejecutando\n");
      sleep(1);
   }
   pthread_exit(NULL);
}

void *Codigo_Hilo2(void *arg)
{
   int i;
   for(i=0;i<P2;i++)
   {
      printf("\nHilo: 2, ejecutando\n");
      sleep(1);
   }
   pthread_exit(NULL);
}

void *Codigo_Hilo3(void *arg)
{
   int i;
   for(i=0;i<P3;i++)
   {
      printf("\nHilo: 3, ejecutando\n");
      sleep(1);
   }
   pthread_exit(NULL);
}

void *Codigo_Hilo4(void *arg)
{
   int i;
   for(i=0;i<P4;i++)
   {
      printf("\nHilo: 4, ejecutando\n");
      sleep(1);
   }
   pthread_exit(NULL);
}

void *Codigo_Hilo5(void *arg)
{
   int i;
   for(i=0;i<P5;i++)
   {
      printf("\nHilo: 5, ejecutando\n");
      sleep(1);
   }
   pthread_exit(NULL);
}

int main()
{
   pthread_t hilo1;
   pthread_t hilo2;
   pthread_t hilo3;
   pthread_t hilo4;
   pthread_t hilo5;
   pthread_create(&hilo1,NULL,Codigo_Hilo1,NULL);
   pthread_join(hilo1,NULL);
   printf("\nHilo numero 1 terminado\n");
   pthread_create(&hilo2,NULL,Codigo_Hilo2,NULL);
   pthread_join(hilo2,NULL);
   printf("\nHilo numero 2 terminado\n");
   pthread_create(&hilo3,NULL,Codigo_Hilo3,NULL);
   pthread_join(hilo3,NULL);
   printf("\nHilo numero 3 terminado\n");
   pthread_create(&hilo4,NULL,Codigo_Hilo4,NULL);
   pthread_join(hilo4,NULL);
   printf("\nHilo numero 4 terminado\n");
   pthread_create(&hilo5,NULL,Codigo_Hilo5,NULL);
   pthread_join(hilo5,NULL);

   printf("\nHilo numero 5 terminado\n");
   printf("\nFinalizando el hilo principal\n");
   pthread_exit(NULL);
}
