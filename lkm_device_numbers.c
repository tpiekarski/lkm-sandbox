/*
 * LKM Sandbox::Allocation of device numbers
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

#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Thomas Piekarski");
MODULE_DESCRIPTION("Illustrating how to allocate and free device numbers");
MODULE_VERSION("0.1");

#define LKM_DYNAMIC_DEV_NAME "ddev"
#define LKM_DYNAMIC_DEV_COUNT 1
#define LKM_DYNAMIC_DEV_MINOR 0
#define LKM_STATIC_DEV_NAME "sdev"
#define LKM_STATIC_DEV_COUNT 1
#define LKM_STATIC_DEV_MAJOR 300
#define LKM_STATIC_DEV_MINOR 0

static dev_t ddev;
static dev_t sdev;

void lkm_device_numbers_print(dev_t dev, char *name)
{
	printk(KERN_INFO "%s: Registered device %s with %d, %d.\n",
	       THIS_MODULE->name, name, MAJOR(dev), MINOR(dev));
}

static int __init lkm_device_numbers_init(void)
{
	int rc;

	printk(KERN_INFO "%s: Allocating major/minor for dynamic device.\n",
	       THIS_MODULE->name);
	rc = alloc_chrdev_region(&ddev, LKM_DYNAMIC_DEV_MINOR,
				 LKM_DYNAMIC_DEV_COUNT, LKM_DYNAMIC_DEV_NAME);

	if (rc < 0) {
		printk(KERN_ERR
		       "%s: Failed allocating major/minor for dynamic device.\n",
		       THIS_MODULE->name);

		return rc;
	}

	lkm_device_numbers_print(ddev, LKM_DYNAMIC_DEV_NAME);

	printk(KERN_INFO "%s: Registering static device.\n", THIS_MODULE->name);
	sdev = MKDEV(LKM_STATIC_DEV_MAJOR, LKM_STATIC_DEV_MINOR);
	rc = register_chrdev_region(sdev, LKM_STATIC_DEV_COUNT,
				    LKM_STATIC_DEV_NAME);

	if (rc < 0) {
		printk(KERN_ERR "%s: Failed registering static device.\n",
		       THIS_MODULE->name);

		return rc;
	}

	lkm_device_numbers_print(sdev, LKM_STATIC_DEV_NAME);

	return 0;
}

static void __exit lkm_device_numbers_exit(void)
{
	if (ddev != 0) {
		printk(KERN_INFO "%s: Unregistering dynamic device.\n",
		       THIS_MODULE->name);
		unregister_chrdev_region(ddev, LKM_DYNAMIC_DEV_COUNT);
	}

	if (sdev != 0) {
		printk(KERN_INFO "%s: Unregistering static device.\n",
		       THIS_MODULE->name);
		unregister_chrdev_region(sdev, LKM_STATIC_DEV_COUNT);
	}
}

module_init(lkm_device_numbers_init);
module_exit(lkm_device_numbers_exit);
