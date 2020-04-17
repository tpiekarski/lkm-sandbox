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
#include <linux/semaphore.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Thomas Piekarski");
MODULE_DESCRIPTION("Driver for memory-based character devices");
MODULE_VERSION("0.1");

#define LKM_MEV_DEVICE_COUNT 1
#define LKM_MEV_DEVICE_MINOR 0
#define LKM_MEV_DEVICE_NAME "mev"
#define LKM_MEV_QSET_SIZE 1000
#define LKM_MEV_QUANTUM_SIZE 4000

// Structures
struct mev_qset {
	void **data;
	struct mev_qset *next;
};

struct mev_container {
	// todo: declare missing members by working through subsequent chapters
	struct mev_qset *data;
	int quantum;
	int qset;
	unsigned long size;
	struct semaphore sem;
	struct cdev cdev;
};

// Prototypes
static long mev_ioctl(struct file *file, unsigned int cmd, unsigned long arg);
static loff_t mev_llseek(struct file *file, loff_t offset, int whence);
struct mev_qset *mev_follow(struct mev_container *container, int n);
static bool mev_io_is_wronly(unsigned int f_flags);
static int mev_open(struct inode *inode, struct file *file);
static int mev_release(struct inode *inode, struct file *file);
static ssize_t mev_read(struct file *file, char __user *buf, size_t count,
			loff_t *f_pos);
void mev_trim(struct mev_container *container);
static ssize_t mev_write(struct file *file, const char __user *buf,
			 size_t count, loff_t *f_pos);

// Global Declaration
static dev_t mev_device;
static struct mev_container *dev_container;
static struct file_operations mev_fops = {
	.owner = THIS_MODULE,
	.llseek = mev_llseek,
	.read = mev_read,
	.write = mev_write,
	// Using new ioctl to avoid BKL (Big Kernel Lock)
	// (for details refer to: https://lwn.net/Articles/119652/)
	.unlocked_ioctl = mev_ioctl,
	.open = mev_open,
	.release = mev_release
};

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
	size_t size = LKM_MEV_DEVICE_COUNT * sizeof(struct mev_container);
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

	printk(KERN_INFO "%s: Setup cdev and added cdev to kernel\n",
	       THIS_MODULE->name);

	return 0;
}
module_init(lkm_mev_init);

