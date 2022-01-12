//------------------------LTC6804-1-----------------------------------------------------------//
//--------------------------------------------------------------------------------------------//
#include <Arduino.h>
#include <stdint.h>
#include "Linduino.h"
#include "LT_SPI.h"
#include "UserInterface.h"
#include "LTC68041.h"
#include <SPI.h>
#include <SoftwareSerial.h>//-----adding
#include <EEPROM.h>




//----- Port output to drive -----------------------------------------------------------------//
#define Discharge     8    //---   For discharge power from battery
#define Charge        7    //---   For Charge power to battery
#define Predischarge  6    //----  Predischarge drive resistor 1k ohm  for atleast 5 seconds
//#define Beep          5




//------- Cell status ------------------------------------------------------------------------//
uint16_t Cell[20];
//--- Curent Value



//-------- Current Variables -----------------------------------------------------------------//
#define  CurrentPin   A1
int      CurrentDefault = 40;
int      CurrentMem;
int      CurrentSet;
float    Current;




//--  Battery Voltage ------------------------------------------------------------------------//
long   VcellMax;
int    VcellMaxMem;
int    VcellMaxDefault =  4200;
long   VcellMin;
int    VcellMinMem;
int    VcellMinDefault =  3100;
//float Vtop;       //----- Volatage summarize 8 cells 
//float Vbottom;    //----- Volatage summarize 12 cells 
//float Vpack;      //----- Volatage Pack battery




//--- Battery Temp ----------------------------------------------------------------------------//
float temperature;
float Temp1;
float Temp2;
float Temp3;
float Temp4;
float TempBoardDrive;
int   TempBatteryCount;
int   TempBoardDriveCount;





//--- Check temp for a timer ------------------------------------------------------------------//
//--- Temperature Battery variable ------------------------------------------------------------//
float   TempBatterySetDefault     =   5500;
float   TempBatteryMinuteDefault  =   5;
float   TempBatterySet;
int     TempBatteryMinute;    
int     TempBatteryMinuteMem ;
float   TempBatterySetMem;



 
//--- Temperature Board Drive variable --------------------------------------------------------//
float   TempBoardDriveSetDefault      =   5500;
float   TempBoardDriveMinuteDefault   =   5;
float   TempBoardDriveSet;   
float   TempBoardDriveSetMem;
int     TempBoardDriveMinuteMem;
int     TempBoardDriveMinute;




//--- Mode Bms Selection to operate -----------------------------------------------------------//
//--------------- Mode Status---------------//
//------- Mode = 1 : Factory Reset ---------//
//------- Mode = 2 : Coustom Data ----------//
int  Mode ;   
int  ModeSelect;




//------ Serial Port Monitor - softwar -----------------------------------------------------------//
//SoftwareSerial mySerial(3, 2); // RX, TX




//--------- Configuration Number of iCs ---------------------------------------------------------//
const uint8_t  TOTAL_IC = 2;     //!<number of ICs in the daisy chain



/******************************************************
 *** Global Battery Variables received from 6804 commands
 These variables store the results from the LTC6804
 register reads and the array lengths must be based
 on the number of ICs on the stack
 ******************************************************/
uint16_t cell_codes[TOTAL_IC][12];
/*!<
  The cell codes will be stored in the cell_codes[][12] array in the following format:

  |  cell_codes[0][0]| cell_codes[0][1] |  cell_codes[0][2]|    .....     |  cell_codes[0][11]|  cell_codes[1][0] | cell_codes[1][1]|  .....   |
  |------------------|------------------|------------------|--------------|-------------------|-------------------|-----------------|----------|
  |IC1 Cell 1        |IC1 Cell 2        |IC1 Cell 3        |    .....     |  IC1 Cell 12      |IC2 Cell 1         |IC2 Cell 2       | .....    |
****/

uint16_t aux_codes[TOTAL_IC][6];
/*!<
 The GPIO codes will be stored in the aux_codes[][6] array in the following format:

 |  aux_codes[0][0]| aux_codes[0][1] |  aux_codes[0][2]|  aux_codes[0][3]|  aux_codes[0][4]|  aux_codes[0][5]| aux_codes[1][0] |aux_codes[1][1]|  .....    |
 |-----------------|-----------------|-----------------|-----------------|-----------------|-----------------|-----------------|---------------|-----------|
 |IC1 GPIO1        |IC1 GPIO2        |IC1 GPIO3        |IC1 GPIO4        |IC1 GPIO5        |IC1 Vref2        |IC2 GPIO1        |IC2 GPIO2      |  .....    |
*/

