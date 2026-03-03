#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>

// hook in a module 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("MPCoding - LDD");
MODULE_DESCRIPTION("Our first dynamically loadable kernel module");

static const char *my_device = "my_cdev";

#define MY_MAJOR 64
//static int major;

// file_operations struct
static struct file_operations fops = {

};

// entry function 
static int __init my_init(void) {
    int status;
    // passing 0 will registerd with the first available major number
    //major = register_chrdev(0, my_device, &fops);
    status = register_chrdev(MY_MAJOR, my_device, &fops);
    if (status < 0) {
        pr_err("%s: character device registraion failed\n", my_device);
        return status;
    }
    pr_info("%s: Character device registered, Major number: %d\n", my_device, (int)MY_MAJOR); 
    return 0;
}

// exit function 
static void __exit my_exit(void){
    unregister_chrdev(MY_MAJOR, my_device);
    pr_info("%s: Goodbye, Kernel\n", my_device);
}

module_init(my_init);
module_exit(my_exit);