/*
 * =====================================================================================
 *
 *       Filename:  mydriver.c
 *
 *    Description:  Simplified driver for basic iotc calls
 *
 *        Version:  1.0
 *        Created:  06/09/16
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Brandon Westmoreland (brandon.westmoreland@oit.edu)
 *   Organization:  Oregon Tech
 *
 * =====================================================================================
 */

#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/errno.h>

#include "myioctl.h"

// MODULE Definitions
#define FIRST_MINOR 0
#define MINOR_CNT 1
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Brandon Westmoreland <brandon.westmoreland@oit.edu>");
MODULE_DESCRIPTION("iotc driver for lab");

// Variables
static dev_t majorNum; 
static struct cdev CharDevice; 
static struct class * CharClass; 
static char myBuf[512];
static int bufStatus = 0;

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,35))
static int my_ioctl(struct inode *i, struct file *f, unsigned int cmd, unsigned long arg)
#else
static long my_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
#endif
{
    printk(KERN_INFO "BrandonW driver: ioctl function called\n"); 

    switch(cmd)
    {
        case QUERY_GET_STATUS:
            if (copy_to_user((int *)arg, &bufStatus, sizeof(int)))
            {
                 return -EACCES;
            }
            break;
        case QUERY_CLR_BUF:
            memset(myBuf, '\0',sizeof(myBuf));
            bufStatus = 0;
            break;
        case QUERY_SET_BUF:
            memset(myBuf, 'A',sizeof(myBuf));
            bufStatus = 1;
            break;
        default:
            return -EINVAL;
    }
    return 0; 
}


// Operational Functions
static int myOpen(struct inode * i, struct file * f)
{
    printk(KERN_INFO "BrandonW Driver: Open Function\n"); 
    return 0; 
}

static int myClose(struct inodede * i,  struct file * f)
{
    printk(KERN_INFO "BrandonW Driver: Close Function\n"); 
    return 0;
}

static ssize_t mycharRead(struct file * f, char __user * buf, 
        size_t len, loff_t * offset)
{
    printk(KERN_INFO "BrandonW Driver: Read Function\n"); 

	// Send my buffer to userland
    if(*offset >=  0 && *offset + len < 512)
    {
	if(copy_to_user(buf, &myBuf + *offset, len) != 0)
	    return -EFAULT; 
	else
        {
	    return 1; 
        }
    }
    else
        return 0; 
}

static ssize_t mycharWrite(struct file * f, char __user * buf, 
        size_t len, loff_t * offset)
{
    printk(KERN_INFO "BrandonW Driver: Write Function\n"); 

	// Copy last char from userland to my buffer
	if(copy_from_user(&myBuf, buf + len - 1, 1) != 0)
		return -EFAULT; 
	else
		return len; 
}

// Setup fops
static struct file_operations fops =
{
    .owner = THIS_MODULE, 
    .open = myOpen, 
    .release = myClose, 
    .read = mycharRead, 
    .write = mycharWrite,
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,35))
    .ioctl = my_ioctl // I think this line was incorrect: myioctl instead.
#else
    .unlocked_ioctl = my_ioctl
#endif
}; 

// Driver Init Function
static int __init mycharInit(void)
{
    printk(KERN_INFO "BrandonW Driver: Registered\n"); 
    
    // Attempt to Register Character device
    if(alloc_chrdev_region(&majorNum, FIRST_MINOR, MINOR_CNT, "Brandon") < 0)
    {
        return -1; 
    }

    // Attempt to create the device class for udev
    if((CharClass = class_create(THIS_MODULE, "chardrv"))  ==  NULL)
    {
        unregister_chrdev_region(majorNum, MINOR_CNT); 
        return -1; 
    }

    // Create the actual device in /dev folder
    if(device_create(CharClass, NULL, majorNum, NULL, "mydriver")  ==  NULL)
    {
        class_destroy(CharClass); 
        unregister_chrdev_region(majorNum, MINOR_CNT); 
        return -1; 
    }

    // Initialize the CHAR device
    cdev_init(&CharDevice, &fops); 
    if(cdev_add(&CharDevice, majorNum, MINOR_CNT)  ==  -1)
    {
        device_destroy(CharClass, majorNum); 
        class_destroy(CharClass); 
        unregister_chrdev_region(majorNum, MINOR_CNT); 
        return -1; 
    }

    return 0; 
}

static void __exit mycharExit(void)
{
    cdev_del(&CharDevice); 
    device_destroy(CharClass, majorNum); 
    class_destroy(CharClass); 
    unregister_chrdev_region(majorNum, 1); 
    printk(KERN_INFO "BrandonW Driver: Unregistered\n"); 
}

module_init(mycharInit); 
module_exit(mycharExit); 

