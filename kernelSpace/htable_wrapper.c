#include "htable_wrapper.h"
#include <linux/fs.h>
#include <asm/segment.h>
#include <asm/uaccess.h>
#include <linux/buffer_head.h>

DEFINE_HASHTABLE(kvs_htable, HASHTABLE_SIZE);

int htable_put(struct kvs_msg *message) {
	struct kvs_htable_entry *temp;
	struct kvs_htable_entry *entry;

	temp = htable_get(message);
	if(temp!=NULL){
		kfree(temp->value);
		temp->value = kmalloc(message->value_size,GFP_KERNEL);
		temp->value_size =message->value_size;
		memcpy(temp->value,message->value,temp->value_size);

	} else {
		entry = kmalloc(sizeof(*entry), GFP_KERNEL);

		if (!entry) {
			return -ENOMEM;
		}

		entry->value = kmalloc(message->value_size, GFP_KERNEL);
		if (!entry->value) {
			return -ENOMEM;
		}
		memcpy(entry->value, message->value, message->value_size);
		entry->key = message->key;
		entry->value_size = message->value_size;
		hash_add_rcu(kvs_htable, &entry->hash_list, entry->key);
	}
	return 1;
}

struct kvs_htable_entry* htable_get(struct kvs_msg *message){
	struct kvs_htable_entry *temp;

	hash_for_each_possible_rcu(kvs_htable,temp,hash_list,message->key){
		if(message->key == temp->key){
			return temp;
		}
	}
	return NULL;
}

int htable_del(struct kvs_msg *message){
	struct kvs_htable_entry *temp;
	temp = htable_get(message);
	if(temp==NULL){
		return -1;
	} else {
		hash_del_rcu(&temp->hash_list);
		kfree(temp->value);
		kfree(temp);
		return 0;
	}
}

int htable_store(void){
	int i;
	struct kvs_htable_entry *temp;
	struct kvs_msg *msg;
	char *data;
	unsigned long long offset=0;
	unsigned long long size;
	struct file *filp=NULL;
	
	
	filp=file_open(STORE_FILE,O_CREAT|O_RDWR,0644);


	hash_for_each_rcu(kvs_htable,i,temp,hash_list)
	{
		if (temp != NULL) {
			size = temp->value_size + sizeof(msg);
			data = kmalloc(size, GFP_KERNEL);
			msg = kmalloc(sizeof(msg),GFP_KERNEL);
			msg->command = KVS_COMMAND_PUT;
			msg->value_size = temp->value_size;
			msg->key = temp->key;
			msg->value = kmalloc(temp->value_size,GFP_KERNEL);
			memcpy(msg->value, temp->value, temp->value_size);
			serialize_kvs_msg(data, msg);

			file_write(filp,offset,data,size);
			offset += size;
			htable_del(msg);
			kfree(data);
			kfree(msg->value);
			kfree(msg);

		} else {
			printk(KERN_INFO "temp is null");
		}
	}
	file_sync(filp);
	file_close(filp);
	return 1;
}

int htable_load(void){
	struct file *filp 	= NULL;
	char *data 		= NULL;
	int data_size 		= 0;
	int read_size 		= 0;
	struct kvs_msg *msg;

	filp = file_open(STORE_FILE, O_RDWR, 0644);
	if(IS_ERR(filp) || !filp) {
		printk(KERN_INFO "Could not open file\n");
		return -1;
	}


	data_size = read_file_to_buffer(filp, &data);
	printk(KERN_INFO "DATA_SIZE: %d\n", data_size);

	while(read_size < data_size) {
		msg = kmalloc(sizeof(msg), GFP_KERNEL);
		msg->value = kmalloc(get_value_length(&data[read_size]), GFP_KERNEL);

		unserialize_kvs_msg(msg, &data[read_size]);
		read_size += sizeof(msg) + msg->value_size;

		htable_put(msg);

		kfree(msg->value);
		kfree(msg);
	}

	kfree(data);
	file_close(filp);
	return 1;

}





