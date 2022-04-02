#include <Arduino.h>
/*
 * There are three serial ports on the ESP known as U0UXD, U1UXD and U2UXD.
 * 
 * U0UXD is used to communicate with the ESP32 for programming and during reset/boot.
 * U1UXD is unused and can be used for your projects. Some boards use this port for SPI Flash access though
 * U2UXD is unused and can be used for your projects.
 * 
 * adapted from https://circuits4you.com/2018/12/31/esp32-hardware-serial2-example/  
 * on 11-20-2021 Mongo@LibrePager.com,  but the circuits for that code didn't work, 
 * so I copied code from Arduino Language Ref for hardware serial
 * 
 * To use this code, load into two ESP32 dev cards. Use jumpers to connect TX2 & RX2 from the first 
 * to RX2 & TX2 on the second, respectively. Plug USB cables from a computer to each of the dev boards
 * and open serial terminals to each board. Characters entered into either dev board's terminal will 
 * appear on the other dev board's terminal.
 * 
 * Code worked (transmiting bytes in DEC decimal) at 3:00 pm 11-20-2021
 * Code as shown below tested with LibrePager Touchscreen Testbed TS_Testbed_V2 on 4-2-2022
*/

#define RXD2  16  // GPIO 16 is ESP32's RX2
#define TXD2  17  // GPIO 17 is ESP32's TX2

void setup() {
  // Note the format for setting a serial port is as follows: 
  // Serial2.begin(baud-rate, protocol, RX pin, TX pin);
  Serial.begin(9600);
  
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  
  Serial.println("TXD2 in code = "+String(TXD2));
  Serial.println("RXD2 in code = "+String(RXD2));
  Serial.println();
}

void loop() { 
  
// from Arduino Language Ref for hardware Serial2

// read from port 0, send to port 2:
  if (Serial.available()) {
    char inByte = Serial.read();
    Serial2.print(inByte/*,DEC*/); 
  }
  // read from port 2, send to port 0:
  if (Serial2.available()) {
    char inByte = Serial2.read();
    Serial.print(inByte/*,DEC*/);   

  }
}

