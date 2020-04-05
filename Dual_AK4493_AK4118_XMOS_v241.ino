/*******************************************************************************************************
 * AKduino Part 3: Controlling a Dual Mono AK4493 DAC with an Si570 or Si544 as a clock generator and an 
 * XMOS / AK4118 board as an I2S source.
 *
 * Project Page: http://www.dimdim.gr
 *
 * v1.72    24/12/2017 : - Minor changes to make compatible with current stm32duino core 
 *                         (changed HardWire.h to Wire.h and other minor stuff).
 *                       - First public release as part of completed dual mono DAC project.
 * 
 * v1.66    10/10/2017 : - Minor volume bugfix.
 *                       - SuperSlow filter still problematic.
 *                       - Enabled DAC synchronization feature (experimental..).
 * 
 * v1.64    30/09/2017 : - Bugfixes.
 *
 * v1.60    20/09/2017 : - Added support of rotary encoder and IR remote control.
 *                       - 3.5" TFT support.
 *
 * v1.50    07/01/2017 : - Added support of rotary encoder for volume control.
 *                       - Bugfixes related to DSD.
 *
 * v1.41    06/01/2017 : - Added support for dual mono mode.
 *
 * v1.36    03/01/2017 : - Added very basic TFT support.
 *
 * v1.35    20/12/2016 : - Code cleanup for first public release.
 *
 * v1.33    19/12/2016 : - Added full control of sound parameters through serial port.
 *
 * v1.27    18/12/2016 : - First functional version.
 *                       - Automatic switching between PCM and DSD by monitoring DSDPIN.    
 ***************************************************************************************************/

/***************************************************************************************************
  Code starts here
 ***************************************************************************************************/
#include <Wire.h>                                   // Library for I2C
TwoWire WIRE2(2);                                   // Use Hardware I2C2 (SCL2 = PB10, SDA2 = PB11)
#define Wire WIRE2                                  // To ease use of the "classic" Arduino libraries with the stm32duino's hardware I2C implementation

#include <RotaryEncoder.h>                          // Library for the encoders
#include "Adafruit_MCP23008.h"                      // Library for the I2C port expander(s)
#include "Si570.h"                                  // Library for the Si570 programmable oscillator
#include <irmp.h>                                   // Library for the IR remote

void AK4118Init();
void AK4493Init();
void SetDACAutoMode();
void SetDACParam();
void SetDACMode();
void SetDAC_SR();
void SetVol (byte RegVal);
void displayDACHelp();
void dispMain(bool full_redraw);
void ReadSpdifStatus(); 
void ReadSpdifSR();
void dispSigType();
void dispSR();
void dispNoLock();
void EnableDirectDSD(byte dac);
void SetSpdifMCKO1(int mcko1);
char getCommand();
void SetSpdifInput(int Input);
void displayMCLKHelp();
void SetDSDFilter(byte dac);
void SetFilter(byte dac);
void SetSoundMode(byte dac);
void SetInvertPhase(byte dac);
void dispInput();

void menu1_disp(bool full_redraw);

#define AutoDSD                                     // Set if automatic selection between PCM & DSD is required.

/*************************************************************************************************** 
  Si570 or Si544 Stuff..
****************************************************************************************************/

//#define SI570                                       // If Si570 is used
#define SI544                                       // If Si544 is used

#define SI570_I2C_ADDRESS 0x55                      // Si570 or Si544 I2C address (should always be 0x55)

#ifdef SI570
  #define SI570_FACTORY_FREQUENCY 22579200            // Change to factory set frequency of the Si570 (Mouser part)
  //#define SI570_FACTORY_FREQUENCY 56320000          // Change to factory set frequency of the Si570 (Ebay part)
  Si570 *vfo;                                         // Set up the Si570.
#else
#endif

/*************************************************************************************************** 
  IR receiver Stuff..
****************************************************************************************************/
#define SERIALX Serial1
#define PIN_LED PB0
IRMP_DATA irmp_data;
HardwareTimer timer(2);
unsigned loopcount = 0; // loop counter
int prev_result;

// Remote control codes. They correspond to an old remote that I use for testing - change to match your remote's.
#define POWER_CODE 18      // Code for power on/off
#define VOLUP_CODE 12      // Code for Volume up
#define VOLDOWN_CODE 6     // Code for Volume down
#define MUTE_CODE  2       // Code for mute
#define SETTINGS_CODE 11   // Code for DAC Settings ("Setup" button on my remote)
#define GLOBAL_SET_CODE 27 // Code for global DAC Settings ("Title" button on my remote)
#define SELECT_CODE 64     // Code for Select button
#define LEFT_CODE 25       // Code for left arrow
#define RIGHT_CODE 17      // Code for right arrow
#define SOURCE1_CODE 65    // Code for Input 1 button
#define SOURCE2_CODE 77    // Code for Input 2 button
#define SOURCE3_CODE 69    // Code for Input 3 button
#define SOURCE4_CODE 66    // Code for Input 4 button
#define SOURCE5_CODE 78    // Code for Input 5 button
#define SOURCE6_CODE 70    // Code for Input 6 button
#define SOURCE7_CODE 67    // Code for Input 7 button

/*************************************************************************************************** 
  TFT Stuff..
  Define used fonts etc.
****************************************************************************************************/
#include <Adafruit_GFX.h>
#include <gfxfont.h>
#include "Adafruit_ILI9481_8bit_STM.h"
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSerif9pt7b.h>
#include "Arimo_Bold_64.h"
#include "Arimo_Bold_32.h"
#include "Arimo_Bold_26.h"
#include "Arimo_100.h"

Adafruit_ILI9481_8bit_STM tft = Adafruit_ILI9481_8bit_STM();

// Color definitions
#define BLACK    0x0000
#define BLUE     0x001F
#define RED      0xF800
#define GREEN    0x07E0
#define CYAN     0x07FF
#define MAGENTA  0xF81F
#define YELLOW   0xFFE0 
#define WHITE    0xFFFF

/*************************************************************************************************** 
  More definitions..
****************************************************************************************************/

#define VOLUPPIN PB14                               // RotEnc A terminal for rotary encoder.
#define VOLDOWNPIN PB13                             // RotEnc B terminal for rotary encoder.
#define SELECTPIN PB15                              // Switch to select function for rotary encoder.
#define Si570_EN PC13                               // Set Enable pin of Si570
//#define DSDPIN PA11                                 // Set DSD pin any digital pin. You should connect this pin to your USB-to-I2S module's "DSD indicator" pin.
#define POWERPIN PB12                               // Power relay pin.
#define I2S_SELECT_PIN PC15                         // I2S source selection pin. Connect to the MUX on the XMOS & AK4118 board.
#define INTERVAL_BOUNCE 2                           // Time in milliseconds to debounce the rotary encoder
#define INTERVAL_SWITCHBOUNCE 200                   // Time in milliseconds to debounce switch
#define INTERVAL_MUTE 1000                           // Minimum mute time in milliseconds

#define eeprom 0x50                                 // device address for the 24LC256 EEPROM chip

RotaryEncoder encoder(VOLDOWNPIN, VOLUPPIN);        // Setup the Rotary Encoder

int irdir=0;                                        // Variable to hold the IR Remote's "rotation" direction

#define int_pin_mcp_iso   PC14                      // Define the bottom isolated I/O expander's interrupt pin.
#define int_pin_mcp_iso_2 PA11                      // Define the top isolated I/O expander's interrupt pin.

Adafruit_MCP23008 mcp_iso_2;                        // Set up the top isolated I/O expander.
Adafruit_MCP23008 mcp_iso;                          // Set up the bottom isolated I/O expander.

// IOPinsOfInterest2 -- list of GPIO pins we'll be monitoring on the top I/O expander... terminate list with any value > 7...
uint8_t IOPinsOfInterest2[] = {
  0, 1, 2, 3, 4, 5, 6, 7,
  0xff // terminator
};

// IOPinsOfInterest -- list of GPIO pins we'll be monitoring on the bottom I/O expander... terminate list with any value > 7...
uint8_t IOPinsOfInterest[] = {
  1, 4,
  0xff // terminator
};

// interrupt flag / service routine for top I/O expander
volatile bool inputsChanged2 = false;
void inputsHaveChanged2() {
  inputsChanged2 = true;
  Serial.println("Top I/O expander interrupt called");
}

// interrupt flag / service routine for bottom I/O expander
volatile bool inputsChanged = false;
//void inputsHaveChanged() {
//  inputsChanged = true;
//  Serial.println("Bottom I/O expander interrupt called");
//}

// Remove comment slashes ("//") to set code up for dual mono mode. 
#define DUALMONO                                    

// Remove comment slashes ("//") from your USB interface. Keep all other interfaces commented out.
//#define Amanero
#define Dim_XMOS

// Set whether you will be using a power on/off relay. Comment out ALWAYSON if you will be using a remote for power on/off.
#define ALWAYSON

#ifdef ALWAYSON
  bool poweron = true;        // Variable to hold whether the DAC is in power-on state or not
#else
  bool poweron = false;       // Variable to hold whether the DAC is in power-on state or not
#endif //ALWAYSON

/*************************************************************************************************** 
  Make sure you use the correct chip address(es) for your AK IC(s). 
  The chip address is determined by how CAD0 & CAD1 are wired.
****************************************************************************************************/

byte ak4493 = 0x13;       // device ak4493, 0010011 = 0x13 (if CAD1=CAD0=H, see datasheet p. 50)
//byte ak4493 = 0x10;     // device ak4493, 0010000 = 0x10 (if CAD1=CAD0=L, see datasheet p. 50)
//byte ak4493 = 0x12;     // device ak4493, 0010010 = 0x12 (if CAD1=H, CAD0=L, see datasheet p. 50)
//byte ak4493 = 0x11;     // device ak4493, 0010001 = 0x11 (if CAD1=L, CAD0=H, see datasheet p. 50)

#ifdef DUALMONO
//byte ak4493r = 0x13;    // device ak4493, 0010011 = 0x13 (if CAD1=CAD0=H, see datasheet p. 50)
//byte ak4493r = 0x10;    // device ak4493, 0010000 = 0x10 (if CAD1=CAD0=L, see datasheet p. 50)
byte ak4493r = 0x12;      // device ak4493, 0010010 = 0x12 (if CAD1=H, CAD0=L, see datasheet p. 50)
//byte ak4493r = 0x11;    // device ak4493, 0010001 = 0x11 (if CAD1=L, CAD0=H, see datasheet p. 50)
#endif //DUALMONO

//byte ak4118 = 0x13;     // device ak4118, 0010011 = 0x13 (if CAD1=CAD0=H, see datasheet p. 50)
//byte ak4118 = 0x10;     // device ak4118, 0010000 = 0x10 (if CAD1=CAD0=L, see datasheet p. 50)
//byte ak4118 = 0x12;     // device ak4118, 0010010 = 0x12 (if CAD1=H, CAD0=L, see datasheet p. 50)
byte ak4118 = 0x11;       // device ak4118, 0010001 = 0x11 (if CAD1=L, CAD0=H, see datasheet p. 50)

byte error;
byte r;
bool interrupt = 0;

