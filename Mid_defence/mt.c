#if defined(__PCM__)
#include<16F877.h>
#device adc=10
#fuses HS,BROWNOUT,NOWDT
#use delay(clock=20M)
#endif
#include <string.h>
#include <stdlib.h>

#define lcd_rs pin_d0
#define lcd_en pin_d1
#define sw pin_d2

#define disp_clear 0b00000001
#define disp_control 0b00001110
#define func_set 0b00111000
#define lscrol 0b00011000
#define rscrol 0b00011100
#define line1 0b10000000
#define line2 0b11000000

float adc_value,adc_value1,adc_value2;
float vout,v1,v2,Rldr;
float voltmeter1,voltmeter2;
float ammeter1,ammeter2;
float temperature,LUX;
float Pin,Pout;

void lcd_init (void);
void lcd_data (unsigned char);
void lcd_cmd (unsigned char);
void lcd_lscrol (unsigned char);
void lcd_rscrol (unsigned char);
void lcd_line1 (unsigned char);
void lcd_line2 (unsigned char);
void relay (unsigned char);

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

void relay (unsigned char threshold)
{
set_tris_b(0x00);

if(threshold<=2)
{
output_high(sw);
delay_ms(100);
}
else
{
output_low(sw);
}
}

void main()
{

lcd_init();

SETUP_ADC_PORTS(ALL_ANALOG);
setup_adc(ADC_CLOCK_DIV_2);

int i=0;

do
{
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

voltmeter1= (5.0*adc_value/1023.0)/0.11;

 set_adc_channel(1);
delay_us(100);

adc_value = read_adc();

ammeter1= ((5.0/1023.0*adc_value)-2.5)/0.16;

set_adc_channel(2);
delay_us(100);

adc_value = read_adc();

voltmeter2= (5.0*adc_value/1023.0)/0.33;

set_adc_channel(3);
delay_us(100);

adc_value = read_adc();

ammeter2= ((5.0/1023.0*adc_value)-2.5)/0.066;

relay (ammeter1);

lcd_line1(0);

printf(lcd_data,"VOLTAGE:%f V",voltmeter1);
delay_ms(1000);
lcd_rscrol(20);
delay_ms(1000);

lcd_cmd(disp_clear);
 delay_ms(50);
 
lcd_line1(0);

 printf(lcd_data,"CURRENT:%f A",ammeter1);
delay_ms(1000);
lcd_rscrol(20);

lcd_cmd(disp_clear);
 delay_ms(50);
 
 Pin=voltmeter1*ammeter1;
 lcd_line1(0);
 lcd_data("INPUT POWER:");
 
 lcd_line2(3);
 printf(lcd_data,"%f W",Pin);
 
delay_ms(1000);
lcd_rscrol(20);

lcd_cmd(disp_clear);
 delay_ms(50);

lcd_line1(0);

printf(lcd_data,"VOLTAGE: %f V",voltmeter2);
delay_ms(1000);
lcd_rscrol(20);

lcd_cmd(disp_clear);
 delay_ms(50);

lcd_line1(0);

printf(lcd_data,"CURRENT: %f A",ammeter2);
delay_ms(1000);
lcd_rscrol(20);

lcd_cmd(disp_clear);
 delay_ms(50);
 
  Pout=voltmeter2*ammeter2;
 lcd_line1(0);
 lcd_data("OUTPUT POWER:");
 
 lcd_line2(3);
 printf(lcd_data,"%f W",Pout);
 
delay_ms(1000);
lcd_rscrol(20);

lcd_cmd(disp_clear);
 delay_ms(50);
 
 set_adc_channel(4);
delay_us(100);

adc_value = read_adc();

lcd_line1(0);

vout= (5.0*adc_value/1023.0);
LUX=(1023-adc_value)*15;

lcd_line1(0);
 lcd_data("LIGHT INTENSITY:");
 
 lcd_line2(3);
 printf(lcd_data,"%f LUX",LUX);
 
delay_ms(1000);
lcd_rscrol(20);

lcd_cmd(disp_clear);
 delay_ms(50);
 
 set_adc_channel(5);
delay_us(100);

adc_value1 = read_adc();

 set_adc_channel(6);
delay_us(100);

adc_value2 = read_adc();

lcd_line1(0);

v1=(5.0*adc_value1)/1023.0;
v2=(5.0*adc_value2)/1023.0;
temperature=(v1-v2)*100.0;

printf(lcd_data,"TEMP: %f C",temperature);
delay_ms(1000);
lcd_rscrol(20);

}
}
