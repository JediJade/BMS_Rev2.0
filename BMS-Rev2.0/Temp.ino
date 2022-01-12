/**********************************************************************************************************************/
/********************* This function get data form ltc6801 and convert to Celsius Temperature Unit ********************/
/********************* Temperature Calculate and Calibrate ************************************************************/
/**********************************************************************************************************************/
void Temp_Cal(double Temp_cal) {
  float thermistor_adc_val;
  double output_voltage, thermistor_resistance,therm_res_ln;
  output_voltage =  3 - Temp_cal;
  thermistor_resistance = ( ( 3 * ( 10.0 / output_voltage ) ) - 10 ); /* Resistance in kilo ohms */  //---- Difference from original 
  thermistor_resistance = thermistor_resistance * 1000 ; /* Resistance in ohms   */
  therm_res_ln = log(thermistor_resistance);
  /*  Steinhart-Hart Thermistor Equation: */
  /*  Temperature in Kelvin = 1 / (A + B[ln(R)] + C[ln(R)]^3)   */
  /*  where A = 0.001129148, B = 0.000234125 and C = 8.76741*10^-8  */
  temperature = ( 1 / ( 0.001129148 + ( 0.000234125 * therm_res_ln ) + ( 0.0000000876741 * therm_res_ln * therm_res_ln * therm_res_ln ) ) ); /* Temperature in Kelvin */
  temperature = (temperature - 273.15)*100; /* Temperature in degree Celsius */   //---------- Adding code to multiply 100 for 4 digits to store data to EEPROM
}
/*********************************************************************************************************************/



/**********************************************************************************************************************/
/******************** This function has a lot of check temperature ****************************************************/
/******************** form battery 4 chnnels and temperature Board drive 1 channel ************************************/
/******************** with timer to check temperature rise times. *****************************************************/
/**********************************************************************************************************************/
void TempCheck(){
 int8_t error = 0;  //---- Error integer to define when data is can't get complete.
 wakeup_sleep();
 LTC6804_adax();
 delay(3);
 //----- Convert AUX -----------------------------//
 error = LTC6804_rdaux(0,TOTAL_IC,aux_codes); // Set to read back all aux registers
 if(error == -1){
   Serial.println("A PEC error was detected in the received data");
 }
 print_Temp_ESP32();
 if(TempBoardDrive >= TempBoardDriveSet){
    TempBoardDriveCount++; //----TempCount                                    
    if(TempBoardDriveCount >= TempBoardDriveMinute ){ 
       //---- Do-Loop
       do{
          OffMosfet();
          Serial.println(F("CAUTION : Board drive are HOT !"));
          delay(1000);
          //-------------- Get temp data from ltc6804----------//
          wakeup_sleep();
          LTC6804_adax();
          delay(3);
          //----- Convert AUX -----------------------____------//
          error = LTC6804_rdaux(0,TOTAL_IC,aux_codes); // Set to read back all aux registers
          if(error == -1){
             Serial.println("A PEC error was detected in the received data");
          }
          print_Temp_ESP32();
          }while(TempBoardDrive >= TempBoardDriveSet);
           //----- End do loop 
    }//---- TempCount
 }//---- TempSet
 else{         
       TempBoardDriveCount = 0;
 }
 //------------------------------------ Temperature Cell Check -----------------------------//
 if(Temp1  > TempBatterySet or Temp2  >= TempBatterySet or Temp3 >= TempBatterySet or Temp4 >= TempBatterySet){
    TempBatteryCount++;
    Serial.println(TempBatteryCount++);
    //--------------------- TempCount --------------------------------//
    if(TempBatteryCount >= TempBatteryMinute ){ 
       //----------Do-Loop--------------------------------------//
        do{
           OffMosfet();
           Serial.println(F("CAUTION : Battery Cell are HOT !"));
           delay(1000);
           //--------------- Temp Read----------------------//
           //----- Read AUX---------------------------------//
           wakeup_sleep();
           LTC6804_adax();
           delay(3);
           //----- Convert AUX -----------------------------//
           error = LTC6804_rdaux(0,TOTAL_IC,aux_codes); // Set to read back all aux registers
           if (error == -1){
               Serial.println("A PEC error was detected in the received data");
           }
           print_Temp_ESP32();
        }while( Temp1 >= TempBatterySet or Temp2 >= TempBatterySet or Temp3 >= TempBatterySet or Temp4 >= TempBatterySet);
        //---- End do loop 
    } //---- TempCount
 } //---- TempCellSetting 
 else{
      TempBatteryCount = 0;
 }
} //---- End of the loop.
//-------------------------------------------------------------------------------------------------------------------------//



/***************************************************************************************************************************/
/************************************ Print function of temperature to serial port *****************************************/
/***************************************************************************************************************************/
void print_Temp_ESP32(){
//-------- Read and Convert Temperature Data ----------//
Temp1  = (aux_codes[0][0]*0.0001);
Temp_Cal(Temp1);
Temp1 = temperature;
Temp2  = (aux_codes[0][1]*0.0001);
Temp_Cal(Temp2);
Temp2 = temperature;
Temp3  = (aux_codes[0][2]*0.0001);
Temp_Cal(Temp3);
Temp3 = temperature;
Temp4  = (aux_codes[0][3]*0.0001);
Temp_Cal(Temp4);
Temp4 = temperature;
TempBoardDrive  = (aux_codes[0][4]*0.0001);
Temp_Cal(TempBoardDrive);
TempBoardDrive = temperature;
//-------- Serial Print data -------------------------//
//Serial.print(F("Temperature Pack"));
//Serial.print(F("T1:"));
Serial.print(Temp1);
Serial.print(F(","));
Serial.print(Temp2);
Serial.print(F(","));
Serial.print(Temp3);
Serial.print(F(","));    
Serial.print(Temp4);
Serial.print(F(","));
Serial.print(TempBoardDrive);
Serial.print(",");
}
//--------------------------------------------------------------------------------------------------------------------//
