/*  main.c  - main */

#include <xinu.h>

process	main(void)
{
    struct accel_data data = {0, 0, 0};
	recvclr();

	// Init Accel
	accel_init();

    // Now try to read sensor data
    accel_read(&data);

    //display
    printf("X=%d, Y=%d, Z=%d\n", data.x, data.y, data.z);

	return OK;
}
