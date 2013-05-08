void mouse_init()
{
  mouse.write(0xff);  // reset
  mouse.read();  // ack byte
  mouse.read();  // blank */
  mouse.read();  // blank */
  mouse.write(0xf0);  // remote mode
  mouse.read();  // ack
  delayMicroseconds(200);
  
  
 
  
  
  
}



void requestMouseData(){
  //Serial.println("requesting");
  /* get a reading from the mouse */
  mouse.write(0xeb);  // Read data
  mouse.read();      // ignore ack
  mstat = mouse.read();
  mx = mouse.read();
  my = mouse.read();

  
  calculateVehicleDistance();
  calculateVehicleSpeed();

  lastMouseRequest = millis();
}



void requestMouseStatus(){

  
  /* get a reading from the mouse */
  mouse.write(0xE9);  // Request status from mouse includes state of all buttons and modes, resolution, frame rate
  mouse.read();      // ignore ack
  mstat = mouse.read();
  mResolution = mouse.read();
  mSamplingRate = mouse.read();
  
  
  
  
  switch(mResolution){   // this case statment determines the DPI conversion factor by requesting the mouse's resolution for the PAW3402 its three resolutions are 500/800/1000

  case 1:
    dpi = 500;      //mouse resolution value 1
    Serial.print("dpi changed to:  ");
    Serial.println(dpi);
    return;

  case 2:
    //dpi = 800;    //mouse resolution value 2
    dpi = 1000;    //mouse resolution value 2
    Serial.print("dpi changed to:  ");
    Serial.println(dpi);
    return;

  case 3:
    dpi = 1000;  //mouse resolution value 3
    Serial.print("dpi changed to:  ");
    Serial.println(dpi);
    return;
  }
  
  
  

}

void printMouseData(){
  
  //calculateVehicleDistance();
  
  /* send the data back up */
  Serial.print(mstat, BIN);
  Serial.print("\tX=");
  Serial.print(mx, DEC);
  Serial.print("\tY=");
  Serial.print(my, DEC);
  Serial.print("\tDistance dots=");
  Serial.print(distanceTraveledDotsY);
  
  Serial.print("\tDistance inches=");
  Serial.print(distanceTraveledInchY);
 
 
  //Serial.println(); 
}



void printVelocity(){
  
  Serial.print("\tvelocity inches=");
  Serial.print(vehicleSpeedY);
 
    Serial.println(); 
  
}


void printMouseStatus(){

  Serial.print(mstat, BIN);
  Serial.print("\tResolution=");
  Serial.print(mResolution, DEC);
  Serial.print("\tSampling Rate=");
  Serial.print(mSamplingRate, DEC);
  Serial.print("\t DIP =");
  Serial.print(dpi, DEC);
  
  
  Serial.println();

}


void setResolution(byte res){
  
    mouse.write(0xE8);  // tell sensor to set resolution
    mouse.read();      // ignore ack
    mouse.write(res);  // sends resolution value to mouse. New DPI = 500
    
}






void calculateVehicleSpeed(){
  
  vehicleSpeedX = (double(mx) / double(dpi)) / double((millis() - lastMouseRequest));  // V = X / T
  vehicleSpeedX = vehicleSpeedX * 1000.0; 
  
  vehicleSpeedY = (double(my) / double(dpi)) / double((millis() - lastMouseRequest));  // V = X / T
  vehicleSpeedY = vehicleSpeedY * 1000.0;
  
  
  
}

void calculateVehicleDistance(){
  
  distanceTraveledDotsX = distanceTraveledDotsX + double(mx);
  distanceTraveledDotsY = distanceTraveledDotsY + double(my);
  
  distanceTraveledInchX = distanceTraveledDotsX / double(dpi);
  distanceTraveledInchY = distanceTraveledDotsY / double(dpi);
  
}

