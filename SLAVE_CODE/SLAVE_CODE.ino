//Sender Code
#include "DHT.h"
#include "slaveSetup.h"
#include <SoftwareSerial.h>

DHT dht;
SoftwareSerial mySerial = SoftwareSerial(rx,tx);

//-----------------------------------------------
// VARIABLE PARAMETERS - CHANGE ON FLY
//-----------------------------------------------
  // UNIVERSAL DELAY BETWEEN READINGS
     int del = 10000;
  // TARGET O2% TO DO REFILL
     float targetHigh = 1.9;
  // TARGET O2% TO STOP REFILL
     float targetLow = 0.9;

void setup() 
{
  //Serial.begin(9600);
  mySerial.begin(9600);
//-----------------------------------------------
// FOR LOOP ITERATIONS FOR PINMODE AND OUTPUT
//-----------------------------------------------


  pinMode(sv[0],OUTPUT);
  pinMode(sv[1],OUTPUT);
  pinMode(sv[2],OUTPUT);
  pinMode(sv[3],OUTPUT);

  digitalWrite(sv[0],HIGH);
  digitalWrite(sv[1],HIGH);
  digitalWrite(sv[2],HIGH);
  digitalWrite(sv[3],HIGH);
  /*
  for (int i = 0; i < 9; i++){
    pinMode(sv[i], OUTPUT);
    delay(10);
  }
  for (int i = 0; i < 9; i++){
    digitalWrite(sv[i], HIGH);
    delay(10);
  }
  */
}
//-------------------------------------------------
// MAIN LOOP
//-------------------------------------------------
void loop(){ 
//------------------------------------------------
// SETS UP FIST INCOMING BYTE OF DATA, MAKES SURE
// IT'S THE CORRECT VARIABLES AND NOT GARBAGE
//------------------------------------------------
 // primeSerialPort();
  delay(100);

  while(mySerial.available() > 0){
    inbyte = mySerial.read();
    while(inbyte == -1 || inbyte == 0){
      inbyte = mySerial.read();
    }
    if(inbyte == 1 || inbyte == 2 || inbyte == 3){
    continue;
    }
  }
//-----------------------------------------------
// CONTAINER/FRIDGE CODE #1
//-----------------------------------------------
  if(inbyte == 1){
    myStringA = doReadings(9, OS1 , VRef1 , DHTPIN1);
      delay(100);
    float os1Con = (readConcentration(9, OS1, VRef1));
      delay(100);
    if(os1Con > targetHigh){
      doRefill(9, osCon , OS1 , VRef1 , sv[2] , sv[3]);
        delay(1000);
      myStringA = doReadings(9 , OS1 , VRef1 , DHTPIN1);
    }
    mySerial.print(myStringA); 
  }
//-----------------------------------------------
// CONTAINER/FRIDGE CODE #2
//-----------------------------------------------
  if(inbyte == 2){
    myStringB = doReadings(10 , OS2 , VRef2 , DHTPIN2);
      delay(100);
    float os2Con = (readConcentration(10, OS2, VRef2));
      delay(100);
    if(os2Con > 2.5){
      //doRefill(10, osCon , OS2 , VRef2 , sv[0] , sv[1]);
        delay(1000);
      //myStringB = doReadings(10 , OS2 , VRef2 , DHTPIN2);
    }
    mySerial.print(myStringB);  
  }
//-----------------------------------------------
// CONTAINER/FRIDGE CODE #3
//-----------------------------------------------
  if(inbyte == 3){
    myStringC = doReadings(11 , OS3 , VRef3 , DHTPIN3);
      delay(100);
    float osCon = (readConcentration(11, OS3, VRef3));
      delay(100);
    if(osCon > 2.5){
     // doRefill(11, osCon , OS3 , VRef3 , sv[0] , sv[1]);
        delay(1000);
     // myStringC = doReadings(11 , OS3 , VRef3 , DHTPIN3);
    }
    mySerial.print(myStringC);
  }
//--------------------------------------------------
// CLEAR SW SERIAL FOR NEXT ITERATION 
//--------------------------------------------------
   inbyte = 0;
    delay(100);
  // primeSerialPort();
    delay(100);
}  
//--------------------------------------------------
// END LOOP  ---  START METHODS
//--------------------------------------------------

