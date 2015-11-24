/*
 * a simple kernel module: work_queue
 *
 * Copyright (C) 2015 PAN Guolin <guolinp@gmail.com>
 *
 * Licensed under GPLv2 or later.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/sched.h>
#include <linux/workqueue.h>

static struct work_struct _work;
static struct delayed_work _dwork;

static void dwork_handler(struct work_struct *work)
{
	struct delayed_work *dwork = to_delayed_work(work);

        printk(KERN_INFO "i am in dwork_handler\n");
	schedule_delayed_work(dwork, HZ);
}

static void work_handler(struct work_struct *work)
{
	printk(KERN_INFO "i am in work_handler\n");
}

static int __init workq_init(void)
{
	printk(KERN_INFO "enter work queue module\n");

	INIT_WORK(&_work, work_handler);
	schedule_work(&_work);

	INIT_DELAYED_WORK(&_dwork, dwork_handler);
	schedule_delayed_work(&_dwork, HZ);

	return 0;
}

module_init(workq_init);

static void __exit workq_exit(void)
{
	printk(KERN_INFO "exit work queue module\n");
	cancel_work_sync(&_work);
	cancel_delayed_work_sync(&_dwork);
}

module_exit(workq_exit);

MODULE_AUTHOR("PAN Guolin <guolinp@gmail.com>");
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("A simple work queue Module");
