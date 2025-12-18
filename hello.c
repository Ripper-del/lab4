// SPDX-License-Identifier: GPL-2.0
/*
 * hello.c - Lab 4 Kernel Module
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/moduleparam.h>
#include <linux/errno.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/ktime.h>

MODULE_AUTHOR("Student IO-XX");
MODULE_DESCRIPTION("Lab 4");
MODULE_LICENSE("Dual BSD/GPL");

static uint howmany = 1;
module_param(howmany, uint, 0444);
MODULE_PARM_DESC(howmany, "Number of greetings to print");

struct hello_data {
	struct list_head list;
	ktime_t time;
};

static LIST_HEAD(hello_list);

static int __init hello_init(void)
{
	uint i;
	struct hello_data *item;

	if (howmany == 0 || (howmany >= 5 && howmany <= 10))
		pr_warn("WARNING: Parameter 'howmany' is %u\n", howmany);

	if (howmany > 10) {
		pr_err("ERROR: Parameter 'howmany' is too large (%u > 10)\n", howmany);
		return -EINVAL;
	}

	for (i = 0; i < howmany; i++) {
		item = kmalloc(sizeof(struct hello_data), GFP_KERNEL);
		if (!item)
			return -ENOMEM;

		item->time = ktime_get();
		list_add_tail(&item->list, &hello_list);
		pr_emerg("Hello, world!\n");
	}

	return 0;
}

static void __exit hello_exit(void)
{
	struct hello_data *item, *tmp;

	list_for_each_entry_safe(item, tmp, &hello_list, list) {
		pr_emerg("Time: %lld ns\n", ktime_to_ns(item->time));
		list_del(&item->list);
		kfree(item);
	}
}

module_init(hello_init);
module_exit(hello_exit);
