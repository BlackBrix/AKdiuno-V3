void fullRedrawMain()
  {
    Serial.println("Full redraw");
    Serial.println("");
    tft.fillScreen(BLACK);
    dispVolume(true);
    dispInput();
    if ((I2S_Input == 1) && (spdifLock == 0))
      {
        dispNoLock();
      }
    else 
    {
      dispSR();
      dispSigType();
    }
    dispFilter();
    dispMCLK();
  }

void dispVolume(bool full_redraw)             // **************************************** Display Volume ************************************************
  {
    int16_t x, y;                             // Helper placement coordinates
    int16_t  x1, y1;                          // Coordinates of top left corner of blanking rectangle.
    uint16_t w, h;                            // Width and height of blanking rectangle.

    if ((volume != volumeOld) || (full_redraw == true))
      {
        tft.setFont(&Arimo_100);
        tft.setTextColor(WHITE);    
      
        float VolFloat = -1*((volume-255)/2);
        int VolDB = (int)VolFloat;
        
        if (volume <= 235)                                          // Determine if volume is 2 digits or one.
          {
            Vol2Digit = true;
          }
        else Vol2Digit = false;
    
        if (full_redraw == true)                                  // If full redraw, erase the full Volume space
          {
            tft.fillRect(90, 0, 300, 83, BLACK);
          }
  
        if (full_redraw == true)                                  // ------------ Print the "-" symbol --------------
          {
            if (Vol2Digit == false)
              {
                tft.setCursor(110, 80);                            // If single digit
              }
            else tft.setCursor(90, 80);                           // If two digits
            tft.println("-");
          }
  
        if (full_redraw == false)                                  // ------------ When switching from one to two digits, move the "-" symbol -------------
          {
            if ((Vol2Digit == false) && (Vol2Digit != Vol2DigitOld))
              {
                tft.fillRect(90, 0, 30, 83, BLACK);               // If two digits
                tft.setCursor(110, 80);                          // If single digit
                tft.println("-");
              }
            else if ((Vol2Digit == true) && (Vol2Digit != Vol2DigitOld))
              {
                tft.fillRect(110, 0, 30, 83, BLACK);               // If two digits
                tft.setCursor(90, 80);                        // If two digits
                tft.println("-");
              }
          }
                  
        if (Vol2Digit == false)                                  // ------------ Print the actual volume -------------
          {
            tft.fillRect(138, 0, 92, 83, BLACK);                // If single digit
            tft.setCursor(160, 80);                               // If single digit
          }
        else 
          {
            tft.fillRect(130, 0, 120, 83, BLACK);               // If two digits
            tft.setCursor(130, 80);                            // If two digits
          }
        tft.println(VolDB);
  
        if (full_redraw == true)                                  // ------------ In case of full redraw, print the "dB" symbol -------------
          {
            if (Vol2Digit == false)
              {
                tft.setCursor(230, 80);                          // If single digit
              }
            else tft.setCursor(255, 80);                        // If two digits
            tft.println("dB");
          }
  
        if (full_redraw == false)                                  // ------------ When switching from one to two digits, move the "dB" symbol -------------
          {
            if ((Vol2Digit == false) && (Vol2Digit != Vol2DigitOld))
              {
                tft.fillRect(230, 0, 150, 83, BLACK);               // If two digits
                tft.setCursor(230, 80);                          // If single digit
                tft.println("dB");
              }
            else if ((Vol2Digit == true) && (Vol2Digit != Vol2DigitOld))
              {
                tft.fillRect(250, 0, 140, 83, BLACK);               // If two digits
                tft.setCursor(255, 80);                        // If two digits
                tft.println("dB");
              }
          }
  
        volumeOld = volume;
        Vol2DigitOld = Vol2Digit;
      }    
    }

  void dispSR()                                                     // **************************************** Display Sampling Rate ************************************************
    {
      tft.setFont(&Arimo_Bold_64);
      tft.setTextColor(WHITE, BLACK);
      int x = 200;
      int y = 230;
      if (dsdSignal == 0)                     // If signal is PCM
        {
          tft.setFont(&Arimo_Bold_64);
          tft.fillRect(x, y-50, 260, 60, BLACK);
          tft.setCursor(x, y);
          tft.println(mySR[SR_DAC]);
        }
      else if (dsdSignal == 1)                     // If signal is DSD
        {
          if (SR_DAC == 5)
            {
              x = x-65;
            }
          else x = x-80;
          tft.setFont(&Arimo_Bold_64);
          tft.fillRect(20, y-50, 460, 60, BLACK);
          
          tft.setCursor(x, y);
          tft.println(mySR[SR_DAC]);
    }
  }

  void dispSigType()                          // **************************************** Display Signal Type ************************************************
    {
      tft.setFont(&Arimo_Bold_64);
      tft.setTextColor(WHITE, BLACK);
      int x = 20;
      int y = 230;
      tft.setFont(&Arimo_Bold_64);
      if (dsdSignal == 1)                     // If signal is DSD
        {
          tft.fillRect(x, y-50, 180, 60, BLACK);
          tft.setCursor(x, y);
          tft.println("DSD");
        }
      else if (dsdSignal == 0)                     // If signal is PCM
        {
          tft.fillRect(x, y-50, 180, 60, BLACK);
          tft.setCursor(x, y);
          tft.println("PCM");
        }
    }

  void dispNoLock()                           // **************************************** Display "No Lock" ************************************************
    {
      tft.setFont(&Arimo_Bold_64);
      tft.setTextColor(WHITE, BLACK);
      tft.fillRect(20, 180, 450, 60, BLACK);
      int x = 120;
      int y = 230;
      tft.setCursor(x, y);
      tft.println("No Lock");
    }

  void dispInput()                            // **************************************** Display selected Input ************************************************
    {
      int x = 0;
      int y = 0;
      
      tft.setFont(&Arimo_Bold_64);
      
      if (I2S_Input == 0)                                                   // If the input is USB
        {
          x = 175;
          //Serial.print("USB x: "); Serial.println(x);
        }
      else if (I2S_Input == 1)                                              // If the input is S/PDIF
        {
          if ((spdifInput == 1) || (spdifInput == 2)) x = 121;
          if ((spdifInput == 3) || (spdifInput == 4)) x = 147;
          //Serial.print("S/PDIF x: "); Serial.println(x);
        }
      y = 160;

      if (I2S_Input == 1)                                                     // If input is S/PDIF
        {
          if (I2S_Input != I2S_InputOld)
            {
              if (selectInputMode==true)
                {
                  tft.fillRect(100, y-50, 300, 50, WHITE); 
                }
              else tft.fillRect(100, y-50, 300, 50, BLACK);
              Serial.println("Erased USB");
              I2S_InputOld = I2S_Input;
            }
          else
            {
              if (selectInputMode==true)
                {
                  tft.fillRect(60, y-50, 400, 50, WHITE); 
                }
              else tft.fillRect(60, y-50, 400, 50, BLACK);
              Serial.println("Erased previous s/pdif input");
            }

          tft.setCursor(x, y);

          if (selectInputMode==true)
            {
              tft.setTextColor(BLACK, WHITE);             //set the background color of the text to white
            }
          else tft.setTextColor(WHITE, BLACK);             //set the background color of the text to black

          tft.println(myInputs[spdifInput]);                                  // Display new input name
          Serial.println("Displayed new s/pdif input");

          //tft.setTextColor(WHITE, BLACK);
          dispSigType();
          dispSR();
          spdifInputOld = spdifInput;
        }
      
      //else if ((I2S_Input == 0) || (I2S_Input != I2S_InputOld))               // If input is USB
      else if (I2S_Input == 0)                                                  // If input is USB
        {
          if (selectInputMode==true)
            {
              tft.fillRect(60, y-50, 400, 50, WHITE);
            }
          else tft.fillRect(60, y-50, 400, 50, BLACK);         
          //tft.fillRect(100, y-50, 300, 50, BLACK);
          tft.setCursor(x, y);
          
          if (selectInputMode==true)
            {
              tft.setTextColor(BLACK, BLUE);             //set the background color of the text to white
            }
          else tft.setTextColor(WHITE, BLACK);             //set the background color of the text to black              
          
          tft.println(myInputs[5]);                                           // Display new input name (USB)
          tft.setTextColor(WHITE, BLACK);
          dispSigType();
          dispSR();
          I2S_InputOld = I2S_Input;
        }
    }

