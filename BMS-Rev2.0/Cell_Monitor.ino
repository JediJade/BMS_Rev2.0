/******************************************************************************************************/
/************** This is a original Code from LTC ******************************************************/
/************** Voltage cell Monitor for Atmega328p ***************************************************/
/******************************************************************************************************/
void VcellMonitor(){
 int8_t error = 0;  
 wakeup_sleep();
 LTC6804_wrcfg(TOTAL_IC,tx_cfg);
 wakeup_idle();
 LTC6804_adcv();
 delay(10);
 wakeup_idle();
 error = LTC6804_rdcv(0, TOTAL_IC,cell_codes);
 if(error == -1){
    Serial.println(F("A PEC error was detected in the received data"));
    }
    print_cells_ESP();
    delay(10); 
}
//-----------------------------------------------------------------------------------------------------//




//******************************************************************************************************/
/************** This is a function was modify to send a data to ESp32 CPU unit via EUART ***************/
/************** Voltage cell Monitor for Atmega328p ****************************************************/
/*******************************************************************************************************/
void print_cells_ESP(){
 long Vcell;
 long Vcell2;
 uint8_t i;
//---------------- Voltage Cell Top -------------------------------------------------------------------//
 for(i=0; i<8; i++){ 
     //Serial.print("C");
     //Serial.print(i+1,DEC);
     //Serial.print(":");
     Vcell2 = (cell_codes[1][i]);
     Serial.print(Vcell2);
     Serial.print(",");
 }
 //Serial.println(""); 
//---------------- Voltage Cell Bottom ----------------------------------------------------------------//  
 for(i=0; i<12; i++){ 
     //Serial.print("C");
    // Serial.print(i+9,DEC);
     //Serial.print(":");
     Vcell = (cell_codes[0][i]);
     Serial.print(Vcell);
     Serial.print(",");
 }
 //Serial.println(""); 
}
//-----------------------------------------------------------------------------------------------------//



//******************************************************************************************************/
/************** This is a function was modify for cells voltage Check **********************************/
/************** a high voltage for charger  and low voltage for discharge ******************************/
/************** Voltage cell Monitor for Linduino (Atmega328p) *****************************************/
/*******************************************************************************************************/
/* Not finish yet !*/
void CheckVoltage(){
 long Vcell;
 long Vcell2;
//Serial.println( VcellMinDefault);
//Serial.println( VcellMin);

 //----- Check Low Voltage -----------------------------//
 //----- Check Cell Voltage Top
 for(uint8_t i=0; i<8; i++){ 
      Vcell2 = (cell_codes[1][i]);
      while(Vcell2 <= VcellMin){
            Serial.println(F(" !BattLow "));
            OffMosfet(); //--------- cut Off
            delay(1000);
      } //---- While loop.
 } //----- Check Cell Voltage Bottom
 for(uint8_t i=0; i<12; i++){ 
      Vcell = (cell_codes[0][i]);
      while(Vcell <= VcellMin){
            Serial.println(F(" !BattLow "));
            OffMosfet(); //--------- cut Off
            delay(1000);
      }  
  }      


//----- Check High Voltage -----------------------------//
  //----- Check Cell Voltage Top
 for(uint8_t i=0; i<8; i++){ 
      Vcell2 = (cell_codes[1][i]);
      while(Vcell2 >= VcellMax){
            Serial.println(F(" !BatT Full "));
            OffMosfet(); //--------- cut Off
            delay(1000);
      } //---- While loop.
 } //----- Check Cell Voltage Bottom
 for(uint8_t i=0; i<12; i++){ 
      Vcell = (cell_codes[0][i]);
      while(Vcell >= VcellMax){
            Serial.println(F(" !Batt Full "));
            OffMosfet(); //--------- cut Off
            delay(1000);
      }  
  } 

  
}
//---------------------------------------------------------------------------------------------------//



