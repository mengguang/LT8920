#include <SPI.h>
#include "LT8920.h"

const uint8_t PIN_NRF_RST = 16;
const uint8_t PIN_NRF_CS = 5;
const uint8_t PIN_NRF_PKT = 4;
const uint8_t PIN_ROLE = 0;  
bool writer;

LT8920 lt(PIN_NRF_CS, PIN_NRF_PKT, PIN_NRF_RST);

void setup()
{
  Serial.begin(74800);
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV16);

  pinMode(PIN_ROLE, INPUT_PULLUP);

  delay(500);

  lt.begin();
  lt.setCurrentControl(4,15);
  lt.setDataRate(LT8920::LT8920_62KBPS);
  lt.setChannel(0x06);

  char sbuf[32];

  //verify chip registers.
  for (int i = 0; i <= 50; i++)
  {
    uint16_t value = lt.readRegister(i);

    sprintf_P(sbuf, PSTR("%d = %04x\r\n"), i, value);
    Serial.print(sbuf);
  }

  if ((digitalRead(PIN_ROLE) == LOW))
  {
      writer = true;
    Serial.println(F("Writer mode"));
  }
  else
  {
      writer = false;
    Serial.println(F("Reader mode"));
    lt.startListening();
  }

  lt.whatsUp(Serial);
  Serial.println(F("Boot completed."));
}

void loop()
{
  if (writer)
  {
    delay(2000);

    //send a packet.
    uint8_t data[] = { 1,2,3,4,5 };

    lt.sendPacket(data, 5);
    lt.whatsUp(Serial);
  }
  else
  {
    if (lt.available())
    {
      Serial.println(F("Data available"));

      uint8_t buf[32];

      int packetSize = lt.read(buf, 32);
      if (packetSize > 0)
      {
        Serial.println(F("Packet read OK"));

        //dump the packet.
        for(int i = 0; i < packetSize; i++)
        {
          Serial.print(i);
          Serial.print("=");
          Serial.println(buf[i]);
        }
      }
      else
      {
        Serial.println(F("Packet read fail"));
        lt.whatsUp(Serial);
      }

      lt.startListening();
    }
  }
}
