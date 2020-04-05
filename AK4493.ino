// --------------------------------------------- DAC chip initialization routine -------------------------------------------------------------------
void AK4493Init()                                            // resets and initializes the DAC
{  
  Serial.println("Initializing AK4493..... ");

  //tft.fillScreen(ILI9481_BLACK);        // Clear the screen
  
  tft.setFont(&Arimo_Bold_26);
  tft.setCursor(0,220);
  tft.println("Initializing AK4493.....");
  
  mcp_iso.digitalWrite(2, LOW);   // Reset the DAC
  delay(30);
  mcp_iso.digitalWrite(2, HIGH);  // Power up the DAC
  delay(100);
    
  Wire.beginTransmission(ak4493);                       // Look for AK4493
  error = Wire.endTransmission();

  tft.setCursor(0,240);
 
  if (error == 0)
  {
    Serial.println("AK4493 found!");
    tft.println("AK4493 found!");
  }
  else if (error==4) 
  {
    Serial.println("Unknown error at AK4493's address.");
    tft.println("Unknown error at AK4493's address.");
    delay(1000);
  }
  else
  {
    Serial.println("No response from AK4493!");
    tft.println("No response from AK4493!");
    delay(1000);
  }
  delay(100);

  #ifdef DUALMONO                                     // Communicate with the second AK4493 (right channel when in dual mono mode).
  
    Wire.beginTransmission(ak4493r);
    error = Wire.endTransmission();

    tft.setCursor(0,260);
    
    if (error == 0)
    {
      Serial.println("AK4493r found!");
      tft.println("AK4493r found!");      
    }
    else if (error==4) 
    {
      Serial.println("Unknown error at AK4493r's address.");
      tft.println("Unknown error at AK4493r's address.");
      delay(1000);
    }
    else
    {
      Serial.println("No response from AK4493r!");
      tft.println("No response from AK4493r!");
      delay(1000);
    }
    delay(100);
  #endif //DUALMONO
  
  WriteRegister(ak4493, 0x00, B10001111);    // Initialize the DAC. Sets Auto MCLK & SF, I2S 32bit and RSTN=1
  WriteRegister(ak4493, 0x01, B10100010);    // Enables Data Zero Detection.
  
  #ifdef AutoDSD
      ChangeBit(ak4493, 0x00, 0, false);      // Reset the dac
      delay(20);
      ChangeBit(ak4493, 0x15, 7, true);       // Enable automatic detection of PCM or DSD
      ChangeBit(ak4493, 0x00, 0, true);       // Come out of reset
      delay(20);
  #endif
  
  #ifdef DUALMONO
    WriteRegister(ak4493r, 0x00, B10001111);   // Initialize the right DAC. Sets Auto MCLK & SF, I2S 32bit and RSTN=1
    WriteRegister(ak4493r, 0x01, B10100010);   // Enables Data Zero Detection.
    #ifdef AutoDSD
      ChangeBit(ak4493r, 0x00, 0, false);      // Reset the dac
      delay(20);
      ChangeBit(ak4493r, 0x15, 7, true);       // Enable automatic detection of PCM or DSD
      ChangeBit(ak4493r, 0x00, 0, true);       // Come out of reset
      delay(20);
    #endif
    // Set first DAC for Mono operation and Left Channel
    ChangeBit(ak4493, 0x02, 3, true);
    ChangeBit(ak4493, 0x02, 1, false);
    // Set second DAC for Mono operation and Right Channel
    ChangeBit(ak4493r, 0x02, 3, true);
    ChangeBit(ak4493r, 0x02, 1, true);
    // Enable first DAC synchronization feature
    ChangeBit(ak4493, 0x07, 0, true);
    // Enable second DAC synchronization feature
    ChangeBit(ak4493r, 0x07, 0, true);
  #endif //DUALMONO

  // Assuming the settings have been read from EEPROM, apply the settings
  SetFilter(ak4493);
  EnableDirectDSD(ak4493);
  SetDSDFilter(ak4493);
  SetSoundMode(ak4493);
  SetInvertPhase(ak4493);

  #ifdef DUALMONO
    SetFilter(ak4493r);
    EnableDirectDSD(ak4493r);
    SetDSDFilter(ak4493r);
    SetSoundMode(ak4493r);
    SetInvertPhase(ak4493r);
  #endif //DUALMONO

  Serial.println("Done with AK4493(s)!");
}