uint8_t tx_cfg[TOTAL_IC][6];
/*!<
  The tx_cfg[][6] stores the LTC6804 configuration data that is going to be written
  to the LTC6804 ICs on the daisy chain. The LTC6804 configuration data that will be
  written should be stored in blocks of 6 bytes. The array should have the following format:

 |  tx_cfg[0][0]| tx_cfg[0][1] |  tx_cfg[0][2]|  tx_cfg[0][3]|  tx_cfg[0][4]|  tx_cfg[0][5]| tx_cfg[1][0] |  tx_cfg[1][1]|  tx_cfg[1][2]|  .....    |
 |--------------|--------------|--------------|--------------|--------------|--------------|--------------|--------------|--------------|-----------|
 |IC1 CFGR0     |IC1 CFGR1     |IC1 CFGR2     |IC1 CFGR3     |IC1 CFGR4     |IC1 CFGR5     |IC2 CFGR0     |IC2 CFGR1     | IC2 CFGR2    |  .....    |

*/

uint8_t rx_cfg[TOTAL_IC][8];
/*!<
  the rx_cfg[][8] array stores the data that is read back from a LTC6804-1 daisy chain.
  The configuration data for each IC  is stored in blocks of 8 bytes. Below is an table illustrating the array organization:

|rx_config[0][0]|rx_config[0][1]|rx_config[0][2]|rx_config[0][3]|rx_config[0][4]|rx_config[0][5]|rx_config[0][6]  |rx_config[0][7] |rx_config[1][0]|rx_config[1][1]|  .....    |
|---------------|---------------|---------------|---------------|---------------|---------------|-----------------|----------------|---------------|---------------|-----------|
|IC1 CFGR0      |IC1 CFGR1      |IC1 CFGR2      |IC1 CFGR3      |IC1 CFGR4      |IC1 CFGR5      |IC1 PEC High     |IC1 PEC Low     |IC2 CFGR0      |IC2 CFGR1      |  .....    |
*/

/*!**********************************************************************
 \brief  Inititializes hardware and variables
 ***********************************************************************/
void setup()
{

  //---------- initial config-----------------------//
  //init_cfg();
  //LTC6804_wrcfg( TOTAL_IC, tx_cfg);
  delay(20);
  pinMode( Discharge,    OUTPUT);
  pinMode( Charge,       OUTPUT);
  pinMode( Predischarge, OUTPUT);
  //pinMode( Beep,         OUTPUT);

  //----Status --- Drive Output --------------------//
  //----- LOW  = OFF ----//
  //----- HIGH = ON  ----//
  /*digitalWrite(Discharge,    LOW);  
  digitalWrite(Predischarge, LOW);
  digitalWrite(Charge,       LOW);
  //digitalWrite(Beep,         LOW);*/
  OffMosfet();

  //------------ Communication-----------------------//
  Serial.begin(115200);
  // mySerial.begin(9600);
  LTC6804_initialize();  //Initialize LTC6804 hardware
  init_cfg();            //initialize the 6804 configuration array to be written
  //print_menu();        //intialize  Menu to 6804 select menu

  Serial.print(F("Start BMS Setup :"));
  for(int i = 0; i<10 ;i++){
  Serial.print("*");
  delay(100);
  }
  Serial.println(F("BMS Ready to Start"));
     //--------------- Load data from EEPROM ----------//
  ModeSelect = EEPROM.read(0);
  delay(10);
  //-------------- Select and loading data to EEPROM---------//
  if(ModeSelect == 1){
     //FactoryToMemUpdate(); //---- force to update data to memory
     FactoryUpdate();
     ModeSelect = EEPROM.read(0);
     Mode = ModeSelect;
     Serial.print(F("Bms Status Factory :"));
     Serial.println(Mode);
     delay(1000);
  }
  else if(ModeSelect == 2){
     MemUpdate();
     Mode = ModeSelect;
     Serial.println(F("Bms Status Custom data "));
     Serial.print(F("Mode          :"));
     Serial.println(Mode);
     Serial.print(F("Current       :"));
     Serial.println(CurrentMem);
     Serial.print(F("VcellMax      :"));
     Serial.println(VcellMaxMem);
     Serial.print(F("VcellMin      :"));
     Serial.println(VcellMinMem);
     Serial.print(F("TempBattery   :"));
     Serial.println(TempBatterySetMem);
     Serial.print(F("TempBatteryMin:"));
     Serial.println(TempBatteryMinuteMem);
  }
  else{
     EEPROM.update(0,1);
     FactoryToMemUpdate();  //---- If first time data store to zero or N/A force to update memery and wite to mode 1(Factory default)
     FactoryUpdate();
     ModeSelect = EEPROM.read(0);
     Mode = ModeSelect;
     Serial.print(F("Bms Status Write to Factory :"));
     Serial.println(Mode);
  }

  
  VcellMin = VcellMin*10;
  VcellMax = VcellMax*10;
  
//--------- Clear Cell status cell ------------------------------------//
  for(int i=0;i<11;i++){
      Cell[i] = 00;
  }
}
//------------------------ End of set up loop -------------------------//



