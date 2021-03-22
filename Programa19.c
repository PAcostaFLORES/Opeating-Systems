#include<sched.h>
#include<pthread.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>

typedef struct 
{
   int id;
   pthread_t hilo;
   pthread_attr_t atributos;
}hilo_t;

#define NUM_HILOS 4

void *Codigo_Hilo(void *arg)
{
   hilo_t *h=(hilo_t *)arg;
   int politica;
   struct sched_param param;
   int i;
   int error;
   for(i=0;;i++)
   {
      pthread_testcancel();
      if(i%1000000==0)
      {
         error=pthread_getschedparam(h->hilo,&politica,&param);
         if(error)
         {
            perror("Error con pthread_getschedparam");
         }
      else
         {
            printf("\n%d: %s, %d\n",h->id,politica==SCHED_FIFO?"FIFO":politica==SCHED_RR?"RR":"OTROS",param.sched_priority);
         }
      sched_yield();
      }
   }
   pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
   hilo_t hilos[NUM_HILOS];
   struct sched_param param;
   int prioridad_minima;
   int politica;
   int error;
   int i;
   if(argc!=2)
   {
      fprintf(stderr,"\nForma de uso: %s 0|1\n",argv[0]);
      exit(-1);
   }
   else
   {
      politica=atoi(argv[1]);
   }
   prioridad_minima=sched_get_priority_min(SCHED_FIFO);
   if(prioridad_minima==-1)
   {
      perror("\nError en sched_get_priority_min\n");
      exit(-1);
   }
   for(i=0;i<NUM_HILOS;i++)
   {
      hilos[i].id=i+1;
      error=pthread_attr_init(&hilos[i].atributos);
      if(error)
      {
         perror("\nError en pthread_attr_init\n");
         exit(-1);
      }
      error=pthread_attr_setscope(&hilos[i].atributos,PTHREAD_SCOPE_SYSTEM);
      if(error)
      {
         perror("\nError en pthread_attr_setscope\n");
         exit(-1);
      }
      error=pthread_attr_setschedpolicy(&hilos[i].atributos,politica?SCHED_FIFO:SCHED_RR);
      if(error)
      {
         perror("\nError en pthread_attr_setschedpolicy\n");
         exit(-1);
      }
      param.sched_priority=prioridad_minima+i*politica;
      error=pthread_attr_setschedparam(&hilos[i].atributos,&param);
      if(error)
      {
         perror("\nError en pthread_attr_setschedparam\n");
         exit(-1);
      }
      error=pthread_attr_setinheritsched(&hilos[i].atributos,PTHREAD_INHERIT_SCHED);
      if(error)
      {
         perror("\nError en pthread_attr_setinheritsched\n");
         exit(-1);
      }
   }
   for(i=0;i<NUM_HILOS;i++)
   {
      error=pthread_create(&hilos[i].hilo,&hilos[i].atributos,Codigo_Hilo,(void *)&hilos[i]);
      if(error)
      {
         perror("\nError en pthread_create\n");
         exit(-1);
      }
   }
   for(i=NUM_HILOS-1;i>=0;i--)
   {
      sleep(2);
      error=pthread_cancel(hilos[i].hilo);
      if(error)
      {
         perror("\nError en pthread_cancel\n");
         exit(-1);
      }
      printf("\nHilo numero: %d terminado\n",hilos[i].id);
      error=pthread_join(hilos[i].hilo,NULL);
      if(error)
      {
         perror("\nError en pthread_join\n");
         exit(-1);
      }
   }
   printf("\nFinalizando el hilo principal\n");
   pthread_exit(NULL);
}
