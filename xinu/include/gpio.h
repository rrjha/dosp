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
#define AM335X_GPIO1_CLKCTRL_ADDR	0x44E000AC
#define AM335X_GPIO2_CLKCTRL_ADDR	0x44E000B0
#define AM335X_GPIO3_CLKCTRL_ADDR	0x44E000B4
#define AM335X_GPIO_CLKCTRL_EN	        0x00000002
#define AM335X_GPIO_CLKCTRL_EN_MASK     0x00000003

/* GPIO Muxing - For Mux requests */
#define AM335X_CONTROL_REGS             0x44E10000

/* GPIO Bank addresses */
#define AM335X_GPIO0_REGS               0x44E07000
#define AM335X_GPIO1_REGS               0x4804C000
#define AM335X_GPIO2_REGS               0x481AC000
#define AM335X_GPIO3_REGS               0x481AE000


enum EGPIO_FUNC
{
    EMUXPIN,
    ESETDIR
};
enum EGPIO_DIR
{
    EGPIO_DIR_OUTPUT,
    EGPIO_DIR_INPUT
};

 /* GPIOs on BBB */
 enum EBBB_GPIO {
    EP8_3,
    EP8_4,
    EP8_5,
    EP8_6,
    EP8_7,
    EP8_8,
    EP8_9,
    EP8_10,
    EP8_11,
    EP8_12,
    EP8_13,
    EP8_14,
    EP8_15,
    EP8_16,
    EP8_17,
    EP8_18,
    EP8_19,
    EP8_20,
    EP8_21,
    EP8_22,
    EP8_23,
    EP8_24,
    EP8_25,
    EP8_26,
    EP8_27,
    EP8_28,
    EP8_29,
    EP8_30,
    EP8_31,
    EP8_32,
    EP8_33,
    EP8_34,
    EP8_35,
    EP8_36,
    EP8_37,
    EP8_38,
    EP8_39,
    EP8_40,
    EP8_41,
    EP8_42,
    EP8_43,
    EP8_44,
    EP8_45,
    EP8_46,
    EP9_11,
    EP9_12,
    EP9_13,
    EP9_14,
    EP9_15,
    EP9_16,
    EP9_17,
    EP9_18,
    EP9_21,
    EP9_22,
    EP9_23,
    EP9_24,
    EP9_25,
    EP9_26,
    EP9_27,
    EP9_28,
    EP9_29,
    EP9_30,
    EP9_31,
    EP9_41,
    EP9_42,
    EGPIO_MAX
    };

struct gpio_mapping {
    uint32 pinNumber;
    reg32 gpio_bank_addr;
    uint32 control_offset;
};
