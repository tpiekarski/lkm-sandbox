/*
 * LKM Sandbox::lkm_process
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
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Thomas Piekarski");
MODULE_DESCRIPTION("Character device for process information");
MODULE_VERSION("0.1");

static int major_num;
static int device_open_count = 0;
static char process_buffer[5];
static char *process_ptr;

static ssize_t device_read(struct file *sf, char *buffer, size_t len,
			   loff_t *offset);
static ssize_t device_write(struct file *sf, const char *buffer, size_t len,
			    loff_t *offset);
static int device_open(struct inode *inode, struct file *file);
static int device_release(struct inode *inode, struct file *file);

static struct file_operations device_fops = { .owner = THIS_MODULE,
					      .open = device_open,
					      .read = device_read,
					      .release = device_release,
					      .write = device_write };

static ssize_t device_read(struct file *sf, char *buffer, size_t len,
			   loff_t *offset)
{
	int bytes_read = 0;
	int op = 0;

	// todo: get current process information and output them

	if (*process_ptr == 0) {
		process_ptr = process_buffer;
	}

	while (len && *process_ptr) {
		op = put_user(*(process_ptr++), buffer++);

		if (op == -EFAULT) {
			break;
		}

		len--;
		bytes_read++;
	}

	return bytes_read;
}

static ssize_t device_write(struct file *sf, const char *buffer, size_t len,
			    loff_t *offset)
{
	printk(KERN_WARNING "lkm_process: Writing is not supported.\n");

	return -EINVAL;
}

static int device_open(struct inode *inode, struct file *file)
{
	if (device_open_count > 0) {
		return -EBUSY;
	}

	device_open_count++;
	try_module_get(THIS_MODULE);

	return 0;
}

static int device_release(struct inode *inode, struct file *file)
{
	device_open_count--;
	module_put(THIS_MODULE);

	return 0;
}

static int __init lkm_process_init(void)
{
	strncpy(process_buffer, "test", 5);
	process_ptr = process_buffer;
	major_num = register_chrdev(0, "process", &device_fops);

	if (major_num < 0) {
		printk(KERN_ERR "lkm_process: Failed registering device.\n");

		return -EIO;
	}

	printk(KERN_INFO "lkm_process: Major number is %d.\n", major_num);

	return 0;
}

static void __exit lkm_process_exit(void)
{
	unregister_chrdev(major_num, "process");
}

module_init(lkm_process_init);
module_exit(lkm_process_exit);
