/* adcread.c - adcread */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  adcread  -  Read one converted word from ADC
 *------------------------------------------------------------------------
 */
devcall	adcread(
	  struct dentry	*devptr,	/* Entry in device switch table	*/
	  int32	*adcval,			/* Converted word - out param	*/
	  int32	pin 			    /* ADC pin to read - AIN1 only  */
	)
{
    struct adc_csreg *adcreg = (struct	adc_csreg *)devptr->dvcsr;
    int32 retries = 6;
    uint32 sample = 0;
    uint32 stepconfval = (ADC_STEP_CONFIG_VAL | (pin << ADC_CHANNEL_START_POS));

    if((pin < 0) || (pin > 7))
        return SYSERR;

    kprintf("stepconfig = 0x%X\n", stepconfval);

    /* Enable step 1 */
    adcreg->stepenable = ADC_STEP1_ENABLE;

    /* VREFN, AINX, VREFP, Data in FIFO 16 samples average, SW enabled, one shot */
    adcreg->stepconfig1 = stepconfval;

    /* TSC_ADC_SS module enabled */
    adcreg->ctrl = ADC_MODULE_EN;

    /* Waiting for MODULEMODE field to reflect the written value. */
    while(ADC_MODULE_DIS == (adcreg->ctrl & ADC_MODULE_EN_MASK));

    /* Now loop to check result ready */
    while((retries-- > 0) &&
        (adcreg->fifo0count & ADC_FIFO0COUNT_MASK) == 0)
            sleepms(1);
    if(retries > 0) {
        /* Read data from fifo 0 */
        sample = (adcreg->fifo0data & ADC_FIFODATA_MASK);
        *adcval = (sample * ADC_VREF)/ADC_STEPS;
    }
    else
        return SYSERR;

    return OK;
}
