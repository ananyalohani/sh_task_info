#include <linux/sched.h>
#include <linux/init.h>
#include <uapi/asm-generic/errno-base.h>
#include <linux/syscalls.h>
#include <fcntl.h>
#include <asm/uaccess.h>
#include <fs.h>
#include <linux/kernel.h>

asmlinkage long sys_sh_task_info(pid_t pid, char *filename);
long check_pid(pid_t pid);
long check_file(int fd); 

asmlinkage long sys_sh_task_info(pid_t pid, char *filename)
{
	long res = check_pid(pid);
	if(res != 0) return res;

	int fd = sys_open(filename, O_WRONLY | O_CREAT, 0644);
	res = check_file(fd);
	if(res < 0) return res;

	struct task_struct *task;
	char *str = (char *)malloc(2000);
	
	mm_segment_t prev_fs = get_fs();
	set_fs(KERNEL_DS);

	for_each_process(task)
	{
		if(pid == task->pid)
		{
			sprintf(str, "Process Name: %s\nPID_Number: %ld\nProcess State: %ld\nPriority: %ld\n", task->comm, (long)task->pid, (long)task->state, (long)task->prio);
			res = sys_write(fd, str, strlen(str));
			if(res < 0) return -errno;
			printk("%s",str);
		}
	}
	set_fs(prev_fs);
	free(str);
	return 0;
}

long check_pid(pid_t pid)
{
	if(pid < 1 || pid > 2147483647)
	{
		return -EINVAL;
	}
	return 0;
}

long check_file(int fd)
{
	if(fd < 0)
	{
		return -errno;
	}
	return 0;
}