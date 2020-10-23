/* 
   Name: Ananya Lohani
   Roll_Number: 2019018 
*/
#include <stdio.h>
#include <unistd.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <errno.h>

int main(int argc, char *argv[])
{
   pid_t pid = (pid_t)atoi(argv[1]);
   long res = syscall(548, pid, argv[2]);
   if(res == 0)
   {
      printf("System call successful.");
   }
   else 
   {
      perror("Error: ");
      fprintf(stderr, "Error number: %d\n", errno);
   }
   return 0;
}