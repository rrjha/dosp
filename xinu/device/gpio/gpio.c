/* gpioinit.c - gpioinit */

#include <xinu.h>

/*------------------------------------------------------------------------
 * gpioinit  -  GPIO initialization function
 *------------------------------------------------------------------------
 */

 struct gpio_mapping gpio_map[EGPIO_MAX] = {
 /*038*/   {6,  AM335X_GPIO1_REGS, 0x818},
 /*039*/   {7,  AM335X_GPIO1_REGS, 0x81C},
 /*034*/   {2,  AM335X_GPIO1_REGS, 0x808},
 /*035*/   {3,  AM335X_GPIO1_REGS, 0x80C},
 /*066*/   {2,  AM335X_GPIO2_REGS, 0x890},
 /*067*/   {3,  AM335X_GPIO2_REGS, 0x894},
 /*069*/   {5,  AM335X_GPIO2_REGS, 0x89C},
 /*068*/   {4,  AM335X_GPIO2_REGS, 0x898},
 /*045*/   {13, AM335X_GPIO1_REGS, 0x834},
 /*044*/   {12, AM335X_GPIO1_REGS, 0x830},
 /*023*/   {23, AM335X_GPIO0_REGS, 0x824},
 /*026*/   {26, AM335X_GPIO0_REGS, 0x828},
 /*047*/   {15, AM335X_GPIO1_REGS, 0x83C},
 /*046*/   {14, AM335X_GPIO1_REGS, 0x838},
 /*027*/   {27, AM335X_GPIO0_REGS, 0x82C},
 /*065*/   {1,  AM335X_GPIO2_REGS, 0x88C},
 /*022*/   {22, AM335X_GPIO0_REGS, 0x820},
 /*063*/   {31, AM335X_GPIO1_REGS, 0x884},
 /*062*/   {30, AM335X_GPIO1_REGS, 0x880},
 /*037*/   {5,  AM335X_GPIO1_REGS, 0x814},
 /*036*/   {4,  AM335X_GPIO1_REGS, 0x810},
 /*033*/   {1,  AM335X_GPIO1_REGS, 0x804},
 /*032*/   {0,  AM335X_GPIO1_REGS, 0x800},
 /*061*/   {29, AM335X_GPIO1_REGS, 0x87C},
 /*086*/   {22, AM335X_GPIO2_REGS, 0x8E0},
 /*088*/   {24, AM335X_GPIO2_REGS, 0x8E8},
 /*087*/   {23, AM335X_GPIO2_REGS, 0x8E4},
 /*089*/   {25, AM335X_GPIO2_REGS, 0x8EC},
 /*010*/   {10, AM335X_GPIO0_REGS, 0x8D8},
 /*011*/   {11, AM335X_GPIO0_REGS, 0x8DC},
 /*009*/   {9,  AM335X_GPIO0_REGS, 0x8D4},
 /*081*/   {17, AM335X_GPIO2_REGS, 0x8CC},
 /*008*/   {8,  AM335X_GPIO0_REGS, 0x8D0},
 /*080*/   {16, AM335X_GPIO2_REGS, 0x8C8},
 /*078*/   {14, AM335X_GPIO2_REGS, 0x8C0},
 /*079*/   {15, AM335X_GPIO2_REGS, 0x8C4},
 /*076*/   {12, AM335X_GPIO2_REGS, 0x8B8},
 /*077*/   {13, AM335X_GPIO2_REGS, 0x8BC},
 /*074*/   {10, AM335X_GPIO2_REGS, 0x8B0},
 /*075*/   {11, AM335X_GPIO2_REGS, 0x8B4},
 /*072*/   {8,  AM335X_GPIO2_REGS, 0x8A8},
 /*073*/   {7,  AM335X_GPIO2_REGS, 0x8AC},
 /*070*/   {6,  AM335X_GPIO2_REGS, 0x8A0},
 /*071*/   {7,  AM335X_GPIO2_REGS, 0x8A4},
 /*030*/   {30, AM335X_GPIO0_REGS, 0x870},
 /*060*/   {28, AM335X_GPIO1_REGS, 0x878},
 /*031*/   {31, AM335X_GPIO0_REGS, 0x874},
 /*050*/   {18, AM335X_GPIO1_REGS, 0x848},
 /*048*/   {16, AM335X_GPIO1_REGS, 0x840},
 /*051*/   {19, AM335X_GPIO1_REGS, 0x84C},
 /*005*/   {5,  AM335X_GPIO0_REGS, 0x95C},
 /*004*/   {4,  AM335X_GPIO0_REGS, 0x958},
 /*003*/   {3,  AM335X_GPIO0_REGS, 0x954},
 /*002*/   {2,  AM335X_GPIO0_REGS, 0x950},
 /*049*/   {17, AM335X_GPIO1_REGS, 0x86C},
 /*015*/   {15, AM335X_GPIO0_REGS, 0x984},
 /*117*/   {21, AM335X_GPIO3_REGS, 0x9AC},
 /*014*/   {14, AM335X_GPIO0_REGS, 0x980},
 /*115*/   {19, AM335X_GPIO3_REGS, 0x9A4},
 /*113*/   {17, AM335X_GPIO3_REGS, 0x99C},
 /*111*/   {15, AM335X_GPIO3_REGS, 0x994},
 /*112*/   {16, AM335X_GPIO3_REGS, 0x998},
 /*110*/   {14, AM335X_GPIO3_REGS, 0x990},
 /*020*/   {20, AM335X_GPIO0_REGS, 0x9B4},
 /*007*/   {7,  AM335X_GPIO0_REGS, 0x964}
 };

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

