#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Thomas Piekarski");
MODULE_DESCRIPTION("A Sandbox device module for the Linux Kernel");
MODULE_VERSION("0.1");

static int __init lkm_device_init(void) {
    printk(KERN_INFO "Initialize Sandbox Device Module...\n");

    return 0;
}

static void __exit lkm_device_exit(void) {
    printk(KERN_INFO "Exiting Sandbox Device Module...\n");
}

module_init(lkm_device_init);
module_exit(lkm_device_exit);