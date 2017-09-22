#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("c13enn & c13evk");
MODULE_DESCRIPTION("A key-value store.");

static int __init key_value_store_init(void)
{
    printk(KERN_INFO "Init key-value store.\n");
    example_kobject = kobject_create_and_add("kobject_example", kernel_kobj);
    if(!example_kobject) {
        return -ENOMEM;
    }
    kobject_put(example_kobject);

    return 0;
}

static void __exit key_value_store_cleanup(void)
{
    printk(KERN_INFO "Cleaning up key-value store.\n");
}

module_init(key_value_store_init);
module_exit(key_value_store_cleanup);