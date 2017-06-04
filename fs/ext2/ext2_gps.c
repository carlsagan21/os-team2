#include <linux/fs.h>
#include "ext2.h"

int ext2_set_gps_location(struct inode *inode)
{
	// copy the current device location to the inode
	printk(KERN_DEBUG "[soo] ext2_set_gps_location\n");
	return 0;
}

int ext2_get_gps_location(struct inode *inode, struct gps_location *loc)
{
	// copy the inode location to the buffer
	printk(KERN_DEBUG "[soo] ext2_get_gps_location\n");
	// struct gps_location tmp_loc;
	// __u32 i_lat_integer;
	// __u32 i_lat_fractional;
	// __u32 i_lng_integer;
	// __u32 i_lng_fractional;
	// __u32 i_accuracy;
	struct ext2_inode_info *i_info = EXT2_I(inode);

	printk(KERN_DEBUG "[soo] inode_info loc: %d, %d, %d, %d, %d", i_info->i_lat_integer, i_info->i_lat_fractional, i_info->i_lng_integer, i_info->i_lng_fractional, i_info->i_accuracy);
	printk(KERN_DEBUG "[soo] inode_info loc done");

	memcpy(&loc->lat_integer, &i_info->i_lat_integer, sizeof(int));
	memcpy(&loc->lat_fractional, &i_info->i_lat_fractional, sizeof(int));
	memcpy(&loc->lng_integer, &i_info->i_lng_integer, sizeof(int));
	memcpy(&loc->lng_fractional, &i_info->i_lng_fractional, sizeof(int));
	memcpy(&loc->accuracy, &i_info->i_accuracy, sizeof(int));
	// memcpy(location, &tmp_loc, sizeof(tmp_loc));
	printk(KERN_DEBUG "[soo] ext2_get_gps_location done");
	return 0;
}
