/* led.h - led declarations */

#define ON  0x1
#define OFF 0x0

extern void led_init(enum EBBB_GPIO);
extern devcall led_on();
extern devcall led_off();