// AK4493 Parameter variables
int volume = 195;         // Variable to hold the volume. From 0 to 255 (0.5db steps).
int volumeOld = 255;      // Previous state.
bool mute = 0;            // Variable to hold mute status.
bool muteDuration = 0;    // Variable to hold mute duration.
bool muteOld = 0;         // Previous state.
bool unmuterequest = 0;   // Variable to track unmute requests.
bool DACAutoMode = false; // Variable to hold status of AK4493 manual or auto mode. TRUE = auto, FALSE = manual.
bool DACAutoModeOld = true;  // Previous state.
int dacMode = 1;          // Variable to hold whether the DAC chips are in PCM or DSD Mode. PCM = 1, DSD = 2.
int dacModeOld = 0;       // Previous state.
int dacFilter = 1;        // Variable to hold selected DAC filter. Values from 1 to 5.
int dacFilterOld = 0;     // Previous state.
bool superSlow = false;   // Variable to hold DAC superslow mode. True = superslow enabled. False = superslow disabled.
bool superSlowOld = false;   // Previous state.
bool directDsd = false;   // Variable to hold DAC DirectDSD mode. True = DSD Direct enabled. False = DSD Direct disabled.
bool directDsdOld = false;   // Previous state.
int dsdFilter = 50;       // Variable to hold DSD cutoff filter frequency. 50 = KHz, 150 = 150KHz.
int dsdFilterOld = 50;    // Previous state.
int soundMode = 1;        // Variable to hold DAC SoundMode setting. Settings 1 to 3.
int soundModeOld = 1;     // Previous state.
bool InvertPhase = 0;     // Variable to hold DAC output phase inversion. Settings 0 or 1.
bool InvertPhaseOld = 1;  // Previous state.
int SR_DAC;               // Variable to hold incoming SR to the DAC chips.
int SR_DACOld = 1;        // Previous state.
bool mode_dac = 0;        // Mode of the AK4493. 0 = PCM, 1 = DSD


// General Parameters
bool preamp = 1;          // Variable to hold whether the DAC is running in preamp mode (with volume control) or not.
bool preampOld = 1;       // Previous state.
int I2S_Input;            // Variable to hold selected I2S input. 1 is s/pdif from AK4118, 0 is I2S input.
int I2S_InputOld = 1;     // Previous state.
int SR_I2S;               // Sampling rate output from I2S input (usualy USB-to-I2S interface).
int SR_I2SOld = 15;       // Previous state.
int dsdSignal = 0;        // Signal type from I2S input (usualy USB-to-I2S interface). PCM = 0, DSD = 1.
int dsdSignalOld = 0;     // Previous state.
int MCLK = 0;             // Variable to hold selected MCLK frequency.
int MCLKOld = 0;          // Previous state.
int MCLK_SET = 0;         // Variable to hold selected MCLK frequencies set. 0 for 22/24MHz, 1 for 45/49MHz.
int MCLK_L = 10;          // Variable to hold selected MCLK frequency for 44.1KHz family & DSD.
int MCLK_H = 11;          // Variable to hold selected MCLK frequency for 48KHz family.
bool Vol2Digit = false;   // Variable to hold whether the volume to be displayed is one or two digits wid
bool Vol2DigitOld = false;   // Previous state.
bool Si570_exists = false;
bool ak4118_exists = false;
bool Si570_enabled = true;

// AK4118 Parameter Variables
int SR_SPDIF;             // Sampling rate output from AK4118.
int SR_SPDIFOld = 99;     // Previous state.
int spdifInput;           // Variable to hold selected s/pdif input. Values from 1 to 8.
int spdifInputOld = 12;   // Previous state.
int spdifLock = 0;        // Detected s/pdif lock state.
int spdifLockOld = 0;     // Previous state.
int spdifStatus = 0;      // Detected s/pdif status code.
int spdifStatusOld = 0;   // Previous state.
int spdifPreemphasis = 0; // Detected s/pdif pre-emphasis flag.
int spdifPreemphasisOld = 0;   // Previous state.
int MCK01;                // Variable to hold MCK01 output frequency. Values 128, 256, 512.
int MCK01Old = 1;         // Previous state.


// Character array to store the names of the filters.
char* myFilters[]={"Invalid", "Linear Phase Sharp Roll-off", "Linear Phase Slow Roll-off", "Short Delay Sharp Roll-off", "Short Delay Slow Roll-off", "Super Slow Roll-off", 0};

// Character array to store the names of the inputs. Limited version - for the XMOS & S/PDIF receiver board
char* myInputs[]={"Invalid", "COAX 1", "COAX 2", "OPT 1", "OPT 2", "USB", 0};

// Character array to store the names of the inputs.
//char* myInputs[]={"Invalid", "COAX 1", "COAX 2", "COAX 3", "COAX 4", "OPT 1", "OPT 2", "OPT 3", "OPT 4", "USB", 0};

// Character array to store the names of the DSD types or Sampling Rates.
// SR = 0 = Invalid, SR = 1..5 = DSD, SR = 6..16 = PCM
char* mySR[]={"Invalid", "DSD1024", "DSD512", "DSD256", "DSD128", "DSD64", "768KHz", "705.6K", "384KHz", "352.8K", "192KHz", "176.4K", "96KHz", "88.2KHz", "48KHz", "44.1KHz", "32KHz", 0};

uint16_t xoffset,yoffset;

int count = 0;
char b[20], c[20], printBuff[32];

// Button stuff
unsigned long keyPrevMillis = 0;
const unsigned long keySampleIntervalMs = 25;
byte longKeyPressCountMax = 80;            // Duration of button press to be considered "long". 80 * 25 = 2000 ms
byte mediumKeyPressCountMin = 10;          // Duration of button press to be considered "medium". 40 * 25 = 1000 ms
byte KeyPressCount = 0;
byte prevKeyState = HIGH;                  // button is active low
const byte keyPin = SELECTPIN;             // button is connected to pin SELECTPIN and GND
int key = 0;                               // Variable to hold the state of the button

volatile int newPos;                       // Rotary encoder variable

unsigned long displayMillis = 0;           // Stores last recorded time for display interval
unsigned long debounceMillis = 0;          // Stores last recorded time for switch debounce interval
unsigned long selectMillis = 0;            // Stores last recorded time for being in select mode
unsigned long muteMillis = 0;              // Stores last recorded time for mute enablement

boolean select1Mode=false;                 // To indicate whether in Input options select mode or not
boolean selectInputMode=false;             // To indicate whether in Input Selection mode or not
unsigned long select1Millis = 0;           // Stores last recorded time for being in select mode
unsigned long selectInputMillis = 0;       // Stores last recorded time for being in Input select mode

byte select1 = 1;                          // To record first level select position (FIL, VOL, etc)

// The order of selection when clicking the select switch
#define FILT 1                             // Filter selection, also number of param variable
#define DIRDSD 2                           // DirectDSD selection, also number of param variable
#define DSDFILT 3                          // DSD filter setting, also number of param variable
#define SOUND 4                            // Sound Mode setting, also number of param variable
#define INVRT 5                            // Inverted output selection, also number of param variable
#define CLK 6                              // MCLK setting, also number of param variable

#define MAXPARAM 7      // Total number of parameters to keep track of for Settings menu.

int param = 0;          // Hold the number of the parameter to be changed

#define INTERVAL_SELECT 6                  // Time in sec to exit select mode when no activity

bool do_select_filter = 0;

// Variables that store the width of each parameter , to aid with "erasing" the text.
int param_1_width;
int param_2_width;
int param_3_width;
int param_4_width;
int param_5_width;
int param_6_width;
int param_7_width;
int param_8_width;

// Variable that stores the height of each line of parameters, to aid with "erasing" the text.
int param_height;

// Variables that store the state of selection of each parameter.
bool param_1_select;
bool param_2_select;
bool param_3_select;
bool param_4_select;
bool param_5_select;
bool param_6_select;
bool param_7_select;
bool param_8_select;

/************************ MAIN PROGRAM ************************************************************/

