/*
 * adc.h
 *
 * Created: 12/20/2024 12:31:08 PM
 *  Author: Amir
 */ 


#ifndef ADC_H_
#define ADC_H_

#include "std_types.h"


#define ADC_MAXIMUM_VALUE     1023
#define ADC_REF_VOL_VALUE     2.56


extern volatile uint16 g_adcResult;
extern volatile uint8  g_lmsensor;


/*
 * Description :
 * Select the way VREF is selected
 * 1.AREF_SEL : VREF is entered throw and external AREF PIN in micro-controller.
 * 2.AVCC_SEL : VREF is the same as AVCC (5v analog).
 * 3.INTERNAL_SEL : VREF is 2.56v from internal micro-controller circuit.
 * Done using (ADMUX) Register.
 */
typedef enum{
	AREF_SEL=0x00 , AVCC_SEL=0x40 , INTERNAL_SEL=0xC0
}VREF_SEL;


/*
 * Description :
 * Adjust ADC DATA Register right/left using (ADMUX) Register.
 */
typedef enum{
	ADLAR_RIGHT=0x00 , ADLAR_LEFT=0x20
}ADLAR_SEL;


/*
 * Description :
 * Select ADC channel to convert it to digital according to channel number with (ADMUX) Register.
 */
typedef enum{
	ADC0,ADC1,ADC2,ADC3,ADC4,ADC5,ADC6,ADC7
}ADMUX_SEL;


/*
 * Description :
 * Enable/Disable ADC using (ADCSRA) Register.
 */
typedef enum{
	ADC_DISABLED,ADC_ENABLED=0x80
}ADC_ENABLE;


/*
 * Description :
 * Select ADC channel to work with pooling/interrupt mode with (ADCSRA) Register.
 */
typedef enum{
	POOLING_MODE=0x00 ,INTERRUPT_MODE=0x08
}ADC_MODE;


/*
 * Description :
 * Select ADC Auto-Trigger mode(source) to of ADC in (SFIOR) Register.
 */
typedef enum{
	AUTO_TRIGGER_DISABLED,ANALOG_COMP=0x20,EXTERNAL_INT0=0x40,TIMER0_CMP=0x60,TIMER0_OVF=0x80,TIMER1_CMPB=0xA0,TIMER1_OVF=0xC0,TIMERR1_CAPTURE=0xE0
}ADC_AUTOTRIGGER;


/*
 * Description :
 * Select ADC clock (F_ADC=F_CPU/Prescaler) with (ADCSRA) Register.
 */
typedef enum{
	FCPU_2,FCPU_4=0x02,FCPU_8=0x03,FCPU_16=0x04,FCPU_32=0x05,FCPU_64=0x06,FCPU_128=0x07
}ADC_PRESCALER;


/*
 * Description :
 * Structure variable passed to ADC_init function as parameter in order to initialize the ADC
 * 1.AREF : VREF is entered throw and external AREF PIN in micro-controller.
 * 2.AVCC : VREF is the same as AVCC (5v analog).
 * 3.INTERNAL : VREF is 2.56v from internal micro-controller circuit.
 */
typedef struct{
	VREF_SEL vref;
	ADLAR_SEL adalr;
	ADC_ENABLE enable;
	ADC_MODE mode;
	ADC_AUTOTRIGGER autotrigger;
	ADC_PRESCALER prescaler;
}adc_config;


/*
 * Description :
 * Function responsible for initialize the ADC driver.
 */
void ADC_init(const adc_config * Config_Ptr);


/*
 * Description :
 * Function responsible for read analog data from a certain ADC channel in case of pooling mode.
 * and convert it to digital using the ADC driver.
 */
uint16 ADC_readChannel_pooling(ADMUX_SEL channel_num);



/*
 * Description :
 * Function responsible for read analog data from a certain ADC channel in case of interrupt mode.
 * and convert it to digital using the ADC driver.
 */
void ADC_readChannel_interrupt(ADMUX_SEL channel_num);

#endif /* ADC_H_ */