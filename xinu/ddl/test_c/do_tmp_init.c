#include<xinu.h>
int32 do_tmp_init(){
int32 p=3, res;
if((p<0)||(p>7)){
printf("Outside pin range..tmp_init");
return SYSERR;
}
res = tmp_init(3);
return res;
}