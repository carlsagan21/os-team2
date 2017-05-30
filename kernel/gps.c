#include <linux/gps.h>

// global gps
// shared object lock
// create modify 는 timestamp 를 업데이트 하는 부분을 찾아서
// generic_permission 함수. 오버라이드 하지 않도록 주의
struct gps_location gps;

int set_gps_location(struct gps_location __user *loc)
{
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
