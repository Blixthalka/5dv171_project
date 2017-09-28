#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netlink.h>
#include <net/sock.h>
#include <net/net_namespace.h>

# define NETLINK_NITRO 17

MODULE_LICENSE("GPL");


static struct sock *nl_sk = NULL;

static void nl_data_ready_callback(struct sk_buff *skb) {

    struct nlmsghdr *nlh = NULL;
    if (skb == NULL) {
        printk("skb is NULL \n");
        return;
    }
    nlh = (struct nlmsghdr *) skb->data;
    printk(KERN_INFO "Received netlink message payload: %s\n", (char*)NLMSG_DATA(nlh));

}

static void netlink_test(void)
{
	struct netlink_kernel_cfg cfg = {
		.input = nl_data_ready_callback,
	};
	nl_sk = netlink_kernel_create(&init_net, NETLINK_NITRO, &cfg);
   // nl_sk = netlink_kernel_create(&init_net, NETLINK_NITRO, 0, nl_data_ready_callback, NULL, THIS_MODULE);
}

static int __init kvs_init(void)
{
    printk(KERN_INFO "Initializing Netlink Socket");
    netlink_test();
    return 0;
}

static void __exit kvs_exit(void)
{
    printk(KERN_INFO "Goodbye");
    sock_release(nl_sk->sk_socket);
}

module_init(kvs_init);
module_exit(kvs_exit);
