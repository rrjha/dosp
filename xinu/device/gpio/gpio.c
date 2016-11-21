/* gpioinit.c - gpioinit */

#include <xinu.h>

/*------------------------------------------------------------------------
 * gpioinit  -  GPIO initialization function
 *------------------------------------------------------------------------
 */

devcall	gpioinit (
		 struct	dentry *devptr
		)
{
	/* Enable module clocks - GPIO0 is already enabled */
    REG(AM335X_GPIO1_CLKCTRL_ADDR) |= AM335X_GPIO_CLKCTRL_EN;
    REG(AM335X_GPIO2_CLKCTRL_ADDR) |= AM335X_GPIO_CLKCTRL_EN;
    REG(AM335X_GPIO3_CLKCTRL_ADDR) |= AM335X_GPIO_CLKCTRL_EN;

    /* Waiting for MODULEMODE field to reflect the written value. */
    while((AM335X_GPIO_CLKCTRL_EN != (REG(AM335X_GPIO1_CLKCTRL_ADDR) & AM335X_GPIO_CLKCTRL_EN_MASK)) &&
        (AM335X_GPIO_CLKCTRL_EN != (REG(AM335X_GPIO2_CLKCTRL_ADDR) & AM335X_GPIO_CLKCTRL_EN_MASK)) &&
        (AM335X_GPIO_CLKCTRL_EN != (REG(AM335X_GPIO3_CLKCTRL_ADDR) & AM335X_GPIO_CLKCTRL_EN_MASK)));

    return OK;
}

/* Control requests must follow the correct sequence of parameters */
/* Op:
 *  Mux -
 *      value1 = offset from AM335X_CONTROL_REGS
 *      value2 = mode (0-7)
 *  Setdir -
 *      value1 = GPIO base reg for bank
 *      value2 = <pin number, pin direction> combo address
 */
devcall gpioctl (
        struct dentry *devptr,
        uint32 operation,
        uint32 value1,
        uint32 value2
        )
{
	struct	gpio_csreg *gpioreg = NULL;
	struct setdir_combo *combo = NULL;

	switch(operation)
	{
        case EMUXPIN:
            /* All muxing requests must provide offset relative to Control reg base */
            /* To do make this friendly and add checks for address */
            REG(AM335X_CONTROL_REGS + value1) = (value2 & 0x07);
            break;
        case ESETDIR:
            if((value1 != 0) && (value2 != 0)) {
                gpioreg = (struct gpio_csreg *) value1;
                combo = (struct setdir_combo *)value2;

                if(EGPIO_DIR_OUTPUT == combo->pinDirection) {
                    gpioreg->gpio_oe &= ~(1 << combo->pinNumber);
                }
                else {
                    gpioreg->gpio_oe |= (1 << combo->pinNumber);
                }
            }
            break;
        default:
            return SYSERR; //No handling so return error
	}

	return OK;
}

/* GPIO write -
 * param1 (ignored)
 * param2 - gpioBaseAddr: base address of bank of GPIO to write to
 * param3 - write_combo_addr: <pin number, pin value> combo address
 */

devcall gpiowrite (
        struct dentry *devptr,
        uint32 gpioBaseAddr,
        uint32 write_combo_addr)
{
	struct gpio_csreg *gpioreg = NULL;
	struct write_combo *combo = NULL;

	if((gpioBaseAddr !=0) && (write_combo_addr != 0)) {
        gpioreg = (struct gpio_csreg *) gpioBaseAddr;
        combo = (struct write_combo *)write_combo_addr;
	}
    if(0x1 == combo->pinValue) {
        gpioreg->gpio_setdataout = (1 << combo->pinNumber);
    }
    else {
        gpioreg->gpio_cleardataout = (1 << combo->pinNumber);
    }

    return OK;
}

/* GPIO write -
 * param1 (ignored)
 * param2 - gpioBaseAddr: base address of bank of GPIO to write to
 * param3 - pin number to read
 * Return - value of this pin
 */

devcall gpioread (
        struct dentry *devptr,
        uint32 gpioBaseAddr,
        uint32 pinNumber)
{
	struct	gpio_csreg *gpioreg = NULL;
	if (gpioBaseAddr != NULL)
        gpioreg = (struct gpio_csreg *) gpioBaseAddr;

    return(gpioreg->gpio_datain & (1 << pinNumber));
}
