//
// Created by Emil Vannebäck on 2017-10-02.
//

#include "htable_wrapper.h"
#include <asm/semaphore.h>

DEFINE_HASHTABLE(kvs_htable, HASHTABLE_SIZE);

int table_put(struct kvs_msg *message) {
	struct kvs_htable_entry *temp;
	struct kvs_htable_entry *entry;

	temp = table_get(message);
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
		hash_add(kvs_htable, &entry->hash_list, entry->key);
	}
	return 1;
}

struct kvs_htable_entry* table_get(struct kvs_msg *message){
	struct kvs_htable_entry *temp;
	//struct kvs_msg *ret_msg;

	hash_for_each_possible(kvs_htable,temp,hash_list,message->key){
		if(message->key == temp->key){
			/*ret_msg = (struct kvs_msg*)kmalloc(sizeof(ret_msg),GFP_KERNEL);
			ret_msg->value = kmalloc(temp->value_size,GFP_KERNEL);

			memcpy(ret_msg->value,temp->value,temp->value_size);
			ret_msg->value_size=temp->value_size;
			ret_msg->key=temp->key;
			ret_msg->command=KVS_COMMAND_SUC;*/
			return temp;
		}
	}
	return NULL;
}

int table_del(struct kvs_msg *message){
	struct kvs_htable_entry *temp;
	temp = table_get(message);
	if(temp==NULL){
		return -1;
	} else {
		hash_del(&temp->hash_list);
		return 0;
	}
}

int store_table(){
	struct kvs_htable_entry *temp;

	hash_for_each_possible(kvs_htable,temp,hash_list,message->key){
		//todo insert to file
	}
	return 1;
}

int load_table(){
	struct kvs_htable_entry *entry;
	entry = kmalloc(sizeof(*entry), GFP_KERNEL);
	if (!entry) {
		return -ENOMEM;
	}
	//TODO RETREIVE VALUE
	/*entry->value = kmalloc(, GFP_KERNEL);
	if (!entry->value) {
		return -ENOMEM;
	}
	memcpy(entry->value, message->value, message->value_size);

	entry->key = message->key;
	entry->value_size = message->value_size;
	hash_add(kvs_htable, &entry->hash_list, entry->key);*/
	return 1;

}



