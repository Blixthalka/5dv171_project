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

#define __ATTR_STR(_name, _mode, _show, _store) {		\
	.attr = { .name = _name, 				\
		  .mode = VERIFY_OCTAL_PERMISSIONS(_mode) },	\
	.show  = _show,						\
	.store = _store,					\
}	

/**
 * Struct to be stored in hashtable.
 */
struct kvs_htable_entry {
	unsigned int key;
	char *value;
	size_t value_size;
	struct hlist_node hash_list;
};


static struct kobject *module_root;
static struct kobject *store_root; 

static ssize_t htable_show(struct kobject *obj, struct kobj_attribute *attr, char *buf)
{
	return 0;
}

static ssize_t htable_store(struct kobject *obj, struct kobj_attribute *attr, const char *buf, size_t count)
{
	return count;
}
 


static ssize_t insert_store(struct kobject *obj, struct kobj_attribute *attr,
	       			const char *buf, size_t count) 
{
	int error;
	struct kobj_attribute *key_attr;
   	key_attr = kmalloc(sizeof *key_attr, GFP_KERNEL);
	key_attr->attr.name = kmalloc(strlen(buf), GFP_KERNEL);
	strcpy(key_attr->attr.name, buf);
	key_attr->attr.mode = 0660;
	key_attr->show  = htable_show;
	key_attr->store = htable_store;
       	
	error = sysfs_create_file(store_root,  &key_attr.attr);
	return count;
}

static struct kobj_attribute insert_attr = __ATTR(insert, 0660, NULL, insert_store);

static int __init kvs_init(void)
{
    	int error = 0;

   	printk(KERN_INFO "Init key-value store.\n");

    	module_root = kobject_create_and_add("key_value_store", kernel_kobj);
    	if(!module_root) 
		return -ENOMEM;
   	
	store_root = kobject_create_and_add("store", module_root);
	if(!store_root)
		return -ENOMEM;

	error = sysfs_create_file(module_root, &insert_attr.attr);

   	return error;
}

static void __exit kvs_cleanup(void)
{
	kobject_put(store_root);
    	kobject_put(module_root);
   	printk(KERN_INFO "Cleaning up key-value store.\n");
}

module_init(kvs_init);
module_exit(kvs_cleanup);
