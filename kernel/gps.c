#include <linux/gps.h>
#include <linux/compiler.h>

// global gps
struct gps_location gps;

int sys_set_gps_location(struct gps_location __user *loc)
{
	printk(KERN_DEBUG "[soo] sys_set_gps_location: %d, %d, %d, %d, %d\n", loc->lat_integer, loc->lat_fractional, loc->lng_integer, loc->lng_fractional, loc->accuracy);
	// kmalloc
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
