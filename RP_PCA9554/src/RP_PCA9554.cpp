

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "RP_PCA9554.h"
#include "Wire.h"

/* 	###########################################################################
*	constructor
*/
PCA9554::PCA9554() {
    Wire.begin();                      			// start I2C communication
}

/* 	###########################################################################
*	initialisation
*/
uint8_t PCA9554::init(uint8_t i2caddr){
	uint8_t err;
	uint8_t input;
	_i2caddr = i2caddr;
	
	//set the input to zero
	_inPort0Level = 0x00;
  //_inPort1Level = 0x00;
	
	//read the status of port direction register
	_cfgPort0Direction = 0xFF;					//default: all input (1)
	//_cfgPort1Direction = 0xFF;
	Wire.beginTransmission(_i2caddr);
	Wire.write(PCA9554_CONFIG);
	err = Wire.endTransmission();
	if(err != 0){								//sucess					
		return err;
	}
	err = Wire.requestFrom(_i2caddr, 0x01);
	if(Wire.available() > 0){
		_cfgPort0Direction = Wire.read();
	}
	else{
		return err;
	}
	
	//read the status of port level register
	_cfgPort0Level = 0xFF;						//default: all high (1)
	//_cfgPort1Level = 0xFF;
	Wire.beginTransmission(_i2caddr);
	Wire.write(PCA9554_OUTPUT);
	err = Wire.endTransmission();
	if(err != 0){								//sucess					
		return err;
	}
	err = Wire.requestFrom(_i2caddr, 0x01);
	if(Wire.available() > 0){
		_cfgPort0Level = Wire.read();
	}
	else{
		return err;
	}
	return 0;
}

/* 	###########################################################################
*	pinModeBuf
*/
uint8_t PCA9554::pinModeBuf(uint8_t pin, uint8_t Mode){
	if(Mode == OUTPUT){
		if(pin < 8){
			_cfgPort0Direction &= ~(1 << pin);			//set pin bit to zero
			return 0;
		}
	}
  
	if(Mode == INPUT){
		if(pin < 8){
			_cfgPort0Direction |= (1 << pin);			  //set pin bit to one
			return 0;
		}
	}
	return 0xFF;
}

/* 	###########################################################################
*	pinModeUpdate
*/
uint8_t PCA9554::pinModeUpdate(){
	uint8_t err;
	Wire.beginTransmission(_i2caddr);
	Wire.write(PCA9554_CONFIG);
	Wire.write(_cfgPort0Direction);
	//Wire.write(_cfgPort1Direction);
	err = Wire.endTransmission();
	if(err != 0){										//sucess					
		return err;
	}
}

/* 	###########################################################################
*	pinMode
*/
uint8_t PCA9554::pinMode(uint8_t pin, uint8_t Mode){
	uint8_t err;
	err = pinModeBuf(pin, Mode);
	if(err != 0){
		return err;
	}
	err = pinModeUpdate();
	return err;	
}

/* 	###########################################################################
*	digitalWriteBuf
*/
uint8_t PCA9554::digitalWriteBuf(uint8_t pin, uint8_t level){
	uint8_t err;
	if(level == 0x00){
		if(pin < 8){
			_cfgPort0Level &= ~(1 << pin);				//set pin bit to zero
			return 0;
		}
	}
	else{
		if(pin < 8){
			_cfgPort0Level |= (1 << pin);				//set pin bit to one
			return 0;	
		}
	}
	return 0xFF;	
}

/* 	###########################################################################
*	writeBufUpdate
*/
uint8_t PCA9554::writeBufUpdate(){
	uint8_t err;
	Wire.beginTransmission(_i2caddr);
	Wire.write(PCA9554_OUTPUT);
	Wire.write(_cfgPort0Level);
	//Wire.write(_cfgPort1Level);
	err = Wire.endTransmission();
	if(err != 0){										//sucess					
		return err;
	}
}

/* 	###########################################################################
*	digitalWrite
*/
uint8_t PCA9554::digitalWrite(uint8_t pin, uint8_t level){
	uint8_t err;
	err = digitalWriteBuf(pin, level);
	if(err != 0){
		return err;
	}
	err = writeBufUpdate();
	return err;	
}

/* 	###########################################################################
*	readBufUpdate
*/
uint8_t PCA9554::readBufUpdate(){
	uint8_t err;
	Wire.beginTransmission(_i2caddr);
	Wire.write(PCA9554_INPUT);
	err = Wire.endTransmission();
	if(err != 0){										//sucess					
		return err;
	}
	err = Wire.requestFrom(_i2caddr, 0x01);
	if(Wire.available() > 0){
		_inPort0Level = Wire.read();
	}
	else{
		return err;
	}
	return 0;
}


/* 	###########################################################################
*	digitalReadBuf
*/
uint8_t PCA9554::digitalReadBuf(uint8_t pin){
	if(pin < 8){
		return IS_SET(_inPort0Level, pin);
	}
	return 0;
}

/* 	###########################################################################
*	digitalRead
*/
uint8_t PCA9554::digitalRead(uint8_t pin){
	readBufUpdate();
	return digitalReadBuf(pin);
}

/* 	###########################################################################
*	readPortA
*/

uint8_t PCA9554::readPortA(){
	readBufUpdate();
	return _inPort0Level;
}



