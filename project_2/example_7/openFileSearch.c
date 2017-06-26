#include <linux/kernel.h>	/* We're doing kernel work */
#include <linux/module.h>	/* Specifically, a module */
#include <linux/fs.h>
#include <asm/uaccess.h>	/* for get_user and put_user */
#include <linux/sched.h>
#include <linux/rcupdate.h>
#include <linux/fdtable.h>
#include <linux/slab.h>
#include <linux/limits.h>

#include "openFileSearch.h"

#define DRIVER_AUTHOR "Tobias Schaffner <schaffne@hm.edu>"
#define DRIVER_DESC "Open File Search Character Device"

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);

static unsigned int device_open = 0;
static unsigned int result_size = 0;
static unsigned int result_offset = 0;
static unsigned int major_number = 0;

static int ofs_open(struct inode *inode, struct file *file) {
	if (device_open)
		return -EBUSY;

	device_open++;
	try_module_get(THIS_MODULE);
	printk(KERN_INFO "Device_opened! (%p)\n", file);
	return SUCCESS;
}

static int ofs_release(struct inode *inode, struct file *file) {
	device_open--;
	module_put(THIS_MODULE);
	printk(KERN_INFO "Device_released! (%p,%p)\n", inode, file);
	return SUCCESS;
}

static ssize_t ofs_read(struct file *file, char __user * buffer, size_t length,	loff_t * offset) {
	size_t size = length;

	if (result_size == 0)
		return -ESRCH;

	if (result_offset == result_size)
		return 0;

	if (length > result_size - result_offset)
		size = result_size - result_offset;

	copy_to_user(buffer, results, size*sizeof(struct ofs_result));
	result_offset += size;

	return size;
}

long ofs_ioctl(struct file *file, unsigned int ofs_id, unsigned long arg) {
	struct task_struct* task;
	struct fdtable* files_table;
	struct path* files_path;
	char* char_buffer = kmalloc(PATH_MAX, GFP_KERNEL);

	rcu_read_lock();

	result_size = 0;
	result_offset = 0;

	for_each_process(task) {
		if ((ofs_id == OFS_PID && task->pid == arg) ||
		    (ofs_id == OFS_UID && task->cred->uid.val == arg) ||
		     ofs_id == OFS_OWNER ||
		     ofs_id == OFS_NAME) {
			int i = 0;
			files_table = files_fdtable(task->files);
			while(files_table->fd[i] != NULL) {

				if (result_size >= 256)
					return SUCCESS;

				files_path = &files_table->fd[i]->f_path;
				if (IS_ERR(files_path) || !files_path) {
					printk(KERN_INFO "Path Error!");
					return 1;

				}

				// Get the needed file informations.
				results[result_size].pid = task->pid;
				results[result_size].uid = task->cred->uid.val;
				results[result_size].owner = files_path->dentry->d_inode->i_uid.val;
				results[result_size].permissions = files_path->dentry->d_inode->i_mode;
				strncpy(results[result_size].name, d_path(files_path, char_buffer, PATH_MAX), 64);
				results[result_size].name[63] = '\0';
				results[result_size].fsize = files_path->dentry->d_inode->i_size;
				results[result_size].inode_no = files_path->dentry->d_inode->i_ino;

				// Only add the result if it matches the conditions.
				if (ofs_id == OFS_PID ||
				    ofs_id == OFS_UID ||
				    (ofs_id == OFS_OWNER && results[result_size].owner == arg) ||
				    (ofs_id == OFS_NAME && strcmp(results[result_size].name, (char*) arg) == 0))
					result_size++;

				// Increment file descriptor index.
				i++;
			}
		}
	}
	kfree(char_buffer);
	rcu_read_unlock();
	return SUCCESS;
}

struct file_operations Fops = {
	.read = ofs_read,
	.unlocked_ioctl = ofs_ioctl,
	.open = ofs_open,
	.release = ofs_release,
};

int init_module() {
	int ret_val;

	ret_val = register_chrdev(0, DEVICE_NAME, &Fops);

	if (ret_val < 0) {
		printk(KERN_ALERT "%s failed with %d\n",
		       "Sorry, registering the character device ", ret_val);
		return ret_val;
	}

	major_number = ret_val;

	printk(KERN_INFO "Device Registration was successfull!\nThe major device number is %d.\n", major_number);
	printk(KERN_INFO "Execute:\n");
	printk(KERN_INFO "    mknod %s c %d 0\n", DEVICE_FILE_NAME, major_number);
	printk(KERN_INFO "to create a device file.\n");

	return 0;
}

void cleanup_module() {
	unregister_chrdev(major_number, DEVICE_NAME);
}
