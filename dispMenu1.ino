// ----------------------------- Function that displays the first level menu --------------------------------------------------------------------
void menu1_disp(bool full_redraw)
  {
    int16_t x, y;                             // Helper placement coordinates
    int16_t  x1, y1;                          // Coordinates of top left corner of blanking rectangle.
    uint16_t w, h;                            // Width and height of blanking rectangle.
    
    if (full_redraw == true)
      {
        tft.fillScreen(BLACK);
        tft.setFont(&Arimo_Bold_64);
        tft.getTextBounds("Settings", 0, 0, &x1, &y1, &w, &h);
        tft.setCursor((480-w)/2, h-5);
        tft.println("Settings");
      }
    
    tft.setFont(&Arimo_Bold_26);
    char disp_buffer[60]="";
    String buffer_temp;
    buffer_temp.reserve(50);
    String temp_1;
    temp_1.reserve(30);
    String temp_2;
    temp_2.reserve(15);
    String temp;
    temp.reserve(5);
    int row_height = 40;
    int gap = 60;
    param_height = 30;
    
    // ------------------- Filter Display -----------------------
    yoffset = row_height + gap;

    tft.setCursor(0, yoffset);
  
    if ((param == 1) || (param == 2) || (full_redraw == true))
      {
        temp_1 = String("Filter: ");
        temp_2 = String(myFilters[dacFilter]);
        buffer_temp = String(temp_1 + temp_2);
        buffer_temp.toCharArray(disp_buffer, 120);
      }
    if (param == 1)
      {
        tft.fillRect(0, yoffset-param_height, 480, param_height+5, BLUE);
        tft.setTextColor(WHITE, BLUE);             //set the background color of the text to blue
        tft.println(disp_buffer);
        param_1_select = true;
      }
    else if ((param == 2) || (full_redraw == true))
      {
        tft.fillRect(0, yoffset-param_height, 480, param_height+5, BLACK);
        tft.setTextColor(WHITE, BLACK);             //set the background color of the text to black        
        tft.println(disp_buffer);
        param_1_select = false;
      }
    else if ((param == 2) && (param_6_select == true))
      {
        tft.fillRect(0, yoffset-param_height, 480, param_height+5, BLACK);
        tft.setTextColor(WHITE, BLACK);             //set the background color of the text to black        
        tft.println(disp_buffer);
        param_6_select = false;
      }
    
    // ------------------- Direct DSD Display -----------------------
    yoffset = 2*row_height + gap;

    tft.setCursor(0, yoffset);
  
    if ((param == 2) || (param == 3) || (full_redraw == true))
      {
        temp_1 = String("DSD Direct: ");
        if (directDsd == 1)
          {
            temp_2 = String("Enabled");
          }
        else if (directDsd == 0)
          {
            temp_2 = String("Disabled");
          }
        buffer_temp = String(temp_1 + temp_2);
        buffer_temp.toCharArray(disp_buffer, 120);
      }
    if (param == 2)
      {
        tft.fillRect(0, yoffset-param_height, 480, param_height+5, BLUE);
        tft.setTextColor(WHITE, BLUE);             //set the background color of the text to blue
        tft.println(disp_buffer);
        param_2_select = true;
      }
    else if ((param == 3) || (full_redraw == true))
      {
        tft.fillRect(0, yoffset-param_height, 480, param_height+5, BLACK);
        tft.setTextColor(WHITE, BLACK);             //set the background color of the text to black        
        tft.println(disp_buffer);
        param_2_select = false;
      }
    else if ((param == 3) && (param_3_select = true))
      {
        tft.fillRect(0, yoffset-param_height, 480, param_height+5, BLACK);
        tft.setTextColor(WHITE, BLACK);             //set the background color of the text to black        
        tft.println(disp_buffer);        
        param_3_select = false;
      }
          
    // ------------------- DSD Filter display ----------------------- 
    yoffset = 3*row_height + gap;

    tft.setCursor(0, yoffset);
  
    if ((param == 3) || (param == 4) || (full_redraw == true))
      {
        temp_1 = String("DSD Filter Frequency: ");
        if (dsdFilter == 50)
          {
            temp_2 = String("50KHz");
          }
        else if (dsdFilter == 150)
          {
            temp_2 = String("150KHz");
          }
        buffer_temp = String(temp_1 + temp_2);
        buffer_temp.toCharArray(disp_buffer, 120);
      }
    if (param == 3)
      {
        tft.fillRect(0, yoffset-param_height, 480, param_height+5, BLUE);        
        tft.setTextColor(WHITE, BLUE);             //set the background color of the text to blue
        tft.println(disp_buffer);        
        param_3_select = true;
      }
    else if ((param == 4) || (full_redraw == true))
      {
        tft.fillRect(0, yoffset-param_height, 480, param_height+5, BLACK);
        tft.setTextColor(WHITE, BLACK);             //set the background color of the text to blue
        tft.println(disp_buffer);        
        param_3_select = false;
      }
    else if ((param == 4) && (param_4_select = true))
      {
        tft.fillRect(0, yoffset-param_height, 480, param_height+5, BLACK);        
        tft.setTextColor(WHITE, BLACK);             //set the background color of the text to blue
        tft.println(disp_buffer);                
        param_4_select = false;
      }
  
    // ------------------- Sound Setting display ----------------------- 
    yoffset = 4*row_height + gap;

    tft.setCursor(0, yoffset);
    
    if ((param == 4) || (param == 5) || (full_redraw == true))
      {
        temp_1 = String("Sound Mode: ");
        if (soundMode == 1)
          {
            temp_2 = String("1");
          }
        else if (soundMode == 2)
          {
            temp_2 = String("2");
          }
        else if (soundMode == 3)
          {
            temp_2 = String("3");
          }          
        buffer_temp = String(temp_1 + temp_2);
        buffer_temp.toCharArray(disp_buffer, 120);
      }
    if (param == 4)
      {
        tft.fillRect(0, yoffset-param_height, 480, param_height+5, BLUE);        
        tft.setTextColor(WHITE, BLUE);             //set the background color of the text to blue
        tft.println(disp_buffer);        
        param_4_select = true;
      }
    else if ((param == 5) || (full_redraw == true))
      {
        tft.fillRect(0, yoffset-param_height, 480, param_height+5, BLACK);                
        tft.setTextColor(WHITE, BLACK);             //set the background color of the text to blue
        tft.println(disp_buffer);        
        param_4_select = false;
      }
    else if ((param == 5) && (param_5_select = true))
      {
        tft.fillRect(0, yoffset-param_height, 480, param_height+5, BLACK);                
        tft.setTextColor(WHITE, BLACK);             //set the background color of the text to blue
        tft.println(disp_buffer);        
        param_5_select = false;
      }
    
    // ------------------- Inverting output display ----------------------- 
    yoffset = 5*row_height + gap;

    tft.setCursor(0, yoffset);
    
    if ((param == 5) || (param == 6) || (full_redraw == true))
      {
        temp_1 = String("DAC Analog Output: ");
        if (InvertPhase == 0)
          {
            temp_2 = String("Normal");
          }
        else if (InvertPhase == 1)
          {
            temp_2 = String("Inverted");
          }
        buffer_temp = String(temp_1 + temp_2);
        buffer_temp.toCharArray(disp_buffer, 120);
      }
    if (param == 5)
      {
        tft.fillRect(0, yoffset-param_height, 480, param_height+5, BLUE);         
        tft.setTextColor(WHITE, BLUE);             //set the background color of the text to blue
        tft.println(disp_buffer);        
        param_5_select = true;
      }
    else if ((param == 6) || (full_redraw == true))
      {
        tft.fillRect(0, yoffset-param_height, 480, param_height+5, BLACK);         
        tft.setTextColor(WHITE, BLACK);             //set the background color of the text to blue
        tft.println(disp_buffer);        
        param_5_select = false;
      }  
    else if ((param == 6) && (param_6_select = true))
      {
        tft.fillRect(0, yoffset-param_height, 480, param_height+5, BLACK);         
        tft.setTextColor(WHITE, BLACK);             //set the background color of the text to blue
        tft.println(disp_buffer);        
        param_6_select = false;
      }  
  
    // ------------------- MCLK display ----------------------- 
    yoffset = 6*row_height + gap;

    tft.setCursor(0, yoffset);
  
    if ((param == 6) || (param == 1) || (full_redraw == true))
      {  
        temp_1 = String("MCLK frequencies: ");
        if (MCLK_SET == 0)
          {
            temp_2 = String("22/24MHz");
          }
        else if (MCLK_SET == 1)
          {
            temp_2 = String("45/49MHz");
          }
        buffer_temp = String(temp_1 + temp_2);
        buffer_temp.toCharArray(disp_buffer, 120);
      }
    if (param == 6)
      {
        tft.fillRect(0, yoffset-param_height, 480, param_height+5, BLUE);        
        tft.setTextColor(WHITE, BLUE);             //set the background color of the text to blue
        tft.println(disp_buffer);        
        param_6_select = true;
      }
    else if ((param == 1) || (full_redraw == true))
      {
        tft.fillRect(0, yoffset-param_height, 480, param_height+5, BLACK);
        tft.setTextColor(WHITE, BLACK);             //set the background color of the text to black
        tft.println(disp_buffer);        
        param_6_select = false;
      }  
    else if ((param == 1) && (param_5_select = true))
      {
        tft.fillRect(0, yoffset-param_height, 480, param_height+5, BLACK);
        tft.setTextColor(WHITE, BLACK);             //set the background color of the text to black
        tft.println(disp_buffer);        
        param_5_select = false;
      } 
  }
