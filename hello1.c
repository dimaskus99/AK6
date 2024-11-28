#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/ktime.h>
#include "hello1.h"

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Horbyk Dmytro");
MODULE_DESCRIPTION("Hello1 Module - Exports print_hello()");

static LIST_HEAD(hello_list);

void print_hello(int count)
{
	struct hello_event *new_event;
	int i;

	for (i = 0; i < count; i++) {
		if (i == 4) {
			new_event = NULL;
		} else {
			new_event = kmalloc(sizeof(*new_event), GFP_KERNEL);
		}

		BUG_ON(!new_event);

		new_event->start_time = ktime_get();
		pr_info("Hello, world! (%d)\n", i + 1);
		new_event->end_time = ktime_get();
		list_add(&new_event->list, &hello_list);
	}
}

static int __init hello1_init(void)
{
	pr_info("hello1 module loaded\n");
	return 0;
}

static void __exit hello1_exit(void)
{
	struct hello_event *event;
	struct list_head *pos, *q;

	pr_info("Unloading hello1 module\n");

	list_for_each_safe(pos, q, &hello_list) {
		event = list_entry(pos, struct hello_event, list);
		pr_info("Event duration: %lld ns\n",
			ktime_to_ns(ktime_sub(event->end_time, event->start_time)));
		list_del(pos);
		kfree(event);
	}
}

EXPORT_SYMBOL(print_hello);

module_init(hello1_init);
module_exit(hello1_exit);
