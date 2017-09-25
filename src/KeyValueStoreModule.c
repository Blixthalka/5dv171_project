#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/fs.h>
#include <linux/hashtable.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("c13enn & c13evk");
MODULE_DESCRIPTION("A key-value store.");

// size of the hashtable will become 2 ^ HASHTABLE_SIZE
#define HASHTABLE_SIZE 3

DEFINE_HASHTABLE(ksv_htable, HASHTABLE_SIZE);

/**
 * Struct to be stored in hashtable.
 */
struct ksv_htable_entry {
	char *value;
	struct hlist_node hash_list;
};


static struct kobject* kobj;

static ssize_t kvs_show(struct kobject *obj, struct kobj_attribute *attr, char *buf)
{
	struct ksv_htable_entry *tmp; 

	// finds the table entry with key 1337
	hash_for_each_possible(ksv_htable, tmp, hash_list, 1337) {
		strcpy(buf, tmp->value);
		return strlen(buf);		 
	}	

	// some error, since entry was not found..
	return -1;
}

static ssize_t kvs_store(struct kobject *obj, struct kobj_attribute *attr, const char *buf, size_t count)
{
	
	int test = 13;
	sscanf(buf, "%du", &test);
	return count;
}

static struct kobj_attribute test_attribute = __ATTR(test, 0660, kvs_show, kvs_store); 

static int __init kvs_init(void)
{
    	int error = 0;
	struct ksv_htable_entry *entry;
	char *value_str = "Chicken dinner!\n";
	

   	printk(KERN_INFO "Init key-value store.\n");

    	kobj = kobject_create_and_add("KeyValueStore", kernel_kobj);
    	if(!kobj) {
        	printk(KERN_INFO "ERROR");
		return -ENOMEM;
   	}

	error = sysfs_create_file(kobj, &test_attribute.attr);
	// check error here instead? 

	entry = kmalloc(sizeof *entry, GFP_KERNEL);
	entry->value = kmalloc(strlen(value_str) + 1, GFP_KERNEL);
	memcpy(entry->value, value_str, strlen(value_str) + 1);

	hash_add(ksv_htable, &entry->hash_list, 1337);
	

   	return error;
}

static void __exit kvs_cleanup(void)
{
	
    kobject_put(kobj);
    printk(KERN_INFO "Cleaning up key-value store.\n");
}

module_init(kvs_init);
module_exit(kvs_cleanup);
