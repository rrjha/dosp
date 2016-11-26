/*  main.c  - main */

#include <xinu.h>

process	main(void)
{
    struct accel_data data = {0, 0, 0};
    int count = 1;
    recvclr();

    // Init Accel
    accel_init();

    while(1) {
        printf("Iteration - %d\n", count++);

        // Now try to read sensor data
        accel_read(&data);

        //display
        printf("X=%d, Y=%d, Z=%d\n\n", data.x, data.y, data.z);

        sleep(3);
    }

    return OK;
}
