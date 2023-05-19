#ifndef _FS00905_H_
#define _FS00905_H_

#include "stm32f10x.h"

struct PM{
	u16 pm1_0;
	u16 pm2_5;
	u16 pm10;
};

struct NUM{
	u16 num0_3;
	u16 num0_5;
	u16 num1_0;
	u16 num2_5;
	u16 num5_0;
	u16 num10;
};

struct TVOC{
	u16 tvoc;
};

struct TEMPERATURE{
	u16 tem;
};

struct CO2{
	u16 co2;
};

struct CH2O{
	u16 ch2o;
};













#endif












