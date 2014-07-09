#include <vfs.h>
#include <lib/tree.h>
#include <logging.h>
#include <memory.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
tree_t * vfs_tree; 
vfs_node_t * vfs_root;
void debug_print_node(vfs_node_t * node);

int init_vfs()
{
	printk("ok","Starting kvfsd...\n");
	//Create the tree and ensure its valid
	vfs_tree = tree_create();
	if(vfs_tree == NULL)
	{
		printk("fail","->\tCouldn't create tree\n");
		return 1;
	}
	//Create root node and then set it as root. This is only temporary until
	//A filesystem is mounted on /, otherwise it will contain nothing but a
	//directory entry listing all of the mountpoints.
	vfs_root = malloc(sizeof(vfs_node_t));
	memset(vfs_root,0,sizeof(vfs_node_t));
	vfs_root->type = VFS_DIRECTORY | VFS_MOUNTPOINT;
	vfs_root->dnode = malloc(sizeof(vfs_dnode_t));
	vfs_root->dnode->name[0] = '/';
	tree_set_root(vfs_tree,vfs_root);
	debug_print_node(vfs_root);
	printk("done","Started kvfsd\n");
	return 0;
}

uint8_t vfs_tree_comparator(vfs_node_t * a, vfs_node_t * b)
{
	if(a == b)
	{
		return 1;
	}
	return 0;
}

void debug_print_node(vfs_node_t * node)
{
	if(node->type ==  VFS_DIRECTORY  ||
	   node->type ==  VFS_MOUNTPOINT ||
	   node->type == (VFS_DIRECTORY  |  VFS_MOUNTPOINT))
	{
		printk("dir","\'%s\' of type 0x%x\n",node->dnode->name,node->type);
	}
	else
	{
		printk("file","\'%s\' of type 0x%x\n",node->fnode->name,node->type);
	}
	//Search tree for vfs node
	tree_node_t * tree_node = tree_find(vfs_tree, node, (tree_comparator_t)vfs_tree_comparator);

	if(tree_node == NULL)
	{
		printk("no","\'%s\' is not in the tree\n",node->fnode->name);
	}
	else
	{
		printk("yes","\'%s\' is in the tree (0x%x)\n",node->fnode->name,tree_node);
		printk("yes","->\tIt has %d/%d children\n",
			tree_node->children->length,vfs_tree->nodes - 1);
		
	}
}