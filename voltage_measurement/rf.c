#include<16F877A.h>
#device adc=10
#fuses HS,BROWNOUT,NOWDT,NOLVP
#use delay(clock=4M)

#define lcd_rs pin_c0
#define lcd_en pin_c1

#define disp_clear 0b00000001
#define disp_control 0b00001110
#define func_set 0b00111000
#define lscrol 0b00011000
#define rscrol 0b00011100
#define line1 0b10000000
#define line2 0b11000000

unsigned int16 adc_value;
float voltmeter;

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

void main()
{
lcd_init();

SETUP_ADC_PORTS(AN0);
setup_adc(ADC_CLOCK_DIV_32);

while(1)
{
set_adc_channel(0);
delay_us(100);

adc_value = read_adc();

lcd_line1(0);

voltmeter= 5.0 * adc_value/1023.0;

printf(lcd_data,"VOLTAGE: %f v",voltmeter);
}
}
