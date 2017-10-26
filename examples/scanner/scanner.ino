#include <SPI.h>
#include "LT8920.h"

const uint8_t PIN_NRF_RST = 16;
const uint8_t PIN_NRF_CS = 5;
const uint8_t PIN_NRF_PKT = 4;
const uint8_t PIN_ROLE = 0; 

LT8920 lt(PIN_NRF_CS, PIN_NRF_PKT, PIN_NRF_RST);

void setup()
{
  Serial.begin(74800);

  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV16);

  lt.begin();
  lt.setCurrentControl(15,15);
  lt.setChannel(0x06);
}

void loop()
{
  uint16_t wifi_Channels[] = {
    2412,
    2417,
    2422,
    2427,
    2432,
    2437,
    2442,
    2447,
    2452,
    2457,
    2462,
    2467,
    2472,
    2484
  };
  
  uint16_t signals[32];
  char sbuf[64];
  
  Serial.println("\n\n");
  
  for(int i = 0; i < sizeof(wifi_Channels)/sizeof(uint16_t); i++)
  {  
    lt.scanRSSI(&signals[i], wifi_Channels[i]-2402, 1); 
  }
    
  for(int i = 0; i < sizeof(wifi_Channels)/sizeof(uint16_t); i++)
  {
    sprintf(sbuf, "[%02d] %d = %04x ", i+1, wifi_Channels[i], signals[i]);
    Serial.print(sbuf);    
    for(int j =0 ; j < signals[i]/2; j++)
    {
      Serial.print('#');
    }
    Serial.println();
  }
  delay(250);
 }
