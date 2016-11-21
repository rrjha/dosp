/* adc.h - ADC definitions */

#define REG(x) (*((volatile uint32 *)(x)))

typedef volatile uint32 reg32;

struct adc_csreg {
	reg32 revision;			/*	0h 	*/
	reg32 resereved1[3];
	reg32 sysconfig;		/*	10h	*/
	reg32 reserved2[4];
	reg32 irqstatus_raw;	/*	24h	*/
	reg32 irqstatus;		/*	28h	*/
	reg32 irqenable_set;	/*	2Ch	*/
	reg32 irqenable_clr;	/*	30h	*/
	reg32 irqwakeup;		/*	34h	*/
	reg32 dmaenable_set;	/*	38h	*/
	reg32 dmaenable_clr;	/*	3Ch	*/
	reg32 ctrl;			    /*	40h	*/
	reg32 adcstat;			/*	44h	*/
	reg32 adcrange;			/*	48h	*/
	reg32 adc_clkdiv;		/*	4Ch	*/
	reg32 adc_misc;			/*	50h	*/
	reg32 stepenable;		/*	54h	*/
	reg32 idleconfig;		/*	58h	*/
	reg32 ts_charge_stepconfig;	/*	5Ch	*/
	reg32 ts_charge_delay;		/*	60h	*/
	reg32 stepconfig1;		/*	64h	*/
	reg32 stepdelay1;		/*	68h	*/
	reg32 stepconfig2;		/*	6Ch	*/
	reg32 stepdelay2;		/*	70h	*/
	reg32 stepconfig3;		/*	74h	*/
	reg32 stepdelay3;		/*	78h	*/
	reg32 stepconfig4;		/*	7Ch	*/
	reg32 stepdelay4;		/*	80h	*/
	reg32 stepconfig5;		/*	84h	*/
	reg32 stepdelay5;		/*	88h	*/
	reg32 stepconfig6;		/*	8Ch	*/
	reg32 stepdelay6;		/*	90h	*/
	reg32 stepconfig7;		/*	94h	*/
	reg32 stepdelay7;		/*	98h	*/
	reg32 stepconfig8;		/*	9Ch	*/
	reg32 stepdelay8;		/*	A0h	*/
	reg32 stepconfig9;		/*	A4h	*/
	reg32 stepdelay9;		/*	A8h	*/
	reg32 stepconfig10;		/*	ACh	*/
	reg32 stepdelay10;		/*	B0h	*/
	reg32 stepconfig11;		/*	B4h	*/
	reg32 stepdelay11;		/*	B8h	*/
	reg32 stepconfig12;		/*	BCh	*/
	reg32 stepdelay12;		/*	C0h	*/
	reg32 stepconfig13;		/*	C4h	*/
	reg32 stepdelay13;		/*	C8h	*/
	reg32 stepconfig14;		/*	CCh	*/
	reg32 stepdelay14;		/*	D0h	*/
	reg32 stepconfig15;		/*	D4h	*/
	reg32 stepdelay15;		/*	D8h	*/
	reg32 stepconfig16;		/*	DCh	*/
	reg32 stepdelay16;		/*	E0h	*/
	reg32 fifo0count;		/*	E4h	*/
	reg32 fifo0threshold;	/*	E8h	*/
	reg32 dma0req;			/*	ECh	*/
	reg32 fifo1count;		/*	F0h	*/
	reg32 fifo1threshold;	/*	F4h	*/
	reg32 dma1req;			/*	F8h	*/
	reg32 reserved3;
	reg32 fifo0data;		/*	100h*/
	reg32 fifo1data			/*	200h*/
};


/* ADC Clock control */
#define AM335X_ADC_CLKCTRL_ADDR	    0x44E004BC
#define AM335X_ADC_CLKCTRL_EN	    0x00000002
#define AM335X_ADC_CLKCTRL_EN_MASK  0x00000003

/* ADC Pin Config */
#define AM335X_ADC_AIN0_CONTROL     0x44E10B2C
#define AM335X_ADC_AIN1_CONTROL     0x44E10B28
#define AM335X_ADC_AIN2_CONTROL     0x44E10B24
#define AM335X_ADC_AIN3_CONTROL     0x44E10B20
#define AM335X_ADC_AIN4_CONTROL     0x44E10B1C
#define AM335X_ADC_AIN5_CONTROL     0x44E10B18
#define AM335X_ADC_AIN6_CONTROL     0x44E10B14
#define AM335X_ADC_AIN7_CONTROL     0x44E10B10
#define AM335X_ADC_VREFP_CONTROL    0x44E10B30
#define AM335X_ADC_VREFN_CONTROL    0x44E10B34
#define AM335X_ADC_RX_EN            0x00000020

/* ADC Masks */
#define ADC_WRITE_PROTECT_DISABLE   0x00000004
#define ADC_NO_IDLE_MODE            0x00000004
#define ADC_MODULE_DIS              0x00000000
#define ADC_MODULE_EN               0x00000001
#define ADC_MODULE_EN_MASK          0x00000001
#define ADC_STEP1_ENABLE            0x00000002
#define ADC_FIFO0COUNT_MASK         0x0000007F
#define ADC_FIFODATA_MASK           0x00000FFF

/* ADC FCLK */
#define ADC_MODULE_CLK              24000000
#define ADC_AFE_CLK                 100000

/* ADC STEP Configuration */
/* VREFN, AIN0, VREFP, Data in FIFO 16 samples average, SW enabled, one shot */
#define ADC_STEP_CONFIG_VAL         0x1803010
#define ADC_CHANNEL_START_POS       19

/* Conversion factor - VREF/STEPS */
#define ADC_VREF                    1800
#define ADC_STEPS                   4096
