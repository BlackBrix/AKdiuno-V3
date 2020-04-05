// --------------------------------------------- Serial Port Stuff -------------------------------------------------------------------

char getCommand()                                            // read a character from the serial port
{
  char c = '\0';
  if (Serial.available())
  {
    c = Serial.read();
  }
  return c;
}

void displayMCLKHelp()                                            // displays available commands through the serial port
{
  Serial.println();
  Serial.println("AK4490 MCLK Selection");
  Serial.println();
  Serial.println("Press Q to 11.2896 MHz");
  Serial.println("Press W to 12.288 MHz");
  Serial.println("Press E to 16.384 MHz");
  Serial.println("Press R to 16.9344 MHz");
  Serial.println("Press T to 18.432 MHz");
  Serial.println("Press Y to 22.5792 MHz");
  Serial.println("Press U to 24.576 MHz");
  Serial.println("Press I to 33.8688 MHz");
  Serial.println("Press O to 36.864 MHz");
  Serial.println("Press P to 45.1584 MHz");
  Serial.println("Press { to 49.152 MHz");
  Serial.println("Press z to display main menu");
  Serial.println();
}

void displayDACHelp()                                            // displays available commands through the serial port
{
  Serial.println();
  Serial.println("AK4490 DAC Settings");
  Serial.println();
  
  Serial.println("Press 1 to select USB input");
  Serial.println("Press 2 to select COAX 1 input");
  Serial.println("Press 3 to select OPT 1 input");
  
  Serial.println("Press v to go to DAC MCLK menu");  
  Serial.println("Press a to display detected s/pdif sampling rate.");  
  Serial.println("Press h to set MCKO1 to 128fs");
  Serial.println("Press i to set MCKO1 to 256fs");
  Serial.println("Press j to set MCKO1 to 512fs");
  Serial.println("Press A to manually select PCM");
  Serial.println("Press B to manually select DSD");
  Serial.println("Press C to enable DirectDSD");
  Serial.println("Press D to disable DirectDSD");
  Serial.println("Press X to set DSD Filter at 50KHz");
  Serial.println("Press F to set DSD Filter at 150KHz");
  Serial.println("Press G to set the Sharp roll-off filter");
  Serial.println("Press H to set the Slow roll-off filter");
  Serial.println("Press V to set the Short delay sharp roll off filter");
  Serial.println("Press J to set the Short delay slow roll off filter");
  Serial.println("Press K to set the Superslow filter");
  Serial.println("Press M to set Sound Setting 1");
  Serial.println("Press N to set Sound Setting 2");
  Serial.println("Press S to set Sound Setting 3");  
  Serial.println("Press < to set output to non inverting");  
  Serial.println("Press > to set output to inverting");
  Serial.println("Press % to display all of AK4490's registers");
  Serial.println("Press z to display this menu");  
  Serial.println();
}
