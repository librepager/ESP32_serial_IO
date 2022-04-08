#include <Arduino.h>
/*
 * sim_transceiver.cpp by Mongo@LibrePager.com 4-5-2022 
 * There are three serial ports on the ESP known as U0UXD, U1UXD and U2UXD.
 * 
 * U0UXD is used to communicate with the ESP32 for programming and during reset/boot.
 * U1UXD is unused and can be used for your projects. Some boards use this port for SPI Flash access.
 * U2UXD is unused and can be used for your projects.  This codes is for U2UXD.
 * 
 * 4-5-2022 Mongo@LibrePager.com expanded code to put the hardware serial io features into a 
 * module library: sim_transceiver.cpp,  containing functions sim_xmit() and sim_rec().
 * The point is to use these functions in IOT_Terminal.cpp which uses LVGL modules. LVGL modules 
 * provide text to eachother using functions that return type: const char * , and that accept 
 * arguments of type: const char *.  Their documentation is silent about the data structure of
 * informantion pointed to, but unless proven wrong, WE WILL ASSUME THAT THESE ARGUMENTS ARE ALL 
 * C-strings, ie, ARRAYS OF CONST CHAR THAT ARE NULL TERMINATED (ie, with '\0') AT THE END OF THE 
 * MEANINFULL CONTENT, AND THAT THE MAXIMUM LENGTH OF THE MEANINGFULL CONTENT IS UNKNOW.  We will
 * rely on that idea to construct the code.
 */

#define RXD2  16  // GPIO 16 is ESP32's RX2
#define TXD2  17  // GPIO 17 is ESP32's TX2
#define MAX_MSG_LEN 255
#define TERMINATOR  '\0'

char msg[MAX_MSG_LEN+1];  // filewide scope


int sim_xmit(char* A)
{ /* A is a pointer to a null terminated array of char, which is a message to be 
    sent to Serial2. 
    
    return 0 for normal function
    return 1 for error

    If A is empty, (ie, first element is null), no action, return 0 .
    If A is longer than MAX_MSG_LEN + 1 , no action, return 1.
    else:
    Print A on Serial2, return 0
    */


if (*A == '\0') 
{
  return 0;
} 
else if (strlen(A) > MAX_MSG_LEN)
{
  return 1;
}
  
Serial2.println(A);
return 0;
  
}



char* sim_rec(void){ 
  /*  sim_rec() is called by code in another file to get a message, if any, 
      from simulated transceiver, the Serial2 port. We don't want sim_rec() to 
      block the flow of the execution loop and we don't know how fast the bytes will
      come from the Serial2 port, so use setTimeOut(...) to stop reading and return a
      message if it takes to tlong.
      
      Serial2.begin(...) and Serial2.setTimeOut(...) must appear in calling program.
      
      See the Arduino Lang Ref for Stream.readBytesUntil(character,buffer,length) 
      and for Stream.setTimeout(long max_duration-miliseconds).
 */

  uint8_t msg_len = Serial2.readBytesUntil(TERMINATOR, msg, MAX_MSG_LEN);
  msg[msg_len] = '\0';

  return msg ;  
  
 
}
