/*  main.c  - main */

#include <xinu.h>

process	main(void)
{
	recvclr();

	/* Initialize the GPIO module */
    init(GPIO);

    /* Set the P9-15 pin as output pin */
    control(GPIO, ESETDIR, EP9_15, EGPIO_DIR_OUTPUT);

    while(1)
    {
        /* Turn LED On */
        write(GPIO, EP9_15, 0x1);

        sleep(10);

        /* Turn LED Off */
        write(GPIO, EP9_15, 0x0);

        sleep(5);
    }

	return OK;
}
