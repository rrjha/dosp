/*  main.c  - main */

#include <xinu.h>

process	main(void)
{
    int32 mvSample, temp_c, res=SYSERR, times=5;

	recvclr();
	tmp_init(3);
	while(times > 0) {
        printf("Iteration-%d\n", (6 - times));
        res = tmp_read(&temp_c);
        if(res == OK) {
            printf("Temp in deg cel = %d\n\n", temp_c);
        }
        times-=1;
        sleep(5);
	}

    return OK;
}
