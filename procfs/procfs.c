/*
 * a simple kernel module: procfs
 *
 * Copyright (C) 2015 PAN Guolin <guolinp@gmail.com>
 *
 * Licensed under GPLv2 or later.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <asm/uaccess.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

#define BUF_SIZE  128

static char data[BUF_SIZE];
static struct proc_dir_entry *procfs_root;

static int procfs_open(struct inode *inode, struct file *filp)
{
	filp->private_data = data;
	return 0;
}

static int procfs_release(struct inode *inode, struct file *filp)
{
	return 0;
}

static ssize_t procfs_read(struct file *filp, char __user * buf, size_t count,
			   loff_t * ppos)
{
	char *pdata = filp->private_data;
	int end_of_data = strlen(pdata);
	int pos = *ppos;

	if (pos >= end_of_data)
		return 0;

	if (count > end_of_data - pos)
		count = end_of_data - pos;

	if (copy_to_user(buf, pdata + pos, count))
		return -EFAULT;

	*ppos += count;

	return count;
}

static ssize_t procfs_write(struct file *filp, const char __user * buf,
			    size_t count, loff_t * ppos)
{
	char *pdata = filp->private_data;

	if (count >= BUF_SIZE)
		count = BUF_SIZE - 1;

	if (copy_from_user(pdata, buf, count))
		return -EFAULT;

	pdata[count] = '\0';

	return count;
}

static const struct file_operations procfs_fops = {
	.open = procfs_open,
	.read = procfs_read,
	.write = procfs_write,
	.release = procfs_release,
};

static int __init procfs_init(void)
{
	procfs_root = proc_mkdir("procfs_root", NULL);
	if (!procfs_root)
		return -ENODEV;

	/* create /proc/procfs_root/data */
	proc_create("data", 0666, procfs_root, &procfs_fops);

	return 0;
}

module_init(procfs_init);

static void __exit procfs_exit(void)
{
	remove_proc_entry("data", procfs_root);
	remove_proc_entry("procfs_root", NULL);
}

module_exit(procfs_exit);

MODULE_AUTHOR("PAN Guolin <guolinp@gmail.com>");
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("A simple procfs Module");
