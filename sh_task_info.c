/* 
   Name: Ananya Lohani
   Roll_Number: 2019018 
*/

#include <linux/list.h>

SYSCALL_DEFINE2(sh_task_info, long, pid, char *, filename)
{
	// check pid range
	if(pid < 1 || pid > 2147483647)
	{
		printk(KERN_ALERT "Invalid value of PID.\n");
		return -EINVAL;			// invalid argument
	}

	const int PATH_SIZE = 512;
	const int BUF_SIZE = 4096;
	char path[PATH_SIZE], buf[BUF_SIZE];
	int i = 0;

	// initialize char arrays
	for(; i < PATH_SIZE; i++)
	{
		path[i] = 0;
		buf[i] = 0;
	}
	for(; i < BUF_SIZE; i++)
	{
		buf[i] = 0;
	}

	// copy filename from userspace to kernel space
	long copied = strncpy_from_user(path, filename, PATH_SIZE);
	if(copied < 0)
	{
		printk(KERN_ALERT "Error in copying data from userspace.\n");
		return -1;
	}

	struct task_struct *task = pid_task(find_vpid(pid), PIDTYPE_PID);	// get task corresponding to pid
	if(task == NULL)
	{
		printk(KERN_ALERT "No such task with PID = %ld\n.", pid);
		return -ESRCH;			// no such process
	}

	printk(KERN_INFO "Syscall loaded.\n");

	struct file *f = filp_open(path, O_WRONLY|O_CREAT, 0);		// open the file
	if(f == NULL)
	{
		printk(KERN_ALERT "Error in opening the file.\n");
		return -EIO;			// I/O error
	}

	// copy task_struct details to buf
	int len = 0;
	len = snprintf(buf, BUF_SIZE, "Process: %s\nPID: %ld\nProcess State: %ld\nPriority: %ld\nParent Process PID: %ld\n", task->comm, (long)task->pid, (long)task->state, (long)task->prio, (long)task->parent->pid);
	if(len < 0)
	{
		printk(KERN_ALERT "Error in snprintf().\n");
		filp_close(f, NULL);
		return -EIO;			// I/O error
	}

	struct list_head *list;
	i = 1;
	// iterate over the children of the process
	list_for_each(list, &((*task).children))
	{
		struct task_struct *child = list_entry(list, struct task_struct, sibling);
		len += snprintf(buf + strlen(buf), BUF_SIZE, "Child Process %d PID: %ld\n", i, (long)child->pid);
		i++;
	}

	printk(KERN_INFO "%s",buf);								// print buf to kernel logs
	int res = kernel_write(f, buf, len, &f->f_pos);			// write buf to file
	if(res < 0)
	{
		printk(KERN_ALERT "Error in writing to the file.\n");
		filp_close(f, NULL);
		return -EIO;			// I/O error
	}
	filp_close(f, NULL);		 // close the file

  	return 0;
}