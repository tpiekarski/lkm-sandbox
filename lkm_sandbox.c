/*
 * LKM Sandbox
 *
 */

// Linux Kernel Headers for Module Development
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>


// Module Meta Data (For available license see include/linux/module.h)
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Thomas Piekarski");
MODULE_DESCRIPTION("A sandbox for Linux Kernel Development");
MODULE_VERSION("0.1");

//
// Module Init & Exit
//

// Function to initialize and load the module
static int __init lkm_sandbox_init(void) {
    printk(KERN_INFO "Initializing and entering the sandbox...\n");
    
    return 0;
}

// Function to exit and unload the module
static void __exit lkm_sandbox_exit(void) {
    printk(KERN_INFO "Exiting the sandbox...\n");
}


// Registering init and exit functions
module_init(lkm_sandbox_init);
module_exit(lkm_sandbox_exit);

