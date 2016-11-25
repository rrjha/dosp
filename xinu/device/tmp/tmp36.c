/* tmp36.c - Temperature sensor source file */

#include <xinu.h>

int32 adc_pin = -1;

/* check 0 <= adcPin <= 7 */
void tmp_init(int32 adcPin)
{
	/* Initialize the ADC module */
    init(ADC);

    adc_pin = adcPin;
}

/* Reads temperature in degree celsius */

devcall tmp_read(int32 *temp_ptr)
{
    int32 mvSample, res;
    if(adc_pin < 0) {
        kprintf("Tmp init not done. Failed to read temperature\n");
        return SYSERR;
    }

    res = read(ADC, &mvSample, adc_pin);
    if (res != OK) {
        kprintf("Temperature read failed. Device failed to respond\n");
        return SYSERR;
    }

    *temp_ptr = (mvSample - 500)/10;

    return OK;
}

