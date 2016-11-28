#include<xinu.h>
int32 do_tmp_read(int32 *tmp_c){
int32 res;
res = tmp_read(tmp_c);
return res;
}