/******************************************************************************
spitest.cpp
Raspberry Pi SPI interface demo
Byron Jacquot @ SparkFun Electronics>
4/2/2014
https://github.com/sparkfun/Pi_Wedge

A brief demonstration of the Raspberry Pi SPI interface, using the SparkFun
Pi Wedge breakout board.

Resources:

This example makes use of the Wiring Pi library, which streamlines the interface
to the the I/O pins on the Raspberry Pi, providing an API that is similar to the
Arduino.  You can learn about installing Wiring Pi here:
http://wiringpi.com/download-and-install/

The wiringPi SPI API is documented here:
https://projects.drogon.net/raspberry-pi/wiringpi/spi-library/

The init call returns a standard file descriptor.  More detailed configuration
of the interface can be performed using ioctl calls on that descriptor.
See the wiringPi SPI implementation (wiringPi/wiringPiSPI.c) for some examples.
Parameters configurable with ioctl are documented here:
http://git.kernel.org/cgit/linux/kernel/git/torvalds/linux.git/tree/Documentation/spi/spidev

Hardware connections:

This file interfaces with the SparkFun Serial 7 Segment display:
https://www.sparkfun.com/products/11629

The board was connected as follows:
(Raspberry Pi)(Serial 7 Segment)
GND  -> GND
3.3V -> Vcc
CE1  -> SS (Shift Select)
SCK  -> SCK 
MOSI -> SDI
MISO -> SDO

To build this file, I use the command:
>  g++ spitest.cpp -lwiringPi

Then to run it, first the spi kernel module needs to be loaded.  This can be 
done using the GPIO utility.
> gpio load spi
> ./a.out

This test uses the single-segment mode of the 7 segment display.  It shifts a 
bit through the display characters, lighting a single character of each at a 
time.

Development environment specifics:
Tested on Raspberry Pi V2 hardware, running Raspbian.
Building with GCC 4.6.3 (Debian 4.6.3-14+rpi1)

This code is beerware; if you see me (or any other SparkFun employee) at the
local, and you've found our code helpful, please buy us a round!

Distributed as-is; no warranty is given.
******************************************************************************/

#include <iostream>
#include <errno.h>
#include <wiringPiSPI.h>
#include <unistd.h>

using namespace std;

// channel is the wiringPi name for the chip select (or chip enable) pin.
// Set this to 0 or 1, depending on how it's connected.
static const int CHANNEL = 1;

#define HEADER 4
#define PIXELS 25
#define TRAILER 5

struct packet{
	char h0,h1,h2,h3;
	char data[PIXELS*4];
	char t0, t1,t2,t3,t4;

	packet(unsigned char b0,unsigned char b1,unsigned char b2,unsigned char b3):
		h0(0),h1(0),h2(0),h3(0),t0(0),t1(0),t2(0),t3(0),t4(0){
			for (int i=0; i< PIXELS; i++){
				data[i*4+0]=b0;
				data[i*4+1]=b1;
				data[i*4+2]=b2;
				data[i*4+3]=b3;
			}

	}
};

int main()
{
   int fd, result;

   cout << "Initializing" << endl ;

   // Configure the interface.
   // CHANNEL insicates chip select,
   // 500000 indicates bus speed.
   fd = wiringPiSPISetup(CHANNEL, 500000 );

   cout << "Init result: " << fd << endl;

   packet p(0xef, 0xff, 0x00, 0x00);
   int status = wiringPiSPIDataRW(CHANNEL, (unsigned char *) &p, sizeof p);

   cout << "Status result: " << status << endl;

   sleep(5);

   packet p2(0xef, 0xff, 0x80, 0x00);
   status = wiringPiSPIDataRW(CHANNEL, (unsigned char *) &p2, sizeof p2);
   cout << "Status result: " << status << endl;

   sleep(5);



   /*

   for(int i = 1; i <= 0x7f; i <<= 1)
   {
      buffer[0] = 0x77;
      buffer[1] = i;
      result = wiringPiSPIDataRW(CHANNEL, buffer, 2);

      buffer[0] = 0x7b;
      buffer[1] = i;
      result = wiringPiSPIDataRW(CHANNEL, buffer, 2);

      // The second character
      buffer[0] = 0x7c;
      buffer[1] = i;
      result = wiringPiSPIDataRW(CHANNEL, buffer, 2);

      // The third character
      buffer[0] = 0x7d;
      buffer[1] = i;
      result = wiringPiSPIDataRW(CHANNEL, buffer, 2);

      // The last character
      buffer[0] = 0x7e;
      buffer[1] = i;
      result = wiringPiSPIDataRW(CHANNEL, buffer, 2);

      // Pause so we can see them
      sleep(5);
   }
   */

}
