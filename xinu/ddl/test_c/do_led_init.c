#include<xinu.h>
int32 do_led_init(){
int32 p=8, res;
if((p<0)||(p>46)){
printf("Outside pin range..led_init");
return SYSERR;
}
res = led_init(EP9_8);
return res;
}