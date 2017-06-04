#include <linux/kernel.h>
// #include <linux/gps.h>
#include <linux/compiler.h>
#include <linux/slab.h> /* for kmalloc(), GFP_KERNEL. linux/gfp 따로 include 안해도 됨. */
#include <linux/uaccess.h> /* for copy_from_user, copy_to_user, strncpy */
#include <linux/errno.h>
#include <linux/path.h>
#include <linux/namei.h>
#include <linux/fs.h>

// global gps
// shared object lock
// create modify 는 timestamp 를 업데이트 하는 부분을 찾아서
// generic_permission 함수. 오버라이드 하지 않도록 주의
struct gps_location gps;

int sys_set_gps_location(struct gps_location __user *loc)
{
	struct gps_location *k_loc;
	int res;

	// kmalloc
	k_loc = kmalloc(sizeof(struct gps_location), GFP_KERNEL);
	if (k_loc == NULL)
		return -ENOMEM;

	res = copy_from_user(k_loc, loc, sizeof(struct gps_location));
	if (res != 0)
		return -EFAULT;

	printk(KERN_DEBUG "[soo] sys_set_gps_location k_loc: %d, %d, %d, %d, %d\n", k_loc->lat_integer, k_loc->lat_fractional, k_loc->lng_integer, k_loc->lng_fractional, k_loc->accuracy);
	// TODO From system boot to the first sys_gps_location system call, location is uninitialized. In this case, internal location information may have arbitrary value but it should meet the above criteria.

	if (
		k_loc->lat_integer < -90 ||
		k_loc->lat_integer > 90 ||
		k_loc->lat_fractional < 0 ||
		k_loc->lat_fractional > 999999 ||
		(k_loc->lat_integer == 90 && k_loc->lat_fractional > 0) ||
		k_loc->lng_integer < -180 ||
		k_loc->lng_integer > 180 ||
		k_loc->lng_fractional < 0 ||
		k_loc->lng_fractional > 999999 ||
		(k_loc->lng_integer == 180 && k_loc->lng_fractional > 0)
	) {
		return -EINVAL;
	}
	// EPARAM 등
	// value validation
	// kfree
	// Fractional parts for latitude and longitude should be nonnegative and do not exceed 999,999.
	// The valid range for latitude is -90 and +90, and longitude is in the range of -180 and +180.
	// accuracy should be nonnegative integer and the units of accuracy are in meter.
	// From system boot to the first sys_gps_location system call, location is uninitialized.
	// In this case, internal location information may have arbitrary value but it should meet the above criteria.
	return 0;
}

int sys_get_gps_location(const char __user *pathname, struct gps_location __user *loc)
{
	printk(KERN_DEBUG "[soo] sys_get_gps_location: %s, %d, %d, %d, %d, %d\n", pathname, loc->lat_integer, loc->lat_fractional, loc->lng_integer, loc->lng_fractional, loc->accuracy);
	struct inode *inode;
	struct path path;
	int res;
	// TODO kmalloc
	res = kern_path(pathname, LOOKUP_FOLLOW, &path);
	inode = path.dentry->d_inode;
	printk(KERN_DEBUG "[soo] Path name : %s, inode :%lu\n", pathname, inode->i_ino);

	return res;
}
