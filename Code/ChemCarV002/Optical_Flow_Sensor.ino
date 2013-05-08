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



void requestMouseData(){


  /* get a reading from the mouse */
  mouse.write(0xeb);  // Read data
  mouse.read();      // ignore ack
  mstat = mouse.read();
  mx = mouse.read();
  my = mouse.read();

  lastMouseRequest = millis();
  calculateVehicleDistance();
  
}

void printMouseData(){
  
    /* send the data back up */
  Serial.print(mstat, BIN);
  Serial.print("\tX=");
  Serial.print(mx, DEC);
  Serial.print("\tY=");
  Serial.print(my, DEC);
  Serial.println(); 
  
  
}



void requestMouseStatus(){


  /* get a reading from the mouse */
  mouse.write(0xe9);  // Request status from mouse
  mouse.read();      // ignore ack
  mstat = mouse.read();
  mResolution = mouse.read();
  mSamplingRate = mouse.read();
  
}

void printMouseStatus(){
  
  Serial.print(mstat, BIN);
  Serial.print("\tResolution=");
  Serial.print(mResolution, DEC);
  Serial.print("\tSampling Rate=");
  Serial.print(mSamplingRate, DEC);
  Serial.println(); 
  
  
}



 void calculateVehicleSpeed(){
   vehicleSpeed = mx / (millis() - lastMouseRequest);  // V = X / T
 }
 
 void calculateVehicleDistance(){
   distanceTraveledDots = distanceTraveledDots + my;
   Serial.print("distance dots");
   Serial.println(distanceTraveledDots);
 //  distanceTraveledInch = distanceTraveledDots /  @@ MOUSE RESOLUTION @@  
 }
