// --------------------------------------------- AK4118 chip initialization routine -------------------------------------------------------------------
void AK4118Init()                                            // resets and initializes the AK4118
{  
  Serial.print("Initializing AK4118..... ");

  tft.setFont(&Arimo_Bold_26);
  tft.setCursor(0,180);
  tft.println("Initializing AK4118..");
  
  mcp_iso.digitalWrite(0, LOW);                           // Keep the AK4118's PDN pin low.
  delay(10);
  mcp_iso.digitalWrite(0, HIGH);                          // Power up the s/pdif receiver.

  Wire.beginTransmission(ak4118);                       // Look for AK4118
  error = Wire.endTransmission();

  tft.setCursor(0,200);
 
  if (error == 0)
  {
    Serial.println("AK4118 found!");
    tft.println("AK4118 found!");
    ak4118_exists = true;
    delay(100);
  }
  else if (error==4) 
  {
    Serial.println("Unknown error at AK4118's address.");
    tft.println("Unknown error at AK4118's address.");
    ak4118_exists = false;
    delay(1000);
  }
  else
  {
    Serial.println("No response from AK4118!");
    tft.println("No response from AK4118!");
    ak4118_exists = false;
    delay(1000);
  }

  if (ak4118_exists == true)
    {
        WriteRegister(ak4118, 0x00, B01000010);
        WriteRegister(ak4118, 0x00, B01000011);
        WriteRegister(ak4118, 0x01, B01010010);
        //WriteRegister(ak4118, 0x02, B00000001);
        WriteRegister(ak4118, 0x03, B11001000);
        WriteRegister(ak4118, 0x04, B00000000);                   // INT0 setup - enable interrupt for all events
        //WriteRegister(ak4118, 0x14, B01110111);
    }
  
  //Serial.println("Done with AK4118!");
}


/****************************** AK4118 input selection *********************************************/
void SetSpdifInput(int Input)                                               // Adapted to fit my "XMOS & S/PDIF Receiver", so only inputs 2,3,4,5 are selectable
{
  switch (Input) 
    {
      /*
      case 1:
      Serial.println("Input RX0");
      WriteRegister(ak4118,0x03,B01001000);
      break;
      */
      case 1:
      Serial.println("Input RX1");
      WriteRegister(ak4118,0x03,B01001001); 
      break;

      case 2:
      Serial.println("Input RX2");
      WriteRegister(ak4118,0x03,B01001010);
      break;
      /*
      case 4:
      Serial.println("Input RX3");
      WriteRegister(ak4118,0x03,B01001011);
      break;
      */
      case 3:
      Serial.println("Input RX4");
      WriteRegister(ak4118,0x03,B01001100); 
      break;

      case 4:
      Serial.println("Input RX5");
      WriteRegister(ak4118,0x03,B01001101); 
      break;
      /*
      case 7:
      Serial.println("Input RX6");
      WriteRegister(ak4118,0x03,B01001110); 
      break;

      case 8:
      Serial.println("Input RX7");
      WriteRegister(ak4118,0x03,B01001111);  
      break;
      */
    }
  //dispMain(false);
}

/****************************** AK4118 read Sample Rate *********************************************/
void ReadSpdifSR() 
{
  byte r = 0;
  //Serial.print("Register 07: ");
  r = ReadRegister(ak4118, 0x07);
  //Serial.println(r, BIN);
  
  switch (r) 
    {
      case B0:
      Serial.println("Sampling Rate 44.1KHz");
      SR_SPDIF = 15;
      break;

      case B100000:
      Serial.println("Sampling Rate 48KHz");
      SR_SPDIF = 14;
      break;

      case B10000000:
      Serial.println("Sampling Rate 88.2KHz");
      SR_SPDIF = 13;
      break;

      case B10100000:
      Serial.println("Sampling Rate 96KHz");
      SR_SPDIF = 12;
      break;

      case B11000000:
      Serial.println("Sampling Rate 176.4KHz");
      SR_SPDIF = 11;
      break;
      
      case B11100000:
      Serial.println("Sampling Rate 192KHz");
      SR_SPDIF = 10;
      break;

      case 4:
      break;
    }
  Serial.print("SR_SPDIF: "); Serial.println(mySR[SR_SPDIF]);
  //dispMain(false);
}

/****************************** AK4118 read Status Register *********************************************/
void ReadSpdifStatus() 
{
  /*
  1: PAR: Parity Error or Biphase Error Status
  2: AUDION: Audio Bit Output
  3: PEM: Pre-emphasis Detect.
  4: DTSCD: DTS-CD Auto Detect
  5: UNLCK: PLL Lock Status
  6: CINT: Channel Status Buffer Interrupt
  7: AUTO: Non-PCM Auto Detect
  */
  
  byte r = 0;
  //Serial.print("Register 06: ");
  r = ReadRegister(ak4118, 0x06);
  //Serial.println(r, BIN);

  if (bitRead(r, 0) == 1)
    {
      spdifStatus = 1;
      Serial.println("S/PDIF: Parity Error");
    }

  if (bitRead(r, 1) == 1)
    {
      spdifStatus = 2;
      Serial.println("S/PDIF: Non-audio signal");
    }

  if (bitRead(r, 2) == 1)
    {
      spdifPreemphasis = 1;
      Serial.println("S/PDIF: Pre-emphasis detected");
    }
  else if (bitRead(r, 2) == 0)
    {
      spdifPreemphasis = 0;
    }

  if (bitRead(r, 3) == 1)
    {
      spdifStatus = 3;
      Serial.println("S/PDIF: DTS-CD signal detected");
    }

  if (bitRead(r, 4) == 1)
    {
      spdifLock = 0;
      //Serial.println("S/PDIF: Lost lock");
    }
  else if (bitRead(r, 4) == 0)
    {
      spdifLock = 1;
      Serial.println("S/PDIF: Signal Locked");
    }

  if (bitRead(r, 5) == 1)
    {
      spdifStatus = 5;
      Serial.println("S/PDIF: Channel Status Buffer Interrupt");
    }
  
  if (bitRead(r, 6) == 1)
    {
      spdifStatus = 6;
      Serial.println("S/PDIF: Non-PCM audio detected");
    }
  
  //dispMain(false);
}

/****************************** AK4118 MCLK01 setting *********************************************/

void SetSpdifMCKO1(int mcko1) 
{
  //byte temp = 0;
  if (mcko1 == 128)
    {
      ChangeBit(ak4118, 0x00, 2, 1);
      ChangeBit(ak4118, 0x00, 3, 1);    
      Serial.println("AK4118 MCKO1 set to 128fs.");
    }
  else if (mcko1 == 256)
    {
      ChangeBit(ak4118, 0x00, 2, 0);
      ChangeBit(ak4118, 0x00, 3, 0);    
      Serial.println("AK4118 MCKO1 set to 256fs.");
    }
  else if (mcko1 == 512)
    {
      ChangeBit(ak4118, 0x00, 2, 0);
      ChangeBit(ak4118, 0x00, 3, 1);      
      Serial.println("AK4118 MCKO1 set to 512fs.");
    }
   
}
