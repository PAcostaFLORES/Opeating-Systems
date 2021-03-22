#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<wait.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/types.h>

void regionCritica(int PID)
{
   int i;
   printf("\nProceso en la region critica proceso con pid=%d\n",PID);
   for(i=0;i<3;i++)
   {
      printf("\nRegión critica: %d\n",i);
      sleep(1);	
   }
}

void regionNoCriticaProcesoHijo(int PID)
{
   int i;
   printf("\nHijo en la region NO critica proceso con pid=%d\n",PID);
   for(i=0;i<20;i++)
   {
      printf("\nHijo: %d\n",i);
      sleep(1);
   }
}

void regionNoCriticaProcesoPadre(int PID)
{
   int i;
   printf("\nPadre en la region NO critica proceso con pid=%d\n",PID);
   for(i=0;i<3;i++)
   {
      printf("\nPadre: %d\n",i);
      sleep(1);
   }
}

int main()
{
   pid_t pid;
   int PID;
   int shmid1;
   int shmid2;
   int shmid3;
   int i;
   int *Hijo_desea_entrar;
   int *Padre_desea_entrar;
   int *Proceso_favorecido;
   key_t llave1;
   key_t llave2;
   key_t llave3;
   llave1=ftok("Prueba1",'k');
   llave2=ftok("Prueba2",'l');
   llave3=ftok("Prueba3",'m');
   shmid1=shmget(llave1,sizeof(int),IPC_CREAT|0600);
   shmid2=shmget(llave2,sizeof(int),IPC_CREAT|0600);
   shmid3=shmget(llave3,sizeof(int),IPC_CREAT|0600);
   Hijo_desea_entrar=shmat(shmid1,0,0);
   Padre_desea_entrar=shmat(shmid2,0,0);
   Proceso_favorecido=shmat(shmid3,0,0);
   *Hijo_desea_entrar=1;
   *Padre_desea_entrar=0;
   *Proceso_favorecido=1;
   pid= fork();
   if(pid==-1)
   {
      perror("\nError al crear el proceso\n");
      exit(-1);
   }
   if(pid==0)
   {
      while (1)
      {
         *Hijo_desea_entrar=1;
         while(*Padre_desea_entrar)
         {
            if(*Proceso_favorecido==2)
            {
               *Hijo_desea_entrar=0;
               while(*Proceso_favorecido==2);
               *Hijo_desea_entrar=1;
            }
         }
         PID=getpid();
         regionCritica(PID);
         *Proceso_favorecido=2;
         *Hijo_desea_entrar=0;
         regionNoCriticaProcesoHijo(PID);
      }
   }
   else
   {
      while(1)
      {
         *Padre_desea_entrar=1;
         while(*Hijo_desea_entrar)
         {
            if(*Proceso_favorecido==1)
            {
               *Padre_desea_entrar=0;
               while(*Proceso_favorecido==1);
               *Padre_desea_entrar=1;
            }
         }
         PID=getpid();
         regionCritica(PID);
         *Proceso_favorecido=1;
         *Padre_desea_entrar=0;
         regionNoCriticaProcesoPadre(PID);
      }
   }
   return 0;
}
