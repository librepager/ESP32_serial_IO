#include <Arduino.h>
#include "sim_transceiver.h"
#include <iostream>

/*
 * There are three serial ports on the ESP known as U0UXD, U1UXD and U2UXD.
 * 
 * U0UXD is used to communicate with the ESP32 for programming and during reset/boot.
 * U1UXD is unused and can be used for your projects. Some boards use this port for SPI Flash access though
 * U2UXD is unused and can be used for your projects.
 * 
 * adapted from https://circuits4you.com/2018/12/31/esp32-hardware-serial2-example/  
 * and from from Arduino Language Ref for hardware serial on 11-20-2021 Mongo@LibrePager.com
 * 
 * To use this code, load into two ESP32 dev cards. Use jumpers to connect TX2 & RX2 from the first 
 * to RX2 & TX2 on the second, respectively. Plug USB cables from a computer to each of the dev boards
 * and open serial terminals to each board. Characters entered into either dev board's terminal will 
 * appear on the other dev board's terminal.
 * 
 * 4-5-2022 Mongo@LibrePager.com expanded code to put the hardware serial io features into a 
 * module library: sim_transceiver.cpp,  containing functions sim_xmit() and sim_rec().
 * The point is to use these functions in IOT_Terminal.cpp which uses LVGL modules. LVGL modules 
 * provide text to eachother using functions that return type: const char * , and that accept 
 * arguments of type: const char *.  LVGL's documentation is silent about the data structure of
 * informantion pointed to, but unless proven wrong, WE WILL ASSUME THAT THESE ARGUMENTS ARE ALL 
 * C-strings, ie, ARRAYS OF CONST CHAR THAT ARE NULL TERMINATED (ie, with '\0') AT THE END OF THE 
 * MEANINFULL CONTENT, AND THAT THE LENGTH OF THE MEANINGFULL CONTENT IS UNKNOW.   
 * 
*/

#define BOB 0
#define RXD2  16  // GPIO 16 is ESP32's RX2
#define TXD2  17  // GPIO 17 is ESP32's TX2
#define LED 2  

#define MAX_MSG_LEN 255 // max length of meaningful content that this module will accept
                        // max length of LVGL meaningfull content could be anything
#define MAX_READ_TIME 500  // millisecond to read  (required for sim_transceiver.cpp)
#define TERMINATOR '\0' 

using namespace std;

  char txt_msg[MAX_MSG_LEN+1] ;

#if BOB == 1
   char test_msg[]  ={'h','e','l','l','o',' ','f','r','o','m',' ','B','o','b',' ',' ','\0'} ;
   #else 
  char test_msg[]  ={'h','e','l','l','o',' ','f','r','o','m',' ','A','l','i','c','e','\0'} ;
  #endif
  char randMsg[453];

 char lorem[] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Fusce tinci"
 "dunt dolor libero. Phasellus finibus lorem quis felis consectetur porta. Ut congue ar"
 "cu dui, in mattis odio sollicitudin id. Suspendisse aliquam felis a risus placerat, a au"
 "ctor nisl volutpat. Maecenas semper consequat nulla, nec dapibus leo dapibus et. Sed iacul"
 "is odio lacus, sit amet convallis nisl elementum in. Quisque feugiat turpis eu eros sagit"
 "tis, eget mattis sem hendrerit.";   // 67 words, 452 characters

  uint16_t iteration = 0; 
  

void setup() {

  pinMode(LED, OUTPUT); 

  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);  //(required for sim_transceiver.cpp)
  Serial2.setTimeout(MAX_READ_TIME);            //(required for sim_transceiver.cpp)

  Serial.begin(9600);
  Serial.println("TXD2 in code = "+String(TXD2));
  Serial.println("RXD2 in code = "+String(RXD2));
  Serial.println();


}

void loop() 
{ 
 
#if BOB == 1
  Serial.print(" Bob - iteration:");  
  #else
  Serial.print(" Alice - iteration:");
  #endif
  Serial.println(iteration++);
  digitalWrite(LED, HIGH);
  Serial.println("LED is on");
  delay(500);
  digitalWrite(LED, LOW);
  Serial.println("LED is off");
  delay(500);

  uint16_t randLen = random(300);
  Serial.print("randLen = ");
  Serial.println(randLen);

  strncpy(randMsg, lorem, randLen );
  randMsg[randLen] = TERMINATOR;
   
   
  uint32_t timeA = millis();
  if (sim_xmit(randMsg )  ) {Serial.println("xmit error");}
  char* ptrA = sim_rec();    
  uint32_t elapsedA = millis()-timeA; 

  Serial.print(ptrA);  
  Serial.print("elapsed time for xmit/rec:");
  Serial.println( elapsedA);
  Serial.println();
  


}

