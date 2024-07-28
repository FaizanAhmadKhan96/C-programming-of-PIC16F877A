#include<16F877.h> 
#fuses HS,BROWNOUT,NOWDT,NOLVP
#use delay(clock=20M)
#include<LCD420.c>
// ConnectionsModuleLCD
#define LCD_RS_PIN PIN_D1
#define LCD_ENABLE_PIN PIN_D3
#define LCD_DATA4 PIN_D4
#define LCD_DATA5 PIN_D5
#define LCD_DATA6 PIN_D6
#define LCD_DATA7 PIN_D7//End ConnectionsModuleLCD
#define ON_OFF PIN_C7
#define BOUTON PIN_C0
#define BOUTON_ON input_state(PIN_C0)==0
#define Choice_bat_ON input_state(PIN_C1)==0
#define LED_RED PIN_C3
#define LED_YELLOW PIN_C4
#define LED_GREEN PIN_C5
#USE PWM (OUTPUT=PIN_C2, FREQUENCY=50kHz, DUTY=100)
float Voltage_pv;
float Voltage_bat;
float Current_pv;
float Current_bat;
float Current_ch;
float Temperature;
float Voltage_absorption;
float Voltage_float;
float alpha;
int duty;
char choice_bat;
const int8 Imin = 1;
void DisplayFormat(void) 
{
if(BOUTON_ON)
{
lcd_putc('\f'); //Clear LCD Screen
printf(lcd_putc,"Upv = %f V",Voltage_pv);
lcd_gotoxy(1,2);
printf(lcd_putc,"Ubat = %f V",Voltage_bat);
lcd_gotoxy(1,3);
if(Current_bat>=0)
{
printf(lcd_putc,"Ibat = %f A +++",Current_bat);
}
if(Current_bat<0)
{
Current_bat = - Current_bat;
printf(lcd_putc,"Ibat = %f A ---",Current_bat);
}
lcd_gotoxy(1,4);
printf(lcd_putc,"Temp = %f C",Temperature);
Delay_ms(2000);
}
else
{
lcd_putc('\f');
//Clear LCD Screenprintf(lcd_putc,"Ipv = %f A",Current_pv);
lcd_gotoxy(1,2);
printf(lcd_putc,"Icharge = %f A",Current_ch);
lcd_gotoxy(1,3);
printf(lcd_putc,"alpha = %f%%",alpha*100);
lcd_gotoxy(1,4);
lcd_putc("Frequency = 50kHz");
Delay_ms(2000);
}
}
void Mesure(void) 
{
set_adc_channel(2);
Delay_ms(10);
Voltage_bat = read_adc();
Delay_ms(10);
Voltage_bat = (Voltage_bat/1023)*5*6;
set_adc_channel(0);Delay_ms(10);
Voltage_pv = read_adc();
Delay_ms(10);
Voltage_pv = (Voltage_pv/1023)*5*4;
set_adc_channel(3);
Delay_ms(10);
Voltage_bat = read_adc();
Delay_ms(10);
Current_bat = (Current_bat/1023)*5;
Current_bat = 10*Current_bat-25;
set_adc_channel(1);
Delay_ms(10);
Current_pv = read_adc();
Delay_ms(10);
Current_pv = (Current_pv/1023)*5;
Current_pv = 10*Current_pv-25;
set_adc_channel(4);Delay_ms(10);
Temperature = read_adc();
Delay_ms(10);
Temperature = (Temperature/1023)*5;
Temperature = Temperature*12.5;
Voltage_absorption = (14.5-0.03*(Temperature-25))*choice_bat;
Voltage_float = (13.6-0.03*(Temperature-25))*choice_bat;
Current_ch = Current_pv - Current_bat;
}
void main()
{
setup_adc(ADC_CLOCK_INTERNAL);
setup_adc_ports(AN0_AN1_AN2_AN3_AN4);
set_tris_a(0xFF);
set_tris_c(0b00000011);
lcd_init();
Delay_ms(100);
while(TRUE)
{
if (Choice_bat_ON)
{
choice_bat=1;
}
else 
{
choice_bat=2;
}
lcd_putc('\f');
Mesure();
Delay_ms(500);
while(Voltage_bat <= (Voltage_absorption) &&Voltage_bat>= (12.6*choice_bat) ) 
{
lcd_putc('\f');
Mesure();
output_high(ON_OFF);
output_low(LED_YELLOW);
output_low(LED_GREEN);
output_high(LED_RED);
alpha=1;
pwm_on();
duty = 1000*alpha;
pwm_set_duty_percent(duty);//Viewing
DisplayFormat();
}
lcd_putc('\f');//
Mesure();
while(Current_bat >= Imin &&Voltage_bat>= (12.6*choice_bat)) 
{
lcd_putc('\f');
Mesure();
Voltage_bat = Voltage_absorption;
alpha = Voltage_bat/Voltage_pv;
output_low(LED_RED);
output_low(LED_GREEN);
output_high(LED_YELLOW);
pwm_on();
duty = 1000*alpha;
pwm_set_duty_percent(duty);
DisplayFormat();
Mesure();
}
Mesure();
Delay_ms(500);
lcd_putc('\f');
while(Voltage_bat >= (12.6*choice_bat))
 {
lcd_putc('\f');
Mesure();
Voltage_bat = Voltage_float;
alpha = Voltage_bat/Voltage_pv;
output_low(LED_YELLOW);
output_low(LED_RED);
output_high(LED_GREEN);
pwm_on();
duty = 1000*alpha;
pwm_set_duty_percent(duty);//Viewing
DisplayFormat();
Mesure();
}
lcd_putc('\f');
if(Voltage_bat < (12.6*choice_bat))
{
output_low(ON_OFF);
alpha=1;
pwm_on();
duty = 1000*alpha;
pwm_set_duty_percent(duty);
lcd_putc('\f'); //Clear LCD Screen
lcd_putc("Excessive discharge");
lcd_gotoxy(1,2);
lcd_putc("Load disconnected");
Mesure();
output_high(LED_YELLOW);
output_high(LED_GREEN);
output_high(LED_RED);
Delay_ms(250);
output_low(LED_YELLOW);
output_low(LED_GREEN);
output_low(LED_RED);
Delay_ms(250);
}
}
}
