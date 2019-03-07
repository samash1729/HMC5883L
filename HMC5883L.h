#include "../I2cPort/I2cPort.h"
#include "../Extras/RAW.h"

#define HMC5883L_ADDRESS	0x1E
#define HMC5883L_CONFIGA	0x00
#define HMC5883L_CONFIGB	0x01

#define LEVEL0			0
#define LEVEL1			1
#define LEVEL2			2
#define LEVEL3			3

#define RATE0			0
#define RATE1			1
#define RATE2			2
#define RATE3			3
#define RATE4			4
#define RATE5			5
#define RATE6			6

#define MODE0			0
#define MODE1			1
#define MODE2			2
#define MODE3			3

#define GAIN0			0
#define GAIN1			1
#define GAIN2			2
#define GAIN3			3
#define GAIN4			4
#define GAIN5			5
#define GAIN6			6
#define GAIN7			7

#define X_MSB			0x03
#define X_LSB			0x04
#define Y_MSB			0x07
#define Y_LSB			0x08
#define Z_MSB			0x05
#define Z_LSB			0x06

class HMC5883L
{
	private :
	int8_t  x1,x0,y1,y0,z1,z0 ;
	public:
	
	I2cPort *i2c;
	float scale_val ;
	HMC5883L(void);
	~HMC5883L(void);
	uint8_t read8(int8_t reg);
	void write8(int8_t reg,int8_t val);
	
	void set_config_A(int8_t,int8_t,int8_t);
	void set_config_B(int8_t);
	
	raw getrawval(void);
	raw scaled_val(void);
};