devcall gpioctl (
        struct dentry *devptr,
        uint32 func,
        uint32 headerPin,
        uint32 value2
        )
{
	struct	gpio_csreg *gpioreg = NULL;

	if(headerPin >= EGPIO_MAX)
        return SYSERR;

	switch(func)
	{
        case EMUXPIN:
            REG(AM335X_CONTROL_REGS + gpio_map[headerPin].control_offset) = 0x07;
            break;

        case ESETDIR:
            REG(AM335X_CONTROL_REGS + gpio_map[headerPin].control_offset) = 0x07;
            gpioreg = (struct gpio_csreg *) (gpio_map[headerPin].gpio_bank_addr);

            if(EGPIO_DIR_OUTPUT == value2) {
                gpioreg->gpio_oe &= ~(1 << gpio_map[headerPin].pinNumber);
            }
            else {
                gpioreg->gpio_oe |= (1 << gpio_map[headerPin].pinNumber);
            }
            break;
        default:
            return SYSERR; //No handling so return error
	}

	return OK;
}


devcall gpiowrite (
        struct dentry *devptr,
        uint32 headerPin,
        uint32 pinValue)
{
	struct gpio_csreg *gpioreg = NULL;

	if(headerPin >= EGPIO_MAX)
        return SYSERR;

	gpioreg = (struct gpio_csreg *) (gpio_map[headerPin].gpio_bank_addr);

    if(0x1 == pinValue) {
        gpioreg->gpio_setdataout = (1 << gpio_map[headerPin].pinNumber);
    }
    else {
        gpioreg->gpio_cleardataout = (1 << gpio_map[headerPin].pinNumber);
    }

    return OK;
}

devcall gpioread (
        struct dentry *devptr,
        uint32 headerPin,
        uint32 unused)
{
	struct gpio_csreg *gpioreg = NULL;

	if(headerPin >= EGPIO_MAX)
        return SYSERR;

	gpioreg = (struct gpio_csreg *) (gpio_map[headerPin].gpio_bank_addr);

    return(gpioreg->gpio_datain & (1 << gpio_map[headerPin].pinNumber));
}
