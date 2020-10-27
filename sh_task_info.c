/* 
   Name: Ananya Lohani
   Roll_Number: 2019018 
*/

SYSCALL_DEFINE2(sh_task_info, long, pid, char *, filename)
{
	// check pid range
	if(pid < 1 || pid > 2147483647)
	{
		printk(KERN_ALERT "Invalid value of PID.\n");
		return -EINVAL;
	}

	struct file *f;
	const int BUF_SIZE = 400;
	char path[BUF_SIZE], buf[2000];
	int i = 0;
	int res;

	// initialize char arrays
	for(; i < BUF_SIZE; i++)
	{
		path[i] = 0;
		buf[i] = 0;
	}
	for(; i < 2000; i++)
	{
		buf[i] = 0;
	}

	long copied = strncpy_from_user(path, filename, sizeof(path));		// copy data from userspace to kernel space
	struct task_struct *task = pid_task(find_vpid(pid), PIDTYPE_PID);	// get task corresponding to pid

	printk(KERN_INFO "Syscall loaded.\n");
	f = filp_open(path, O_WRONLY|O_CREAT, 0);		// open the file
	if(f == NULL)
	{
		printk(KERN_ALERT "Error in opening the file.\n");
		return -1;
	}
	if(copied < 0)
	{
		printk(KERN_ALERT "Error in copying data from userspace.\n");
		return -1;
	}

	// copy task_struct details to buf
	int len = 0;
	len = snprintf(buf, 2000, "Process: %s\nPID: %ld\nProcess State: %ld\nPriority: %ld\nParent Process PID: %ld\n", task->comm, (long)task->pid, (long)task->state, (long)task->prio, (long)task->parent->pid);

	if(copied < 0)
	{
		printk(KERN_ALERT "Error in sprintf().\n");
		return -1;
	}

	struct task_struct *child;
	struct list_head *list;
	i = 1;
	list_for_each(list, &((*task).children))
	{
		child = list_entry(list, struct task_struct, sibling);
		len += snprintf(buf + strlen(buf), 2000, "Child Process %d PID: %ld\n", i, (long)child->pid);
		i++;
	}

	printk(KERN_INFO "%s",buf);							// print buf to kernel logs
	res = kernel_write(f, buf, len, &f->f_pos);			// write buf to file
	if(res < 0)
	{
		printk(KERN_ALERT "Error in writing to the file.\n");
		return -1;	
	}
	filp_close(f, NULL);			 // close the file

  	return 0;
}