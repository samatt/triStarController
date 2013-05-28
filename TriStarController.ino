#include <SimpleTimer.h>
#include <GSM.h>
#include <ModbusMaster.h>
#include "triStarHeader.h"
#include "InfoHeader.h"


GSMClient client;
GPRS gprs;
GSM gsmAccess;

ModbusMaster triStar(1, 1);
int NumValues =13;
uint16_t data[13] = {
  0,0,0,0,0,0,0,0,0,0,0,0,0};
  
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
// IPAddress server(216,52,233,121);     // numeric IP for api.pachube.com
char server[] = "api.pachube.com";       // name address for Pachube API

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
  // initialize Modbus communication baud rate and parity bit N to 2
//  UCSR1C = UCSR1C | B00001000;
  triStar.begin(9600);
  timer.setInterval(10000, repeatMe);
  timeToSend = false;
  // initialize serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  
  // connection state
  boolean notConnected = true;
  Serial.println("connecting...");
  // After starting the modem with GSM.begin()
  // attach the shield to the GPRS network with the APN, login and password 
  while(notConnected)
  {
    if((gsmAccess.begin(PINNUMBER)==GSM_READY) &
        (gprs.attachGPRS(GPRS_APN, GPRS_LOGIN, GPRS_PASSWORD)==GPRS_READY))
      notConnected = false;
    else
    {
      Serial.println("Not connected");
      delay(1000);
    }
  }

  Serial.println("Connected to GPRS network");
    

}

void loop() {
    timer.run(); 
   // read the sensor on A0
//  int sensorReading =100; 
//  
//  // convert the data to a String
//  String dataString = "ControlMode,";
//  dataString += sensorReading;
////
////  // you can append multiple readings to this String to 
////  // send the pachube feed multiple values
//  int otherSensorReading = 50;
//  dataString += "\nControlState,";
//  dataString += otherSensorReading;  
//  dataString += "\n";    
String dataString = "";
  if(timeToSend){
    
    dataString = readChargeController(triStar,data);
  }
 // String dataString =readChargeController (); 
  // if there's incoming data from the net connection.
  // send it out the serial port.  This is for debugging
  // purposes only
//  
//  int otherSensorReading2 = 500;
//  dataString += "\nalarm,";
//  dataString += otherSensorReading2;    
  if (client.available())
  {
    char c = client.read();
    Serial.print(c);
  }

  // if there's no net connection, but there was one last time
  // through the loop, then stop the client
  if (!client.connected() && lastConnected)
  {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
  }

  // if you're not connected, and ten seconds have passed since
  // your last connection, then connect again and send data
  //if(!client.connected() && (millis() - lastConnectionTime > postingInterval))
  if(timeToSend)
  {
   // Serial.println("sending");
    sendData(dataString);
    timeToSend = false;
  }
  // store the state of the connection for next time through
  // the loop
  lastConnected = client.connected();
}


// this method makes a HTTP connection to the server
void sendData(String thisData)
{
  // if there's a successful connection:
  if (client.connect(server, 80))
  {
        Serial.print(thisData);
    Serial.println("connecting...");
    
    // send the HTTP PUT request:
    client.print("PUT /v2/feeds/");
    client.print(FEEDID);
    client.println(".csv HTTP/1.1");
    client.print("Host: api.pachube.com\n");
    client.print("X-ApiKey: ");
    client.println(APIKEY);
    client.print("User-Agent: ");
    client.println(USERAGENT);
    client.print("Content-Length: ");
    client.println(thisData.length());

    // last pieces of the HTTP PUT request
    client.print("Content-Type: text/csv\n");
    client.println("Connection: close\n");
    client.println();
    
    // here's the actual content of the PUT request
    client.println(thisData);
  } 
  else
  {
    // if you couldn't make a connection
    Serial.println("connection failed");
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
  }
  // note the time that the connection was made or attempted:
  lastConnectionTime = millis();
}


String readChargeController(ModbusMaster master,uint16_t* readings){
  unsigned char i =0;
String values;
  readings[i++]=readControlState(master);
  values += "ControlState,";
  values+=String(readings[0]);
  delay(10);
  readings[i++]= readControlMode(master);
  values += "\nControlMode,";
  values+=String(readings[1]);
  delay(10);
  readings[i++]= readDipSwitch(master);
  values += "\nDipSwitch,";
  values+=String(readings[2]);
  delay(10);
  readings[i++]=readFault(master);
  values += "\nFault,";
  values+=String(readings[3]);
  delay(10);
  readings[i++]= readChargeRegRefVoltage(master);
  values += "\nChargeRefVoltage,";
  values+=String(readings[4]);
  delay(10);
  readings[i++]= readBatteryTemp(master);
  values += "\nBattery Temp,";
  values+=String(readings[5]);
  delay(10);
  readings[i++] = readHeatSinkTemp(master);
  values += "\nHeat Sink Temp,";
  values+=String(readings[6]);
  delay(10);
  readings[i++]= readBatteryVoltageSlowFilter(master);
  values += "\nBattery Voltage Slow Filter,";
  values+=String(readings[7]);  
  delay(10);
  readings[i++]=readLoadCurrent(master);
  values += "\nLoad Current,";
  values+=String(readings[8]);  
  delay(10);
  readings[i++]=readChargingCurrent(master);
  values += "\nCharging Current,";
  values+=String(readings[9]);    
  delay(10);
  readings[i++]=readLoadVoltage(master);
  values += "\n Load Voltage,";
  values += String(readings[10]);
  delay(10);
  readings[i++]= readBatteryVoltage(master);
  values += "\nBattery Voltage,";
  values += String(readings[11]);
  delay(10);
  readings[i++]= readBatterySense(master);
  values += "\nBattery Sense,";
  values += String(readings[12]);
return values;

}