// ----------------------------- Set DAC chip parameters (including MCLK) according to incoming SR  ----------------------------------------------------
void SetDACParam()
  {
    if (SR_DAC == 15)                 // 44.1K
      {
        if (DACAutoMode == true)
          {
            MCLK = MCLK_L;                   // Set MCLK to 22.5792 MHz
            SetMCLK(MCLK);
          }
        if (DACAutoMode == false)       // If DAC is set to Manual mode
          {
            SetDAC_SR();                // Set the AK4493 to the appropriate SR mode.  
          }
      }
    else if (SR_DAC == 14)            // 48K
      {
        if (DACAutoMode == true)
          {
            MCLK = MCLK_H;                   // Set MCLK to 24.576 MHz
            SetMCLK(MCLK);
          }
        if (DACAutoMode == false)       // If DAC is set to Manual mode
          {
            SetDAC_SR();                // Set the AK4493 to the appropriate SR mode.  
          }

      }
    else if (SR_DAC == 13)            // 88.2K
      {
        if (DACAutoMode == true)
          {
            MCLK = MCLK_L;                   // Set MCLK to 22.5792 MHz
            SetMCLK(MCLK);
          }
        if (DACAutoMode == false)       // If DAC is set to Manual mode
          {
            SetDAC_SR();                // Set the AK4493 to the appropriate SR mode.  
          }

      }
    else if (SR_DAC == 12)            // 96K
      {
        if (DACAutoMode == true)
          {
            MCLK = MCLK_H;                   // Set MCLK to 24.576 MHz
            SetMCLK(MCLK);
          }
        if (DACAutoMode == false)       // If DAC is set to Manual mode
          {
            SetDAC_SR();                // Set the AK4493 to the appropriate SR mode.  
          }

      }    
    else if (SR_DAC == 11)            // 176.4K
      {
        if (DACAutoMode == true)
          {
            MCLK = MCLK_L;                   // Set MCLK to 22.5792 MHz
            SetMCLK(MCLK);
          }
        if (DACAutoMode == false)       // If DAC is set to Manual mode
          {
            SetDAC_SR();                // Set the AK4493 to the appropriate SR mode.  
          }

      }
    else if (SR_DAC == 10)            // 192K
      {
        if (DACAutoMode == true)
          {
            MCLK = MCLK_H;                   // Set MCLK to 24.576 MHz
            SetMCLK(MCLK);
          }
        if (DACAutoMode == false)       // If DAC is set to Manual mode
          {
            SetDAC_SR();                // Set the AK4493 to the appropriate SR mode.  
          }
      }
  }

// ----------------------------- Set DAC chip volume  -------------------------------------------------------------------
void SetVol (byte RegVal) 
{ 
  WriteRegister (ak4493, 0x03, RegVal); // Set Up Volume In DAC-L ATT 
  WriteRegister (ak4493, 0x04, RegVal); // Set Up Volume In DAC-R ATT
  #ifdef DUALMONO
    WriteRegister (ak4493r, 0x03, RegVal); // Set Up Volume In DAC-L ATT 
    WriteRegister (ak4493r, 0x04, RegVal); // Set Up Volume In DAC-R ATT
  #endif //DUALMONO

  /*
  if (select1Mode==false)
    {
      dispMain(false);
    }
  */
} 

// ----------------------------- Read PCM / DSD Status Bit ------------------------------------------------------------
void ReadAKStatus() 
{
  byte r = 0;
  //Serial.print("Register 02: ");
  r = ReadRegister(ak4493, 0x02);
  //Serial.println(r, BIN);

  if (bitRead(r, 6) == 0)
    {
      mode_dac = 0;
      Serial.println("AK4493 detected PCM signal");
    }

  if (bitRead(r, 6) == 1)
    {
      mode_dac = 1;
      Serial.println("AK4493 detected DSD signal");
    }
}

