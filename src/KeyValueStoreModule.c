#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/fs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("c13enn & c13evk");
MODULE_DESCRIPTION("A key-value store.");

static struct kobject* kobj;
static int test;

static ssize_t test_show(struct kobject *obj, struct kobj_attribute *attr, char* buf)
{
	return sprintf(buf, "%d\n",test);
}

static ssize_t test_store(struct kobject* obj, struct kobj_attribute *attr, char* buf, size_t count)
{
	sscanf(buf, "%du", &test);
	return count;
}

static struct kobj_attribute test_attribute = __ATTR(test,0660,test_show, test_store); 

static int __init key_value_store_init(void)
{
    	int error = 0;
    
   	printk(KERN_INFO "Init key-value store.\n");
    	kobj = kobject_create_and_add("KeyValueStore", kernel_kobj);
    	if(!kobj) {
        	printk(KERN_INFO "ERROR");
		return -ENOMEM;
   	}

	error = sysfs_create_file(kobj, &test_attribute.attr);


   	return error;
}

static void __exit key_value_store_cleanup(void)
{
	
    kobject_put(kobj);
    printk(KERN_INFO "Cleaning up key-value store.\n");
}

module_init(key_value_store_init);
module_exit(key_value_store_cleanup);
