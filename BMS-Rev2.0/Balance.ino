void Balancing(){

 if(Current <= 1){
 long Vcell;
 long Vcell2;
 uint8_t i;
 //---------------- Voltage Cell Top -------------------------------------------------------------------//
 for(i=0; i<8; i++){ 
     Vcell2 = (cell_codes[1][i]);
     if(Vcell2 > 41500){

        balance_cfg(1,i);
      
      }//------- if
 }//------------ for

 //tx_cfg[1][5] = tx_cfg[1][5]  || 0xF0; 
 
//---------------- Voltage Cell Bottom ----------------------------------------------------------------//  
 for(i=0; i<12; i++){ 
     Vcell = (cell_codes[0][i]);
     if(Vcell > 41500){

        balance_cfg(0,i);
      
      }
      //------ if
  }//----------- for
 
  //tx_cfg[0][5] = tx_cfg[0][5]   0xF0; 

  //----------- Write to register
  //LTC6804_wrcfg( TOTAL_IC, tx_cfg);

  /*Serial.print("Cell_Top_cfg4");
  Serial.print(tx_cfg[1][4],BIN);
  Serial.print("cfg5");
  Serial.println(tx_cfg[1][5],BIN);
  
  delay(10);
  Serial.print("Cell_Bottom_cfg4");
  Serial.print(tx_cfg[0][4],BIN);
  Serial.print("cfg5");
  Serial.println(tx_cfg[0][5],BIN);*/
  delay(2500);

  
  
    
 }//----------- if current 

if (Current > 1){
  
  
  //Serial.println("Unbalance High current");
  tx_cfg[0][4] = 0x00;
  tx_cfg[0][5] = 0x00;
  tx_cfg[1][4] = 0x00;
  tx_cfg[1][5] = 0x00;
  
  }

}//------------ Void
//-------------------------------------------------------------------------------------------------------//







/*!***********************************
  \brief sets  the configuration array for cell balancing
  uses CFGR4 and lowest 4 bits of CGFR5
 **************************************/
void balance_cfg(int ic, int cell)
{
  //tx_cfg[ic][4] = 0x00; // clears S1-8
  //tx_cfg[ic][5] = tx_cfg[ic][5]  & 0xF0; // clears S9-12 and sets software timer to 1 min
  //Serial.println(tx_cfg[ic][5] & 0xF0,BIN);
  if (cell >= 0 and cell <= 7) {
    tx_cfg[ic][4] = tx_cfg[ic][4] | 1 << cell;
  }
  if ( cell > 7) {
    tx_cfg[ic][5] = tx_cfg[ic][5] | ( 1 << (cell - 8));
  }
}
