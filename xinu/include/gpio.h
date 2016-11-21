/* gpio.h - GPIO definitions */

#define REG(x) (*((volatile uint32 *)(x)))

typedef volatile uint32 reg32;

struct gpio_csreg {
    reg32 gpio_revision;		/*0h*/
    reg32 reserved1[3];
    reg32 gpio_sysconfig;		/*10h*/
    reg32 reserved2[3];
    reg32 gpio_eoi;			/*20h*/
    reg32 gpio_irqstatus_raw_0;		/*24h*/
    reg32 gpio_irqstatus_raw_1;		/*28h*/
    reg32 gpio_irqstatus_0;		/*2ch*/
    reg32 gpio_irqstatus_1;		/*30h*/
    reg32 gpio_irqstatus_set_0;		/*34h*/
    reg32 gpio_irqstatus_set_1;		/*38h*/
    reg32 gpio_irqstatus_clr_0;		/*3ch*/
    reg32 gpio_irqstatus_clr_1;		/*40h*/
    reg32 gpio_irqwaken_0;		/*44h*/
    reg32 gpio_irqwaken_1;		/*48h*/
    reg32 reserved3[50];
    reg32 gpio_sysstatus;		/*114h*/
    reg32 reserved4[6];
    reg32 gpio_ctrl;			/*130h*/
    reg32 gpio_oe;			/*134h*/
    reg32 gpio_datain;			/*138h*/
    reg32 gpio_dataout;			/*13ch*/
    reg32 gpio_leveldetect0;		/*140h*/
    reg32 gpio_leveldetect1;		/*144h*/
    reg32 gpio_risingdetect;		/*148h*/
    reg32 gpio_fallingdetect;		/*14ch*/
    reg32 gpio_debouncenable;		/*150h*/
    reg32 gpio_debouncingtime;		/*154h*/
    reg32 reserved5[14];
    reg32 gpio_cleardataout;		/*190h*/
    reg32 gpio_setdataout;		/*194h*/
};

/* GPIO ICLK - skip 0 */
#define AM335X_GPIO1_CLKCTRL_ADDR	    0x44E000AC
#define AM335X_GPIO2_CLKCTRL_ADDR	    0x44E000B0
#define AM335X_GPIO3_CLKCTRL_ADDR	    0x44E000B4
#define AM335X_GPIO_CLKCTRL_EN	            0x00000002
#define AM335X_GPIO_CLKCTRL_EN_MASK         0x00000003

/* GPIO Muxing - For Mux requests */
#define AM335X_CONTROL_REGS                 0x44E10000

/* GPIO Bank addresses */
#define AM335X_GPIO0_REGS                   0x44E07000
#define AM335X_GPIO1_REGS                   0x4804C000
#define AM335X_GPIO2_REGS                   0x481AC000
#define AM335X_GPIO3_REGS                   0x481AE000


enum EGPIO_OP
{
    EMUXPIN,
    ESETDIR
};
enum EGPIO_DIR
{
    EGPIO_DIR_OUTPUT,
    EGPIO_DIR_INPUT
};

/* Set direction requires 4 params So we
 * create a pin number dir combo as third param */
 struct setdir_combo {
     uint32 pinNumber;
     enum EGPIO_DIR pinDirection;
 };

 struct write_combo {
    uint32 pinNumber;
    uint32 pinValue;
 };
