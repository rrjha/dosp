/*  main.c  - main */

#include <xinu.h>

process	main(void)
{
	recvclr();

	/* Initialize the LED */
    led_init(EP9_15);

    while(1)
    {
        /* Turn LED On */
        led_on();

        sleep(10);

        /* Turn LED Off */
        led_off();

        sleep(5);
    }

	return OK;
}
