#include <SPI.h>
#include "LT8920.h"

const uint8_t PIN_NRF_RST = 16;
const uint8_t PIN_NRF_CS = 5;
const uint8_t PIN_NRF_PKT = 4;

LT8920 lt(PIN_NRF_CS, PIN_NRF_PKT, PIN_NRF_RST);

uint8_t number = 0;

void setup()
{
	Serial.begin(74880);
	SPI.begin();
	SPI.setBitOrder(MSBFIRST);
	SPI.setClockDivider(SPI_CLOCK_DIV64);
	delay(500);
	lt.begin(); 
	lt.setCurrentControl(0,0);  
	lt.setDataRate(LT8920::LT8920_1MBPS); 
	lt.setChannel(0x20);
}

void loop()
{
	delay(100);
	uint8_t data[16];
	sprintf((char *)data,"number: %d",number++);
	lt.sendPacket(data, strlen((char *)data)+1);
	Serial.println((char *)data);
}
