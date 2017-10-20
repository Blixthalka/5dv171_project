//
// Created by Emil Vannebäck on 2017-10-20.
//

#include "file_handler.h"
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/file.h>
#include <linux/mm.h>
#include <linux/stat.h>
#include <asm/uaccess.h>

int read_file_to_buffer(struct file *filp, char **data) {
	int read_amt = 4096;
	int ret;
	int iter = 0;

	*data = kmalloc(0, GFP_KERNEL);
	do {
		*data = krealloc(*data, read_amt + read_amt * iter, GFP_KERNEL);
		ret = file_read(filp, read_amt * iter, &(*data)[iter * read_amt], read_amt);

		if(ret < 0) {
			kfree(data);
			return -1;
		}

		iter++;
	} while (ret == read_amt);

	return ret + read_amt * (iter - 1);
}

struct file *file_open(const char *path, int flags, int rights){
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
