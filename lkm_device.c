/*
 * LKM Sandbox::Device
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
#include <linux/moduleparam.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Thomas Piekarski");
MODULE_DESCRIPTION("A Sandbox device module for the Linux Kernel");
MODULE_VERSION("0.1");

static int device_init_sub(void);
static int proc_init_sub(void);
static int device_open(struct inode *inode, struct file *file);
static int device_release(struct inode *inode, struct file *file);
static ssize_t device_read(struct file *flip, char *buffer, size_t len,
			   loff_t *offset);
static ssize_t device_write(struct file *flip, const char *buffer, size_t len,
			    loff_t *offset);
static int proc_show(struct seq_file *seq, void *v);

#define DEVICE_NAME "lkm_device"
#define MESSAGE "Hello, Linux!\n"
#define MESSAGE_BUFFER_LENGTH 15
#define PARAM_MAJOR_NUM_PERMISSION 0664
#define PROC_FILE_NAME "lkm_device_major"
#define PROC_PARENT NULL // root of /proc fs
#define PROC_PERMISSION 0444

static int major_num;
static int param_major_num = 0;
static int device_open_count = 0;
static char message_buffer[MESSAGE_BUFFER_LENGTH];
static char *message_ptr;

static struct file_operations device_fops = { .owner = THIS_MODULE,
					      .open = device_open,
					      .read = device_read,
					      .release = device_release,
					      .write = device_write };

module_param(param_major_num, int, PARAM_MAJOR_NUM_PERMISSION);

static ssize_t device_read(struct file *flip, char *buffer, size_t len,
			   loff_t *offset)
{
	int bytes_read = 0;
	printk(KERN_INFO "lkm_device: Starting to read from sandbox device.\n");

	if (*message_ptr == 0)
		message_ptr = message_buffer;

	while (len && *message_ptr) {
		printk(KERN_INFO "lkm_device: Reading from device.\n");
		if (put_user(*(message_ptr++), buffer++) == -EFAULT) {
			printk(KERN_ALERT
			       "lkm_device: Failed copying message from kernel to user space.\n");

			break;
		}

		len--;
		bytes_read++;
	}

	return bytes_read;
}

static ssize_t device_write(struct file *flip, const char *buffer, size_t len,
			    loff_t *offset)
{
	printk(KERN_ALERT
	       "lkm_device: Writing to sandbox device is not supported.\n");

	return -EINVAL;
}

static int device_open(struct inode *inode, struct file *file)
{
	if (device_open_count > 0) {
		printk(KERN_INFO "lkm_device: Sandbox device already open.\n");
		return -EBUSY;
	}

	printk(KERN_INFO "lkm_device: Opening sandbox device.\n");
	device_open_count++;
	try_module_get(THIS_MODULE);

	return 0;
}

static int device_release(struct inode *inode, struct file *file)
{
	printk(KERN_INFO "lkm_device: Closing sandbox device.\n");
	device_open_count--;
	module_put(THIS_MODULE);

	return 0;
}

static int proc_show(struct seq_file *seq, void *v)
{
	seq_printf(seq, "%d", major_num);

	return 0;
}

static int __init lkm_device_init(void)
{
	printk(KERN_INFO "lkm_device: Initialize Sandbox Device Module.\n");

	if (device_init_sub() < 0) {
		return -1;
	}

	if (proc_init_sub() < 0) {
		return -2;
	}

	return 0;
}

static void __exit lkm_device_exit(void)
{
	printk(KERN_INFO "lkm_device: Exiting Sandbox Device Module.\n");
	unregister_chrdev(major_num, DEVICE_NAME);
	remove_proc_entry(PROC_FILE_NAME, PROC_PARENT);
}

static int device_init_sub(void)
{
	printk(KERN_INFO
	       "lkm_device: Registering character device to print test message.\n");

	strncpy(message_buffer, MESSAGE, MESSAGE_BUFFER_LENGTH);
	message_ptr = message_buffer;

	if (param_major_num != 0) {
		printk(KERN_INFO
		       "lkm_device: Failed allocating %d as major for sandbox device.\n",
		       param_major_num);
	}

	major_num = register_chrdev(param_major_num, DEVICE_NAME, &device_fops);

	if (major_num < 0) {
		printk(KERN_ALERT
		       "lkm_device: Failed to register sandbox device with major %d.\n",
		       major_num);

		return -1;
	}

	return 0;
}

static int proc_init_sub(void)
{
	struct proc_dir_entry *proc_major_entry = NULL;

	printk(KERN_INFO
	       "lkm_device: Registered sandbox device with major number %d.\n",
	       major_num);

	printk(KERN_INFO
	       "lkm_device: Creating /proc file %s for storing major number %d.\n",
	       PROC_FILE_NAME, major_num);

	proc_major_entry = proc_create_single(PROC_FILE_NAME, PROC_PERMISSION,
					      PROC_PARENT, proc_show);

	if (proc_major_entry == NULL) {
		printk(KERN_ALERT
		       "lkm_device: Failed to create /proc entry '%s' for device major.\n",
		       PROC_FILE_NAME);

		return -1;
	}

	return 0;
}

module_init(lkm_device_init);
module_exit(lkm_device_exit);
