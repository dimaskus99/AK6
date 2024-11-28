#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/moduleparam.h>
#include "hello1.h"

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Horbyk Dmytro");
MODULE_DESCRIPTION("Hello2 Module - Calls print_hello() from hello1");

static int call_count = 1;
module_param(call_count, int, 0644);
MODULE_PARM_DESC(call_count, "Number of times to call print_hello");

static int __init hello2_init(void)
{
	BUG_ON(call_count <= 0);

	pr_info("hello2 module loaded, calling print_hello(%d)\n", call_count);
	print_hello(call_count);
	return 0;
}

static void __exit hello2_exit(void)
{
	pr_info("Unloading hello2 module\n");
}

module_init(hello2_init);
module_exit(hello2_exit);