// ----------------------------- Set DAC chip to PCM or DSD operation ----------------------------------------------------
void SetDACMode() 
{
  //digitalWrite(POWERPIN, LOW);              // Mute DAC chip
  //delay(100);
  if (dacMode == 1)                             // Set To PCM Mode
    {
      Serial.println("");
      Serial.println("Setting up for PCM.");
      ChangeBit(ak4493, 0x00, 0, false);              // Reset the AK4493
      delay(40);
      //ChangeBit(ak4493, 0x01, 0, true);         // Enable soft mute
      ChangeBit(ak4493, 0x02, 7, false);        // Set To PCM Mode
      //WriteRegister(ak4493,0x00,B00000000);     // Reset
      delay(40);
      ChangeBit(ak4493, 0x00, 0, true);               // Start the AK4493
      //ChangeBit(ak4493, 0x01, 0, false);        // Disable soft mute
      #ifdef DUALMONO
        ChangeBit(ak4493r, 0x00, 0, false);           // Reset the AK4493
        delay(40);
        //ChangeBit(ak4493r, 0x01, 0, true);      // Enable soft mute
        ChangeBit(ak4493r, 0x02, 7, false);     // Set To PCM Mode
        //WriteRegister(ak4493r,0x00,B00000000);  // Reset
        delay(40);
        ChangeBit(ak4493r, 0x00, 0, true);            // Start the AK4493
        //ChangeBit(ak4493r, 0x01, 0, false);     // Disable soft mute
      #endif //DUALMONO
      //SetDACAutoMode();                         // Change mode to either Auto or Manual
    }
  else if (dacMode == 2)                        // Set To DSD Mode
    {
      Serial.println("");
      Serial.println("Setting up for DSD.");
      ChangeBit(ak4493, 0x00, 0, false);              // Reset the AK4493
      delay(40);
      //ChangeBit(ak4493, 0x01, 0, true);         // Enable soft mute
      ChangeBit(ak4493, 0x02, 7, true);         // Set To DSD Mode
      delay(40);
      ChangeBit(ak4493, 0x00, 0, true);               // Start the AK4493
      WriteRegister(ak4493,0x06,B10011001);     // Set To DSD Data Mute / DSD Mute Control / DSD Mute Release
      WriteRegister(ak4493,0x09,B00000001);     // Set To DSD Sampling Speed Control
      //ChangeBit(ak4493, 0x01, 0, false);        // Disable soft mute
      #ifdef DUALMONO
        ChangeBit(ak4493r, 0x00, 0, false);              // Reset the AK4493
        delay(40);
        //ChangeBit(ak4493r, 0x01, 0, true);      // Enable soft mute
        ChangeBit(ak4493r, 0x02, 7, true);      // Set To DSD Mode    
        delay(40);
        ChangeBit(ak4493r, 0x00, 0, true);               // Start the AK4493
        WriteRegister(ak4493r,0x06,B10011001);  // Set To DSD Data Mute / DSD Mute Control / DSD Mute Release
        WriteRegister(ak4493r,0x09,B00000001);  // Set To DSD Sampling Speed Control  
        //ChangeBit(ak4493r, 0x01, 0, false);     // Disable soft mute
      #endif //DUALMONO
      //SetDACAutoMode();                         // Change mode to either Auto or Manual
    }
  //delay(100);
  //digitalWrite(POWERPIN, HIGH);             // Un-mute DAC chip
}

// ----------------------------- Set DAC chip filter  --------------------------------------------------------------------------
void SetFilter(byte dac)
{
  if (dacFilter == 1)
    {
      Serial.println("Sharp roll-off filter");              // SD=0, SLOW=0
      ChangeBit(dac, 0x05, 0, false);   // SuperSlow disabled
      ChangeBit(dac, 0x01, 5, false);
      ChangeBit(dac, 0x02, 0, false);
    }
  else if (dacFilter == 2)
    {
      Serial.println("Slow roll-off filter");               // SD=0, SLOW=1
      ChangeBit(dac, 0x05, 0, false);   // SuperSlow disabled
      ChangeBit(dac, 0x01, 5, false);
      ChangeBit(dac, 0x02, 0, true);
    }
  else if (dacFilter == 3)
    {
      Serial.println("Short delay sharp roll off filter");  // SD=1, SLOW=0
      ChangeBit(dac, 0x05, 0, false);   // SuperSlow disabled
      ChangeBit(dac, 0x01, 5, true);
      ChangeBit(dac, 0x02, 0, false);
    }
  else if (dacFilter == 4)
    {
      Serial.println("Short delay slow roll off filter");   // SD=1, SLOW=1
      ChangeBit(dac, 0x05, 0, false);   // SuperSlow disabled
      ChangeBit(dac, 0x01, 5, true);
      ChangeBit(dac, 0x02, 0, true);
    }
  else if (dacFilter == 5)
    {
      Serial.println("SuperSlow filter");
      ChangeBit(dac, 0x01, 5, false);  // SD = 0
      ChangeBit(dac, 0x02, 0, true);   // SLOW = 1
      ChangeBit(dac, 0x05, 0, true);   // SuperSlow enabled
    }
  /*
  if (select1Mode==false)
    {
      dispMain(false);
    }
  */
}

