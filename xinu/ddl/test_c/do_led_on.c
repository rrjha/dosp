#include<xinu.h>
int32 do_led_on(){
int32 res;
res = led_on();
return res;
}