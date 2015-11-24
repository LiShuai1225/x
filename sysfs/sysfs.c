/*
 * a simple kernel module: sysfs_example
 *
 * Copyright (C) 2015 PAN Guolin <guolinp@gmail.com>
 *
 * Licensed under GPLv2 or later.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/err.h>
#include <linux/device.h>

static int value = 0;
static struct device *sysfs_example_dev;
static struct class *sysfs_example_class;

static ssize_t sysfs_example_show(struct device *dev,
				  struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", value);
}

static ssize_t sysfs_example_store(struct device *dev,
				   struct device_attribute *attr,
				   const char *buf, size_t count)
{
	if (1 != sscanf(buf, "%d", &value))
		return -EINVAL;
	return count;
}

static DEVICE_ATTR(data, 0644, sysfs_example_show, sysfs_example_store);

static struct attribute *sysfs_example_attrs[] = {
	&dev_attr_data.attr,
	NULL,
};

static struct attribute_group sysfs_example_attr_grp = {
	.attrs = sysfs_example_attrs,
};

static int __init sysfs_example_init(void)
{
	int ret = 0;

	sysfs_example_class = class_create(THIS_MODULE, "sysfs_example_class");
	if (IS_ERR(sysfs_example_class))
		return PTR_ERR(sysfs_example_class);

	sysfs_example_dev = device_create(sysfs_example_class, 0, 0, 0, "sysfs_example_dev");
	if (IS_ERR(sysfs_example_dev))
		return PTR_ERR(sysfs_example_dev);

	ret = sysfs_create_group(&sysfs_example_dev->kobj, &sysfs_example_attr_grp);
	if (ret)
		return ret;

	return 0;
}

module_init(sysfs_example_init);

static void __exit sysfs_example_exit(void)
{
	sysfs_remove_group(&sysfs_example_dev->kobj, &sysfs_example_attr_grp);
	device_unregister(sysfs_example_dev);
	class_unregister(sysfs_example_class);
}

module_exit(sysfs_example_exit);

MODULE_AUTHOR("PAN Guolin <guolinp@gmail.com>");
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("A simple sysfs_example Module");
