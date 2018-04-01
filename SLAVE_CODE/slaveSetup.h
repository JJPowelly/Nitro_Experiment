#ifndef slaveSetup_H    // Put these two lines at the top of your file.
#define slaveSetup_H    // (Use a suitable name, usually based on the file name.)

int rx = 53;
int tx = 51;
char inbyte;
String myString;
String myStringA;
String myStringB;
String myStringC;
String Q = Q;

float osCon = 0;
int sv[] = {49, 47, 45, 43, 41, 39, 37, 35, 33};
//O2 Sensor 1
  const int OS1   = A0;
  const float VRef1 = 7;
  #define DHTPIN1 A1
//O2 Sensor 2
  const int OS2   = A2;
  const float VRef2 = 7;
  #define DHTPIN2 A3
//O2 Sensor 3
  const int OS3   = A4;
  const float VRef3 = 6;
  #define DHTPIN3 A5
//O2 Sensor 4
  const int OS4   = A6;
  const float VRef4 = 5;
  #define DHTPIN4 A7
//O2 Sensor 5
  const int OS5   = A8;
  const float VRef5 = 5;
  #define DHTPIN5 A9
//O2 Sensor 6
  const int OS6   = A10;
  const float VRef6 = 6;
  #define DHTPIN6 A11
//O2 Sensor 7
  const int OS7   = A12;
  const float VRef7 = 7;
  #define DHTPIN7 A13
//O2 Sensor 8
  const int OS8   = A14;
  const float VRef8 = 7;
  #define DHTPIN8 A15

#endif
