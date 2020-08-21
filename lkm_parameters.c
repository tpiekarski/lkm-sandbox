/*
 * LKM Sandbox::Parameters
 * <https://github.com/tpiekarski/lkm-sandbox>
 * ---
 * Copyright 2020 Thomas Piekarski <t.piekarski@deloquencia.de>
 *
 * This file is part of LKM Sandbox.
 * 
 * LKM Sandbox is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 * 
 * LKM Sandbox is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with LKM Sandbox. If not, see <https://www.gnu.org/licenses/>.
 * 
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/string.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Thomas Piekarski");
MODULE_DESCRIPTION("Module for passing parameters from user to kernel space.");
MODULE_VERSION("0.1");

#define DEFAULT_PERMISSIONS 0664

static int number = 42;
static char *message = "That's a parameter.";

module_param(number, int, DEFAULT_PERMISSIONS);
module_param(message, charp, DEFAULT_PERMISSIONS);

static int __init lkm_parameters_init(void)
{
	printk(KERN_INFO
	       "lkm_parameters: Initializing Sandbox Parameters Module.\n");

	if (number == 42 && strcmp(message, "That's a parameter.") == 0) {
		printk(KERN_INFO
		       "lkm_parameters: Module was loaded with default parameters.\n");

		return 0;
	}

	printk(KERN_INFO
	       "lkm_parameters: Module was loaded with number = %d and message = %s.\n",
	       number, message);

	return 0;
}

static void __exit lkm_parameters_exit(void)
{
	// nothing to do here
}

module_init(lkm_parameters_init);
module_exit(lkm_parameters_exit);
