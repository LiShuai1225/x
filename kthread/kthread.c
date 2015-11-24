/*
 * a simple kernel module: kthd
 *
 * Copyright (C) 2015 PAN Guolin <guolinp@gmail.com>
 *
 * Licensed under GPLv2 or later.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/err.h>
#include <linux/device.h>
#include <linux/kthread.h>

static int wake = 0;
static struct device *kthd_dev;
static struct class *kthd_class;
static struct task_struct *thread;

static ssize_t kthd_store(struct device *dev, struct device_attribute *attr,
			const char *buf, size_t count)
{
	if (1 != sscanf(buf, "%d", &wake))
		return -EINVAL;

	printk(KERN_INFO "%s: before calling wake_up_process\n", __func__);
	wake_up_process(thread);
	printk(KERN_INFO "%s: after calling wake_up_process\n", __func__);

	return count;
}

static DEVICE_ATTR(wake, 0200, NULL, kthd_store);

static struct attribute *kthd_attrs[] = {
	&dev_attr_wake.attr,
	NULL,
};

static struct attribute_group kthd_attr_grp = {
	.attrs = kthd_attrs,
};

static int kthread_fun(void *args)
{
	while (!kthread_should_stop()) {
		printk(KERN_INFO "%s: I am running\n", __func__);
		set_current_state(TASK_INTERRUPTIBLE);
		printk(KERN_INFO "%s: before calling schedule\n", __func__);
		schedule();
		printk(KERN_INFO "%s: after calling schedule\n", __func__);
	}
	return 0;
}

static int __init kthd_init(void)
{
	int ret = 0;

	printk(KERN_INFO "%s: enter module\n", __func__);

	kthd_class = class_create(THIS_MODULE, "kthd_class");
	if (IS_ERR(kthd_class)) {
		ret = PTR_ERR(kthd_class);
		goto reg_class_fail;
	}

	kthd_dev = device_create(kthd_class, 0, 0, 0, "kthd_dev");
	if (IS_ERR(kthd_dev)) {
		ret = PTR_ERR(kthd_dev);
		goto reg_dev_fail;
	}

	ret = sysfs_create_group(&kthd_dev->kobj, &kthd_attr_grp);
	if (ret)
		goto reg_grp_fail;

	thread = kthread_run(kthread_fun, NULL, "kthd");
	if (!IS_ERR(thread))
		return 0;

	ret = PTR_ERR(thread);

	sysfs_remove_group(&kthd_dev->kobj, &kthd_attr_grp);

  reg_grp_fail:
	device_unregister(kthd_dev);
  reg_dev_fail:
	class_unregister(kthd_class);
  reg_class_fail:

	return ret;
}

module_init(kthd_init);

static void __exit kthd_exit(void)
{
	printk(KERN_INFO "%s: exit module\n", __func__);

	kthread_stop(thread);
	sysfs_remove_group(&kthd_dev->kobj, &kthd_attr_grp);
	device_unregister(kthd_dev);
	class_unregister(kthd_class);
}

module_exit(kthd_exit);

MODULE_AUTHOR("PAN Guolin <guolinp@gmail.com>");
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("A simple kthread Module");
