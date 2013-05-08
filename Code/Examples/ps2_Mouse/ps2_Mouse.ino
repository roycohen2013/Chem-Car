#include <ps2.h>

/*
 * an arduino sketch to interface with a ps/2 mouse.
 * Also uses serial protocol to talk back to the host
 * and report what it finds.
 */

/*
 * Pin 5 is the mouse data pin, pin 6 is the clock pin
 * Feel free to use whatever pins are convenient.
 */
PS2 mouse(6, 5);

/*
 * initialize the mouse. Reset it, and place it into remote
 * mode, so we can get the encoder data on demand.
 */
void mouse_init()
{
  mouse.write(0xff);  // reset
  mouse.read();  // ack byte
  mouse.read();  // blank */
  mouse.read();  // blank */
  mouse.write(0xf0);  // remote mode
  mouse.read();  // ack
  delayMicroseconds(100);
}







void setup()
{
  Serial.begin(9600);
  mouse_init();
  
    mouse.write(0xE8);  // tell sensor to set resolution
    mouse.read();      // ignore ack
    mouse.write(0x01);  // sends resolution value to mouse. New DPI = 500
  
  
   mouse.write(0xF3);  // give me data!
  mouse.read();      // ignore ack
    mouse.write(0xC8);  // give me data!
     // mouse.read();      // ignore ack

  
  
  
}

/*
 * get a reading from the mouse and rewait till the wait till the port it back to the
 * host via the serial line.
 */
void loop()
{
  char mstat;
  byte mx;
  byte my;
  
  
  /* get a reading from the mouse */
  mouse.write(0xeb);  // give me data!
  mouse.read();      // ignore ack
  mstat = mouse.read();
  
  mouse.read();
  //mx = mouse.read();
  my = mouse.read();
  
  

  
 // mx = mx << 1;
  //my = my << 1; 
  
 

  /* send the data back up */
  
  Serial.print(mstat, BIN);
  Serial.print("\tX=");
  Serial.print(mx, DEC);
  
  Serial.print("\tY=");
  Serial.print(my, DEC);
  
  Serial.println();
 
//  delay(20);  /* twiddle */
}
