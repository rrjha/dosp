/*  main.c  - main */

#include <xinu.h>

process	main(void)
{
    struct setdir_combo dir = {16, EGPIO_DIR_OUTPUT};
    struct write_combo wc = {16, 0x1};
	recvclr();

    init(GPIO);
    control(GPIO, EMUXPIN, 0x840, 7);
    control(GPIO, ESETDIR, AM335X_GPIO1_REGS, &dir);

    while(1)
    {
        wc.pinValue = 0x1;
        write(GPIO, AM335X_GPIO1_REGS, &wc);


        sleep(10);

        wc.pinValue = 0x0;
        write(GPIO, AM335X_GPIO1_REGS, &wc);

        sleep(5);
    }

	return OK;

}
