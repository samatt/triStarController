#include <SimpleTimer.h>
#include <ModbusMaster.h>
#include "triStarHeader.h"
#include "InfoHeader.h"
#include <Wire.h>

ModbusMaster triStar(1, 1);
int NumValues =13;
uint16_t data[13] = {
  0,0,0,0,0,0,0,0,0,0,0,0,0};




unsigned long lastConnectionTime = 0;           // last time you connected to the server, in milliseconds
boolean lastConnected = false;                  // state of the connection last time through the main loop
const unsigned long postingInterval = 10*1000;  // delay between updates to Pachube.com

boolean timeToSend = false;
// the timer object
SimpleTimer timer;

// a function to be executed periodically
void repeatMe() {
  Serial.print("Uptime (s): ");
  Serial.println(millis() / 1000);
  timeToSend = true;

}

void setup() {
  
  Wire.begin();
  triStar.begin(9600);
  timer.setInterval(10000, repeatMe);
  timeToSend = false;
  Serial.begin(9600);
  
}

void loop() {
  
  timer.run(); 
  
  // When it's time, send to i2c slave
  if(timeToSend){
  
    // Call readCharge, return string 
    String dataString = readChargeController(triStar,data);
    // Iterate over all characters in string
    // For each character, send to i2c slave
    for (int i = 0; i<dataString.length(); i++) {
        Wire.beginTransmission(2);
        Wire.write(dataString.charAt(i));
        Serial.print(dataString.charAt(i));
        Wire.endTransmission();
    }     
  } 
}


// Reads state of TriStar controller, builds JSON
String readChargeController(ModbusMaster master,uint16_t* readings){
  unsigned char i =0;
  String values;
  readings[i++]=readControlState(master);
  values += "{\"ControlState\":";
  values+=String(readings[0]);

  delay(10);
  readings[i++]= readControlMode(master);
  values += ",\"ControlMode\":";
  values+=String(readings[1]);

  delay(10);
  readings[i++]= readDipSwitch(master);
  values += ",\"DipSwitch\":";
  values+=String(readings[2]);

  delay(10);
  readings[i++]=readFault(master);
  values += ",\"Fault\":";
  values+=String(readings[3]);
  delay(10);

  readings[i++]= readChargeRegRefVoltage(master);
  values += ",\"ChargeRefVoltage\":";
  values+=String(readings[4]);

  delay(10);
  readings[i++]= readBatteryTemp(master);
  values += ",\"BatteryTemp\":";
  values+=String(readings[5]);

  delay(10);
  readings[i++] = readHeatSinkTemp(master);
  values += ",\"HeatSinkTemp\":";
  values+=String(readings[6]);

  delay(10);
  readings[i++]= readBatteryVoltageSlowFilter(master);
  values += ",\"BatteryVoltageSlowFilter\":";
  values+=String(readings[7]);

  delay(10);
  readings[i++]=readLoadCurrent(master);
  values += ",\"LoadCurrent\":";
  values+=String(readings[8]);

  delay(10);
  readings[i++]=readChargingCurrent(master);
  values += ",\"ChargingCurrent\":";
  values+=String(readings[9]);

  delay(10);
  readings[i++]=readLoadVoltage(master);
  values += ",\"LoadVoltage\":";
  values+=String(readings[10]);

  delay(10);
  readings[i++]= readBatteryVoltage(master);
  values += ",\"BatteryVoltage\":";
  values+=String(readings[11]);

  delay(10);
  readings[i++]= readBatterySense(master);
  values += ",\"BatterySense\":";
  values+=String(readings[12]);
  values += "}";
  return values;
  
}

