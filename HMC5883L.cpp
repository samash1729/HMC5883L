#include "HMC5883L.h"

HMC5883L::HMC5883L(void)
{
	this->i2c = new I2cPort(HMC5883L_ADDRESS,2);
	write8(0x02,0b00000000); //Continuous Measurement Mode...Mode Register
	scale_val = 0.92 ; //Default Scaling Value
}
HMC5883L::~HMC5883L(void)
{
	delete i2c;
}

uint8_t HMC5883L::read8(int8_t reg)
{
	i2c->openConnection();
	int8_t val = i2c->readByte(reg);
	i2c->closeConnection();
	return val;
}

void HMC5883L::write8(int8_t reg , int8_t val)
{
	i2c->openConnection();
	i2c->writeByte(reg,val);
	i2c->closeConnection();
}

//CONFIGURATION REGISTER A
// NUMBER OF SAMPLES AVERAGED PER MEASUREMENT INPUT
//VALUE TO BE SENT 		NUMBER OF SAMPLES	ACTUAL_VALUE
//LEVEL0			1(DEFAULT)		00
//LEVEL1			2			01
//LEVEL2			8			11
//LEVEL3			10			10

//DATA OUTPUT RATE SETTING
//VALUE TO BE SENT 		OUTPUT RATE		ACTUAL_VALUE
//RATE0				0.75			000
//RATE1				1.5			001
//RATE2				3			010
//RATE3				7.5			011
//RATE4				15(DEFAULT)		100
//RATE5				30			101
//RATE6				75			110
//RATE7				Reserved		111

//MEASUREMENT MODE SETTING
//VALUE TO BE SET		MEANING				ACTUAL_VALUE
//MODE0				NORMAL MEASUREMENT MODE		00
//MODE1				POSITIVE BIAS			01
//MODE2				NEGATIVE BIAS			10
//MODE3				Reserved			11

//FOR MORE INFO LOOK UP THE DATASHEET

void HMC5883L::set_config_A(int8_t sampling_rate ,int8_t output_rate,int8_t mode)
{
	int8_t val ;
	val = sampling_rate ;
	val = val<<3 ;
	val = val | output_rate ;
	val = val<<2 ;
	val = val | mode ;
	write8(HMC5883L_CONFIGA,val);
	write8(0x02,0x00);

}

//CONFIGURATION REGISTER B
//VALUE TO BE SET	SENSOR FIELD RANGE	DIGITAL RESOLUTION	GAIN(LSb/Gauss)		ACTUAL_VALUE
//GAIN0			+/- 0.88 Ga		0.73			1370			000
//GAIN1			+/- 1.3  Ga		0.92			1090			001
//GAIN2			+/- 1.9  Ga		1.22			820			010
//GAIN3			+/- 2.5  Ga		1.52			660			011
//GAIN4			+/- 4.0  Ga		2.27			440			100
//GAIN5			+/- 4.7  Ga		2.56			390			101
//GAIN6			+/- 5.6  Ga		3.03			330			110
//GAIN7			+/- 8.1  Ga		4.35			230			111

void HMC5883L::set_config_B(int8_t gain_level)
{
	gain_level = gain_level<<5;
	write8(HMC5883L_CONFIGB,gain_level);
	switch(gain_level)
	{
		case 0 : scale_val = 0.73 ;
			break;
		case 1 : scale_val = 0.92 ;
			break;
		case 2 : scale_val = 1.22 ;
			 break;
		case 3 : scale_val = 1.52 ;
			break;
		case 4 : scale_val = 2.27 ;
			break;
		case 5 : scale_val = 2.56 ;
			break;
		case 6 : scale_val = 3.03 ;
			break;
		case 7 : scale_val = 4.35 ;
			break;
		default: scale_val = 0.92 ;
	}
}

//MODE REGISTER HASNT BEEN ADDED AS IT IS OF NOT MUCH USE

raw HMC5883L::getrawval(void)
{
	raw temp_raw ;
	x1 = read8(X_MSB);
	x0 = read8(X_LSB);
	temp_raw.xaxis = ((x1<<8) | x0 );
	y1 = read8(Y_MSB);
	y0 = read8(Y_LSB);
	temp_raw.yaxis = ((y1<<8) | y0);
	z1 = read8(Z_MSB);
	z0 = read8(Z_LSB);
	temp_raw.zaxis = ((z1<<8) | z0);
	return temp_raw;
}

raw HMC5883L::scaled_val(void)
{
	raw s_val = getrawval();
	s_val.xaxis = s_val.xaxis * scale_val * 0.001 ;
	s_val.yaxis = s_val.yaxis * scale_val * 0.001;
	s_val.zaxis = s_val.zaxis * scale_val * 0.001;
	return s_val;
}
