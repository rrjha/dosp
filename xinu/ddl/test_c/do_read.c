#include<xinu.h>
int32 do_accel_read(struct *accel_d){
int32 res;
res = read(accel_d);
return res;
}