//---------------------- Mian loop ------------------------------------//
void loop(){

  //----------------------User Command ---------------------------------//
  /*if (Serial.available())           // Check for user input
  {
    uint32_t user_command;
    user_command = read_int();      // Read the user command
    Serial.println(user_command);
    run_command(user_command);
  }*/

   //----------- function of BMS---------------------------------------//
   //FlashMem();    //------ Function command for flash data to EEPROM.
   //delay(10);
   OnMosfet();
   //delay(100);
   VcellMonitor();
   CheckVoltage();
   CheckVolageDiff();
   TempCheck();
   CurrentMonitor();
   Balancing();
   delay(1000);
 }
//----------------------------- End of the loop --------------------------------------------//
//------------------------------------------------------------------------------------------//




//------------------------------ User command------------------------------------------------//
/*!*****************************************
  \brief executes the user inputted command

  Menu Entry 1: Write Configuration \n
   Writes the configuration register of the LTC6804. This command can be used to turn on the reference
   and increase the speed of the ADC conversions.

 Menu Entry 2: Read Configuration \n
   Reads the configuration register of the LTC6804, the read configuration can differ from the written configuration.
   The GPIO pins will reflect the state of the pin

 Menu Entry 3: Start Cell voltage conversion \n
   Starts a LTC6804 cell channel adc conversion.

 Menu Entry 4: Read cell voltages
    Reads the LTC6804 cell voltage registers and prints the results to the serial port.

 Menu Entry 5: Start Auxiliary voltage conversion
    Starts a LTC6804 GPIO channel adc conversion.

 Menu Entry 6: Read Auxiliary voltages6118
    Reads the LTC6804 axiliary registers and prints the GPIO voltages to the serial port.

 Menu Entry 7: Start cell voltage measurement loop
    The command will continuously measure the LTC6804 cell voltages and print the results to the serial port.
    The loop can be exited by sending the MCU a 'm' character over the serial link.

*******************************************/
/*void run_command(uint32_t cmd)
{
  int8_t error = 0;

  char input = 0;
  switch (cmd)
  {

    case 1:
      wakeup_sleep();
      LTC6804_wrcfg(TOTAL_IC,tx_cfg);
      print_config();
      break;

    case 2:
      wakeup_sleep();
      error = LTC6804_rdcfg(TOTAL_IC,rx_cfg);
      if (error == -1)
      {
        Serial.println("A PEC error was detected in the received data");
      }
      print_rxconfig();
      break;

    case 3:
      wakeup_sleep();
      LTC6804_adcv();
      delay(3);
      Serial.println("cell conversion completed");
      Serial.println();
      break;

    case 4:
      wakeup_sleep();
      error = LTC6804_rdcv(0, TOTAL_IC,cell_codes); // Set to read back all cell voltage registers
      if (error == -1)
      {
        Serial.println("A PEC error was detected in the received data");
      }
      //print_cells();
      break;

    case 5:
      wakeup_sleep();
      LTC6804_adax();
      delay(3);
      Serial.println("aux conversion completed");
      Serial.println();
      break;

    case 6:
      wakeup_sleep();
      error = LTC6804_rdaux(0,TOTAL_IC,aux_codes); // Set to read back all aux registers
      if (error == -1)
      {
        Serial.println("A PEC error was detected in the received data");
      }
      //print_aux();
      break;

    case 7:
      Serial.println("transmit 'm' to quit");
      wakeup_sleep();
      LTC6804_wrcfg(TOTAL_IC,tx_cfg);
      while (input != 'm')
      {
        if (Serial.available() > 0)
        {
          input = read_char();
        }
        wakeup_idle();
        LTC6804_adcv();
        delay(10);
        wakeup_idle();
        error = LTC6804_rdcv(0, TOTAL_IC,cell_codes);
        if (error == -1)
        {
          Serial.println("A PEC error was detected in the received data");
        }
        //print_cells();
        VcellMonitor();
   TempCheck();
   CheckVolage();
   CheckVolageDiff();
   CurrentMonitor();
        delay(500);

      }
      print_menu();
      break;

    default:
      Serial.println("Incorrect Option");
      break;
  }
}*/