// --------------------------------------------- Set DAC chip DSD Direct mode -------------------------------------------------------------------
void EnableDirectDSD(byte dac) 
{
  switch (directDsd) 
    {
      case 0:
      Serial.println("DSD Normal Path Set");
      ChangeBit(dac, 0x06, 1, false);
      break;

      case 1:
      Serial.println("DSD Direct Path Set");
      ChangeBit(dac, 0x06, 1, true);      
      break;
    }
//PrintDebugReg();
}

// --------------------------------------------- Set DAC chip DSD filter frequency -------------------------------------------------------------------
void SetDSDFilter(byte dac)
{
  switch (dsdFilter)
    {
      case 50:
      Serial.println("DSD Cut Off Filter at 50KHz");
      ChangeBit(dac, 0x09, 1, false);
      break;

      case 150:
      Serial.println("DSD Cut Off Filter at 150KHz");
      ChangeBit(dac, 0x09, 1, true);      
      break;
    }
}

// --------------------------------------------- Set DAC chip Sound Mode -------------------------------------------------------------------
void SetSoundMode(byte dac) 
{
  if (soundMode == 1)
    {
      Serial.println("Sound Setting 1");
      WriteRegister(dac,0x08,B00000000);      
    }
  if (soundMode == 2)
    {
      Serial.println("Sound Setting 2");
      WriteRegister(dac,0x08,B00000001);      
    }      
  if (soundMode == 3)
    {
      Serial.println("Sound Setting 3");
      WriteRegister(dac,0x08,B00000010);      
    } 
}

// --------------------------------------------- Set DAC chip normal or inverting output -------------------------------------------------------------------
void SetInvertPhase(byte dac) 
{
  if (InvertPhase == 0)
    {
      Serial.println("Output phase normal");
      ChangeBit(dac, 0x05, 7, false);
      ChangeBit(dac, 0x05, 6, false);
    }
  else if (InvertPhase == 1)
    {
      Serial.println("Output phase inverted");
      ChangeBit(dac, 0x05, 7, true);
      ChangeBit(dac, 0x05, 6, true);
    }      
}

// --------------------------------------------- Set DAC chip to Auto or Manual mode  -------------------------------------------------------------------
void SetDACAutoMode() 
{
  if (DACAutoMode == true) 
    {
      ChangeBit(ak4493, 0x00, 7, true);
      #ifdef DUALMONO
        ChangeBit(ak4493r, 0x00, 7, true);
      #endif //DUALMONO
      Serial.println("DAC set to Auto mode.");
    }
  else if (DACAutoMode == false) 
    {
      ChangeBit(ak4493, 0x00, 7, false);
      #ifdef DUALMONO
        ChangeBit(ak4493r, 0x00, 7, false);
      #endif //DUALMONO
      Serial.println("DAC set to Manual mode.");
    }
//PrintDebugReg();
}