//--------------------------------------------------
// DO READINGS METHOD, DOES ALL READINGS AND PRINTS 
// DATA LINES IN CSV FOR EXCEL DAQ
//--------------------------------------------------
String doReadings(uint8_t contain, uint8_t OS, uint8_t VRef, uint8_t dhtpin)
{
  String myString;
    delay(20);
  dht.setup(dhtpin);
    delay(100);
  myString = "DATA,TIME,DATE,";
    delay(20);
  myString += contain;
    delay(20);
  myString += ",";
    delay(100);
  float osCon = (readConcentration(contain, OS, VRef));
    delay(100);
  myString += osCon;
    delay(10);
  myString += ",";
    delay(dht.getMinimumSamplingPeriod());
  float temp = dht.getTemperature();
    delay(dht.getMinimumSamplingPeriod());
  float hum = dht.getHumidity();
    delay(100);
  myString += temp;
    delay(10);
  myString += ",";
    delay(10);
  myString += hum;
    delay(10);
  myString += ",";
    delay(10); 
  if (osCon > targetHigh) {
    myString += "1";
  } else {
    myString += "0";
  }
    delay(100);
  myString += "Q";
  return myString;
}
//--------------------------------------------------
// REFILL METHOD IF CONTAINER IS OVER TARGET O2%
//--------------------------------------------------
  void doRefill(uint8_t contain , uint8_t osCon , uint8_t OS , int16_t VRef , uint8_t sv1 , uint8_t sv2) {
    digitalWrite(sv1, LOW);
      delay(100);
    digitalWrite(sv2, LOW);
      delay(100);
    if(contain == 9){
      do {
        osCon = (readConcentration(contain, OS, VRef));
        delay(1500);
      } while (osCon > targetLow);   
    }
    if(contain == 10){
      do {
        osCon = (readConcentration(contain, OS, VRef));
        delay(1500);
      } while (osCon > 1.5);
    }
      
      delay(100);
    digitalWrite(sv1, HIGH);
      delay(100);
    digitalWrite(sv2, HIGH);
      delay(100);
    return;
  } 
//--------------------------------------------------
// READ VOLTAGE OF O2 SENSORS METHOD
//--------------------------------------------------
  float readO2Vout(uint8_t analogpin, uint8_t VRefer){
    long sum = 0;
    for (int i = 0; i < 32; i++) {
      sum += analogRead(analogpin);
    }
    sum >>= 5;
    float MeasuredVout = sum * (VRefer / 1023.0);
    return MeasuredVout;
  }
//--------------------------------------------------
// READS O2 CONCENTRATION FROM SENSOR VOLTAGE
//--------------------------------------------------
  float readConcentration(uint8_t contain , uint8_t analogpin, int16_t VRefer) {
    
    float mod;
    if(contain == 9){ mod = 0.25;}
    if(contain == 10){ mod = 0.25;}
    if(contain == 11){ mod = 0.23;}
    
    float MeasuredVout = readO2Vout(analogpin, VRefer);
    float preConcentration = MeasuredVout * mod;
    float Concentration = preConcentration / 2.0;
    float Concentration_Percentage = Concentration * 100;
    return Concentration_Percentage;
  }
//--------------------------------------------------
// CLEARS SERIAL PORT DATA - COLLECTS/CLEARS GARBAGE 
//--------------------------------------------------
  void primeSerialPort(){
    digitalWrite(rx, HIGH);
    digitalWrite(tx, HIGH);
    mySerial.listen();
    mySerial.flush();
  }
