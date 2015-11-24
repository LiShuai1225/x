#include <linux/init.h>
#include <linux/module.h>

#include "export_symb.h"

static int __init export_symb_caller_init(void)
{
	int a = 10, b = 5, c = 0;
	printk(KERN_INFO "export_symb_caller module enter\n ");
	c = add_integar(a, b);
	printk(KERN_INFO "%d + %d = %d\n", a, b, c);

	c = sub_integar(a, b);
	printk(KERN_INFO "%d - %d = %d\n", a, b, c);

	return 0;
}

module_init(export_symb_caller_init);

static void __exit export_symb_caller_exit(void)
{
	printk(KERN_INFO "export_symb_caller module exit\n ");
}

module_exit(export_symb_caller_exit);

MODULE_AUTHOR("Barry Song <baohua@kernel.org>");
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("A simple Module for testing module export_symb_caller");
MODULE_VERSION("V1.0");
