/*****************************************************************************************************/
/************ This function is a flash memory data from Serial port to update EEPROM *****************/
/************ Read the data table 1.0 of EEPROM Memory address and values digit **********************/
/************ Step : 1 Put the data type String into the address *************************************/
/************ Step : 2 Put the data type String into the valus ***************************************/
/*****************************************************************************************************/

/*****************************************************************************************************/
/************ This is a table 1.0  data of memory EEPROM  ********************************************/
//*********** Paramemter- EEPROM *********************************************************************/
/*----- Name Variable----------[   Address  |   Value    ]--------------------------------------------/
int   Mode                      |     0     |    1-2     |     *** Notice*********   
int   CurrentrentMem            |     1     |    0-100   |     Please careful for input of digit of
int   VcellMaxMem               |     2-3   |    0-4200  |     Parameter such as VcellMAX has a 4 digits
int   VcellMinMem               |     4-5   |    0-4200  |     use address 2 and 3 to store a data,
int   TempBatterySetMem         |     6-7   |    0-5500  |     adress 2 is  a two high digits and      
int   TempBatteryMinuteMem      |     8-9   |    0-5500  |     address 3 is a two low digits.
int   TempBoardDriveSetMem      |     10-11 |    0-9999  |     Example : [addr,val]=[2,42] [3,00] 
int   TempBoardDriveMinuteMem   |     11-12 |    0-9999  |     data is a 4200                         
/------------------------------------------------------------------------------------------------------*/    
/******************************************************************************************************/




/******************************************************************************************************/
/**************  Flash memmory ************************************************************************/
/******************************************************************************************************/
void FlashMem(){ 
 String input;
 String Addr;
 String Val;
 int  addr;
 int  val;
 Serial.print(F("Enter the address  : "));
 do{
    input = read_string();
    Addr = input;
    Serial.print(F("Addess: "));
    Serial.println(Addr);
    input = "";
    delay(50);
 }while(Serial.available() > 0 || input == 'q');   
 Serial.print(F("Enter the value : "));
 do{
    input = read_string();
    Val   = input;
    Serial.print("Val : ");
    Serial.println(Val);
    addr = Addr.toInt();
    val  = Val.toInt();
    EEPROM.update(addr,val);
    delay(100);
    EEPROM.read(addr);
    delay(100);
    Serial.print(F("Wite to addr :"));
    Serial.print(addr);
    Serial.print(",");
    Serial.println(val);
    Serial.println(F("Write Complete"));
    delay(1000);
 }while(Serial.available() > 0 || input == 'q');    
}
//-----------------------------------------------------------// 





/******************************************************************************************************/
/**************  Factory Update ************************************************************************/
/******************************************************************************************************/
void FactoryUpdate(){
Mode                         = 1;
CurrentSet                   = CurrentDefault;        
VcellMax                     = VcellMaxDefault;   
VcellMin                     = VcellMinDefault;    
TempBatterySet               = TempBatterySetDefault;            
TempBatteryMinute            = TempBatteryMinuteDefault;           
TempBoardDriveSet            = TempBoardDriveSetDefault;          
TempBoardDriveMinute         = TempBoardDriveMinuteDefault;       
}
//-------------------------------------------------------------//




/******************************************************************************************************/
/**************  Memory update from EEROM to a Variable. ***********************************************/
/******************************************************************************************************/
void MemUpdate(){
int VatH;             
int VatL;            
int sum;
//--------------- modeSelect----------------//
ModeSelect                   = EEPROM.read(0);
delay(10);
Mode                         = ModeSelect;

//------------- Current Read ----------------//
CurrentMem                   = EEPROM.read(1);
CurrentSet                   = CurrentMem; 

//------------- Vcell Max Read ---------------//
VatH                         = EEPROM.read(2);
VatL                         = EEPROM.read(3);
sum = ((VatH*100)+ VatL );
VcellMaxMem                  = sum;
VcellMax                     = VcellMaxMem;  

//------------ Vcell Min Read ----------------//
VatH                         = EEPROM.read(4);
VatL                         = EEPROM.read(5);
sum = ((VatH*100)+ VatL );
VcellMinMem                  = sum;
VcellMin                     = VcellMinMem;   

//-------------- TempBattery set---------------//
VatH                         = EEPROM.read(6);
VatL                         = EEPROM.read(7);
sum = ((VatH*100)+ VatL );
TempBatterySetMem            = sum;
TempBatterySet               = TempBatterySetMem; 

//------------- TempBatteryMinute----------------//
VatH                         = EEPROM.read(8);
VatL                         = EEPROM.read(9);
sum = ((VatH*100)+ VatL );
TempBatteryMinuteMem         = sum;
TempBatteryMinute            = TempBatteryMinuteMem;  

//-------------- TempBoardDriveSet --------------//
VatH                         = EEPROM.read(10);
VatL                         = EEPROM.read(11);
sum = ((VatH*100)+ VatL );
TempBoardDriveSetMem         = sum;
TempBoardDriveSet            = TempBoardDriveSetMem; 

//---------------- TempBoardDriveMinute -----------//
VatH                         = EEPROM.read(12);
VatL                         = EEPROM.read(13);
sum = ((VatH*100)+ VatL );
TempBoardDriveMinuteMem      = sum;
TempBoardDriveMinute         = TempBoardDriveMinuteMem;         
}
//------------------------------------------------------------//




/******************************************************************************************************/
/************** Factory data update to EEPROM *********************************************************/
/******************************************************************************************************/
void FactoryToMemUpdate(){
//------------ Mode Update ----------------------//
ModeSelect                   = 1;
EEPROM.update(0,1);
delay(10);

//----------- Current Update -------------------//
CurrentMem                   = CurrentDefault;  
EEPROM.update(1,70);
delay(10);

//----------- Vcell Max Update ----------------//
VcellMaxMem                  = VcellMaxDefault;  
EEPROM.update(2,42);
delay(10);
EEPROM.update(3,00);
delay(10);

//----------- Vcell Min Update -----------------//
VcellMinMem                  = VcellMinDefault;  
EEPROM.update(4,30);
delay(10);
EEPROM.update(5,00);
delay(10);

//------------ Temp Battery Update ------------// 
TempBatterySetMem            = TempBatterySetDefault;   
EEPROM.update(6,50);
delay(10);
EEPROM.update(7,00);
delay(10);

//------------ Temp Minute Update -------------//        
TempBatteryMinuteMem         = TempBatteryMinuteDefault;
EEPROM.update(8,01);
delay(10);
EEPROM.update(9,20);
delay(10);

//------------ Temp Board Drive ----------------//        
TempBoardDriveSetMem         = TempBoardDriveSetDefault; 
EEPROM.update(10,55);
delay(10);
EEPROM.update(11,00);
delay(10);

           
TempBoardDriveMinuteMem      = TempBoardDriveMinuteDefault; 
EEPROM.update(12,03);
delay(10);
EEPROM.update(13,00);
delay(10);

}
//---------------------------------------------------------------//