void dispFilter()                                         // **************************************** Display Filter ************************************************
  {
    int16_t x, y;                             // Helper placement coordinates
    int16_t  x1, y1;                          // Coordinates of top left corner of blanking rectangle.
    uint16_t w, h;                            // Width and height of blanking rectangle.

    //if (dacFilter != dacFilterOld)
   //   {
        tft.setFont(&Arimo_Bold_32);
        tft.setTextColor(WHITE, BLACK);
        tft.getTextBounds(myFilters[dacFilter], 0, 270, &x1, &y1, &w, &h);
        tft.fillRect(0, 240, 480, 40, BLACK);
        tft.setCursor((480-w)/2, 270);
        tft.println(myFilters[dacFilter]);
        dacFilterOld = dacFilter;
   //   }
  }

void dispMCLK()                                         // **************************************** Display MCLK frequency ************************************************
  {
    int16_t x, y;                             // Helper placement coordinates
    int16_t  x1, y1;                          // Coordinates of top left corner of blanking rectangle.
    uint16_t w, h;                            // Width and height of blanking rectangle.
    
    //if (MCLK != MCLKOld)
    //  {
        tft.setFont(&Arimo_Bold_32);
        tft.setTextColor(WHITE, BLACK);
        tft.getTextBounds("MCLK set to 45.1584 MHz", 0, 315, &x1, &y1, &w, &h);
        tft.fillRect(0, 290, 480, h+5, BLACK);
        tft.setCursor((480-w)/2, 315);
        if (MCLK == 1)
          {
            tft.println("MCLK set to 11.2896 MHz");
          }
        else if (MCLK == 2)
          {
            tft.println("MCLK set to 12.288 MHz");
          }
        else if (MCLK == 3)
          {
            tft.println("MCLK set to 16.384 MHz");
          }
        else if (MCLK == 4)
          {
            tft.println("MCLK set to 16.9344 MHz");
          }
        else if (MCLK == 5)
          {
            tft.println("MCLK set to 18.432 MHz");
          }
        else if (MCLK == 6)
          {
            tft.println("MCLK set to 22.5792 MHz");
          }                                              
        else if (MCLK == 7)
          {
            tft.println("MCLK set to 24.576 MHz");
          }
        else if (MCLK == 8)
          {
            tft.println("MCLK set to 33.8688 MHz");
          }
        else if (MCLK == 9)
          {
            tft.println("MCLK set to 36.864 MHz");
          }
        else if (MCLK == 10)
          {
            tft.println("MCLK set to 45.1584 MHz");
          }
        else if (MCLK == 11)
          {
            tft.println("MCLK set to 49.152 MHz");
          }
        MCLKOld = MCLK;
      //}
  }