//******************************************************************************************************/
/************** This is a function was modify for cells voltage Check **********************************/
/************** Voltage difference each other 20 cells and find cells Error per piece ******************/
/*******************************************************************************************************/
void CheckVolageDiff(){
 long CellError;
 long CellError2;
 long CellDiffSet  = 2000;
 long Vcell[12];
 long CellDiff[12];
 long Vcell2[8];
 long CellDiff2[8];
 long i;
//---------------- Voltage Cell Top --------------------------------------------------------------------//
 for(i=0; i<8; i++){ 
      Vcell2[i]    =  (cell_codes[1][i]);
 }
 for(i=0; i<7; i++){ 
     CellDiff2[i]  =  (Vcell2[i] - Vcell2[i+1]);
     //Serial.println (Vcell2[i]);
     //Serial.println (Vcell2[i+1]);
     //Serial.print   ("Voltage Cell diff  "); 
     //Serial.print   (i+1); 
     //Serial.print   (" : ");
     //Serial.println (CellDiff2[i]);
 }
 for(i=0; i<12; i++){ 
     Vcell[i]      =  (cell_codes[0][i]);
 }
 for(i=0; i<11; i++){ 
     CellDiff[i]   =  (Vcell[i] - Vcell[i+1]);
      //Serial.println(Vcell2[i]);
      //Serial.println(Vcell2[i+1]);
      //Serial.print  ("Voltage Cell diff  "); 
      //Serial.print  (i+8); 
      //Serial.print  (" : ");
      //Serial.println(CellDiff[i]);
 }
//--------- Find cell error ---------------------------------------------------------------------------//
//---------- Vtop -------------------------------------------------------------------------------------//
 for (i=0; i<7; i++){ 
      if(CellDiff2[i] <= (CellDiffSet*(-1))){
      //Serial.print     ("Number -T of  Cell diff  "); 
      //Serial.print     (i+1+1); 
      //Serial.print     (" : ");
      //Serial.println   (CellDiff2[i]);
      //Serial.print     ("Cell Error : "); 
      //Serial.println   (i+1+1);
      Cell[i+1] = 255;  
      }
     if(CellDiff2[i] >= CellDiffSet){
        //Serial.print  ("Number +T Cell diff  "); 
        //Serial.print  (i+1); 
        //Serial.print  (" : ");
        //Serial.println(CellDiff2[i]);
        //Serial.print  ("Cell Error : "); 
        //Serial.println(i+1);
        Cell[i] = 255;
     }
 }
//--------------- Vbottom -----------------------------------------------------------------------------//
 for(i=0; i<11; i++){ 
     if(CellDiff[i] <= (CellDiffSet*(-1))){
        //Serial.print  ("Number -B Cell diff  "); 
        //Serial.print  (i+9+1); 
        //Serial.print  (" : ");
        //Serial.println(CellDiff[i]);
        //Serial.print  ("Cell Error : "); 
        //Serial.println(i+9+1);
        Cell[i+9] = 255;  
     }
//-------------- Vtop --------------------------------------------------------------------------------//     
     if(CellDiff[i] >= CellDiffSet){
        //Serial.print  ("Number +B Cell diff  "); 
        //Serial.print  (i+9); 
        //Serial.print  (" : ");
        //Serial.println(CellDiff[i]);
        //Serial.print  ("Cell Error : "); 
        //Serial.println(i+9);
        Cell[i+8] = 255;
    }
 }
 /*for (i=0; i<20; i++){ //---- List Cell status monitor
     Serial.print  (F("ListCell Status  ")); 
     Serial.print  (i+1); 
     Serial.print  (" : ");
     Serial.println(Cell[i]); 
     delay(10);
 }*/
 //---------- Check Vcell Diff Error ---------------------------------------------------------------//
 for(i=0; i<20; i++){ 
     while( Cell[i] == 255){    
     Serial.println (F("Error Code : BF02")); 
     OffMosfet();//---------------- Cut off ---------------------//
     delay(5000);
     } 
 } 
}
//--------------------------------------------------------------------------------------------------//
