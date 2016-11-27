#include<xinu.h>
int32 do_led_off(){
int32 res;
res = led_off();
return res;
}