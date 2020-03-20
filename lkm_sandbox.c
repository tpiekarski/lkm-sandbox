/*
 * LKM Sandbox::Sandbox
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

