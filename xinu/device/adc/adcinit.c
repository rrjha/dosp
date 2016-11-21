/* adcinit.c - adcinit */

#include <xinu.h>

/*------------------------------------------------------------------------
 * adcinit  -  ADC initialization function
 *------------------------------------------------------------------------
 */

devcall	adcinit (
		 struct	dentry *devptr
		)
{
	struct	adc_csreg *adcreg = (struct	adc_csreg *)devptr->dvcsr;

    /* ADC clock enable */
    REG(AM335X_ADC_CLKCTRL_ADDR) |= AM335X_ADC_CLKCTRL_EN;

    /* Waiting for MODULEMODE field to reflect the written value. */
    while(AM335X_ADC_CLKCTRL_EN != (REG(AM335X_ADC_CLKCTRL_ADDR) & AM335X_ADC_CLKCTRL_EN_MASK));

    REG(AM335X_ADC_AIN0_CONTROL)  = AM335X_ADC_RX_EN;
    REG(AM335X_ADC_AIN1_CONTROL)  = AM335X_ADC_RX_EN;
    REG(AM335X_ADC_AIN2_CONTROL)  = AM335X_ADC_RX_EN;
    REG(AM335X_ADC_AIN3_CONTROL)  = AM335X_ADC_RX_EN;
    REG(AM335X_ADC_AIN4_CONTROL)  = AM335X_ADC_RX_EN;
    REG(AM335X_ADC_AIN5_CONTROL)  = AM335X_ADC_RX_EN;
    REG(AM335X_ADC_AIN6_CONTROL)  = AM335X_ADC_RX_EN;
    REG(AM335X_ADC_AIN7_CONTROL)  = AM335X_ADC_RX_EN;
    REG(AM335X_ADC_VREFP_CONTROL) = AM335X_ADC_RX_EN;
    REG(AM335X_ADC_VREFN_CONTROL) = AM335X_ADC_RX_EN;

    /* Unprotect step registers, TSC_ADC_SS module disabled because configuration in progress */
    adcreg->ctrl = ADC_WRITE_PROTECT_DISABLE;

    /* Waiting for MODULEMODE field to reflect the written value. */
    while(ADC_MODULE_DIS != (adcreg->ctrl & ADC_MODULE_EN_MASK));

    /* No idle mode */
    adcreg->sysconfig = ADC_NO_IDLE_MODE;

    /* Set AFE clock */
    adcreg->adc_clkdiv = (ADC_MODULE_CLK/ADC_AFE_CLK);

    return OK;
}
