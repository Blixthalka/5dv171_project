#include "htable_wrapper.h"
#include <linux/fs.h>
#include <asm/segment.h>
#include <asm/uaccess.h>
#include <linux/buffer_head.h>
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/stat.h>

DEFINE_HASHTABLE(kvs_htable, HASHTABLE_SIZE);
#define STORE_FILE "/.kvs"
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
		hash_add_rcu(kvs_htable, &entry->hash_list, entry->key);
	}
	return 1;
}

struct kvs_htable_entry* table_get(struct kvs_msg *message){
	struct kvs_htable_entry *temp;
	//struct kvs_msg *ret_msg;

	hash_for_each_possible_rcu(kvs_htable,temp,hash_list,message->key){
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
		hash_del_rcu(&temp->hash_list);
		kfree(temp->value);
		kfree(temp);
		return 0;
	}
}

static struct file *filp = NULL;

int store_htable(void){
	int i;
	struct kvs_htable_entry *temp;
	struct kvs_msg *msg;
	char *data;
	unsigned long long offset=0;
	unsigned long long size;
	struct file *filp=NULL;
	
	
	filp=open_file(STORE_FILE,O_CREAT|O_RDWR,0644);


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
			table_del(msg);
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

int load_htable(void){
	struct file *filp=NULL;
	char *data;
	int data_size = 0;
	int read_size = 0;
	struct kvs_msg *msg;

	filp = open_file(STORE_FILE, O_RDWR, 0644);
	if(IS_ERR(filp)) {
		printk(KERN_INFO "Could not open file\n");
	}


	data_size = read_file_to_buffer(filp, data);

	while(read_size < data_size) {
		msg = kmalloc(sizeof(msg), GFP_KERNEL);
		msg->value = kmalloc(get_value_length(data));

		unserialize_kvs_msg(msg, &data[read_size]);
		read_size += sizeof(msg) + msg->value_size;

		table_put(msg);

		kfree(msg->value);
		kfree(msg);
	}

	kfree(data);
	file_close(filp);
	return 1;

}

static int read_file_to_buffer(struct file *filp, char *data) {
	char* data = kmalloc(0, GFP_KERNEL);
	int read_amt = 4096;
	int ret;
	int iter = 0;

	do {
		krealloc(data, read_amt + read_amt * iter, GFP_KERNEL);
		ret = file_read(filp, read_amt * iter, &data[iter * read_amt], read_amt);

		if(ret < 0) {
			kfree(data);
			return -1;
		}

		iter++;
	} while (ret == read_amt);

	return 	data_size = ret + read_amt * iter;
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




