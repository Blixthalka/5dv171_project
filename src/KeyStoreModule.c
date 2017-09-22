#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/sysdev.h>
#include <linux/major.h>
#include <asm/uaccess.h>
#include <linux/slab.h>
#include <linux/cdev.h>
#include <linux/kdev_t.h>

// Global variable for attribute value
static char *gvar = NULL;

// Store and Show functions……
static ssize_t attr1_store(struct class *cls, struct class_attribute *attr, const char *buf, size_t count);
static ssize_t attr1_show(struct class *cls, struct class_attribute *attr, char *buf);

// Attributes declaration: Here I have declared only one attribute attr1
static struct class_attribute pwm_class_attrs[] = {
        __ATTR(attr1, S_IRUGO | S_IWUSR , attr1_show, attr1_store), //use macro for permission
        __ATTR_NULL
};

//Struct class is basic struct needed to create classs attributes.”dev_jes” is the directory under /sys/class.
static struct class pwm_class =
{
.name = “dev_jes”,
.owner = THIS_MODULE,
.class_attrs = pwm_class_attrs
};

//Module initialization.
static int hello_init(void)
{
    char string[] = “nothing”;
    gvar = kmalloc(sizeof(char)*strlen(string), GFP_KERNEL);
    class_register(&pwm_class);
    snprintf(gvar, sizeof(char)*strlen(string)+1, “%s”, string);
    printk(“jeshwanth In hello_init function \n”);
    return 0;
}

// class attribute show function.
static ssize_t attr1_show(struct class *cls, struct class_attribute *attr, char *buf)
{

    printk(“In attr1_show function\n”);
    printk(“%s\n”, gvar);
    return sprintf(buf, “%s”, gvar);
}

//class attribute store function
static ssize_t attr1_store(struct class *cls, struct class_attribute *attr, const char *buf, size_t count)
{
    printk(“the string is : %s count %d\n”, buf, count);
    return snprintf(gvar, count, “%s\n”, buf);
}

// module exit function.
static void  hello_exit(void)
{
    class_unregister(&pwm_class);
    printk(“In hello_exit function \n”);
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE(“GPL”);
MODULE_AUTHOR(“Jeshwanth”);
MODULE_DESCRIPTION(“Learning sysfs”);