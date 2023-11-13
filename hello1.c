 /*
 * Copyright (c) 2017, GlobalLogic Ukraine LLC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    This product includes software developed by the GlobalLogic.
 * 4. Neither the name of the GlobalLogic nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY GLOBALLOGIC UKRAINE LLC ``AS IS`` AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL GLOBALLOGIC UKRAINE LLC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/list.h>
#include <linux/printk.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <hello1.h>

MODULE_AUTHOR("Mark Semeniuk <markflint2608@gmail.com>");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");

static unsigned int print_count = 1;  // Доданий параметр зі значенням за замовчуванням 1

module_param(print_count, uint, S_IRUGO); // Визначення параметра
MODULE_PARM_DESC(print_count, "Number of times to print 'Hello, world!'");

static LIST_HEAD(my_list);

// Function to print "Hello, world!" and record timestamps
void print_hello(int count)
{
	unsigned int i;
	for (i = 0; i < count; i++) {
		struct my_data *entry = kmalloc(sizeof(struct my_data), GFP_KERNEL);
		if (!entry) {
			pr_err("Memory allocation failed.\n");
			return -ENOMEM;
		}

		entry->timestamp = ktime_get();
		pr_info("Hello world!\n");

		entry->after = ktime_get();

		list_add(&entry->list, &my_list);
	}
}

EXPORT_SYMBOL(print_hello);


static int __init hello1_init(void)
{
	if (print_count == 0 || (print_count >= 5 && print_count <= 10)) {
		pr_warn("Invalid parameter value. Using the default value of 1.\n");
		print_count = 1;
	} else if (print_count > 10) {
		pr_err("Invalid parameter value. Module will not load.\n");
		return -EINVAL;
	} // Повертаємо помилку, модуль не завантажиться
	
	print_hello(print_count);
	return 0;
}

static void __exit hello1_exit(void)
{
	struct my_data *entry;
	struct my_data *temp;
	list_for_each_entry_safe(entry, temp, &my_list, list) {
		pr_info("Time taken for printing: %lld ns\n", ktime_to_ns(ktime_sub(entry->after, entry->timestamp)));
		list_del(&entry->list);
		kfree(entry);
	}
	pr_info("Hello1 module exited.\n");
}

module_init(hello1_init);
module_exit(hello1_exit);
