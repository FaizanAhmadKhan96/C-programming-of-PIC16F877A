#if defined(__PCM__)
#include <16F877A.h>
#device adc=10
#fuses HS,NOWDT,NOPROTECT,NOLVP
#use delay(clock=20000000)
#endif

#include <string.h>
#include <stdlib.h>

float adc_value;
float Vsolar;
float Isolar;
float Psolar;
float Vbattery;
float Ibattery;
float Pbattery;
float PrevVsolar=0;
float PrevIsolar=0;
float PrevPsolar=0;
int pwm;
int pwm_start=10;
int pwm_min=6;
int pwm_max=14;
int delta=1;



void main()
{

pwm=pwm_start;

SETUP_ADC_PORTS(ALL_ANALOG);
setup_adc(ADC_CLOCK_DIV_8);

output_low(PIN_C1);           

setup_ccp1(CCP_PWM);         

setup_timer_2(T2_DIV_BY_1,19,1);

set_adc_channel(0);
delay_us(100);

adc_value = read_adc();

Vsolar = (5.0*adc_value/1023.0)/0.111;

 set_adc_channel(1);
delay_us(100);

adc_value = read_adc();

Isolar = ((5.0/1023.0*adc_value)-2.5)/0.066;

set_adc_channel(2);
delay_us(100);

adc_value = read_adc();

Vbattery = (5.0*adc_value/1023.0)/0.333;

set_adc_channel(3);
delay_us(100);

adc_value = read_adc();

Ibattery = ((5.0/1023.0*adc_value)-2.5)/0.066;

float delta_P=Psolar-PrevPsolar; 
float delta_V=Vsolar-PrevVsolar; 
float delta_I=Isolar-PrevIsolar;
float gradient=delta_P/delta_V; 
int pwm;

if(delta_V!=0)
{ 
if(gradient>0)
{ 
pwm+=delta; 
PrevPsolar=Psolar;
PrevVsolar=Vsolar; 
PrevIsolar=Isolar; 
if (pwm<pwm_min)
{
pwm=pwm_min;
set_pwm1_duty(pwm);
}
else if (pwm>pwm_max || pwm==pwm_max)
{
pwm=pwm_max;
set_pwm1_duty(pwm);
}
else if (pwm<pwm_max && pwm>pwm_min)
{
pwm=pwm;
set_pwm1_duty(pwm);
}
}
else if(gradient<0)
{ 
pwm-=delta; 
PrevPsolar=Psolar; 
PrevVsolar=Vsolar; 
PrevIsolar=Isolar; 
if (pwm<pwm_min)
{
pwm=pwm_min;
set_pwm1_duty(pwm);
}
else if (pwm>pwm_max || pwm==pwm_max)
{
pwm=pwm_max;
set_pwm1_duty(pwm);
}
else if (pwm<pwm_max && pwm>pwm_min)
{
pwm=pwm;
set_pwm1_duty(pwm);
}
} 

else 
{
if(delta_I>0)
{ 
pwm+=delta; 
PrevPsolar=Psolar; 
PrevVsolar=Vsolar; 
PrevIsolar=Isolar;
if (pwm<pwm_min)
{
pwm=pwm_min;
set_pwm1_duty(pwm);
}
else if (pwm>pwm_max || pwm==pwm_max)
{
pwm=pwm_max;
set_pwm1_duty(pwm);
}
else if (pwm<pwm_max && pwm>pwm_min)
{
pwm=pwm;
set_pwm1_duty(pwm);
}
}

else if(delta_I<0)
{ 
pwm-=delta; 
PrevPsolar=Psolar; 
PrevVsolar=Vsolar; 
PrevIsolar=Isolar;
if (pwm<pwm_min)
{
pwm=pwm_min;
set_pwm1_duty(pwm);
}
else if (pwm>pwm_max || pwm==pwm_max)
{
pwm=pwm_max;
set_pwm1_duty(pwm);
}
else if (pwm<pwm_max && pwm>pwm_min)
{
pwm=pwm;
set_pwm1_duty(pwm);
}
}
}
}
}


