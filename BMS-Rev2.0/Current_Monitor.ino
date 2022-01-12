
/******************************************************************************************************/
/**************  This function is a measurement current sensing form R mOhm on board drive ************/
/******************************************************************************************************/
void CurrentMonitor(){
 //digitalWrite(Discharge,HIGH); 
 Current = 0;
 for(int i =0 ; i < 10 ;i++ ){ //---- store a data to 10 times 
     Current  += analogRead(CurrentPin);
     delay(10);
 }
 Current = Current/10; //---- Divide by 10 times to average Current was happen.
 Current =  ((( Current - 511 )));// * 0.0049 )*60)*0.06; //---- Average the data comparison with clamp amp meter  
 /*if(Current <0.1 ){   //----- Push the noise on the signal to zero 
    Current = 0;
 }*/

  /*if(Current >= CurrentSet){   //----- Push the noise on the signal to zero 
       Serial.println(F(" Current OVER "));
       OffMosfet(); //--------- cut Off
       delay(5000);
 }*/


 /*while(Current >= CurrentSet){
       Serial.println(F(" Current "));
       OffMosfet(); //--------- cut Off
       delay(1000);
 }*/ 
 //Serial.print(F("Current(A): "));
 Serial.println(Current);
}

//-------------------------------------------------------------------------------------------------------//