void dispMain(bool full_redraw)
  {
      Serial.println("Refreshing Main Screen");

      //Serial.print("I2S_Input: "); Serial.println(I2S_Input);
      //Serial.print("I2S_InputOld: "); Serial.println(I2S_InputOld);
      //Serial.print("spdifInput: "); Serial.println(spdifInput);
      //Serial.print("spdifInputOld: "); Serial.println(spdifInputOld);      
      //Serial.print("dsdSignal: "); Serial.println(dsdSignal);
      //Serial.print("dsdSignalOld: "); Serial.println(dsdSignalOld);      
            
      int16_t x, y;                             // Helper placement coordinates
      int16_t  x1, y1;                          // Coordinates of top left corner of blanking rectangle.
      uint16_t w, h;                            // Width and height of blanking rectangle.

      if (full_redraw == true)
        {
          Serial.println("Full redraw");
          Serial.println("");
          tft.fillScreen(BLACK);
        }
      
      // **************************************** Display Volume ************************************************
      
      if ((volume != volumeOld) || (full_redraw == true))
        {
          tft.setFont(&Arimo_100);
          tft.setTextColor(WHITE);    
        
          float VolFloat = -1*((volume-255)/2);
          int VolDB = (int)VolFloat;
          
          if (volume <= 235)                                          // Determine if volume is 2 digits or one.
            {
              Vol2Digit = true;
            }
          else Vol2Digit = false;
      
          if (full_redraw == true)                                  // If full redraw, erase the full Volume space
            {
              tft.fillRect(90, 0, 300, 83, BLACK);
            }

          if (full_redraw == true)                                  // ------------ Print the "-" symbol --------------
            {
              if (Vol2Digit == false)
                {
                  tft.setCursor(110, 80);                            // If single digit
                }
              else tft.setCursor(90, 80);                           // If two digits
              tft.println("-");
            }

          if (full_redraw == false)                                  // ------------ When switching from one to two digits, move the "-" symbol -------------
            {
              if ((Vol2Digit == false) && (Vol2Digit != Vol2DigitOld))
                {
                  tft.fillRect(90, 0, 30, 83, BLACK);               // If two digits
                  tft.setCursor(110, 80);                          // If single digit
                  tft.println("-");
                }
              else if ((Vol2Digit == true) && (Vol2Digit != Vol2DigitOld))
                {
                  tft.fillRect(110, 0, 30, 83, BLACK);               // If two digits
                  tft.setCursor(90, 80);                        // If two digits
                  tft.println("-");
                }
            }
                    
          if (Vol2Digit == false)                                  // ------------ Print the actual volume -------------
            {
              tft.fillRect(138, 0, 92, 83, BLACK);                // If single digit
              tft.setCursor(160, 80);                               // If single digit
            }
          else 
            {
              tft.fillRect(130, 0, 120, 83, BLACK);               // If two digits
              tft.setCursor(130, 80);                            // If two digits
            }
          tft.println(VolDB);

          if (full_redraw == true)                                  // ------------ In case of full redraw, print the "dB" symbol -------------
            {
              if (Vol2Digit == false)
                {
                  tft.setCursor(230, 80);                          // If single digit
                }
              else tft.setCursor(255, 80);                        // If two digits
              tft.println("dB");
            }

          if (full_redraw == false)                                  // ------------ When switching from one to two digits, move the "dB" symbol -------------
            {
              if ((Vol2Digit == false) && (Vol2Digit != Vol2DigitOld))
                {
                  tft.fillRect(230, 0, 150, 83, BLACK);               // If two digits
                  tft.setCursor(230, 80);                          // If single digit
                  tft.println("dB");
                }
              else if ((Vol2Digit == true) && (Vol2Digit != Vol2DigitOld))
                {
                  tft.fillRect(250, 0, 140, 83, BLACK);               // If two digits
                  tft.setCursor(255, 80);                        // If two digits
                  tft.println("dB");
                }
            }

          volumeOld = volume;
          Vol2DigitOld = Vol2Digit;
        }

      // **************************************** Display Input ************************************************
      
      if ((I2S_Input != I2S_InputOld) || (spdifInput != spdifInputOld) || (selectInputMode == true) || (full_redraw == true))
        {
          dispInput();
        }
      
      // **************************************** Display Signal Type ************************************************
      
      if ((dsdSignal != dsdSignalOld) || (full_redraw == true))
        {
          tft.setTextColor(WHITE, BLACK);
          dispSigType();
        }

      // **************************************** Display Sampling Rate ************************************************
      
      if ((SR_DAC != SR_DACOld) || (full_redraw == true))
        {
          tft.setTextColor(WHITE, BLACK);
          dispSR();
        }


      // **************************************** Display "No Lock" Condition ************************************************
      
      if ((I2S_Input == 1) && (spdifLock == 0))
        {
          dispNoLock();
        }

      // **************************************** Display DAC filter ************************************************
      
      if ((dacFilter != dacFilterOld) || (full_redraw == true))
        {
          tft.setFont(&Arimo_Bold_32);
          tft.setTextColor(WHITE, BLACK);
          tft.getTextBounds(myFilters[dacFilter], 0, 270, &x1, &y1, &w, &h);
          tft.fillRect(0, 240, 480, 40, BLACK);
          tft.setCursor((480-w)/2, 270);
          tft.println(myFilters[dacFilter]);
          dacFilterOld = dacFilter;
        }

      // **************************************** Display MCLK frequency ************************************************
      
      if ((MCLK != MCLKOld) || (full_redraw == true))
        {
          tft.setFont(&Arimo_Bold_32);
          tft.setTextColor(WHITE, BLACK);
          tft.getTextBounds("MCLK set to 45.1584 MHz", 0, 315, &x1, &y1, &w, &h);
          tft.fillRect(0, 290, 480, h+5, BLACK);
          tft.setCursor((480-w)/2, 315);
          if (MCLK == 1)
            {
              tft.println("MCLK set to 11.2896 MHz");
            }
          else if (MCLK == 2)
            {
              tft.println("MCLK set to 12.288 MHz");
            }
          else if (MCLK == 3)
            {
              tft.println("MCLK set to 16.384 MHz");
            }
          else if (MCLK == 4)
            {
              tft.println("MCLK set to 16.9344 MHz");
            }
          else if (MCLK == 5)
            {
              tft.println("MCLK set to 18.432 MHz");
            }
          else if (MCLK == 6)
            {
              tft.println("MCLK set to 22.5792 MHz");
            }                                              
          else if (MCLK == 7)
            {
              tft.println("MCLK set to 24.576 MHz");
            }
          else if (MCLK == 8)
            {
              tft.println("MCLK set to 33.8688 MHz");
            }
          else if (MCLK == 9)
            {
              tft.println("MCLK set to 36.864 MHz");
            }
          else if (MCLK == 10)
            {
              tft.println("MCLK set to 45.1584 MHz");
            }
          else if (MCLK == 11)
            {
              tft.println("MCLK set to 49.152 MHz");
            }
          MCLKOld = MCLK;
        }
  }