void setup()
{
  Serial.begin(115200);                 // Start the serial port (for troubleshooting)
  Serial.println("Serial port initialized");
  
  tft.begin();                          // Start the TFT
  //tft.invertDisplay(1);                 // Invert the display - use if necessary with your TFT!!
  tft.setRotation(3);                   // Set the correct orientation
  tft.fillScreen(ILI9481_BLACK);        // Clear the screen
  tft.setCursor(0, 50);
  tft.setFont(&Arimo_Bold_64);
  tft.println("AKM Controller");
  delay(100);

  pinMode(Si570_EN,OUTPUT);                               // Si570 enable pin pin. 
  digitalWrite(Si570_EN, HIGH);                           // Power up Si570.
  
  Wire.begin();                        // Join The I2C Bus As A Master 
  Serial.println("I2C port initialized");

  // Set up bottom isolated MCP23008 pins
  mcp_iso.begin(6);                     // use address 0x26 (A0=L, A1=A2=H)

  // IOConfiguration acts like a struct with the same fields as the IOCON register.  Each field uses an enum for clear and error free settings.
  Adafruit_MCP23008::IOConfiguration config = mcp_iso.configuration();
  config.intpol = Adafruit_MCP23008::ActiveHighInterrupt; // (or ActiveHighInterrupt)
  config.odr = Adafruit_MCP23008::ActiveDriver; // (or Adafruit_MCP23008::OpenDrain)
  mcp_iso.setConfiguration(config);
  mcp_iso.setInterruptOnChange(0); // clear all interrupts-on-change
  
  Serial.println("mcp_iso initialized");
  mcp_iso.pinMode(0, OUTPUT);
  mcp_iso.setInterruptOnChangePin(0, false);
  mcp_iso.pinMode(1, INPUT);
  mcp_iso.setInterruptOnChangePin(1, true, Adafruit_MCP23008::CompareToPreviousValue);
  mcp_iso.pinMode(2, OUTPUT);           // AK4493 PDN Control Pin
  mcp_iso.setInterruptOnChangePin(2, false);
  mcp_iso.pinMode(3, OUTPUT);           // Mute pin
  mcp_iso.setInterruptOnChangePin(3, false);
  mcp_iso.pinMode(4, INPUT);
  mcp_iso.setInterruptOnChangePin(4, true, Adafruit_MCP23008::CompareToPreviousValue);
  mcp_iso.pinMode(5, OUTPUT);
  mcp_iso.setInterruptOnChangePin(5, false);
  mcp_iso.pinMode(6, OUTPUT);
  mcp_iso.setInterruptOnChangePin(6, false);
  mcp_iso.pinMode(7, OUTPUT);
  mcp_iso.setInterruptOnChangePin(7, false);

  pinMode(int_pin_mcp_iso, INPUT);
  //attachInterrupt(int_pin_mcp_iso, inputsHaveChanged, CHANGE);
  
  Serial.println("Isolated MCP23008 initialized");
  tft.setCursor(0, 80);
  tft.setFont(&Arimo_Bold_26);
  tft.println("Isolated MCP23008 initialized");

  mcp_iso.digitalWrite(3, LOW);           // Mute the DAC
  
  // Set up top isolated MCP23008 pins (use to connect to USB to I2S board)
  mcp_iso_2.begin(2);                     // use address 0x22 (A0=A2=L, A1=H)

  // IOConfiguration acts like a struct with the same fields as the IOCON register.  Each field uses an enum for clear and error free settings.
  Adafruit_MCP23008::IOConfiguration config2 = mcp_iso_2.configuration();
  config2.intpol = Adafruit_MCP23008::ActiveHighInterrupt; // (or ActiveHighInterrupt)
  config2.odr = Adafruit_MCP23008::ActiveDriver; // (or Adafruit_MCP23008::OpenDrain)
  mcp_iso_2.setConfiguration(config2);
  mcp_iso_2.setInterruptOnChange(0); // clear all interrupts-on-change
  
  Serial.println("mcp_iso_2 initialized");
  mcp_iso_2.pinMode(0, INPUT);
  mcp_iso_2.setInterruptOnChangePin(0, true, Adafruit_MCP23008::CompareToPreviousValue);
  mcp_iso_2.pinMode(1, INPUT);
  mcp_iso_2.setInterruptOnChangePin(1, true, Adafruit_MCP23008::CompareToPreviousValue);
  mcp_iso_2.pinMode(2, INPUT);
  mcp_iso_2.setInterruptOnChangePin(2, true, Adafruit_MCP23008::CompareToPreviousValue);
  mcp_iso_2.pinMode(3, INPUT);
  mcp_iso_2.setInterruptOnChangePin(3, true, Adafruit_MCP23008::CompareToPreviousValue);
  mcp_iso_2.pinMode(4, INPUT);
  mcp_iso_2.setInterruptOnChangePin(4, true, Adafruit_MCP23008::CompareToPreviousValue);  
  mcp_iso_2.pinMode(5, INPUT);
  mcp_iso_2.setInterruptOnChangePin(5, true, Adafruit_MCP23008::CompareToPreviousValue);  
  mcp_iso_2.pinMode(6, INPUT);
  mcp_iso_2.setInterruptOnChangePin(6, true, Adafruit_MCP23008::CompareToPreviousValue);  
  mcp_iso_2.pinMode(7, INPUT);
  mcp_iso_2.setInterruptOnChangePin(7, true, Adafruit_MCP23008::CompareToPreviousValue);

  pinMode(int_pin_mcp_iso_2, INPUT);
//  attachInterrupt(int_pin_mcp_iso_2, inputsHaveChanged, RISING);
  
  Serial.println("Isolated_2 MCP23008 initialized");
  tft.setCursor(0, 100);
  tft.setFont(&Arimo_Bold_26);
  tft.println("Isolated_2 MCP23008 initialized");

  // IR decoder setup
  irmp_init();   // initialize irmp
  timer2_init(); // initialize timer2

  // Set up the pin modes
  pinMode(VOLUPPIN, INPUT);                               // Button switch or Encoder pin for volume up
  digitalWrite(VOLUPPIN, HIGH);                           // If H/W debouncing is implemented, set to LOW
  attachInterrupt(VOLUPPIN, interruptFunction, CHANGE);   // Attach interrrupt to this pin

  pinMode(VOLDOWNPIN, INPUT);                             // Button switch or Encoder pin for volume down
  digitalWrite(VOLDOWNPIN, HIGH);                         // If H/W debouncing is implemented, set to LOW
  attachInterrupt(VOLDOWNPIN, interruptFunction, CHANGE); // Attach interrrupt to this pin

  pinMode(SELECTPIN, INPUT_PULLUP);                       // Button switch or Encoder pin for Select  
  
  mcp_iso.digitalWrite(0, LOW);                           // Keep the AK4118's PDN pin low.
  delay(10);
  mcp_iso.digitalWrite(0, HIGH);                          // Power up the s/pdif receiver.
  
  pinMode(POWERPIN, OUTPUT);                              // Power relay control pin
  digitalWrite(POWERPIN, LOW);                            // Keep low on powerup

  pinMode(I2S_SELECT_PIN, OUTPUT);                        // I2S selector control pin
  digitalWrite(I2S_SELECT_PIN, LOW);                      // Keep low on powerup. Low = USB input.

  if ((readData(eeprom, 199) == 199) && (readData(eeprom, 200) == 200) && (readData(eeprom, 201) == 201))
    {
      Serial.println("EEPROM has already been initialized");
      tft.setCursor(0, 120);
      tft.setFont(&Arimo_Bold_26);
      tft.println("EEPROM already initialized");
    }
  else initEEPROM();
  
  readSettings();                                         // Read settings variables from EEPROM

  if (MCLK_SET == 0)
    {
      MCLK_L = 6;          // MCLK frequency for 44.1KHz family = 22.5792 MHz
      MCLK_H = 7;          // MCLK frequency for 48KHz family = 24.576 MHz
      Serial.println("22/24MHz clocks selected");
      tft.setCursor(0, 140);
      tft.setFont(&Arimo_Bold_26);
      tft.println("22/24MHz clocks selected");
    }
  else if (MCLK_SET == 1)
    {
      MCLK_L = 10;          // MCLK frequency for 44.1KHz family = 45.1584 MHz
      MCLK_H = 11;          // MCLK frequency for 48KHz family = 49.152 MHz
      Serial.println("45/49MHz clocks selected");
      tft.setCursor(0, 140);
      tft.setFont(&Arimo_Bold_26);
      tft.println("45/49MHz clocks selected");
    }

  if (poweron == false)
    {
      Serial.println("DAC in power off condition, going to sleep..");
      tft.setCursor(0, 160);
      tft.setFont(&Arimo_Bold_26);
      tft.println("DAC in power off condition, going to sleep..");
      delay(4000);
      tft.fillScreen(BLACK);
    }
  
  if (poweron == true)                                     // If DAC is always powered on, initialize everything..
    {
      #ifdef SI570
        Si570Init();
      #else
        Si544Init();
      #endif

      SetMCLK(10);                                        // Start up MCLK
      
      AK4118Init();
      AK4493Init();
      //I2S_Input = 1;                                            // Select S/PDIF Input
      I2S_Input = 0;                                            // Select USB Input
      SetI2S_Input();

      //SRC_EN = true;                                            // Enable AK4137 ASRC mode
      //SetSRC(SRC_EN);
  
      DACAutoMode = false;                                      // Set initial DAC mode to Auto or Manual
      SetDACAutoMode();                                         // Set initial DAC mode to Auto or Manual
      USB_SR();                               // Check signal type & sampling rate
      SR_DAC = SR_I2S;
      SetDACMode();
      SetDAC_SR();
      SetVol(volume);
      
      displayDACHelp();
      fullRedrawMain();
      delay(1000);
      //digitalWrite(POWERPIN, HIGH);                             // Un-mute DAC
      dacMute(0);
    }

}                               

/*********************** Loop Section *************************************************************/

