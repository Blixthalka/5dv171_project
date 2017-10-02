#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netlink.h>
#include <net/sock.h>
#include <net/net_namespace.h>
#include "../common/kvs_protocol.h"

# define NETLINK_KVS 17

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

MODULE_LICENSE("GPL");

int table_put(struct kvs_msg *message, size_t length);
int table_del(struct kvs_msg *message);
int table_get(struct kvs_msg *message, struct nlmsghdr *nlh);
int send_message(struct kvs_msg *msg, struct nlmsghdr *nlh);

static struct sock *nl_sk = NULL;

static void nl_data_ready_callback(struct sk_buff *skb) {
	struct nlmsghdr *nlh = NULL;
	struct kvs_msg *message;
	size_t value_length;
	char* buf;

	if (skb == NULL) {
		printk("skb is NULL \n");
		return;
	}
	nlh = (struct nlmsghdr *) skb->data;
	buf=(char*)nlmsg_data(nlh);

	message = (struct kvs_msg *) kmalloc(sizeof(message),GFP_KERNEL);
	value_length = get_value_length(buf);
	message->value = kmalloc(sizeof(value_length), GFP_KERNEL);
	unserialize_kvs_msg(message,buf);


	if(message->command==KVS_COMMAND_PUT){
		table_put(message,value_length);
	} else if(message->command==KVS_COMMAND_DEL){
		table_del(message);
	} else if(message->command==KVS_COMMAND_GET){
		table_get(message,nlh);
	} else{
		printk(KERN_INFO "invalid message command received");
		kfree(message->value);
		kfree(message);
	}

	printk(KERN_INFO
	"Received netlink message payload: %s\n", (char *) NLMSG_DATA(nlh));
	kfree(message->value);
	kfree(message);

}

int table_put(struct kvs_msg *message, size_t length){
	struct kvs_htable_entry *entry;
	entry = kmalloc(sizeof(*entry),GFP_KERNEL);
	
	printk(KERN_INFO "storing value %s with key %u ...",message->value,message->key);
	if(!entry){
		printk(KERN_INFO "FAILED\n");
		return -ENOMEM;
	}
	entry->value = kmalloc(length,GFP_KERNEL);
	entry->value = message->value;
	if(!entry->value){
		printk(KERN_INFO "FAILED\n");
		kfree(entry);
		return -ENOMEM;
	}

	entry->key = message->key;
	entry->value_size = length;
	hash_add(kvs_htable, &entry->hash_list, entry->key);
	printk(KERN_INFO "SUCCESS\n");
	return 0;

}

int table_del(struct kvs_msg *message){
		struct kvs_htable_entry *temp;

		printk(KERN_INFO "deleting key %u ...",message->key);
		hash_for_each_possible(kvs_htable,temp,hash_list,message->key){
			if(message->key == temp->key){
				hash_del(&temp->hash_list);
				kfree(temp->value);
				kfree(temp);
				printk(KERN_INFO "key deleted\n",temp->value);
				return 0;
			}
		}
		printk(KERN_INFO "did not find key\n");
		return -1;
}

int table_get(struct kvs_msg *message, struct nlmsghdr *nlh){
	struct kvs_htable_entry *temp;
	struct kvs_msg *send_msg;

	printk(KERN_INFO "getting value from key %u ...",message->key);
	hash_for_each_possible(kvs_htable,temp,hash_list,message->key){
		if(message->key == temp->key){
			printk(KERN_INFO "found value %s\n",temp->value);
			send_msg = (struct kvs_msg*)kmalloc(sizeof(send_msg),GFP_KERNEL);
			send_msg->value = kmalloc(strlen(temp->value)+1,GFP_KERNEL);
			send_msg->value=temp->value;
			send_msg->value_size=temp->value_size;
			send_msg->key=temp->key;
			send_msg->command=KVS_COMMAND_GET;
			send_message(send_msg,nlh);

			kfree(send_msg->value);
			kfree(send_msg);
			return 0;
		}
	}
	printk(KERN_INFO "did not find key\n");
	return -1;
}

int send_message(struct kvs_msg *msg, struct nlmsghdr *nlh){
	struct sk_buff *skb_out;
	int pid;
	int res;
	size_t size;
	char* buf;

	pid = nlh->nlmsg_pid;
	size = sizeof(msg) + msg->value_size;
	buf = kmalloc(size,GFP_KERNEL);
	serialize_kvs_msg(buf,msg);
	skb_out = nlmsg_new(size, 0);
	if (!skb_out) {
		printk(KERN_ERR "Failed to allocate new skb\n");
		return -ENOMEM;
	}

	nlh = nlmsg_put(skb_out, 0, 0, NLMSG_DONE, size, 0);
	NETLINK_CB(skb_out).dst_group = 0; /* not in mcast group */
	strncpy(nlmsg_data(nlh), buf, size);

	res = nlmsg_unicast(nl_sk, skb_out, pid);
	if (res < 0) {
		printk(KERN_INFO
		"Error while sending bak to user\n");
		kfree(buf);
		return -1;
	}
	kfree(buf);
	return res;
}

static void netlink_test(void) {
	struct netlink_kernel_cfg cfg = {
		.input = nl_data_ready_callback,
	};
	nl_sk = netlink_kernel_create(&init_net, NETLINK_KVS, &cfg);
	// nl_sk = netlink_kernel_create(&init_net, NETLINK_NITRO, 0, nl_data_ready_callback, NULL, THIS_MODULE);
}

static int __init kvs_init(void) {
	printk(KERN_INFO "Initializing Netlink Socket");
	netlink_test();
	return 0;
}

static void __exit kvs_exit(void) {
	printk(KERN_INFO "Goodbye");
	sock_release(nl_sk->sk_socket);
}

module_init(kvs_init);
module_exit(kvs_exit);
