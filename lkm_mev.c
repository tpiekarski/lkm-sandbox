/*
 * LKM Sandbox::Driver for memory-based character devices
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

#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Thomas Piekarski");
MODULE_DESCRIPTION("Driver for memory-based character devices");
MODULE_VERSION("0.1");

#define LKM_MEV_DEVICE_NAME "mev"
#define LKM_MEV_DEVICE_COUNT 1
#define LKM_MEV_DEVICE_MINOR 0
#define LKM_MEV_DEVICE_AMOUNT 1

static long mev_ioctl(struct file *file, unsigned int cmd, unsigned long arg);
static int mev_open(struct inode *inode, struct file *file);
static int mev_release(struct inode *inode, struct file *file);
static loff_t mev_llseek(struct file *file, loff_t offset, int whence);
static ssize_t mev_read(struct file *file, char __user *buf, size_t count,
			loff_t *ppos);
static ssize_t mev_write(struct file *file, const char __user *buf,
			 size_t count, loff_t *ppos);

static struct file_operations mev_fops = {
	.owner = THIS_MODULE,
	.llseek = mev_llseek,
	.read = mev_read,
	.write = mev_write,
	// Using new ioctl and avoiding BKL (Big Kernel Lock)
	// For details refer to: https://lwn.net/Articles/119652/
	.unlocked_ioctl = mev_ioctl,
	.open = mev_open,
	.release = mev_release
};

struct mev_container {
	// todo: declare all members by working through subsequent chapters
	struct cdev cdev;
};

static dev_t mev_device;
static struct mev_container *dev_container;

static int lkm_mev_setup_cdev(struct mev_container *dev, int index)
{
	int dev_no = MKDEV(MAJOR(mev_device), MINOR(mev_device) + index);

	printk(KERN_INFO "%s: Trying to initialize and add cdev to kernel\n",
	       THIS_MODULE->name);

	cdev_init(&dev->cdev, &mev_fops);
	dev->cdev.owner = THIS_MODULE;
	dev->cdev.ops = &mev_fops;

	int rc = cdev_add(&dev->cdev, dev_no, 1);

	if (rc < 0) {
		printk(KERN_ERR "%s: Failed adding cdev, %d, %d\n",
		       THIS_MODULE->name, MAJOR(dev_no), MINOR(dev_no));

		return rc;
	}

	return 0;
}

static int __init lkm_mev_init(void)
{
	printk(KERN_INFO "%s: Initializing module\n", THIS_MODULE->name);
	printk(KERN_INFO
	       "%s: Trying to allocate major and minor for device '%s'\n",
	       THIS_MODULE->name, LKM_MEV_DEVICE_NAME);

	int rc = alloc_chrdev_region(&mev_device, LKM_MEV_DEVICE_MINOR,
				     LKM_MEV_DEVICE_COUNT, LKM_MEV_DEVICE_NAME);

	if (rc < 0) {
		printk(KERN_ERR
		       "%s: Failed allocating major/minor for device '%s'\n",
		       THIS_MODULE->name, LKM_MEV_DEVICE_NAME);

		return rc;
	}

	printk(KERN_INFO
	       "%s: Allocated major %d and minor %d for device '%s'\n",
	       THIS_MODULE->name, MAJOR(mev_device), MINOR(mev_device),
	       LKM_MEV_DEVICE_NAME);

	printk(KERN_INFO "%s: Allocating memory for device container\n",
	       THIS_MODULE->name);
	size_t size = LKM_MEV_DEVICE_AMOUNT * sizeof(struct mev_container);
	dev_container = kmalloc(size, GFP_KERNEL);

	if (dev_container == NULL) {
		printk(KERN_ERR
		       "%s: Failed allocating memory for device container\n",
		       THIS_MODULE->name);

		return -ENOMEM;
	}

	rc = lkm_mev_setup_cdev(dev_container, 0);

	if (rc < 0) {
		printk(KERN_ERR "%s: Failed setting up cdev\n",
		       THIS_MODULE->name);

		return rc;
	}

	printk(KERN_INFO "%s: Setup cdev and added to kernel\n",
	       THIS_MODULE->name);

	return 0;
}
module_init(lkm_mev_init);

static void __exit lkm_mev_exit(void)
{
	printk(KERN_INFO "%s: Exiting module\n", THIS_MODULE->name);

	if (dev_container != NULL) {
		printk(KERN_INFO
		       "%s: Deleting cdev and deallocating memory of device container\n",
		       THIS_MODULE->name);
		cdev_del(&dev_container->cdev);
		kfree(dev_container);
	}

	if (mev_device != 0) {
		printk(KERN_INFO "%s: Unregistering major/minor of '%s'\n",
		       THIS_MODULE->name, LKM_MEV_DEVICE_NAME);
		unregister_chrdev_region(mev_device, LKM_MEV_DEVICE_COUNT);
	}
}
module_exit(lkm_mev_exit);

static long mev_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	// todo: implement callback mev_ioctl

	return 0l;
}

static int mev_open(struct inode *inode, struct file *file)
{
	// todo: implement callback mev_open

	return 0;
}

static int mev_release(struct inode *inode, struct file *file)
{
	// todo: implement callback mev_release

	return 0;
}

static loff_t mev_llseek(struct file *file, loff_t offset, int whence)
{
	// todo: implement callback mev_llseek

	return 0;
}

static ssize_t mev_read(struct file *file, char __user *buf, size_t count,
			loff_t *ppos)
{
	// todo: implement callback mev_read

	return 0;
}

static ssize_t mev_write(struct file *file, const char __user *buf,
			 size_t count, loff_t *ppos)
{
	// todo: implement callback mev_write

	return 0;
}
