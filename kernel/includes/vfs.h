#ifndef CEDILLE_VFS_H
#define CEDILLE_VFS_H
#include <stdint.h>

#define VFS_FILE        0x01
#define VFS_DIRECTORY   0x02
#define VFS_CHARDEVICE  0x03
#define VFS_BLOCKDEVICE 0x04
#define VFS_PIPE        0x05
#define VFS_SYMLINK     0x06
#define VFS_MOUNTPOINT  0x08

typedef struct vfs_fnode
{
	char name[128];
	uint32_t mask;
} vfs_fnode_t;

typedef struct vfs_dnode
{
	char name[128];
	uint32_t mask;
} vfs_dnode_t;

typedef struct vfs_node
{
	uint32_t type;
	uint32_t flags;
	uint32_t uid;
	uint32_t gid;
	uint32_t inode, inode_dev;
	union
	{
		vfs_fnode_t * fnode;
		vfs_dnode_t * dnode;
	};
} vfs_node_t;

int init_vfs();
#endif