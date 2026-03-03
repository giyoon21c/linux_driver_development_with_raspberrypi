#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>


// hook in a module 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("MPCoding - LDD");
MODULE_DESCRIPTION("Our first dynamically loadable kernel module");

static const char *my_device = "my_cdev";

static dev_t dev_nr;

static struct cdev my_cdev;

// file_operations struct
static struct file_operations fops = {

};

// entry function 
static int __init my_init(void) {
    int status;

    // new way
    status = alloc_chrdev_region(&dev_nr, 0, MINORMASK + 1, my_device); 
    if (status < 0) {
        pr_err("%s: character device registraion failed\n", my_device);
        return status;
    }

    // cdev_init
    cdev_init(&my_cdev, &fops);
    my_cdev.owner = THIS_MODULE;
  
    // add the device to kernel's device list
    status = cdev_add(&my_cdev, dev_nr, MINORMASK + 1);
    if (status) {
        pr_err("%s: error adding cdev\n", my_device);
        unregister_chrdev_region(dev_nr, MINORMASK+1);
        return status;
    }

    pr_info("%s: Character device registered, Major number: %d Minor number: %d\n", 
        my_device, MAJOR(dev_nr), MINOR(dev_nr)); 
    return 0;
}

// exit function 
static void __exit my_exit(void){
    cdev_del(&my_cdev);
    unregister_chrdev_region(dev_nr, MINORMASK+1);
    pr_info("%s: Goodbye, Kernel\n", my_device);
}

module_init(my_init);
module_exit(my_exit);