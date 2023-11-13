
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
#include <linux/kernel.h>
#include <linux/moduleparam.h>
#include <linux/slab.h>
#include <hello1.h>
MODULE_AUTHOR("Mark Semeniuk <markflint2608@gmail.com>");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training - Part 2");
MODULE_LICENSE("Dual BSD/GPL");

static unsigned int print_count = 1;

module_param(print_count, uint, S_IRUGO); // Define the parameter
MODULE_PARM_DESC(print_count, "Number of times to print 'Hello, world!'");

extern void print_hello(int count);

static int __init hello2_init(void)
{
	pr_info("Hello2 module initialized.\n");
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

static void __exit hello2_exit(void)
{
	pr_info("Hello2 module exited.\n");
}

module_init(hello2_init);
module_exit(hello2_exit);
