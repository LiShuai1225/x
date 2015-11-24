/*
 * a simple kernel module: tasklet
 *
 * Copyright (C) 2015 PAN Guolin <guolinp@gmail.com>
 *
 * Licensed under GPLv2 or later.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/of.h>
#include <linux/of_irq.h>
#include <linux/of_gpio.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>

struct key {
	const char *desc;
	unsigned int irq;
	struct tasklet_struct t;
};

struct platdata {
	int nr_key;
	struct key *k;
};

static void tasklet_handler(unsigned long data)
{
	struct key *k = (struct key *)data;
	printk(KERN_INFO "In tasklet handler, key [%s], irq %d\n", k->desc, k->irq);
}

static irqreturn_t irq_handler(int irq, void *dev_data)
{
	struct key *k = dev_data;
	printk(KERN_INFO "In irq handler, key [%s], irq %d\n", k->desc, k->irq);

	tasklet_init(&k->t, tasklet_handler, (unsigned long)k);
	tasklet_schedule(&k->t);

	return IRQ_HANDLED;
}

struct platdata *key_get_devtree_pdata(struct device *dev)
{
	struct device_node *node, *pp;
	struct platdata *pdata;
	struct key *k;
	int n;

	node = dev->of_node;
	if (!node)
		return ERR_PTR(-ENODEV);

	n = of_get_child_count(node);
	if (n == 0)
		return ERR_PTR(-ENODEV);

	pdata = devm_kzalloc(dev, sizeof(*pdata) + n * sizeof(*k), GFP_KERNEL);
	if (!pdata)
		return ERR_PTR(-ENOMEM);

	pdata->k = (struct key *)&pdata[1];
	pdata->nr_key = n;

	k = pdata->k;
	for_each_child_of_node(node, pp) {
		enum of_gpio_flags flags;
		int gpio = of_get_gpio_flags(pp, 0, &flags);

		k->irq = gpio_to_irq(gpio);
		k->desc = (const char *)of_get_property(pp, "label", NULL);
		printk(KERN_INFO "irq %d, desc %s\n", k->irq, k->desc);
		k++;
	}
	return pdata;
}

static int gpio_keys_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct platdata *pdata = dev_get_platdata(dev);
	struct key *k;
	int ret, i;

	if (!pdata) {
		pdata = key_get_devtree_pdata(dev);
		if (IS_ERR(pdata))
			return PTR_ERR(pdata);
	}

	k = pdata->k;
	for (i = 0; i < pdata->nr_key; i++, k++) {
		ret = request_threaded_irq(k->irq, irq_handler, NULL, IRQF_SHARED, k->desc, k);
		printk(KERN_INFO "find a new key: irq %d, key [%s] %s\n", k->irq, k->desc,
		       ret ? "fail" : "ok");
	}

	return 0;
}

static int gpio_keys_remove(struct platform_device *pdev)
{
	struct platdata *pdata = dev_get_platdata(&pdev->dev);
	struct key *k;
	int i;

	if (!pdata)
		return 0;

	k = pdata->k;
	for (i = 0; i < pdata->nr_key; i++, k++)
		free_irq(k->irq, k);

	return 0;
}

static const struct of_device_id gpio_keys_of_match[] = {
	{.compatible = "gpio-keys",},
	{},
};

MODULE_DEVICE_TABLE(of, gpio_keys_of_match);

static struct platform_driver gpio_keys_device_driver = {
	.probe = gpio_keys_probe,
	.remove = gpio_keys_remove,
	.driver = {
		   .name = "gpio-keys",
		   .of_match_table = of_match_ptr(gpio_keys_of_match),
	}
};

static int __init interrupt_init(void)
{
	return platform_driver_register(&gpio_keys_device_driver);
}

module_init(interrupt_init);

static void __exit interrupt_exit(void)
{
	platform_driver_unregister(&gpio_keys_device_driver);
}

module_exit(interrupt_exit);

MODULE_AUTHOR("PAN Guolin <guolinp@gmail.com>");
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("A simple tasklet Module");
