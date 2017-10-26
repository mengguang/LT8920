#include <SPI.h>
#include "LT8920.h"

const uint8_t PIN_NRF_RST = 16;
const uint8_t PIN_NRF_CS = 5;
const uint8_t PIN_NRF_PKT = 4;


LT8920 lt(PIN_NRF_CS, PIN_NRF_PKT, PIN_NRF_RST);

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
	if (lt.available()) 
	{
		uint8_t data[16]; 
		int packetSize = lt.read(data, 16);  
		if (packetSize > 0)
		{ 
			Serial.println(F("Packet read OK"));
			Serial.println((char *)data);
		}
		else
		{
			Serial.println(F("CRC is ERROR"));
		}
		lt.startListening(); 
	}
}