void loop()                 
{
  if ((millis() - muteMillis > INTERVAL_MUTE) && (unmuterequest==1))
    {
      Serial.println("Now we are allowed to unmute the DAC");
      dacMute(0);
      unmuterequest = 0;
    }
  
  if (I2S_Input == 1)                         // If using the S/PDIF input
    {
      if (digitalRead(int_pin_mcp_iso) == true)
      //if (inputsChanged == true)
        {
          Serial.println("Bottom I/O expander interrupt detected");
          uint8_t val = mcp_iso.interruptCapture(); // clears the interrupt flag on the MCP (INT pin goes low)
          //inputsChanged=false;
          interrupt = mcp_iso.digitalRead(1);     // Check to see if the interrupt pin of AK4118 has changed.
          if (interrupt == true)                  // If it has, read the new status of the AK4118.
            {
              //Serial.println("AK4118 interrupt detected");
              //ReadSpdifStatus();
              //Serial.print("spdifLock: "); Serial.println(spdifLock);            
              //digitalWrite(POWERPIN, LOW);
              //dacMute(1);
              //delay(200);
              ReadSpdifStatus();
              //Serial.print("spdifLock: "); Serial.println(spdifLock);            
              if (spdifLock == 1)
                {
                  ReadSpdifSR();                           // Detect the current sampling rate.
                  delay(200);
                  ReadSpdifSR();                           // Detect the current sampling rate.
                  SR_DAC = SR_SPDIF;
                  Serial.print("SR_SPDIF: "); Serial.println(SR_SPDIF);
                  SetDACParam();
                  dispSigType();
                  dispSR();
                  //dispMain(false);
                  spdifLockOld = spdifLock;
                }
              if ((spdifLock == 0) && (spdifLockOld == 1))
                {
                  Serial.println("S/PDIF receiver lock status changed");
                  dispNoLock();
                  //dispMain(false);
                  spdifLockOld = spdifLock;
                }            
              //digitalWrite(POWERPIN, HIGH);
              //unmuterequest = 1;
              //dispMain(false);
            }
        }      
    }  
  else if (I2S_Input == 0)                         // If using the I2S input
    {
      if (digitalRead(int_pin_mcp_iso_2) == true)
        {
          USB_SR();                               // Check signal type & sampling rate
          SR_DAC = SR_I2S;
        }
    }
      
  if (dsdSignal != dsdSignalOld)
    {
      Serial.println("Signal type changed");
      //Serial.print("dsdSignal: "); Serial.println(dsdSignal);
      //Serial.print("dacMode: "); Serial.println(dacMode);
      //digitalWrite(POWERPIN, LOW);
      //dacMute(1);
      //Serial.println("DAC muted");
      //delay(100);
      if ((dsdSignal == 1) && (dacMode == 1)) // toggle PCM & DSD modes
        {
          dacMode = 2;                        // Set to DSD
          SetDACMode();
          MCLK = MCLK_L;
          SetMCLK(MCLK);
          if (select1Mode==false)
            {
              dispMain(false);
            }
          Serial.println("Signal is DSD");
        }
  
      if ((dsdSignal == 0) && (dacMode == 2)) // toggle PCM & DSD modes
        {
          directDsd = 0;
          EnableDirectDSD(ak4493);
          #ifdef DUALMONO
            EnableDirectDSD(ak4493r);
          #endif //DUALMONO
          dacMode = 1;                        // Set to PCM
          SetDACMode();
          if (select1Mode==false)
            {
              dispMain(false);
            }
          Serial.println("Signal is PCM");
        }
      dsdSignalOld = dsdSignal;
      delay(100);
      //digitalWrite(POWERPIN, HIGH);
      //unmuterequest = 1;
    }
  
  if (SR_DAC != SR_DACOld)                                      // If the SR has changed, .
    {
      //digitalWrite(POWERPIN, LOW);
      //dacMute(1);
      //delay(100);
      Serial.print(F("I2S sampling rate has changed, new one is "));
      Serial.println(mySR[SR_DAC]);

      //SR_DAC = SR_I2S;
      //SetDAC_SR();
      
      if ((SR_DAC == 15) || (SR_DAC == 13) || (SR_DAC == 11) || (SR_DAC == 9) || (SR_DAC == 5) || (SR_DAC == 4) || (SR_DAC == 3) || (SR_DAC == 2))
        {
          MCLK = MCLK_L;
          SetMCLK(MCLK);
          if (select1Mode==false)
            {
              dispMain(false);
            }
          SR_DACOld = SR_DAC;
        }
      else 
        {
          MCLK = MCLK_H;
          SetMCLK(MCLK);
          if (select1Mode==false)
            {
              dispMain(false);
            }
          SR_DACOld = SR_DAC;
        }
    SetDAC_SR();
    delay(100);
    //digitalWrite(POWERPIN, HIGH);
    //unmuterequest = 1;
    }


// ----------------------------------------------- Serial Port Prompt Code -------------------------------------------------------------------

char command = getCommand();
switch (command)
  {
    case '1':
      Serial.println(F("Select Input 1 (USB)."));
      I2S_Input = 0;                                            // Select USB Input
      SetI2S_Input();                                           // Apply new I2S input
      dispMain(false);
      break;
    case '2':
      Serial.println(F("Select Input 2 (COAX 1)."));
      I2S_Input = 1;                                            // Select S/PDIF Input
      dsdSignal = 0;                                            // Signal type can only be PCM
      SetSpdifMCKO1(512);
      SetI2S_Input();                                           // Apply new I2S input
      spdifInput = 1;                                           // Select Input RX1 (COAX 1) on AK4118 receiver.
      SetSpdifInput(spdifInput);                                // Apply new S/PDIF input
      ReadSpdifStatus();            
      delay(100);
      ReadSpdifStatus();            
        if (spdifLock == 1)
          {
            ReadSpdifSR();                           // Detect the current sampling rate.
            SR_DAC = SR_SPDIF;
            Serial.print("SR_SPDIF: "); Serial.println(SR_SPDIF);
            SetDACParam();
          }
      dispMain(false);
      break;
    case '3':
      Serial.println(F("Select Input 5 (OPT 1)."));
      I2S_Input = 1;                                            // Select S/PDIF Input
      dsdSignal = 0;                                            // Signal type can only be PCM
      SetSpdifMCKO1(512);
      SetI2S_Input();                                           // Apply new I2S input
      spdifInput = 3;                                           // Select Input RX4 (OPT 1) on AK4118 receiver.
      SetSpdifInput(spdifInput);                                // Apply new S/PDIF input
      ReadSpdifStatus();   
      delay(100);
      ReadSpdifStatus();       
        if (spdifLock == 1)
          {
            ReadSpdifSR();                           // Detect the current sampling rate.
            SR_DAC = SR_SPDIF;
            Serial.print("SR_SPDIF: "); Serial.println(SR_SPDIF);
            SetDACParam();
          }
      dispMain(false);
      break;
    case 'v':
      displayMCLKHelp();
      break;
    case 'w':
      displayDACHelp();
      break;
    case 'z':
      displayDACHelp();
      break;
    case '%':
      PrintDebugReg(ak4493r);
      PrintDebugReg(ak4493);
      break;
    case 'Q':
      SetMCLK(1);
      dispMain(false);
      break;
    case 'W':
      SetMCLK(2);
      dispMain(false);
      break;
    case 'E':
      SetMCLK(3);
      dispMain(false);
      break;
    case 'R':
      SetMCLK(4);
      dispMain(false);
      break;
    case 'T':
      SetMCLK(5);
      dispMain(false);
      break;
    case 'Y':
      SetMCLK(6);
      dispMain(false);
      break;
    case 'U':
      SetMCLK(7);
      dispMain(false);
      break;
    case 'I':
      SetMCLK(8);
      dispMain(false);
      break;
    case 'O':
      SetMCLK(9);
      dispMain(false);
      break;
    case 'P':
      SetMCLK(10);
      dispMain(false);
      break;
    case '{':
      SetMCLK(11);
      dispMain(false);
      break;
    case 'A':
      dacMode = 1;
      SetDACMode();
      break;
    case 'B':
      dacMode = 2;
      SetDACMode();
      break;
    case 'C':
      directDsd = 1;
      EnableDirectDSD(ak4493);
      #ifdef DUALMONO
        EnableDirectDSD(ak4493r);
      #endif //DUALMONO
      break;
    case 'D':
      directDsd = 0;
      EnableDirectDSD(ak4493);
      #ifdef DUALMONO
        EnableDirectDSD(ak4493r);
      #endif //DUALMONO
      break;
    case 'X':
      dsdFilter = 50;
      SetDSDFilter(ak4493);
      #ifdef DUALMONO
        SetDSDFilter(ak4493r);
      #endif //DUALMONO
      break;
    case 'F':
      dsdFilter = 150;
      SetDSDFilter(ak4493);
      #ifdef DUALMONO
        SetDSDFilter(ak4493r);
      #endif //DUALMONO
      break;
    case 'G':
      dacFilter = 1;
      SetFilter(ak4493);
      #ifdef DUALMONO
        SetFilter(ak4493r);
      #endif //DUALMONO
      break;
    case 'H':
      dacFilter = 2;
      SetFilter(ak4493);
      #ifdef DUALMONO
        SetFilter(ak4493r);
      #endif //DUALMONO
      break;
    case 'V':
      dacFilter = 3;
      SetFilter(ak4493);
      #ifdef DUALMONO
        SetFilter(ak4493r);
      #endif //DUALMONO
      break;
    case 'J':
      dacFilter = 4;
      SetFilter(ak4493);
      #ifdef DUALMONO
        SetFilter(ak4493r);
      #endif //DUALMONO
      break;
    case 'K':
      dacFilter = 5;
      SetFilter(ak4493);
      #ifdef DUALMONO
        SetFilter(ak4493r);
      #endif //DUALMONO
      break;
    case 'M':
      soundMode = 1;
      SetSoundMode(ak4493);      
      #ifdef DUALMONO
        SetSoundMode(ak4493r);
      #endif //DUALMONO
      break;
    case 'N':
      soundMode = 2;
      SetSoundMode(ak4493);      
      #ifdef DUALMONO
        SetSoundMode(ak4493r);
      #endif //DUALMONO
      break;
    case 'S':
      soundMode = 3;
      SetSoundMode(ak4493);      
      #ifdef DUALMONO
        SetSoundMode(ak4493r);
      #endif //DUALMONO
      break;
    case '<':
      InvertPhase = 0;
      SetInvertPhase(ak4493);      
      #ifdef DUALMONO
        SetInvertPhase(ak4493r);
      #endif //DUALMONO
      break;      
    case '>':
      InvertPhase = 1;
      SetInvertPhase(ak4493);      
      #ifdef DUALMONO
        SetInvertPhase(ak4493r);
      #endif //DUALMONO
      break;
    case 'a':
      ReadSpdifSR();
      SR_DAC = SR_SPDIF;
      SetDACParam();
      dispSigType();
      dispSR();
      break;
    case 'h':
      SetSpdifMCKO1(128);
      break;
    case 'i':
      SetSpdifMCKO1(256);
      break;
    case 'j':
      SetSpdifMCKO1(512);
      break;
      
    default:
      break;
  }

// ------------------------  Rotary encoder code ----------------------------------------------
  static int pos = 0;
  int dir = 0;

  if (pos != newPos)                  // Determine direction of rotation of the rotary encoder.
  {
    if (pos < newPos)
    {
      dir = 1;
      pos = newPos;
    }
    else if (pos > newPos)
      {
        dir = 2;
        pos = newPos;
      }
  }
  
  if ((dir==1 || dir==2 || irdir==1 || irdir==2) && select1Mode==false && selectInputMode==false && poweron == true)      // Default operation of the encoder: Change volume
  {
    if (dir==1 || irdir==1)                                   // CCW
      {
        if (volume < 255)
        {
          volume=volume+2;
          Serial.print("Volume: "); Serial.println(volume);
          SetVol(volume);
          dispVolume(false);
          //dispMain(false);
        }
        irdir=0;
      }
      else                                                    // If not CCW, then it is CW
      {
        if (volume > 56)
        {
          volume=volume-2;
          Serial.print("Volume: "); Serial.println(volume);
          SetVol(volume);
          dispVolume(false);
          //dispMain(false);
        }
        irdir=0;
      }
  }

  if((dir==1 || dir==2 || irdir==1 || irdir==2) && select1Mode==true && selectInputMode==false && poweron == true)     // When in select1Mode
  {
    switch(select1%(MAXPARAM))
      {  
      case FILT:
      if (dir==1 || irdir==1)                                   // CW
        {
          if (dacFilter < 5)
            {
              dacFilter++;
            }
          else dacFilter = 1;
        }
      else                                                      // If not CW, then it is CCW
        {
          if (dacFilter > 1)
            {
              dacFilter--;
            }
          else dacFilter = 5;
        }
      param=FILT;
      SetFilter(ak4493);
      #ifdef DUALMONO
        SetFilter(ak4493r);
      #endif //DUALMONO
      menu1_disp(false);
      writeSettings();
      select1Millis=millis();
      irdir=0;
      break;
      
      case DIRDSD:
      if (dir==1 || irdir==1)                                   // CW
        {
          if (directDsd == true)
            {
              directDsd=false;
            }
          else if (directDsd == false)
                 {
                   directDsd=true;
                 }
        }
      else                                                      // If not CW, then it is CCW
        {
          if (directDsd == false)
            {
              directDsd = true;
            }
          else if (directDsd == true)
                 {
                   directDsd = false;
                 }            
        }
      param=DIRDSD;
      EnableDirectDSD(ak4493);
      #ifdef DUALMONO
        EnableDirectDSD(ak4493r);
      #endif //DUALMONO
      menu1_disp(false);
      writeSettings();
      select1Millis=millis();
      irdir=0;
      break;

      case DSDFILT:
      if (dir==1 || irdir==1)                               // CW
        {
          if (dsdFilter == 50)
            {
              dsdFilter = 150;
            }
          else if (dsdFilter == 150)
                 {
                   dsdFilter = 50;
                 }
        }
      else                                                  // If not CW, then it is CCW
        {
          if (dsdFilter == 150)
            {
              dsdFilter = 50;
            }
          else if (dsdFilter == 50)
                 {
                   dsdFilter = 150;
                 }            
        }
      param=DSDFILT;
      SetDSDFilter(ak4493);
      #ifdef DUALMONO
        SetDSDFilter(ak4493r);
      #endif //DUALMONO
      menu1_disp(false);
      writeSettings();
      select1Millis=millis();
      irdir=0;
      break;

      case SOUND:
      if (dir==1 || irdir==1)                             // CW
        {
          if (soundMode < 3)
            {
              soundMode++;
            }
          else soundMode = 1;
        }
      else                                                // If not CW, then it is CCW
        {
          if (soundMode > 1)
            {
              soundMode--;
            }
          else soundMode = 3;          
        }
      param=SOUND;
      SetSoundMode(ak4493);      
      #ifdef DUALMONO
        SetSoundMode(ak4493r);
      #endif //DUALMONO
      menu1_disp(false);
      writeSettings();
      select1Millis=millis();
      irdir=0;
      break;

      case INVRT:
      if (dir==1 || irdir==1)                           // CW
        {
          if (InvertPhase == 0)
            {
              InvertPhase = 1;
            }
          else if (InvertPhase == 1)
                {
                  InvertPhase = 0;
                }
        }
      else                                              // If not CW, then it is CCW
        {
          if (InvertPhase == 1)
            {
              InvertPhase = 0;
            }
          else if (InvertPhase == 0)
                {
                  InvertPhase = 1;
                }            
        }
      param=INVRT;
      SetInvertPhase(ak4493);      
      #ifdef DUALMONO
        SetInvertPhase(ak4493r);
      #endif //DUALMONO
      menu1_disp(false);
      writeSettings();
      select1Millis=millis();
      irdir=0;
      break;
      
      case CLK:
      if (dir==1 || irdir==1)                         // CW
        {
          if (MCLK_SET == 0)
            {
              MCLK_SET = 1;
            }
          else if (MCLK_SET == 1)
            {
              MCLK_SET = 0;
            }
        }
      else                                            // If not CW, then it is CCW
        {
          if (MCLK_SET == 0)
            {
              MCLK_SET = 1;
            }
          else if (MCLK_SET == 1)
            {
              MCLK_SET = 0;
            }            
        }
      if (MCLK_SET == 0)
        {
          MCLK_L = 6;          // MCLK frequency for 44.1KHz family = 22.5792 MHz
          MCLK_H = 7;          // MCLK frequency for 48KHz family = 24.576 MHz
        }
      else if (MCLK_SET == 1)
        {
          MCLK_L = 10;          // MCLK frequency for 44.1KHz family = 45.1584 MHz
          MCLK_H = 11;          // MCLK frequency for 48KHz family = 49.152 MHz
        }
      writeSettings();
      param=CLK;
      menu1_disp(false);
      select1Millis=millis();
      irdir=0;
      break;

    }
    dir=0;
  }  // End of "in select1Mode"
    
  if((dir==1 || dir==2 || irdir==1 || irdir==2) && select1Mode==false && selectInputMode==true && poweron == true)     // When in selectInputMode
  {
      if (dir==1 || irdir==1)                                           // CW
        {
          if (I2S_Input == 0)                                           // If in USB mode
            {
              I2S_Input = 1;                                            // Select S/PDIF Input
              dsdSignal = 0;                                            // Signal type can only be PCM
              SetSpdifMCKO1(512);
              SetI2S_Input();                                           // Apply new I2S input
              spdifInput = 1;                                           // Select Input RX1 (COAX 1) on AK4118 receiver.
              SetSpdifInput(spdifInput);                                // Apply new S/PDIF input
              ReadSpdifStatus();            
              delay(100);
              ReadSpdifStatus();            
                if (spdifLock == 1)
                  {
                    ReadSpdifSR();                           // Detect the current sampling rate.
                    SR_DAC = SR_SPDIF;
                    Serial.print("SR_SPDIF: "); Serial.println(SR_SPDIF);
                    SetDACParam();
                  }
              dispMain(false);              
            }
          else if (I2S_Input == 1)                                      // If in S/PDIF mode
            {
              if (spdifInput == 4)                                      // If S/PDIF input 4 is selected, switch to USB
                {
                  I2S_Input = 0;                                        // Select USB Input
                  SetI2S_Input();                                       // Apply new I2S input
                  dispMain(false);
                }
              else if (spdifInput < 4)                                  // If other S/PDIF input is selected, switch to next S/PDIF input
                {
                  spdifInput = spdifInput + 1;                          // Select next Input on AK4118 receiver.
                  SetSpdifInput(spdifInput);                            // Apply new S/PDIF input
                  ReadSpdifStatus();            
                  delay(100);
                  ReadSpdifStatus();            
                    if (spdifLock == 1)
                      {
                        ReadSpdifSR();                                  // Detect the current sampling rate.
                        SR_DAC = SR_SPDIF;
                        Serial.print("SR_SPDIF: "); Serial.println(SR_SPDIF);
                        SetDACParam();
                      }
                  dispMain(false);
                }
            }
        }
      else                                                              // If not CW, then it is CCW
        {
          if (I2S_Input == 0)                                           // If in USB mode
            {
              I2S_Input = 1;                                            // Select S/PDIF Input
              dsdSignal = 0;                                            // Signal type can only be PCM
              SetSpdifMCKO1(512);
              SetI2S_Input();                                           // Apply new I2S input
              spdifInput = 4;                                           // Select Input RX5 (OPT 2) on AK4118 receiver.
              SetSpdifInput(spdifInput);                                // Apply new S/PDIF input
              ReadSpdifStatus();            
              delay(100);
              ReadSpdifStatus();            
                if (spdifLock == 1)
                  {
                    ReadSpdifSR();                                      // Detect the current sampling rate.
                    SR_DAC = SR_SPDIF;
                    Serial.print("SR_SPDIF: "); Serial.println(SR_SPDIF);
                    SetDACParam();
                  }
              dispMain(false);              
            }
          else if (I2S_Input == 1)                                      // If in S/PDIF mode
            {
              if (spdifInput == 1)                                      // If S/PDIF input 1 is selected, switch to USB
                {
                  I2S_Input = 0;                                        // Select USB Input
                  SetI2S_Input();                                       // Apply new I2S input
                  dispMain(false);
                }
              else if (spdifInput > 1)                                  // If other S/PDIF input is selected, switch to next S/PDIF input
                {
                  spdifInput = spdifInput - 1;                          // Select previous Input on AK4118 receiver.
                  SetSpdifInput(spdifInput);                            // Apply new S/PDIF input
                  ReadSpdifStatus();            
                  delay(100);
                  ReadSpdifStatus();            
                    if (spdifLock == 1)
                      {
                        ReadSpdifSR();                                  // Detect the current sampling rate.
                        SR_DAC = SR_SPDIF;
                        Serial.print("SR_SPDIF: "); Serial.println(SR_SPDIF);
                        SetDACParam();
                      }
                  dispMain(false);
                }
            }
        }
      dispMain(false);
      selectInputMillis=millis();
      irdir=0;
      dir=0;
  }  // End of "in selectInputMode"

// ------------------------  Rotary encoder button code ----------------------------------------------

  buttonPressed();                                                  // Get status of button
  if (key==1)                                                       // Momentary press of the button
    {
      if (select1Mode == false)                                     // If not in Select mode
        {
          if (poweron == false)                                     // If not powered on, do power on.
          {
            poweron = true;
            //digitalWrite(POWERPIN, HIGH);
            Serial.println(F("Powering on"));
            #ifdef SI570
              Si570Init();
            #else
              Si544Init();
            #endif
            AK4493Init();
            I2S_Input = 0;
            //dacMode = 1;                                              // Set default AK4493 signal type to PCM.
            //SetDACMode();
            DACAutoMode = false;                                      // Set initial DAC mode to Auto or Manual
            SetDACAutoMode();                                         // Set initial DAC mode to Auto or Manual
            USB_SR();
            SetDACMode();
            SetDAC_SR();
            SetVol(volume);
                        
            displayDACHelp();
            fullRedrawMain();
            delay(100);
            key=0;
          }
          else if (poweron == true)                                     // If powered on, go into input selection mode.
          {
            selectInputMode = true;
            Serial.println("Go into Input selection mode.");
            //delay(100);
            selectInputMillis=millis();
            //dispMain(false);
            dispInput();
            key=0;
          }
        }      
      else if (select1Mode == true)                                 // If in Select mode
        {
          select1++;
          Serial.println(select1);
          switch(select1%(MAXPARAM))
            {  
              case FILT:
              param=FILT;
              menu1_disp(false);
              select1Millis=millis();
              key=0;
              break;
      
              case DIRDSD:
              param=DIRDSD;
              menu1_disp(false);
              select1Millis=millis();
              key=0;
              break;

              case DSDFILT:
              param=DSDFILT;
              menu1_disp(false);
              select1Millis=millis();
              key=0;
              break;              

              case SOUND:
              param=SOUND;
              menu1_disp(false);
              select1Millis=millis();
              key=0;
              break;

              case INVRT:
              param=INVRT;
              menu1_disp(false);
              select1Millis=millis();
              key=0;
              break;

              case CLK:
              param=CLK;
              menu1_disp(false);
              select1Millis=millis();
              key=0;
              break;
           }
         Serial.print("Param: "); Serial.println(param);
      }
    }
 
  else if (key==2)                                                  // Long press of the button
    {
      if (select1Mode == false)                                     // If not in Select mode
        {
          if (poweron == false)                                     // If not powered on, do power on.
          {
            poweron = true;
            //digitalWrite(POWERPIN, HIGH);
            Serial.println(F("Powering on"));
            #ifdef SI570
              Si570Init();
            #else
              Si544Init();
            #endif
            AK4493Init();
            I2S_Input = 0;
            //dacMode = 1;                                              // Set default AK4493 signal type to PCM.
            //SetDACMode();
            DACAutoMode = false;                                      // Set initial DAC mode to Auto or Manual
            SetDACAutoMode();                                         // Set initial DAC mode to Auto or Manual
            USB_SR();
            SetDACMode();
            SetDAC_SR();
            SetVol(volume);
                        
            displayDACHelp();
            fullRedrawMain();
            delay(100);
            key=0;
          }
          else if (poweron == true)                                 // If powered on, go to menu mode.
          {
            Serial.println(F("Go into menu mode."));
            select1Mode = true;
            select1Millis=millis();                                         // Start being-in-select-mode timer
            param=1;
            menu1_disp(true);
            key=0;  
          }
        }

    }
  
  else if (key==3)                                                        // Very long press of the button
    {

    }
// End of rotary encoder button code

// ------------------------  IR receiver code ----------------------------------------------
   
  if (irmp_get_data (&irmp_data))
    {
      if (irmp_data.command == POWER_CODE)                                      // What to do if the Power button is pressed
      {
        if (poweron == false)                                             // If not powered on, do power on.
          {
            poweron = true;
            //digitalWrite(POWERPIN, HIGH);
            Serial.println(F("Powering on"));
            #ifdef SI570
              Si570Init();
            #else
              Si544Init();
            #endif
            AK4493Init();
            I2S_Input = 0;
            DACAutoMode = false;                                      // Set initial DAC mode to Auto or Manual
            SetDACAutoMode();                                         // Set initial DAC mode to Auto or Manual
            SetVol(volume);
            displayDACHelp();
            fullRedrawMain();
            delay(500);
            prev_result = 0;
          }
        else if (poweron == true)                                         // If powered on, power off.
          {
            poweron = false;
            //digitalWrite(POWERPIN, LOW);
            Serial.println(F("Powering off"));
            tft.fillScreen(BLACK);
          }
      } 

    else if (irmp_data.command == SELECT_CODE)                               // What to do if the Select button is pressed
      {
        if (poweron == true) 
          {
            key = 1;                                                     // Set the variable "key" to 1, so same as pressing the rot enc.
            prev_result = 0;
          }
      }  

    else if (irmp_data.command == LEFT_CODE)                                 // What to do if the Left button is pressed
      {
        //Serial.write("Left\n");
        if (poweron == true)
          {
            irdir = 2;    
            //prev_result = 0;
          }
      } 
    
    else if (irmp_data.command == RIGHT_CODE)                                 // What to do if the Right button is pressed
      {
        //Serial.write("Right\n");
        if (poweron == true)
          { 
            irdir = 1;  
            //prev_result = 0;
          }
      } 

    else if (irmp_data.command == MUTE_CODE)                                  // What to do if the Mute button is pressed
      {
      //Serial.write("Mute\n");
        if ((poweron == true) && (mute == false))                         // If powered on and not muted, do mute
          {
            mute = true;
            //mcp_iso.digitalWrite(3, LOW);                                // Mute the DAC
            //digitalWrite(POWERPIN, LOW);                                 // Alternative way of driving mute relay
            dacMute(1);
            //setMute();
            //dispMain(false);
          }
        else if ((poweron == true) && (mute == true))                     // If powered on and mute, do unmute
          {
            mute = false;
            //mcp_iso.digitalWrite(3, HIGH);                               // Un-mute the DAC
            //digitalWrite(POWERPIN, HIGH);                                // Alternative way of driving mute relay
            unmuterequest = 1;
            //dispMain(false);
          }
        //prev_result = 0;
      }     
    
    else if ((irmp_data.command == VOLUP_CODE) && (poweron == true))          // What to do if the Volume Up button is pressed
      {  
        if (poweron)
          {
            if ((volume < 255) && (preamp == true))                         // If in preamp mode, change volume
              {
                volume=volume+2;
                Serial.print("Volume: "); Serial.println(volume);
                SetVol(volume);
                irdir=0;
              }      
            //prev_result = VOLUP_CODE;
          }
      }

    else if ((irmp_data.command == VOLDOWN_CODE) && (poweron == true))        // What to do if the Volume Down button is pressed
      {    
        if (poweron)
          {
            if ((volume > 56) && (preamp == true))                       // If in preamp mode, change volume
              {
                volume=volume-2;
                Serial.print("Volume: "); Serial.println(volume);
                SetVol(volume);
                irdir=0;       
              } 
            //prev_result = VOLDOWN_CODE;
          }
      }

    else if ((irmp_data.command == SETTINGS_CODE) && (poweron == true))       // What to do if the Settings button is pressed
      {    
        if (poweron)
          {
            key = 1;
            //prev_result = SETTINGS_CODE;
          }
      }

    else if ((irmp_data.command == GLOBAL_SET_CODE) && (poweron == true))       // What to do if the Settings button is pressed
      {    
        if (poweron)
          {
            Serial.println(F("Go into global settings menu mode."));
            //select1Mode = false;
            //select2Mode = true;
            //select2Millis=millis();                                         // Start being-in-select-mode timer
            //setting=1;
            //param=1;
            //settings_disp(true);
            //prev_result = GLOBAL_SET_CODE;
          }
      }

    else if ((irmp_data.command == SOURCE1_CODE) && (poweron == true))       // What to do if the Input 1 button is pressed
      {    
        if (poweron)
          {
            Serial.println(F("Select Input 1 (USB)."));
            I2S_Input = 0;                                            // Select USB Input
            SetI2S_Input();                                           // Apply new I2S input
            if (select1Mode==false ) dispMain(false);                 // If not inside a menu, refresh the main screen.
          }
      }
    else if ((irmp_data.command == SOURCE2_CODE) && (poweron == true))       // What to do if the Input 2 button is pressed
      {    
        if (poweron)
          {
            Serial.println(F("Select Input 1 (COAX 1)."));
            I2S_Input = 1;                                            // Select S/PDIF Input
            SetI2S_Input();                                           // Apply new I2S input
            spdifInput = 1;                                           // Select Input RX1 (COAX 1) on AK4118 receiver.
            SetSpdifInput(spdifInput);                                // Apply new S/PDIF input
            if (select1Mode==false ) dispMain(false);                 // If not inside a menu, refresh the main screen.
          }
      }
    else if ((irmp_data.command == SOURCE3_CODE) && (poweron == true))       // What to do if the Input 3 button is pressed
      {    
        if (poweron)
          {
            Serial.println(F("Select Input 3 (COAX 2)."));
            I2S_Input = 1;                                            // Select S/PDIF Input
            SetI2S_Input();                                           // Apply new I2S input
            spdifInput = 2;                                           // Select Input RX2 (COAX 2) on AK4118 receiver.
            SetSpdifInput(spdifInput);                                // Apply new S/PDIF input
            if (select1Mode==false ) dispMain(false);                 // If not inside a menu, refresh the main screen.
          }
      }
    else if ((irmp_data.command == SOURCE4_CODE) && (poweron == true))       // What to do if the Input 4 button is pressed
      {    
        if (poweron)
          {
            Serial.println(F("Select Input 4 (COAX 3)."));
            I2S_Input = 1;                                            // Select S/PDIF Input
            SetI2S_Input();                                           // Apply new I2S input
            spdifInput = 3;                                           // Select Input RX4 (OPT 1) on AK4118 receiver.
            SetSpdifInput(spdifInput);                                // Apply new S/PDIF input
            if (select1Mode==false ) dispMain(false);                 // If not inside a menu, refresh the main screen.
          }
      }
    else if ((irmp_data.command == SOURCE5_CODE) && (poweron == true))       // What to do if the Input 5 button is pressed
      {    
        if (poweron)
          {
            Serial.println(F("Select Input 5 (OPT 1)."));
            I2S_Input = 1;                                            // Select S/PDIF Input
            SetI2S_Input();                                           // Apply new I2S input
            spdifInput = 4;                                           // Select Input RX5 (OPT 2) on AK4118 receiver.
            SetSpdifInput(spdifInput);                                // Apply new S/PDIF input
            if (select1Mode==false ) dispMain(false);                 // If not inside a menu, refresh the main screen.
          }
      }
    else if ((irmp_data.command == SOURCE6_CODE) && (poweron == true))       // What to do if the Input 6 button is pressed
      {    
        if (poweron)
          {
            Serial.println(F("Select Input 6 (OPT 2)."));
            I2S_Input = 1;                                            // Select S/PDIF Input
            SetI2S_Input();                                           // Apply new I2S input
            spdifInput = 5;                                           // Select Input RX6 (OPT 3) on AK4118 receiver.
            SetSpdifInput(spdifInput);                                // Apply new S/PDIF input
            if (select1Mode==false ) dispMain(false);                 // If not inside a menu, refresh the main screen.
          }
      }
 
    else 
      {
        Serial.print(F("unexpected value: "));                               // If the IR code is not recognized, output it to Serial
        Serial.println(irmp_data.command);
        prev_result = 0;
      }
    }


// ----------------------------------------------- Come out of Select1Mode if it times out ---------------------------------------------------------------

  if((select1Mode==true) && (select1Mode&&millis()-select1Millis > INTERVAL_SELECT*1000))
    {
      select1Mode=false;                        // No longer in select mode
      param = 0;
      select1 = 1;
      fullRedrawMain();
      Serial.println(F("Came out of menu mode"));
    } 

// ----------------------------------------------- Come out of SelectInputMode if it times out ---------------------------------------------------------------

  if((selectInputMode==true) && (selectInputMode&&millis()-selectInputMillis > INTERVAL_SELECT*1000))
    {
      selectInputMode=false;                        // No longer in select mode
      //dispMain(true);
      dispInput();
      Serial.println(F("Came out of Input Selection mode"));
    } 

} // loop

