#include "usart1.h"
#include "usart2.h"
#include "systick.h"
#include "led.h"
#include "string.h"
#include "fs00905.h"

extern u16 ucRxBuffer[40];
extern u16 ucRxCnt;
extern u8 Flag;

struct PM pm;
struct NUM num;
struct TVOC tvoc;
struct TEMPERATURE temperature;
struct CO2 co2;
struct CH2O ch2o;


void CopeSerial2Data( u16 RxData[40])//将串口2得到的数据发送到串口1并处理
{
	int i=0;
	if(RxData[0]!=0x42&&RxData[1]!=0x4d){
		Flag=FALSE;
	}
	else{
	for(i=0;i<40;i++){
		if(i==10||i==11)
			pm.pm1_0+=RxData[i];
		if(i==12||i==13)
			pm.pm2_5+=RxData[i];
		if(i==14||i==15)
			pm.pm10+=RxData[i];
		if(i==16||i==17)
			num.num0_3+=RxData[i];
		if(i==18||i==19)
			num.num0_5+=RxData[i];
		if(i==20||i==21)
			num.num1_0+=RxData[i];
		if(i==22||i==23)
			num.num2_5+=RxData[i];
		if(i==24||i==25)
			num.num5_0+=RxData[i];
		if(i==26||i==27)
			num.num10+=RxData[i];
		if(i==28||i==29)
			tvoc.tvoc+=RxData[i];
		if(i==30||i==31)
			temperature.tem+=RxData[i];
		if(i==34||i==35)
			co2.co2+=RxData[i];
		if(i==36||i==37)
			ch2o.ch2o+=RxData[i];
	}
	LED1_ON;
	SysTick_Delay_Ms(10);
	LED1_OFF;
	Flag=FALSE;
	}
}

void CopeSerial1Data(unsigned char ucData){
	USART2_Put_Char(ucData);
}


int main(){
	usart1_init(9600);
	USART2_init(9600);
	led_config();
	while(1){
		if(Flag==TRUE){
			CopeSerial2Data(ucRxBuffer);
//			printf("pm1.0  %dug/m^3\r\n",pm.pm1_0);
//			printf("pm2.5  %dug/m^3\r\n",pm.pm2_5);
//			printf("pm10  %dug/m^3\r\n",pm.pm10);
//			printf(" 0.3um %d\r\n",num.num0_3);
//			printf(" 0.5um %d\r\n",num.num0_5);
//			printf(" 1.0um %d\r\n",num.num1_0);
//			printf(" 2.5um %d\r\n",num.num2_5);
//			printf(" 5.0um %d\r\n",num.num5_0);
//			printf(" 10um %d\r\n",num.num10);
//			printf("TVOC %dug/m^3\r\n",tvoc.tvoc);
//			printf(".1%f\r\n",((float)temperature.tem-450)/10);
//			printf("co2  %dppm\r\n",co2.co2);
//			printf("ch2o  %dug/m^3\r\n",ch2o.ch2o);
		}
	}

}






