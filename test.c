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
   long res = syscall(SH_TASK_INFO, 1000, "task_details.txt");
   system("dmesg | tail");
   printf("System call returned %ld.\n", res);
   return 0;
}