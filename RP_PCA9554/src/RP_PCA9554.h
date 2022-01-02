/*
 * classPCA9539.h
 *
 * Created on: 02.01.2022
 * Author: Ralf
 * https://www.nxp.com/docs/en/data-sheet/PCA9554_9554A.pdf
 * https://42project.net/c-basic-bitmanipulation-set-clear-change-bit-in-c/
 *
 * polarity register not supported yet
 *
 */

#ifndef RP_PCA9554_H_
#define RP_PCA9554_H_

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


/*
 *	PCA9554 defines
 *  invert register is not supported
 */
#define PCA9554_INPUT     0x00							//register 0
#define PCA9554_OUTPUT    0x01							//register 1
#define PCA9554_INVREG    0x02              //register 2
#define PCA9554_CONFIG    0x03							//register 3

/*
 * macros for simple bit handling
*/
#define CLEAR_BIT(byte,bit) ((byte) &= ~(1UL << (bit)))
#define SET_BIT(byte, bit) ((byte) |= (1UL << (bit)))
#define IS_SET(byte,bit) (((byte) & (1UL << (bit))) >> (bit))


class PCA9554 {
public:
  PCA9554();                            					        //constructor
  uint8_t init(uint8_t i2caddr);							            //intialize, returns 1 if success otherwise 0
	uint8_t pinModeBuf(uint8_t pin, uint8_t Mode);			    //call this to set several pins in a buffer, transmit to PCA9554 by calling "PinModeUpdate" (faster, only write once)
	uint8_t pinModeUpdate(void);							              //call to update pin mode registers in PCA9554
	uint8_t pinMode(uint8_t pin, uint8_t Mode);				      //call to set the desired pin direction immediately
	uint8_t digitalWriteBuf(uint8_t pin, uint8_t level);	  //call to set the desired pin level in a buffer, transmit to PCA9554 by calling "WriteBufUpdate" (faster, only write once)
	uint8_t writeBufUpdate(void);							              //call to update pin level registers in PCA9554
	uint8_t digitalWrite(uint8_t pin, uint8_t level);		    //call to set the desired pin level immediately
	uint8_t readBufUpdate(void);							              //call to update the input read buffer
	uint8_t digitalReadBuf(uint8_t pin);					          //call to read the specified pin, call "ReadBufUpdate" first to ensure an actual level (faster, only read once)
	uint8_t digitalRead(uint8_t pin);						            //call to read the desired pin level immediately
	uint8_t readPortA(void);								                //returns the actual PortA level
	//uint8_t readPortB(void);								                //returns the actual PortB level


private:
  uint8_t _i2caddr;										                    //holds the i2c address of PCA9554
	uint8_t _cfgPort0Direction;								
	//uint8_t _cfgPort1Direction;
	uint8_t _cfgPort0Level;
	//uint8_t _cfgPort1Level;
	uint8_t _inPort0Level;
	//uint8_t _inPort1Level;

};

 /* RP_PCA9554_H_ */
#endif