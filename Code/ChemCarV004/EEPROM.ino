

byte getMixerSpeed(){
  return EEPROM.read(MAG_MIXER_SPEED_ADDRESS);
}

void saveMixerSpeed(byte mixerSpeed){
   EEPROM.write(MAG_MIXER_SPEED_ADDRESS, mixerSpeed);
}

byte getLedArrayBrightness(){
  return EEPROM.read(LED_ARRAY_BRIGHTNESS_ADDRESS); 
}

void saveLedArrayBrightness(byte ledArrayBrightness){
   EEPROM.write(LED_ARRAY_BRIGHTNESS_ADDRESS, ledArrayBrightness);
}


