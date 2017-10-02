#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <net/sock.h>
#include <net/net_namespace.h>
#include "netlink_kvs.h"
#include "htable_wrapper.h"

# define NETLINK_KVS 17

MODULE_LICENSE("GPL");

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
		put(message,nlh);
	} else if(message->command==KVS_COMMAND_DEL){
		del(message,nlh);
	} else if(message->command==KVS_COMMAND_GET){
		get(message,nlh);
	} else{
		printk(KERN_INFO "invalid message command received");
		kfree(message->value);
		kfree(message);
	}
	kfree(message->value);
	kfree(message);

}

void put(struct kvs_msg *msg, struct nlmsghdr *nlh){
	struct kvs_msg send_msg;
	if(table_put(msg)==0){
		printk(KERN_INFO "Value %s stored on %d\n",msg->value,msg->key);
		send_msg = CREATE_KVS_MSG_SUC;
	} else{
		printk(KERN_INFO "ERROR on store\n");
		send_msg = CREATE_KVS_MSG_ERR;
	}
	send_message(&send_msg,nlh);
}

void del(struct kvs_msg *msg, struct nlmsghdr *nlh){
	struct kvs_msg send_msg;
	if(table_del(msg)==0){
		send_msg = CREATE_KVS_MSG_SUC;
		printk(KERN_INFO "Key %d deleted\n",msg->key);
	} else {
		send_msg = CREATE_KVS_MSG_ERR;
		printk(KERN_INFO "could not find key %d\n",msg->key);
	}
	send_message(&send_msg,nlh);
}

void get(struct kvs_msg *msg, struct nlmsghdr *nlh){
	struct kvs_msg* send_msg;
	struct kvs_msg err_msg;
	send_msg = table_get(msg);
	if(send_msg != NULL){
		printk(KERN_INFO "Retrieved value %s from key %d\n",send_msg.value,send_msg->key);
		send_msg->command=KVS_COMMAND_SUC;
		send_message(send_msg,nlh);
		kfree(send_msg->value);
		kfree(send_msg);
	} else {
		printk(KERN_INFO "Could not find key %d\n",msg->key);
		err_msg = CREATE_KVS_MSG_ERR;
		send_message(&err_msg,nlh);
	}
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
	memcpy(nlmsg_data(nlh), buf, size);

	res = nlmsg_unicast(nl_sk, skb_out, pid);
	if (res < 0) {
		printk(KERN_INFO
		"Error while sending back to user\n");
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