// --------------------------------------------- Set DAC chip Sampling Rate -------------------------------------------------------------------
void SetDAC_SR() 
{
  //digitalWrite(POWERPIN, LOW);                                    // Mute the DAC (via relay)
  //delay(100);
  ChangeBit(ak4493, 0x00, 0, false);              // Reset the AK4493
    #ifdef DUALMONO
      ChangeBit(ak4493r, 0x00, 0, false);           // Reset the AK4493
    #endif //DUALMONO
  delay(30);
  
  if ((SR_DAC == 16) || (SR_DAC == 15) || (SR_DAC == 14))         // DAC SR set to 30K - 54K
    {
      ChangeBit(ak4493, 0x01, 3, false);
      ChangeBit(ak4493, 0x01, 4, false);
      ChangeBit(ak4493, 0x05, 1, false);
      #ifdef DUALMONO
        ChangeBit(ak4493r, 0x01, 3, false);
        ChangeBit(ak4493r, 0x01, 4, false);
        ChangeBit(ak4493r, 0x05, 1, false);
      #endif //DUALMONO
      Serial.println("DAC SR set to 30K - 54K.");
    }
  else if ((SR_DAC == 13) || (SR_DAC == 12))                      // DAC SR set to 54K - 108K
    {
      ChangeBit(ak4493, 0x01, 3, true);
      ChangeBit(ak4493, 0x01, 4, false);
      ChangeBit(ak4493, 0x05, 1, false);
      #ifdef DUALMONO
        ChangeBit(ak4493r, 0x01, 3, true);
        ChangeBit(ak4493r, 0x01, 4, false);
        ChangeBit(ak4493r, 0x05, 1, false);
      #endif //DUALMONO
      Serial.println("DAC SR set to 54K - 108K.");
    }
  else if ((SR_DAC == 11) || (SR_DAC == 10))                      // DAC SR set to 120K - 216K
    {
      ChangeBit(ak4493, 0x01, 3, false);
      ChangeBit(ak4493, 0x01, 4, true);
      ChangeBit(ak4493, 0x05, 1, false);
      #ifdef DUALMONO
        ChangeBit(ak4493r, 0x01, 3, false);
        ChangeBit(ak4493r, 0x01, 4, true);
        ChangeBit(ak4493r, 0x05, 1, false);
      #endif //DUALMONO
      Serial.println("DAC SR set to 120K - 216K.");
    }
  else if ((SR_DAC == 9) || (SR_DAC == 8))                        // DAC SR set to 352K - 384K
    {
      ChangeBit(ak4493, 0x01, 3, false);
      ChangeBit(ak4493, 0x01, 4, false);
      ChangeBit(ak4493, 0x05, 1, true);
      #ifdef DUALMONO
        ChangeBit(ak4493r, 0x01, 3, false);
        ChangeBit(ak4493r, 0x01, 4, false);
        ChangeBit(ak4493r, 0x05, 1, true);
      #endif //DUALMONO
      Serial.println("DAC SR set to 352K - 384K.");
    }
  else if ((SR_DAC == 7) || (SR_DAC == 6))                         // DAC SR set to 705K - 768K
    {
      ChangeBit(ak4493, 0x01, 3, true);
      ChangeBit(ak4493, 0x01, 4, false);
      ChangeBit(ak4493, 0x05, 1, true);
      #ifdef DUALMONO
        ChangeBit(ak4493r, 0x01, 3, true);
        ChangeBit(ak4493r, 0x01, 4, false);
        ChangeBit(ak4493r, 0x05, 1, true);
      #endif //DUALMONO
      Serial.println("DAC SR set to 705K - 768K.");
    }
  else if (SR_DAC == 5)                                            // DAC SR set to DSD 64
    {
      ChangeBit(ak4493, 0x06, 0, false);
      ChangeBit(ak4493, 0x09, 0, false);
      #ifdef DUALMONO
        ChangeBit(ak4493r, 0x06, 0, false);
        ChangeBit(ak4493r, 0x09, 0, false);
      #endif //DUALMONO
      Serial.println("DAC SR set to DSD 64");
    }
  else if (SR_DAC == 4)                                            // DAC SR set to DSD 128
    {
      ChangeBit(ak4493, 0x06, 0, true);
      ChangeBit(ak4493, 0x09, 0, false);
      #ifdef DUALMONO
        ChangeBit(ak4493r, 0x06, 0, true);
        ChangeBit(ak4493r, 0x09, 0, false);
      #endif //DUALMONO
      Serial.println("DAC SR set to DSD 128");
    }
  else if (SR_DAC == 3)                                            // DAC SR set to DSD 256
    {
      ChangeBit(ak4493, 0x06, 0, false);
      ChangeBit(ak4493, 0x09, 0, true);
      #ifdef DUALMONO
        ChangeBit(ak4493r, 0x06, 0, false);
        ChangeBit(ak4493r, 0x09, 0, true);
      #endif //DUALMONO
      Serial.println("DAC SR set to DSD 256");
    }
    
    ChangeBit(ak4493, 0x00, 0, true);               // Start the AK4493
    #ifdef DUALMONO
      ChangeBit(ak4493r, 0x00, 0, true);            // Start the AK4493
    #endif //DUALMONO
  //delay(100);
  //digitalWrite(POWERPIN, HIGH);                     // Unmute the dac (via relay)
//PrintDebugReg();
}
