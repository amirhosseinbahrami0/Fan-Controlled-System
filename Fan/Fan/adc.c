/*
 * adc.c
 *
 * Created: 12/20/2024 12:31:28 PM
 *  Author: Amir
 */ 
#include "avr/io.h" /* To use the ADC Registers */
#include "adc.h"
#include "common_macros.h" /* To use the macros like SET_BIT */
#include <avr/interrupt.h> /* For Timer ISR */




/*******************************************************************************
 *                          Global Variables                                   *
 *******************************************************************************/
/* Global volatile variable used in case interrupt mode is enabled */
volatile uint16 g_adcResult = 0;
/* Global volatile variable flag used for Lm-Sensor in case interrupt mode is enabled */
volatile uint8 g_lmsensor;



/*******************************************************************************
 *                          ISR's Definitions                                  *
 *******************************************************************************/
/*
 * Interrupt Service Routine Function if interrupt is enable for ADC.
 */
ISR(ADC_vect)
{
	/* Read ADC Data after conversion complete */
	g_adcResult = ADC;
}




/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
void ADC_init(const adc_config * Config_Ptr)
{
	/* ADMUX Register Bits Description:
	 * REFS1:0 = 00 to choose to connect external reference voltage by input this voltage through AREF pin
	 * ADLAR   = 0 right adjusted
	 * MUX4:0  = 00000 to choose channel 0 as initialization
	 */
	ADMUX |= (Config_Ptr->vref) | (Config_Ptr->adalr) ;

	/* ADCSRA Register Bits Description:
	 * ADEN    = 1 Enable ADC
	 * ADIE    = 0 Disable ADC Interrupt
	 * ADATE   = 0 Disable Auto Trigger
	 * ADPS2:0 = 011 to choose ADC_Clock=F_CPU/8=1Mhz/8=125Khz --> ADC must operate in range 50-200Khz
	 */
	ADCSRA = (Config_Ptr->enable) | (Config_Ptr->mode) | (Config_Ptr->prescaler) | (Config_Ptr->autotrigger);
	/* Global volatile varibale flag set in case interrupt mode is enabled for Lm-Sensor */
	if(((Config_Ptr->mode)==INTERRUPT_MODE))
	{
		g_lmsensor=1;
	}
	else
	{
		g_lmsensor=0;
	}
}


/*
 * Description :
 * Function responsible for read analog data from a certain ADC channel in case of pooling mode.
 * and convert it to digital using the ADC driver.
 */
uint16 ADC_readChannel_pooling(ADMUX_SEL channel_num)
{
	channel_num &= 0x07; /* Input channel number must be from 0 --> 7 */
	ADMUX &= 0xE0; /* Clear first 5 bits in the ADMUX (channel number MUX4:0 bits) before set the required channel */
	ADMUX = ADMUX | channel_num; /* Choose the correct channel by setting the channel number in MUX4:0 bits */

	SET_BIT(ADCSRA,ADSC); /* Start conversion write '1' to ADSC */

	while(BIT_IS_CLEAR(ADCSRA,ADIF)); /* Wait for conversion to complete, ADIF becomes '1' */


	SET_BIT(ADCSRA,ADIF); /* Clear ADIF by write '1' to it :) */


	return ADC; /* Read the digital value from the data register */
}


/*
 * Description :
 * Function responsible for read analog data from a certain ADC channel in case of interrupt mode.
 * and convert it to digital using the ADC driver.
 */
void ADC_readChannel_interrupt(ADMUX_SEL channel_num)
{
	channel_num &= 0x07; /* Input channel number must be from 0 --> 7 */
	ADMUX &= 0xE0; /* Clear first 5 bits in the ADMUX (channel number MUX4:0 bits) before set the required channel */
	ADMUX = ADMUX | channel_num; /* Choose the correct channel by setting the channel number in MUX4:0 bits */
	SET_BIT(ADCSRA,ADSC); /* Start conversion write '1' to ADSC */
}