static void __exit lkm_mev_exit(void)
{
	printk(KERN_INFO "%s: Exiting module\n", THIS_MODULE->name);

	if (dev_container != NULL) {
		printk(KERN_INFO
		       "%s: Trimming data, deleting cdev and deallocating memory of device container\n",
		       THIS_MODULE->name);
		mev_trim(dev_container);
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

static loff_t mev_llseek(struct file *file, loff_t offset, int whence)
{
	// todo: implement callback mev_llseek

	return 0;
}

// todo: review and understand how following exactly works, why is there memory allocated
// Source of function: https://github.com/jesstess/ldd3-examples/blob/master/examples/scull/main.c#L262
struct mev_qset *mev_follow(struct mev_container *container, int n)
{
	struct mev_qset *qset = container->data;

	// If it's needed allocate first qset
	// todo: check under what exactly circumstances qset could be emtpy
	// todo: consider to extract this into an own, well named function
	if (!qset) {
		qset = container->data =
			kmalloc(sizeof(struct mev_qset), GFP_KERNEL);

		if (qset == NULL) {
			return NULL;
		}

		// todo: check memset - what does it do exactly?
		memset(qset, 0, sizeof(struct mev_qset));
	}

	// Follow the list
	// todo: get to know why qsets are read from back to front
	while (n--) {
		if (!qset->next) {
			// todo: figure out why allocating memory in advance (for writing?)
			qset->next =
				kmalloc(sizeof(struct mev_qset), GFP_KERNEL);

			if (qset->next == NULL) {
				return NULL;
			}

			// todo: check memset - what does it do exactly?
			memset(qset->next, 0, sizeof(struct mev_qset));
		}

		qset = qset->next;

		continue; // todo: figure out why such explicit continue
	}

	return qset;
}

static bool mev_io_is_wronly(unsigned int f_flags)
{
	if ((f_flags & O_ACCMODE) == O_WRONLY) {
		return true;
	}

	return false;
}

static int mev_open(struct inode *inode, struct file *file)
{
	printk(KERN_INFO "%s: Opening device\n", THIS_MODULE->name);
	printk(KERN_INFO "%s: Looking for container with cdev at inode\n",
	       THIS_MODULE->name);
	struct mev_container *container =
		container_of(inode->i_cdev, struct mev_container, cdev);

	if (container == NULL) {
		printk(KERN_WARNING
		       "%s: Failed getting container by cdev at inode\n",
		       THIS_MODULE->name);

		return -EFAULT;
	}

	printk(KERN_INFO "%s: Found container for device (%d, %d)\n",
	       THIS_MODULE->name, MAJOR(container->cdev.dev),
	       MINOR(container->cdev.dev));

	// todo: check if the pointer to container won't point to NULL as soon as function is left
	file->private_data = container;

	if (mev_io_is_wronly(file->f_flags)) {
		mev_trim(container);
	}

	return 0;
}

static int mev_release(struct inode *inode, struct file *file)
{
	printk(KERN_INFO "%s: Releasing device\n", THIS_MODULE->name);
	// nothing to do for a memory-based device

	return 0;
}

// todo: review and understand how reading exactly works
static ssize_t mev_read(struct file *file, char __user *buf, size_t count,
			loff_t *f_pos)
{
	struct mev_container *container = file->private_data;
	struct mev_qset *dptr = NULL;
	int quantum = container->quantum;
	int qset = container->qset;
	int itemsize = quantum * qset;
	int item = 0;
	// todo: figure out what s and q (quantum?) prefixes are
	// todo: check what are these positions exactly
	int s_pos = 0;
	int q_pos = 0;
	int rest = 0;
	ssize_t retval = 0;

	if (down_interruptible(&container->sem)) {
		// todo: check why is 'out' not used (maybe use it)?
		return -ERESTARTSYS;
	}

	if (*f_pos >= container->size) {
		// todo: rephrase condition and statement in words
		// todo: check why no retval is set or why it is left 0?
		goto out;
	}

	if (*f_pos + count > container->size) {
		// todo: rephrase condition and statement in words
		count = container->size - *f_pos;
	}

	// Find listitem, qset index and offset in quantum (same as in write)
	// todo: output all values with printk
	// todo: figure out why one time is a division where another is modulo?
	item = (long)*f_pos / itemsize;
	rest = (long)*f_pos % itemsize;
	// todo: what do those prefixes s and q (quantum?) mean?
	s_pos = rest / quantum;
	q_pos = rest % quantum;

	// todo: check how to follow the list up to the right position
	// The book using a function named scull_follow without mentioning it further
	// (https://github.com/jesstess/ldd3-examples/blob/master/examples/scull/main.c#L262)
	dptr = mev_follow(container, item); // -> corresponds to scull_follow()

	// todo: extract boolean expression into well-named function
	// (something like is_data_holey or is_data_empty (so java-like ;))
	if (dptr == NULL || !dptr->data || !dptr->data[s_pos]) {
		goto out;
	}

	if (count > quantum - q_pos) {
		// todo: rephrase condition and statement in words
		count = quantum - q_pos; // read up to the end of this quantum
	}

	if (copy_to_user(buf, dptr->data[s_pos] + q_pos, count)) {
		retval = -EFAULT;
		goto out;
	}

	*f_pos += count;
	retval = count;

out:
	up(&container->sem);
	return retval;
}

// todo: review and understand how trimming exactly works
// todo: printk values in container before and after trimming
void mev_trim(struct mev_container *container)
{
	printk(KERN_INFO "%s: Trimming device to '0'\n", THIS_MODULE->name);

	struct mev_qset *next = NULL;
	struct mev_qset *dptr = NULL;
	int qset = container->qset;
	int i = 0;

	// todo: printk entries into outer and inner loop
	for (dptr = container->data; dptr; dptr = next) {
		// todo: rephrase expressions in words
		if (dptr->data) {
			for (i = 0; i < qset; i++) {
				kfree(dptr->data[i]);
			}

			kfree(dptr->data);
			dptr->data = NULL;
		}

		next = dptr->next;
		kfree(dptr);
	}

	container->size = 0;
	container->quantum = LKM_MEV_QUANTUM_SIZE;
	container->qset = LKM_MEV_QSET_SIZE;
	container->data = NULL;
}

// todo: review and understand how writing exactly works
static ssize_t mev_write(struct file *file, const char __user *buf,
			 size_t count, loff_t *f_pos)
{
	struct mev_container *container = file->private_data;
	struct mev_qset *dptr;
	int quantum = container->quantum;
	int qset = container->qset;
	int itemsize = quantum * qset;
	int item = 0;
	// todo: figure out what s and q (quantum?) prefixes are
	// todo: check what are these positions exactly
	int s_pos = 0;
	int q_pos = 0;
	int rest = 0;
	ssize_t retval = -ENOMEM;

	if (down_interruptible(&container->sem)) {
		// todo: check why is 'out' not used (maybe use it)?
		return -ERESTARTSYS;
	}

	// Find listitem, qset index and offset in quantum (same as in read)
	// todo: output all values with printk
	// todo: figure out why one time is a division where another is modulo?
	item = (long)*f_pos / itemsize;
	rest = (long)*f_pos % itemsize;
	// todo: what do those prefixes s and q (quantum?) mean?
	s_pos = rest / quantum;
	q_pos = rest % quantum;

	// todo: check how to follow the list up to the right position
	// The book using a function named scull_follow without mentioning it further
	// (https://github.com/jesstess/ldd3-examples/blob/master/examples/scull/main.c#L262)
	dptr = mev_follow(container, item); // -> corresponds to scull_follow()

	if (dptr == NULL) {
		goto out;
	}

	// todo: get more comfortable with memory allocation in ANSI C and in the kernel
	// (For example write a little lkm_kmalloc module and let your VM run out of memory :))

	if (!dptr->data) {
		dptr->data = kmalloc(qset * sizeof(char *), GFP_KERNEL);

		if (!dptr->data) {
			goto out;
		}

		// todo: check what and why memset is used
		memset(dptr->data, 0, qset * sizeof(char *));
	}

	if (!dptr->data[s_pos]) {
		dptr->data[s_pos] = kmalloc(quantum, GFP_KERNEL);

		if (!dptr->data[s_pos]) {
			goto out;
		}

		// todo: check why is memset not used here like in the parent data
	}

	if (count > quantum - q_pos) {
		// todo: rephrase condition and statement in words
		count = quantum - q_pos; // write up to the end of this quantum
	}

	if (copy_from_user(dptr->data[s_pos] + q_pos, buf, count)) {
		retval = -EFAULT;
		goto out;
	}

	*f_pos += count;
	retval = count;

	if (container->size < *f_pos) {
		// todo: rephrase condition and statement in words
		container->size = *f_pos;
	}

out:
	up(&container->sem);
	return retval;
}