// ------------------------------------------------------------- Functions -------------------------------------------------------------------------------

// ----------------------------- Runs when interrupt happens on either of the rotary encoder's pins ------------------------------------------------------
void interruptFunction() 
  {
    encoder.tick();
    newPos = encoder.getPosition();
  }

// ----------------------------- Set MCLK by changing the output frequency of the Si570 clock generator ----------------------------------------------------
void SetMCLK (int MCLK) 
{ 
/* 1 : 11.2896 MHz
 * 2 : 12.288 MHz
 * 3 : 16.384 MHz
 * 4 : 16.9344 MHz
 * 5 : 18.432 MHz
 * 6 : 22.5792 MHz
 * 7 : 24.576 MHz
 * 8 : 33.8688 MHz
 * 9 : 36.864 MHz
 * 10 : 45.1584 MHz
 * 11 : 49.152 MHz
 */

  String frequency = "";

  ChangeBit(ak4493, 0x00, 0, false);      // Reset the dac
  #ifdef DUALMONO
    ChangeBit(ak4493r, 0x00, 0, false);      // Reset the dac
  #endif
  delay(20);

  switch (MCLK) 
    {
      case 1:
      #ifdef SI570
      vfo->setFrequency(11289600L);
      Serial.println("MCLK set to 11.2896 MHz.");
      #endif
      break;

      case 2:
      #ifdef SI570
      vfo->setFrequency(12288000L);
      Serial.println("MCLK set to 12.288 MHz.");
      #endif
      break;

      case 3:
      #ifdef SI570
      vfo->setFrequency(16384000L);
      Serial.println("MCLK set to 16.384 MHz.");
      #endif
      break;

      case 4:
      #ifdef SI570
      vfo->setFrequency(16934400L);
      Serial.println("MCLK set to 16.9344 MHz.");
      #endif
      break;

      case 5:
      #ifdef SI570
      vfo->setFrequency(18432000L);
      Serial.println("MCLK set to 18.432 MHz.");
      #endif
      break;

      case 6:
      #ifdef SI570
      vfo->setFrequency(22579200L);
      Serial.println("MCLK set to 22.5792 MHz.");
      #endif
      break;

      case 7:
      #ifdef SI570
      vfo->setFrequency(24576000L);
      Serial.println("MCLK set to 24.576 MHz.");
      #endif
      break;

      case 8:
      #ifdef SI570
      vfo->setFrequency(33868800L);
      Serial.println("MCLK set to 33.8688 MHz.");
      #endif
      break;

      case 9:
      #ifdef SI570
      vfo->setFrequency(36864000L);
      Serial.println("MCLK set to 36.864 MHz.");
      #endif
      break;    
      
      case 10:
      #ifdef SI570
        vfo->setFrequency(45158400L);
      #else
        WriteRegister(SI570_I2C_ADDRESS, 0xFF, 0);            // set page register to zero
        WriteRegister(SI570_I2C_ADDRESS, 0x45, 0);            // disable FCAL override
        WriteRegister(SI570_I2C_ADDRESS, 0x11, 0);            // disable output
        delay(5);
        WriteRegister(SI570_I2C_ADDRESS, 0x17, 0xF0);         // Value for 45,158400000 MHz
        WriteRegister(SI570_I2C_ADDRESS, 0x18, 0x00);         // Value for 45,158400000 MHz
        WriteRegister(SI570_I2C_ADDRESS, 0x1A, 0x10);         // Value for 45,158400000 MHz
        WriteRegister(SI570_I2C_ADDRESS, 0x1B, 0x40);         // Value for 45,158400000 MHz
        WriteRegister(SI570_I2C_ADDRESS, 0x1C, 0x38);         // Value for 45,158400000 MHz
        WriteRegister(SI570_I2C_ADDRESS, 0x1D, 0xE0);         // Value for 45,158400000 MHz
        WriteRegister(SI570_I2C_ADDRESS, 0x1E, 0xC4);         // Value for 45,158400000 MHz
        WriteRegister(SI570_I2C_ADDRESS, 0x1F, 0x00);         // Value for 45,158400000 MHz
        delay(5);
        WriteRegister(SI570_I2C_ADDRESS, 0x07, 0x08);         // start FCAL
        WriteRegister(SI570_I2C_ADDRESS, 0x11, 0x01);         // enable output
      #endif
      Serial.println("MCLK set to 45.1584 MHz.");
      break;

      case 11:
      #ifdef SI570
        vfo->setFrequency(49152000L);
      #else
        WriteRegister(SI570_I2C_ADDRESS, 0xFF, 0);            // set page register to zero
        WriteRegister(SI570_I2C_ADDRESS, 0x45, 0);            // disable FCAL override
        WriteRegister(SI570_I2C_ADDRESS, 0x11, 0);            // disable output
        delay(5);
        WriteRegister(SI570_I2C_ADDRESS, 0x17, 0xDC);         // Value for 49,152000000 MHz
        WriteRegister(SI570_I2C_ADDRESS, 0x18, 0x00);         // Value for 49,152000000 MHz
        WriteRegister(SI570_I2C_ADDRESS, 0x1A, 0xD6);         // Value for 49,152000000 MHz
        WriteRegister(SI570_I2C_ADDRESS, 0x1B, 0xFA);         // Value for 49,152000000 MHz
        WriteRegister(SI570_I2C_ADDRESS, 0x1C, 0xEE);         // Value for 49,152000000 MHz
        WriteRegister(SI570_I2C_ADDRESS, 0x1D, 0x6D);         // Value for 49,152000000 MHz
        WriteRegister(SI570_I2C_ADDRESS, 0x1E, 0xC4);         // Value for 49,152000000 MHz
        WriteRegister(SI570_I2C_ADDRESS, 0x1F, 0x00);         // Value for 49,152000000 MHz
        delay(5);
        WriteRegister(SI570_I2C_ADDRESS, 0x07, 0x08);         // start FCAL
        WriteRegister(SI570_I2C_ADDRESS, 0x11, 0x01);         // enable output
      #endif
      Serial.println("MCLK set to 49.152 MHz.");
      break; 
    }

  ChangeBit(ak4493, 0x00, 0, true);      // Come out of reset
  #ifdef DUALMONO
    ChangeBit(ak4493r, 0x00, 0, true);      // Come out of reset
  #endif
  delay(20);

} 