/*!*********************************
  \brief Prints the main menu
***********************************/
/*
void print_menu()
{
  Serial.println("Please enter LTC6804 Command");
  Serial.println("Write Configuration: 1");
  Serial.println("Read Configuration: 2");
  Serial.println("Start Cell Voltage Conversion: 3");
  Serial.println("Read Cell Voltages: 4");
  Serial.println("Start Aux Voltage Conversion: 5");
  Serial.println("Read Aux Voltages: 6");
  Serial.println("loop cell voltages: 7");
  Serial.println("Please enter command: ");
  Serial.println();
}*/
//----------------------------------------------------------------------------//




/*!***********************************
 \brief Initializes the configuration array
 **************************************/
void init_cfg()
{
  for (uint8_t i = 0; i<TOTAL_IC; i++)
  {
    tx_cfg[i][0] = 0xFE;
    tx_cfg[i][1] = 0x00 ;
    tx_cfg[i][2] = 0x00 ;
    tx_cfg[i][3] = 0x00 ;
    tx_cfg[i][4] = 0x00 ;
    tx_cfg[i][5] = 0x00 ;
  }
}
//-------------------------------------------//
//------------------------------------------//



/*!*****************************************************************
 \brief Prints the configuration data that was read back from the
 LTC6804 to the serial port.
 *******************************************************************/
void serial_print_hex(uint8_t data)
{
  if (data< 16)
  {
    Serial.print("0");
    Serial.print((byte)data,HEX);
  }
  else
    Serial.print((byte)data,HEX);
}
//-------------------------------------------------------------------/




/*!*****************************************************************
 \brief Prints the configuration data that was read back from the
 LTC6804 to the serial port.
 *******************************************************************/
void print_rxconfig()
{
  Serial.println("Received Configuration ");
  for (int current_ic=0; current_ic<TOTAL_IC; current_ic++)
  {
    Serial.print(" IC ");
    Serial.print(current_ic+1,DEC);
    Serial.print(": 0x");
    serial_print_hex(rx_cfg[current_ic][0]);
    Serial.print(", 0x");
    serial_print_hex(rx_cfg[current_ic][1]);
    Serial.print(", 0x");
    serial_print_hex(rx_cfg[current_ic][2]);
    Serial.print(", 0x");
    serial_print_hex(rx_cfg[current_ic][3]);
    Serial.print(", 0x");
    serial_print_hex(rx_cfg[current_ic][4]);
    Serial.print(", 0x");
    serial_print_hex(rx_cfg[current_ic][5]);
    Serial.print(", Received PEC: 0x");
    serial_print_hex(rx_cfg[current_ic][6]);
    Serial.print(", 0x");
    serial_print_hex(rx_cfg[current_ic][7]);
    Serial.println();
  }
  Serial.println();
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//




/*!******************************************************************************
 \brief Prints the configuration data that is going to be written to the LTC6804
 to the serial port.
 ********************************************************************************/
void print_config()
{
  int cfg_pec;

  Serial.println(F("Written Configuration: "));
  for (int current_ic = 0; current_ic<TOTAL_IC; current_ic++)
  {
    Serial.print(" IC ");
    Serial.print(current_ic+1,DEC);
    Serial.print(": ");
    Serial.print("0x");
    serial_print_hex(tx_cfg[current_ic][0]);
    Serial.print(", 0x");
    serial_print_hex(tx_cfg[current_ic][1]);
    Serial.print(", 0x");
    serial_print_hex(tx_cfg[current_ic][2]);
    Serial.print(", 0x");
    serial_print_hex(tx_cfg[current_ic][3]);
    Serial.print(", 0x");
    serial_print_hex(tx_cfg[current_ic][4]);
    Serial.print(", 0x");
    serial_print_hex(tx_cfg[current_ic][5]);
    Serial.print(", Calculated PEC: 0x");
    cfg_pec = pec15_calc(6,&tx_cfg[current_ic][0]);
    serial_print_hex((uint8_t)(cfg_pec>>8));
    Serial.print(", 0x");
    serial_print_hex((uint8_t)(cfg_pec));
    Serial.println();
  }
  Serial.println();
}
//-----------------------------------------------------------------//
