#if defined(__PCM__)
#include <16F877.h>
#device adc=10
#fuses HS,BROWNOUT,NOWDT,NOLVP
#use delay(clock=20M)
#endif
#include <string.h>
#include <stdlib.h>
#define lcd_rs pin_d0
#define lcd_en pin_d1
#define sd pin_c0
#define on output_high
#define off output_low
#define fan pin_d7
#define relay pin_d6
#define green pin_d5
#define yellow2 pin_d4
#define yellow1 pin_c7
#define red pin_c6
#define WAPDA pin_d3
#define solar pin_d2
#define disp_clear 0b00000001
#define disp_control 0b00001110
#define func_set 0b00111000
#define lscrol 0b00011000
#define rscrol 0b00011100
#define line1 0b10000000
#define line2 0b11000000
float PrevVsolar=0,PrevIsolar=0,PrevPsolar=0,adc_value,Vsolar,Vbattery;
float Isolar,Ibattery,Pin,Pout,delta_V,delta_P,delta_I;
int pwm,pwm_start=40,pwm_min=35,pwm_max=49,delta=1;
void lcd_init (void);
void lcd_data (unsigned char);
void lcd_cmd (unsigned char);
void lcd_lscrol (unsigned char);
void lcd_rscrol (unsigned char);
void lcd_line1 (unsigned char);
void lcd_line2 (unsigned char);
void lcd_init (void)
{
delay_ms(300);
lcd_cmd (func_set);
lcd_cmd (disp_control);
lcd_cmd(disp_clear);
 delay_ms(50);
}
void lcd_cmd (unsigned char i)
{
output_low(lcd_rs);
output_b(i);
output_high(lcd_en);
delay_us(100);
output_low(lcd_en);
}
void lcd_data (unsigned char i)
{
output_high(lcd_rs);
output_b(i);
output_high(lcd_en);
delay_us(100);
output_low(lcd_en);
}
void lcd_lscrol (unsigned char i)
{
unsigned int8 count;
for(count=0;count<i;count++)
{
lcd_cmd(lscrol);
delay_ms(300);
}
}
void lcd_rscrol (unsigned char i)
{
unsigned int8 count;
for(count=0;count<i;count++)
{
lcd_cmd(rscrol);
delay_ms(300);
}
}
void lcd_line1 (unsigned char position)
{
lcd_cmd(line1 + position);
}
void lcd_line2 (unsigned char position)
{
lcd_cmd(line2 + position);
}
void main(void)
{
pwm=pwm_start;
lcd_init();
SETUP_ADC_PORTS(ALL_ANALOG);
setup_adc(ADC_CLOCK_DIV_8);
output_low(PIN_C1);           
setup_ccp1(CCP_PWM);       
setup_timer_2(T2_DIV_BY_1,49,1);
int i=0;
do
{
lcd_line1 (3);
lcd_data ("PV Fed ");
lcd_line2 (0);
lcd_data ("Conversion With");
delay_ms(1000);
lcd_rscrol(20);
lcd_cmd(disp_clear);
 delay_ms(50);
 lcd_line1 (0);
lcd_data ("Existing UPS");
delay_ms(1000);
lcd_rscrol(20);
lcd_cmd(disp_clear);
 delay_ms(50);
lcd_line1 (3);
lcd_data ("Made by:");
lcd_line2 (0);
lcd_data ("Faizan,Shakir");
delay_ms(1000);
lcd_rscrol(20);
lcd_cmd(disp_clear);
 delay_ms(50);
lcd_line1 (3);
lcd_data ("Made by:");
lcd_line2 (2);
lcd_data ("And Hamza");
delay_ms(1000);
lcd_rscrol(20);
i++;
}
while(i==0);
lcd_cmd(disp_clear);
 delay_ms(50);
while(1)
{
set_adc_channel(0);
delay_us(100);
adc_value = read_adc();
Vsolar= (5.0*adc_value/1023.0)/0.167;
 set_adc_channel(1);
delay_ms(100);
adc_value = read_adc();
Isolar= ((5.0/1023.0*adc_value)-2.47)/0.064;
set_adc_channel(2);
delay_us(100);
adc_value = read_adc();
Vbattery= (5.0*adc_value/1023.0)/0.333;
 set_adc_channel(3);
delay_ms(100);
adc_value = read_adc();
Ibattery= ((5.0/1023.0*adc_value)-2.47)/0.064;
 lcd_line1(0);
printf(lcd_data,"VOLTAGE:%f V",Vsolar);
delay_ms(1000);
lcd_rscrol(20);
lcd_cmd(disp_clear);
 delay_ms(50);
lcd_line1(0);
 printf(lcd_data,"CURRENT:%f A",Isolar);
delay_ms(1000);
lcd_rscrol(20);
lcd_cmd(disp_clear);
 delay_ms(50);
 Pin=Vsolar*Isolar;
 lcd_line1(0);
 lcd_data("INPUT POWER:");
 lcd_line2(3);
 printf(lcd_data,"%f W",Pin);
delay_ms(1000);
lcd_rscrol(20);
lcd_cmd(disp_clear);
 delay_ms(50);
lcd_line1(0);
printf(lcd_data,"VOLTAGE: %f V",Vbattery);
delay_ms(1000);
lcd_rscrol(20);
lcd_cmd(disp_clear);
 delay_ms(50);
lcd_line1(0);
printf(lcd_data,"CURRENT: %f A",Ibattery);
delay_ms(1000);
lcd_rscrol(20);
lcd_cmd(disp_clear);
 delay_ms(50);
  Pout=Vbattery*Ibattery;
 lcd_line1(0);
 lcd_data("OUTPUT POWER:");
 lcd_line2(3);
 printf(lcd_data,"%f W",Pout);
delay_ms(1000);
lcd_rscrol(20);
lcd_cmd(disp_clear);
 delay_ms(50);
  if(Vbattery<11.8)
 {on(red);}
 if(Vbattery<12.5)
 {on(red);
 on(yellow1);}
 if(Vbattery<13)
 {on(red);
 on(yellow1);
 on(yellow2);}
 if(Vbattery<13)
 {on(red);
 on(yellow1);
 on(yellow2);
 on(green);}
 if(Vsolar<Vbattery + 0.5 || Isolar<1)
 {off(relay);
 off(sd);
 off(fan);
 on(WAPDA);}
 if(Vbattery==13.5)
 {off(sd);
 off(fan);}}
 if(11.5<Vbattery<13.5 || Vsolar>12)
 {on(relay);
 on(sd);
 on(fan);
 on(solar);
 float delta_P=Pin-PrevPsolar,delta_V=Vsolar-PrevVsolar;
 float delta_I=Isolar-PrevIsolar,gradient=delta_P/delta_V;
int pwm;
if(delta_V!=0)
{ if(gradient>0)
{ pwm+=delta;
PrevPsolar=Pin;
PrevVsolar=Vsolar; 
PrevIsolar=Isolar; 
if (pwm<pwm_min)
{pwm=pwm_min;
set_pwm1_duty(pwm);}
else if (pwm>pwm_max || pwm==pwm_max)
{pwm=pwm_max;
set_pwm1_duty(pwm);}
else if (pwm<pwm_max && pwm>pwm_min)
{
pwm=pwm;
set_pwm1_duty(pwm);
}}
else if(gradient<0)
{ 
pwm-=delta;
PrevPsolar=Pin; 
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
}}
else 
{
if(delta_I>0)
{ 
pwm+=delta;
PrevPsolar=Pin; 
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
}}
else if(delta_I<0)
{ 
pwm-=delta;
PrevPsolar=Pin; 
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
set_pwm1_duty(pwm);}}}}}}
 




