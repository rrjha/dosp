/*  main.c  - main */

#include <xinu.h>

process	main(void)
{
    int32 mvSample, temp_c, res=SYSERR, times=5;

	recvclr();
	res = init(ADC);
	while(times > 0) {
        read(ADC, &mvSample, 3);
        temp_c = (mvSample - 500)/10;
        printf("Iteration-%d\n", (6 - times));
        printf("The result is %d and sample in mv = %d and temp in deg cel = %d\n\n\n", res, mvSample, temp_c);
        times-=1;
        sleep(5);
	}

	return OK;
}
