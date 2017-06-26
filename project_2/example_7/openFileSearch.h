#ifndef CHARDEV_H
#define CHARDEV_H

#define IOC_MAGIC 'k' // defines the magic number

#define OFS_PID _IO(IOC_MAGIC,0) // defines our ioctl call
#define OFS_UID _IO(IOC_MAGIC,1) // defines our ioctl call
#define OFS_OWNER _IO(IOC_MAGIC,2) // defines our ioctl call
#define OFS_NAME _IO(IOC_MAGIC,3) // defines our ioctl call

/* 
 * The major device number. We can't rely on dynamic 
 * registration any more, because ioctls need to know 
 * it. 
 */
#define MAJOR_NUM 100

/* 
 * The name of the device file 
 */
#define DEVICE_FILE_NAME "/dev/openFileSearchDev"
#define DEVICE_NAME "/dev/openFileSearchDev"

#define SUCCESS 0

struct ofs_result {
        pid_t pid;
        uid_t uid;
        uid_t owner;
        unsigned short permissions;
        char name[64];
        unsigned int fsize;
        unsigned long inode_no;
};

struct ofs_result results[256];

#endif
