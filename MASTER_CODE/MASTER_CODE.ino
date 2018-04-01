//MASTER Code
#include "DHT.h"
#include "masterSetup.h"
#include <SoftwareSerial.h>
SoftwareSerial mySerial = SoftwareSerial(rx, tx);
DHT dht;

// VARIABLE PARAMETERS - CHANGE ON FLY
  // UNIVERSAL DELAY BETWEEN READINGS
     int del = 10000;
  // TARGET O2% TO DO REFILL
     float targetHigh = 1.9;
  // TARGET O2% TO STOP REFILL
     float targetLow = 0.9;
  
void setup(){
  Serial.begin(9600);
  mySerial.begin(9600);
  
  pinMode(sv[0],OUTPUT);
  pinMode(sv[1],OUTPUT);
  pinMode(sv[2],OUTPUT);
  pinMode(sv[3],OUTPUT);
  pinMode(sv[4],OUTPUT);
  pinMode(sv[5],OUTPUT);
  pinMode(sv[6],OUTPUT);
  pinMode(sv[7],OUTPUT);
  pinMode(sv[8],OUTPUT);
  digitalWrite(sv[0],HIGH);
  digitalWrite(sv[1],HIGH);
  digitalWrite(sv[2],HIGH);
  digitalWrite(sv[3],HIGH);
  digitalWrite(sv[4],HIGH);
  digitalWrite(sv[5],HIGH);
  digitalWrite(sv[6],HIGH);
  digitalWrite(sv[7],HIGH);
  digitalWrite(sv[8],HIGH);
}

void loop(){
  String myStringA;
  String myStringB;
  String myStringC;
    delay(10000);
  doReadings(1 , OS1 , VRef1 , DHTPIN1 , sv[0] , sv[1]);
    delay(del);
  doReadings(2 , OS2 , VRef2 , DHTPIN2 , sv[0] , sv[2]);
    delay(del);
  doReadings(3 , OS3 , VRef3 , DHTPIN3 , sv[0] , sv[3]);
    delay(del);
  doReadings(4 , OS4 , VRef4 , DHTPIN4 , sv[0] , sv[4]);
    delay(del);
  doReadings(5 , OS5 , VRef5 , DHTPIN5 , sv[0] , sv[5]);
    delay(del);
  doReadings(6 , OS6 , VRef6 , DHTPIN6 , sv[0] , sv[6]);
    delay(del);
  doReadings(7 , OS7 , VRef7 , DHTPIN7 , sv[0] , sv[7]);
    delay(del);
  doReadings(8 , OS8 , VRef8 , DHTPIN8 , sv[0] , sv[8]);
    delay(del);
//--------------------------------------------------
// SLAVE CONTAINERS/FRIDGE CODE #1
//--------------------------------------------------
  primeSerialPort();
    delay(100);
  if(A == 0) {
    mySerial.print(one);
      delay(3000);
    while (mySerial.available() > 0) {
      character = mySerial.read();
      if (character == 'Q') {
        Serial.println(myStringA);
        continue;
      } else {
        myStringA += character;
      }
    }
    A = 1;
  }
  A = 0;
  delay(del);
//--------------------------------------------------
// SLAVE CONTAINERS/FRIDGE CODE #2
//--------------------------------------------------
  primeSerialPort();
  delay(100);
  if(B == 0){
     mySerial.print(two);
       delay(3000);
     while(mySerial.available() > 0){
       character = mySerial.read();
       if(character == 'Q'){
         Serial.println(myStringB);
         continue;
       }else{
         myStringB += character;
       }
     }
    B = 1;
   }
   B = 0;
   primeSerialPort();
   delay(del);  
//--------------------------------------------------
// SLAVE CONTAINERS/FRIDGE CODE #3
//--------------------------------------------------
  primeSerialPort();
  delay(100);
  if(C == 0){
     mySerial.print(three);
       delay(3000);
     while(mySerial.available() > 0){
       character = mySerial.read();
       if(character == 'Q'){
         Serial.println(myStringC);
         continue;
       }else{
         myStringC += character;
       }
     }
    C = 1;
   }
   C = 0;
   primeSerialPort();
   delay(del);    
}
//--------------------------------------------------
// END LOOP
// DO READINGS METHOD, DOES ALL READINGS AND PRINTS 
// DATA LINES IN CSV FOR EXCEL DAQ
//--------------------------------------------------
  void doReadings(uint8_t contain, uint8_t OS, int16_t VRef, uint8_t dhtpin , uint8_t sv1 , uint8_t sv2) {
    dht.setup(dhtpin);
      delay(100);
    Serial.print("DATA,TIME,DATE,");
      delay(100);
    Serial.print(contain);
    Serial.print(",");
      delay(100);
    float osCon = (readConcentration(contain, OS, VRef));
      delay(100);
    Serial.print(osCon);
    Serial.print(",");
      delay(dht.getMinimumSamplingPeriod());
    Serial.print(dht.getTemperature());
    Serial.print(",");
      delay(dht.getMinimumSamplingPeriod());
    Serial.print(dht.getHumidity());
    Serial.print(",");
      delay(100);
    if (osCon > targetHigh) {
      Serial.println("1");
    } else {
      Serial.println("0");
    }
    delay(100);
    if (osCon > targetHigh){
      digitalWrite(sv1, LOW);
        delay(100);
      digitalWrite(sv2, LOW);
        delay(100);
      do {
      osCon = (readConcentration(contain, OS, VRef));
        delay(1500);
      } while (osCon > targetLow);   
        delay(100);
      digitalWrite(sv1, HIGH);
        delay(8000);
      digitalWrite(sv2, HIGH);
        delay(3000);
    }
      delay(100);
  }
//--------------------------------------------------
// REFILL METHOD IF CONTAINER IS OVER TARGET O2%
//--------------------------------------------------
  void doRefill(uint8_t contain , uint8_t osCon , uint8_t OS , int16_t VRef , uint8_t sv1 , uint8_t sv2) {
    digitalWrite(sv1, LOW);
      delay(100);
    digitalWrite(sv2, LOW);
      delay(100);
    do {
      osCon = (readConcentration(contain, OS, VRef));
      delay(1500);
    } while (osCon > targetLow);   
    
      delay(100);
    digitalWrite(sv1, HIGH);
      delay(1000);
    digitalWrite(sv2, HIGH);
      delay(100);
    int good = 1;
  }
//--------------------------------------------------
// READ VOLTAGE OF O2 SENSORS METHOD
//--------------------------------------------------
  float readO2Vout(uint8_t analogpin, int16_t VRefer) {
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
    if(contain == 1){ mod = 0.2505;}
    if(contain == 2){ mod = 0.25;}
    if(contain == 3){ mod = 0.243;}
    if(contain == 4){ mod = 0.2457;}
    if(contain == 5){ mod = 0.255;}
    if(contain == 6){ mod = 0.268;}
    if(contain == 7){ mod = 0.26;}
    if(contain == 8){ mod = 0.25;}
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
