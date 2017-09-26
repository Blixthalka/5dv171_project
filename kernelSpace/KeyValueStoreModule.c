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

DEFINE_HASHTABLE(kvs_htable, HASHTABLE_SIZE);

/**
 * Struct to be stored in hashtable.
 */
struct kvs_htable_entry {
	unsigned int key;
	char *value;
	size_t value_size;
	struct hlist_node hash_list;
};


static struct kobject* kobj;

static ssize_t kvs_show(struct kobject *obj, struct kobj_attribute *attr, char *buf)
{
	struct kvs_htable_entry *tmp;
	unsigned int key;

	memcpy(&key, buf, sizeof(key));
	
	printk(KERN_INFO "Fetching key: %u. ", key);

	hash_for_each_possible(kvs_htable, tmp, hash_list, key) {
		if(key == tmp->key) {
			printk(KERN_INFO "Found value: %s\n", tmp->value);
			memcpy(buf, tmp->value, tmp->value_size);
			return tmp->value_size;
		}		
	}	

	printk(KERN_INFO "Did not find any value.");
	return -EINVAL;
}

static ssize_t kvs_store(struct kobject *obj, struct kobj_attribute *attr, const char *buf, size_t count)
{
	struct kvs_htable_entry *entry;
	unsigned int key;

	if(count < sizeof(key))
		return -EINVAL;

	memcpy(&key, buf, sizeof(key));
	
	if(count == sizeof(key)) {
		//delete__htable(key);

	} else {
	//	add_to_htable(key, buf + sizeof(key), count - sizeof(key));
	}

	
	return count;
}

static struct kobj_attribute test_attribute = __ATTR(test, 0660, kvs_show, kvs_store); 

int add_htable(unsigned int key, char *val_buf, size_t count) 
{
	struct kvs_htable_entry *entry;

	entry = kmalloc(sizeof *entry, GFP_KERNEL);
	if(!entry)
		return -ENOMEM;
	entry->value_size = count;
	entry->key = key;
	entry->value = kmalloc(count, GFP_KERNEL);
	memcpy(entry->value, val_buf, count);
	if(!entry->value) {
		kfree(entry);
		return -ENOMEM;
	}
			
	hash_add(kvs_htable, &entry->hash_list, entry->key);
}

static int __init kvs_init(void)
{
    	int error = 0;
	struct kvs_htable_entry *entry;
	char *value_str = "Chicken dinner!\n";

   	printk(KERN_INFO "Init key-value store.\n");

    	kobj = kobject_create_and_add("key_value_store", kernel_kobj);
    	if(!kobj) {
        	printk(KERN_INFO "ERROR");
		return -ENOMEM;
   	}

	error = sysfs_create_file(kobj, &test_attribute.attr);
	// check error here instead? 

	entry = kmalloc(sizeof *entry, GFP_KERNEL);
	entry->key = 1337;
	entry->value_size = strlen(value_str) + 1;
	entry->value = kmalloc(entry->value_size, GFP_KERNEL);

	memcpy(entry->value, value_str, value->size);

	hash_add(kvs_htable, &entry->hash_list, entry->key);

   	return error;
}

static void __exit kvs_cleanup(void)
{
    kobject_put(kobj);
    printk(KERN_INFO "Cleaning up key-value store.\n");
}

module_init(kvs_init);
module_exit(kvs_cleanup);
