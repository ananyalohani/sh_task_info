/* 
   Name: Ananya Lohani
   Roll_Number: 2019018 
*/
#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <stdlib.h>

#define SH_TASK_INFO 440

int main(int argc, char **argv)
{
   long res = syscall(SH_TASK_INFO, 1, "task_details.txt");
   if(res != 0)
   {
      perror("Error");
   }
   else 
   {
       printf("System call executed succesfully.\n");
   }
   return 0;
}