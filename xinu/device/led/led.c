#include <xinu.h>

#define ON  0x1
#define OFF 0x0

bool8 ledinit = FALSE;
enum EBBB_GPIO ledpin;

/* In generated HL can we check strings? */
void led_init(enum EBBB_GPIO pinNumber)
{
	/* Initialize the GPIO module */
    init(GPIO);

    /* Set the P9-15 pin as output pin */
    control(GPIO, ESETDIR, (uint32)pinNumber, EGPIO_DIR_OUTPUT);

    ledpin = pinNumber;
    ledinit = TRUE;
}

/* No check */
devcall led_on()
{
    if(!ledinit) {
        kprintf("LED init not done. Failed to switch on LED\n");
        return SYSERR;
    }

    /* Turn LED On */
    write(GPIO, (uint32)ledpin, ON);
    return OK;
}

devcall led_off()
{
    if(!ledinit) {
        kprintf("LED init not done. Failed to switch on LED\n");
        return SYSERR;
    }

    /* Turn LED On */
    write(GPIO, (uint32)ledpin, OFF);
    return OK;
}
