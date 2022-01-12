/***** This is a name of parameter drive to turn on ****************/
//----- Port output to drive ---------------------------------------/
/*Discharge    8     //---   For discharge power from battery ------/
 Charge        7     //---   For Charge power to battery      ------/
 Predischarge  6     //----  Predischarge drive resistor 1k ohm  
                             for atleast 5 seconds ----------------*/
//------------------------------------------------------------------/

//--------------------------Tested battery sourcing power with pulse to load ----------------------// 
/*
digitalWrite(Predischarge,HIGH); //---- Output HIGH = ON
delay(1000);                     //---- Time to hold Pre-discharge ON
digitalWrite(Predischarge,LOW);  //---- Predischarge Output LOW = OFF
digitalWrite(Discharge,HIGH);    //---- Discharge Output HIGH = ON
//-------------------------------------------------------------------------------------------------//




/*******************************************************************/
/****** This function is a command to drive mosfet to turn on ******/
/*******************************************************************/
void OnMosfet(){
 //digitalWrite(Predischarge,HIGH); //---- Output HIGH = ON
 //delay(2000);                     //---- Time to hold Pre-discharge ON
 digitalWrite(Predischarge,LOW);  //---- Predischarge Output LOW = OFF
 digitalWrite(Discharge,HIGH);    //---- Discharge Output HIGH = ON
 digitalWrite(Charge,HIGH);
 }
//------------------------------------------------------------------//




/*******************************************************************/
/** This function is a command to drive mosfet to turn off  ********/
/*******************************************************************/
void OffMosfet(){
 digitalWrite(Discharge,LOW);
 digitalWrite(Predischarge, LOW);
 digitalWrite(Charge,       LOW);
 digitalWrite(Charge,LOW);
 }
 //-----------------------------------------------------------------//



 