// ----------------------------- Select I2S input by controlling the MUX on the AK4118/XMOS board ----------------------------------------------------
void SetI2S_Input()
{ 
  if (I2S_Input == 0)                                         // Select USB
    {
      digitalWrite(I2S_SELECT_PIN, LOW);                      // Switch the MUX to USB
      delay(30);
      digitalWrite(Si570_EN, HIGH);                           // Power up Si570.
      mcp_iso.digitalWrite(3, HIGH);                          // Enable the Si570
    }
  else if (I2S_Input == 1)                                    // Select S/PDIF
    {
      digitalWrite(Si570_EN, LOW);                           // Disable up Si570.
      mcp_iso.digitalWrite(3, LOW);                           // Disable the Si570.
      delay(30);
      digitalWrite(I2S_SELECT_PIN, HIGH);                     // Switch the MUX to S/PDIF
    }
  //dispMain(false);
} 




// --------------------------------------------- Si570 clock generator initialization routine -------------------------------------------------------------------
#ifdef SI570
void Si570Init()                                            // resets and initializes the Si570
  {  
    // The library automatically reads the factory calibration settings of your Si570
    // but it needs to know for what frequency it was calibrated for (SI570_FACTORY_FREQUENCY defined at start of code).
    Serial.println("Initializing Si570");
    vfo = new Si570(SI570_I2C_ADDRESS, SI570_FACTORY_FREQUENCY);
    
    if (vfo->status == SI570_ERROR) 
      {
        // The Si570 is unreachable. Show an error for 2 seconds and continue.
        Serial.println("Si570 comm error or no Si570 installed");
        tft.setCursor(0, 160);
        tft.setFont(&Arimo_Bold_26);
        tft.println("Si570 comm error");
        delay(1000);
      }
  
    if (vfo->status == SI570_READY) 
      {
        // The Si570 is found.
        Serial.println("Si570 found");
        tft.setCursor(0, 160);
        tft.setFont(&Arimo_Bold_26);
        tft.println("Si570 found, is Clock Master");
        delay(100);
      }
  }
