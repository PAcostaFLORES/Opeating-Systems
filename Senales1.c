#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<signal.h>

void trampa(int senal)
{
   signal(senal,trampa);
   printf("\nSenal recibida: %d\n",senal);
}

int main()
{
   int i;
   for(i=1;i<=64;i++)
   {
      signal(i,trampa);
   }
   printf("\nProceso: %d\n",getpid());
   pause();
   printf("\nContinuando...\n");
   return 0;
}
