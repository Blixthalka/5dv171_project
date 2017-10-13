//
// Created by Emil Vannebäck on 2017-10-02.
//

#include "htable_wrapper.h"
#include <linux/fs.h>
#include <asm/segment.h>
#include <asm/uaccess.h>
#include <linux/buffer_head.h>

DEFINE_HASHTABLE(kvs_htable, HASHTABLE_SIZE);
struct file *open_file(const char *path, int flags, int rights);
void file_close(struct file *file);
int file_read(struct file *file, unsigned long long offset, unsigned char *data, unsigned int size);
int file_write(struct file *file, unsigned long long offset, unsigned char *data, unsigned int size);
int file_sync(struct file *file);


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

int store_htable(void){
	int i;
	struct kvs_htable_entry *temp;
	struct kvs_msg msg;
	char *data;
	struct file *temp_file;
	unsigned long long offset=0;
	unsigned long long size;
	msg.command = KVS_COMMAND_PUT;

	temp_file = open_file("/home/.kvs",O_CREAT,S_IRWXU);

	hash_for_each(kvs_htable,i,temp,hash_list)
	{
		if (temp != NULL) {
			size = temp->value_size + sizeof(msg);
			data = kmalloc(size, GFP_KERNEL);
			memcpy(msg.value, temp->value, temp->value_size);
			msg.key = temp->key;
			serialize_kvs_msg(data, &msg);

			file_write(temp_file, offset, data, size);
			offset += size;
			free(data);
		} else {
			printk("temp is null"
			KERN_INFO);
		}
	}
	file_close(temp_file);

	return 1;
}

int load_htable(void){
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

struct file *open_file(const char *path, int flags, int rights){
	struct file *filp = NULL;
	mm_segment_t oldfs;
	int err = 0;

	oldfs = get_fs();
	set_fs(get_ds());
	filp = filp_open(path, flags, rights);
	set_fs(oldfs);
	if (IS_ERR(filp)) {
		err = PTR_ERR(filp);
		return NULL;
	}
	return filp;

}

void file_close(struct file *file)
{
	filp_close(file, NULL);
}

int file_read(struct file *file, unsigned long long offset, unsigned char *data, unsigned int size)
{
	mm_segment_t oldfs;
	int ret;

	oldfs = get_fs();
	set_fs(get_ds());

	ret = vfs_read(file, data, size, &offset);

	set_fs(oldfs);
	return ret;
}

int file_write(struct file *file, unsigned long long offset, unsigned char *data, unsigned int size)
{
	mm_segment_t oldfs;
	int ret;

	oldfs = get_fs();
	set_fs(get_ds());

	ret = vfs_write(file, data, size, &offset);

	set_fs(oldfs);
	return ret;
}

int file_sync(struct file *file)
{
	vfs_fsync(file, 0);
	return 0;
}