#endif
// --------------------------------------------- Si544 clock generator initialization routine -------------------------------------------------------------------
void Si544Init()                                            // resets and initializes the Si544
  {  
    Serial.println("Initializing Si544");

    Wire.beginTransmission(SI570_I2C_ADDRESS);                       // Look for Si544
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.println("Si544 found");
      tft.setCursor(0, 160);
      tft.setFont(&Arimo_Bold_26);
      tft.println("Si544 found, is Clock Master");
      delay(100);
    }
    else if (error==4) 
    {
      Serial.println("Unknown error at Si544's address.");
      tft.setCursor(0, 160);
      tft.setFont(&Arimo_Bold_26);
      tft.println("Unknown error at Si544's address.");
      delay(1000);
    }
    else
    {
      Serial.println("No response from Si544!");
      tft.setCursor(0, 160);
      tft.setFont(&Arimo_Bold_26);
      tft.println("No response from Si544!");
      delay(1000);
    }
  }


// ----------------------------- Function that determines the USB-to-I2S' SR --------------------------------------------------------------------

void USB_SR()
{
  // ---------------------------- Amanero ------------------------------------------------------------------------------
  //Serial.println("Running USB_SR");
  #ifdef Amanero
  /*
   * mcp_iso_2.GP0 = F0 (I1)
   * mcp_iso_2.GP1 = F1 (I2)
   * mcp_iso_2.GP2 = F2 (I3)
   * mcp_iso_2.GP3 = F3 (I4)
   * mcp_iso_2.GP4 = DSDOE (I5)
   */
  // Check for PCM or DSD
  if (mcp_iso_2.digitalRead(4) == false)    // PCM detected
  {  
    delay(200);
    if (mcp_iso_2.digitalRead(4) == false)  // PCM confirmed
      {
      dsdSignal = 0;
      //Serial.println("Amanero PCM");
      if (mcp_iso_2.digitalRead(0) == false && mcp_iso_2.digitalRead(1) == false && mcp_iso_2.digitalRead(2) == false && mcp_iso_2.digitalRead(3) == false)
        {
          SR_I2S = 16;      // 32KHz
          //Serial.println("Amanero 32KHz");
        }
        else if (mcp_iso_2.digitalRead(0) == true && mcp_iso_2.digitalRead(1) == false && mcp_iso_2.digitalRead(2) == false && mcp_iso_2.digitalRead(3) == false)
          {
            SR_I2S = 15;      // 44.1KHz
            //Serial.println("Amanero 44.1KHz");
          }
          else if (mcp_iso_2.digitalRead(0) == false && mcp_iso_2.digitalRead(1) == true && mcp_iso_2.digitalRead(2) == false && mcp_iso_2.digitalRead(3) == false)
            {
              SR_I2S = 14;      // 48KHz
              //Serial.println("Amanero 48KHz");
            }
            else if (mcp_iso_2.digitalRead(0) == true && mcp_iso_2.digitalRead(1) == true && mcp_iso_2.digitalRead(2) == false && mcp_iso_2.digitalRead(3) == false)
              {
                SR_I2S = 13;      // 88.2KHz
                //Serial.println("Amanero 88.2KHz");
              }
              else if (mcp_iso_2.digitalRead(0) == false && mcp_iso_2.digitalRead(1) == false && mcp_iso_2.digitalRead(2) == true && mcp_iso_2.digitalRead(3) == false)
                {
                  SR_I2S = 12;      // 96KHz
                  //Serial.println("Amanero 96KHz");
                }
                else if (mcp_iso_2.digitalRead(0) == true && mcp_iso_2.digitalRead(1) == false && mcp_iso_2.digitalRead(2) == true && mcp_iso_2.digitalRead(3) == false)
                  {
                    SR_I2S = 11;      // 176.4KHz
                    //Serial.println("Amanero 176.4KHz");
                  }
                  else if (mcp_iso_2.digitalRead(0) == false && mcp_iso_2.digitalRead(1) == true && mcp_iso_2.digitalRead(2) == true && mcp_iso_2.digitalRead(3) == false)
                    {
                      SR_I2S = 10;      // 192KHz
                      //Serial.println("Amanero 192KHz");
                    }
                    else if (mcp_iso_2.digitalRead(0) == true && mcp_iso_2.digitalRead(1) == true && mcp_iso_2.digitalRead(2) == true && mcp_iso_2.digitalRead(3) == false)
                      {
                        SR_I2S = 9;      // 352.8KHz
                        //Serial.println("Amanero 352.8KHz");
                      }
                      else if (mcp_iso_2.digitalRead(0) == false && mcp_iso_2.digitalRead(1) == false && mcp_iso_2.digitalRead(2) == false && mcp_iso_2.digitalRead(3) == true)
                        {
                          SR_I2S = 8;      // 384KHz
                          //Serial.println("Amanero 384KHz");
                        }
      }
  }
  else if (mcp_iso_2.digitalRead(4) == true)    // DSD detected
    {
      delay(200);
      if (mcp_iso_2.digitalRead(4) == true)     // DSD confirmed
        {      
          dsdSignal = 1;
          //Serial.println("Amanero DSD");
          if (mcp_iso_2.digitalRead(3) == true && mcp_iso_2.digitalRead(2) == false && mcp_iso_2.digitalRead(1) == false && mcp_iso_2.digitalRead(0) == true)
            {
              SR_I2S = 5;      // DSD64
              //Serial.println("Amanero DSD64");
            }
            else if (mcp_iso_2.digitalRead(3) == true && mcp_iso_2.digitalRead(2) == false && mcp_iso_2.digitalRead(1) == true && mcp_iso_2.digitalRead(0) == false)
              {
                SR_I2S = 4;     // DSD128
                //Serial.println("Amanero DSD128");
              }
              else if (mcp_iso_2.digitalRead(3) == true && mcp_iso_2.digitalRead(2) == false && mcp_iso_2.digitalRead(1) == true && mcp_iso_2.digitalRead(0) == true)
                {
                  SR_I2S = 3;     // DSD256
                  //Serial.println("Amanero DSD256");
                }
                  else if (mcp_iso_2.digitalRead(3) == true && mcp_iso_2.digitalRead(2) == true && mcp_iso_2.digitalRead(1) == false && mcp_iso_2.digitalRead(0) == false)
                  {
                    SR_I2S = 2;     // DSD512
                    //Serial.println("Amanero DSD512");
                  }     
        }
    }
  #endif //Amanero

  // ---------------------------- Dimdim XMOS ------------------------------------------------------------------------------
  //Serial.println("Running USB_SR");
  #ifdef Dim_XMOS
  /*
   * mcp_iso_2.GP0 = P_GPIO_SR_1
   * mcp_iso_2.GP1 = P_GPIO_SR_2
   * mcp_iso_2.GP2 = P_GPIO_SR_3
   * mcp_iso_2.GP3 = P_GPIO_SR_4
   * mcp_iso_2.GP4 = DSD_ON
   */
  // Check for PCM or DSD
  if (mcp_iso_2.digitalRead(4) == false)    // PCM detected
  {  
    delay(10);
    if (mcp_iso_2.digitalRead(4) == false)  // PCM confirmed
      {
      ReadAKStatus();                       // Crosscheck status of signal by seeing what the AK4493 thinks.
      if (mode_dac == 0) 
        {
           dsdSignal = 0;
           Serial.println("Both XMOS and AK say the signal is PCM");
           
           if (mcp_iso_2.digitalRead(0) == false && mcp_iso_2.digitalRead(1) == false && mcp_iso_2.digitalRead(2) == false && mcp_iso_2.digitalRead(3) == false)
             {
               SR_I2S = 15;      // 44.1KHz
               //Serial.println("44.1KHz");
             }
           else if (mcp_iso_2.digitalRead(0) == true && mcp_iso_2.digitalRead(1) == false && mcp_iso_2.digitalRead(2) == false && mcp_iso_2.digitalRead(3) == false)
             {
               SR_I2S = 14;      // 48KHz
               //Serial.println("48KHz");
             }
           else if (mcp_iso_2.digitalRead(0) == false && mcp_iso_2.digitalRead(1) == true && mcp_iso_2.digitalRead(2) == false && mcp_iso_2.digitalRead(3) == false)
             {
               SR_I2S = 13;      // 88.2KHz
               //Serial.println("88.2KHz");
             }
           else if (mcp_iso_2.digitalRead(0) == true && mcp_iso_2.digitalRead(1) == true && mcp_iso_2.digitalRead(2) == false && mcp_iso_2.digitalRead(3) == false)
             {
               SR_I2S = 12;      // 96KHz
               //Serial.println("96KHz");
             }
           else if (mcp_iso_2.digitalRead(0) == false && mcp_iso_2.digitalRead(1) == false && mcp_iso_2.digitalRead(2) == true && mcp_iso_2.digitalRead(3) == false)
             {
               SR_I2S = 11;      // 176.4KHz
               //Serial.println("176.4KHz");
             }
           else if (mcp_iso_2.digitalRead(0) == true && mcp_iso_2.digitalRead(1) == false && mcp_iso_2.digitalRead(2) == true && mcp_iso_2.digitalRead(3) == false)
             {
               SR_I2S = 10;      // 192KHz
               //Serial.println("192KHz");
             }
           else if (mcp_iso_2.digitalRead(0) == false && mcp_iso_2.digitalRead(1) == true && mcp_iso_2.digitalRead(2) == true && mcp_iso_2.digitalRead(3) == false)
             {
               SR_I2S = 9;      // 352.8KHz
               //Serial.println("352.8KHz");
             }
           else if (mcp_iso_2.digitalRead(0) == true && mcp_iso_2.digitalRead(1) == true && mcp_iso_2.digitalRead(2) == true && mcp_iso_2.digitalRead(3) == false)
             {
               SR_I2S = 8;      // 384KHz
               //Serial.println("384KHz");
             }
           else if (mcp_iso_2.digitalRead(0) == false && mcp_iso_2.digitalRead(1) == true && mcp_iso_2.digitalRead(2) == true && mcp_iso_2.digitalRead(3) == true)
             {
               SR_I2S = 7;      // 705.6KHz
               //Serial.println("706.5KHz");
             }
           else if (mcp_iso_2.digitalRead(0) == false && mcp_iso_2.digitalRead(1) == false && mcp_iso_2.digitalRead(2) == true && mcp_iso_2.digitalRead(3) == true)
             {
               SR_I2S = 6;      // 768KHz
               //Serial.println("768KHz");
             }                                                    
        }
      }
  }
  else if (mcp_iso_2.digitalRead(4) == true)    // DSD detected
    {
      delay(50);
      if (mcp_iso_2.digitalRead(4) == true)     // DSD confirmed
        {      
          ReadAKStatus();                       // Crosscheck status of signal by seeing what the AK4493 thinks.
          if (mode_dac == 1) 
            {
              dsdSignal = 1;
              Serial.println("Both XMOS and AK say the signal is DSD");
              
              if (mcp_iso_2.digitalRead(3) == true && mcp_iso_2.digitalRead(2) == true && mcp_iso_2.digitalRead(1) == false && mcp_iso_2.digitalRead(0) == true)
                {
                  SR_I2S = 5;      // DSD64
                  //Serial.println("DSD64");
                }
                else if (mcp_iso_2.digitalRead(3) == true && mcp_iso_2.digitalRead(2) == true && mcp_iso_2.digitalRead(1) == false && mcp_iso_2.digitalRead(0) == false)
                  {
                    SR_I2S = 4;     // DSD128
                    //Serial.println("DSD128");
                  }
                else if (mcp_iso_2.digitalRead(3) == true && mcp_iso_2.digitalRead(2) == false && mcp_iso_2.digitalRead(1) == true && mcp_iso_2.digitalRead(0) == true)
                  {
                    SR_I2S = 3;     // DSD256
                    //Serial.println("DSD256");
                  }
                else if (mcp_iso_2.digitalRead(3) == true && mcp_iso_2.digitalRead(2) == false && mcp_iso_2.digitalRead(1) == false && mcp_iso_2.digitalRead(0) == true)
                  {
                    SR_I2S = 2;     // DSD512
                    //Serial.println("DSD512");
                  }     
            }
          }
    }
  #endif //Dim_XMOS

}


