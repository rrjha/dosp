#include<xinu.h>
int32 do_accel_init(){
int32 res;
res = init(ACCEL);
return res;
}