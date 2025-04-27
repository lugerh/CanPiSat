// rf69_reliable_datagram_server.pde
// -*- mode: C++ -*-
// Example sketch showing how to create a simple addressed, reliable messaging server
// with the RHReliableDatagram class, using the RH_RF69 driver to control a RF69 radio.
// It is designed to work with the other example rf69_reliable_datagram_client
// Tested on Moteino with RFM69 http://lowpowerlab.com/moteino/
// Tested on miniWireless with RFM69 www.anarduino.com/miniwireless
// Tested on Teensy 3.1 with RF69 on PJRC breakout board

// #include <RHReliableDatagram.h>
#include <RH_RF69.h>
#include <SPI.h>

#define CANSAT 1
#define BASE 2

// Pines de conexión
#define RFM69_INT 3 // DIO0
#define RFM69_CS 10 // NSS
#define RFM69_RST 4 // RESET
#define RF69_FREQ 868.0

#define NODE_ID 1		 // Este nodo
#define DESTINATION_ID 2 // Nodo al que se envía el mensaje

RH_RF69 driver(RFM69_CS, RFM69_INT);

// Class to manage message delivery and receipt, using the driver declared above
// RHReliableDatagram manager(driver, BASE);

void setup() 
{
  Serial.begin(9600);
  while (!Serial) 
    ;

  Serial.println();
  Serial.println("soy la base");
  pinMode(RFM69_RST, OUTPUT);
	digitalWrite(RFM69_RST, LOW);
	delay(100);
	digitalWrite(RFM69_RST, HIGH);
	delay(100);
	digitalWrite(RFM69_RST, LOW);
	delay(100); // Esperar un poco después del reset
  
  if (!driver.init())
    Serial.println("init failed");
  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM (for low power module)

  // If you are using a high power RF69 eg RFM69HW, you *must* set a Tx power with the
  // ishighpowermodule flag set like this:
  driver.setTxPower(20, true);
  driver.setFrequency(868.0);
  driver.setCADTimeout(50);
  driver.setModemConfig(RH_RF69::GFSK_Rb250Fd250);

  Serial.print("init done");
  // delay(3000);
}

uint8_t data[] = "And hello back to you";
// Dont put this on the stack:
uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];

void loop()
{
  // if (manager.available())
  // {
    // Serial.println("data available");
    // Wait for a message addressed to us from the client
    uint8_t len = sizeof(buf);
    uint8_t from;
    // if (manager.recvfromAck(buf, &len, &from))
    if (driver.recv(buf, &len))
    {
      Serial.print("Respuesta: ");
      for (uint8_t i=0; i<len; i++ ) {
        Serial.print((char)buf[i]);
      }
      Serial.println();

      // Send a reply back to the originator client
      // if (!manager.sendtoWait(data, sizeof(data), from))
      //   Serial.println("sendtoWait failed");
    }
  // }
}