// ----------------------------- Function that reads a register's data value --------------------------------------------------------------------
byte ReadRegister(int devaddr, byte regaddr)                                // Read a data register value
  {                              
    Wire.beginTransmission(devaddr);
    Wire.write(regaddr);
    Wire.endTransmission();
    Wire.requestFrom(devaddr, 1);                 // only one byte
    if(Wire.available())                          // Wire.available indicates if data is available
      return Wire.read();                         // Wire.read() reads the data on the wire
    else
    return 99;                                     // If no data in the wire, then return 99 to indicate error
  }

// ----------------------------- Function that writes a data value to a register --------------------------------------------------------------------
void WriteRegister(int devaddr, byte regaddr, byte dataval)                // Write a data register value
  {
    Wire.beginTransmission(devaddr); // device
    Wire.write(regaddr); // register
    Wire.write(dataval); // data
    Wire.endTransmission();
  }

// ----------------------------- Function that changes a single bit in a register --------------------------------------------------------------------
void ChangeBit(int devaddr, byte regaddr, int data, boolean setting)
  {
    byte r = ReadRegister(devaddr, regaddr);
    if (setting == 1)
      {
        bitSet(r, data);
      } else
      if (setting == 0)
        {
          bitClear(r, data);
        }
      WriteRegister(devaddr, regaddr, r);
  }

// ----------------------------- Function that prints out registers for debugging --------------------------------------------------------

void PrintDebugReg(byte dac)
{
Serial.println("");
Serial.print("Register values for ");
Serial.println(dac, HEX);

byte r = 0;
Serial.print("Register 00: ");
r = ReadRegister(dac, 0x00);
Serial.println(r,BIN);
r = 0;

Serial.print("Register 01: ");
r = ReadRegister(dac, 0x01);
Serial.println(r,BIN);
r = 0;

Serial.print("Register 02: ");
r = ReadRegister(dac, 0x02);
Serial.println(r,BIN);
r = 0;

Serial.print("Register 03: ");
r = ReadRegister(dac, 0x03);
Serial.println(r,BIN);
r = 0;

Serial.print("Register 04: ");
r = ReadRegister(dac, 0x04);
Serial.println(r,BIN);
r = 0;

Serial.print("Register 05: ");
r = ReadRegister(dac, 0x05);
Serial.println(r,BIN);
r = 0;

Serial.print("Register 06: ");
r = ReadRegister(dac, 0x06);
Serial.println(r,BIN);
r = 0;

Serial.print("Register 07: ");
r = ReadRegister(dac, 0x07);
Serial.println(r,BIN);
r = 0;

Serial.print("Register 08: ");
r = ReadRegister(dac, 0x08);
Serial.println(r,BIN);
r = 0;

Serial.print("Register 15H: ");
r = ReadRegister(dac, 0x15);
Serial.println(r,BIN);
r = 0;
}

// ----------------------------- Function that checks the pressing of the button --------------------------------------------------------------------
/* Returns key values:
 * 0 = momentarily pressed
 * 1 = sustained for 3sec
 * 2 = sustained for 6sec
*/
void buttonPressed()
  {
    if (millis() - keyPrevMillis >= keySampleIntervalMs) 
      {
        keyPrevMillis = millis();
        
        byte currKeyState = digitalRead(keyPin);
        
        if ((prevKeyState == HIGH) && (currKeyState == LOW)) 
          {
            KeyPressCount = 0;
          }
        else if ((prevKeyState == LOW) && (currKeyState == HIGH)) 
          {
            if (KeyPressCount < longKeyPressCountMax && KeyPressCount >= mediumKeyPressCountMin) 
              {
                Serial.println("Medium");
                key=2;
              }
            else 
              {
                if (KeyPressCount < mediumKeyPressCountMin) 
                  {
                    Serial.println("short");
                    key=1;
                  }
              }
          }
        else if (currKeyState == LOW) 
          {
            KeyPressCount++;
            if (KeyPressCount >= longKeyPressCountMax) 
              {
                Serial.println("long");                
                key=3;
              }
          }
        prevKeyState = currKeyState;
        }
  }

// ----------------------------- Functions used by the IR receiver --------------------------------------------------------------------

void timer2_init ()
  {
    timer.pause();
    timer.setPrescaleFactor( ((F_CPU / F_INTERRUPTS)/8) - 1);
    timer.setOverflow(7);
    timer.setChannel1Mode(TIMER_OUTPUT_COMPARE);
    timer.setCompare(TIMER_CH1, 1);  // Interrupt 1 count after each update
    timer.attachCompare1Interrupt(TIM2_IRQHandler);
        // Refresh the timer's count, prescale, and overflow
    timer.refresh();

    // Start the timer counting
    timer.resume();
  }

void TIM2_IRQHandler()                                                       // Timer2 Interrupt Handler
  {
    (void) irmp_ISR(); // call irmp ISR
     loopcount++;
   
  }

// ----------------------------- Functions used to read & write to the 24LC256 EEPROM chip --------------------------------------------------------------------

void writeData(int device, unsigned int add, byte data)               // writes a byte of data 'data' to the chip at I2C address 'device', in memory location 'add'
{
  Wire.beginTransmission(device);
  Wire.write((int)(add >> 8));                                       // left-part of pointer address
  Wire.write((int)(add & 0xFF));                                     // and the right
  Wire.write(data);
  Wire.endTransmission();
  delay(6);
  Serial.println("Data written to eeprom");
}

byte readData(int device, unsigned int add)                           // reads a byte of data from memory location 'add' in chip at I2C address 'device'
{
  byte result;                                                        // returned value
  Wire.beginTransmission(device);                                    // these three lines set the pointer position in the EEPROM
  Wire.write((int)(add >> 8));                                       // left-part of pointer address
  Wire.write((int)(add & 0xFF));                                     // and the right
  Wire.endTransmission();
  Wire.requestFrom(device,1);                                        // now get the byte of data...
  result = Wire.read();
  return result;                                                      // and return it as a result of the function readData
  Serial.print("Read from EEPROM: "); Serial.println(result);
}


// ----------------------------- Functions used to store & retrieve setting saved to the 24LC256 EEPROM chip --------------------------------------------------------------------

void writeSettings()
{
  if (readData(eeprom, 1) != dacFilter)                               // 1 = filter selection address
    {                                                                 // Check an see if there are any changes
      writeData(eeprom, 1, dacFilter);                                // If there are, write the changes in eeprom
      Serial.println("dacFilter written");
    }
  if (readData(eeprom, 2) != directDsd)                               // 2 = DSD Direct enabled
    {                                                                 // Check an see if there are any changes
      writeData(eeprom, 2, directDsd);                                // If there are, write the changes in eeprom
      Serial.println("directDsd written");
    }
  if (readData(eeprom, 3) != dsdFilter)                               // 3 = dsdFilter (50 or 150)
    {                                                                 // Check an see if there are any changes
      writeData(eeprom, 3, dsdFilter);                                // If there are, write the changes in eeprom
      Serial.println("dsdFilter written");      
    }
  if (readData(eeprom, 4) != soundMode)                               // 4 = soundMode
    {                                                                 // Check an see if there are any changes
      writeData(eeprom, 4, soundMode);                                // If there are, write the changes in eeprom
      Serial.println("soundMode written");            
    }
  if (readData(eeprom, 5) != InvertPhase)                             // 5 = Phase inversion
    {                                                                 // Check an see if there are any changes
      writeData(eeprom, 5, InvertPhase);                              // If there are, write the changes in eeprom
      Serial.println("InvertPhase written");            
    }
  if (readData(eeprom, 6) != MCLK_SET)                                // 6 = MCLK_SET
    {                                                                 // Check an see if there are any changes
      writeData(eeprom, 6, MCLK_SET);                                 // If there are, write the changes in eeprom
      Serial.println("MCLK_SET written");            
    }
  //Serial.println("Setting written");
}

void readSettings()
{
  dacFilter = readData(eeprom, 1);                                    // 1 = filter selection address
  directDsd = readData(eeprom, 2);                                    // 2 = DSD Direct enabled address
  dsdFilter = readData(eeprom, 3);                                    // 3 = dsdFilter (50 or 150) address
  soundMode = readData(eeprom, 4);                                    // 4 = soundMode address
  InvertPhase = readData(eeprom, 5);                                  // 5 = Phase inversion address
  MCLK_SET = readData(eeprom, 6);                                     // 6 = MCLK_SET address
  Serial.println("Settings read");
}

// ----------------------------- Function that initalizes the EEPROM to default values for all inputs --------------------------------------------------------------------
void initEEPROM()
  {
    Serial.println(F("Initializing EEPROM"));
    tft.setCursor(0, 200);
    tft.setFont(&Arimo_Bold_26);
    tft.println("Initializing EEPROM");
    
    writeData(eeprom, 1, 1);
    writeData(eeprom, 2, 0);
    writeData(eeprom, 3, 50);
    writeData(eeprom, 4, 1);
    writeData(eeprom, 5, 1);
    writeData(eeprom, 6, 1);
    
    // To tell if EEPROM is initialized or not
    writeData(eeprom, 199, 199);
    writeData(eeprom, 200, 200);
    writeData(eeprom, 201, 201);
  }

// ----------------------------- Function that hardware (relay) mutes the DACs --------------------------------------------------------------------
void dacMute(bool mute)
  {
    if (mute == 1)
      {
        digitalWrite(POWERPIN, LOW);
        muteMillis = millis();
        Serial.println("DAC muted");
      }
    else 
      //if (muteDuration&&millis()-muteMillis > INTERVAL_MUTE*1000)
        {
          digitalWrite(POWERPIN, HIGH);
          Serial.println("DAC un-muted");
        }
  }
