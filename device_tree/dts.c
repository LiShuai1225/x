/*
 * a simple kernel module: dts
 *
 * Copyright (C) 2015 PAN Guolin <guolinp@gmail.com>
 *
 * Licensed under GPLv2 or later.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/of.h>

static void dts_show(void)
{
	struct device_node *node, *pp;
	struct property *prop;
	const char *string;
	u8 data[4];
	int n, ret;

	node = of_find_node_by_name(0, "node");
	if (!node) {
		printk(KERN_INFO "can not get the node\n");
		return;
	}

	/* dump property */
	ret = of_property_read_string(node, "a-string-property", &string);
	if (ret == 0)
		printk(KERN_INFO "property: a-string-property, value: %s\n",
		       string);

	n = 0;
	prop = of_find_property(node, "a-string-list-property", NULL);
	for (string = of_prop_next_string(prop, NULL);
	     string;
	     string = of_prop_next_string(prop, string)) {
		printk(KERN_INFO "property: a-string-list-property, value[%d]=%s\n", n, string);
		n++;
	}

	ret = of_property_read_u8_array(node, "a-byte-data-property", data, 4);
	if (ret == 0)
		printk(KERN_INFO "property: a-byte-data-property, values: %02x/%02x/%02x/%02x\n",
		       data[0], data[1], data[2], data[3]);

	n = of_get_child_count(node);
	printk(KERN_INFO "%d child nodes\n", n);

	for_each_child_of_node(node, pp) {
		struct device_node *np =
		    of_parse_phandle(pp, "a-reference-to-something", 0);
		if (np) {
			u32 val;
			ret = of_property_read_u32_index(np, "a-cell-property", 0, &val);
			if (ret == 0)
				printk(KERN_INFO "phandle property: a-cell-property, value: %d\n", val);
		}
	}
}

static int __init dts_init(void)
{
	printk(KERN_INFO "enter dts module.\n");
	dts_show();
	return 0;
}

module_init(dts_init);

static void __exit dts_exit(void)
{
	printk(KERN_INFO "exit dts module.\n ");
}

module_exit(dts_exit);

MODULE_AUTHOR("PAN Guolin <guolinp@gmail.com>");
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("A simple dts Module");
