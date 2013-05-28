#include <ModbusMaster.h>

/*
TriStar supports RTU mode MODBUS only.
 Serial Parameters 9600, N, 8 ,2
 */
// discrete coils
//#define NANO_DO(n)   (0x0000 + n) ///< returns triStar discrete output address
//#define NANO_FLAG(n) (0x1000 + n) ///< returns triStar flag address

// discrete inputs
//#define NANO_DI(n)   (0x0000 + n) ///< returns triStar discrete input address

// analog holding registers
#define TRISTAR_adc_vb_f(n)        (0x0008 + 2 * n) ///< Battery Voltage, filtered
#define TRISTAR_adc_vs_f(n)        (0x0009 + 2 * n) ///< Battery Sense Voltage, filtered
#define TRISTAR_adc_vx_f(n)        (0x000A + 2 * n) ///< Array/Load voltage, filtered
#define TRISTAR_adc_ipv_f(n)       (0x000B + 2 * n) ///< Charging current, filtered 
#define TRISTAR_adc_iload_f(n)     (0x000C + 2 * n) ///< Load Current, filtered
#define TRISTAR_Vb_f(n)            (0x000D + 2 * n) ///< Battery voltage, slow filter
#define TRISTAR_T_hs(n)            (0x000E + 2 * n) ///< Heatsink temperature
#define TRISTAR_T_batt(n)          (0x000F + 2 * n) ///< Battery temp (RTS connected), (0x80, if not connected)
#define TRISTAR_V_ref(n)           (0x0010 + 2 * n) ///< Charge regulator reference voltage
#define TRISTAR_Ah_r_HI(n)         (0x0011 + 2 * n) ///< Ah resetable, HI word(should make n 2 for this )
#define TRISTAR_Ah_r_LO(n)         (0x0012 + 2 * n) ///< Ah resetable, LO word
#define TRISTAR_Ah_t_HI(n)         (0x0013 + 2 * n) ///< Ah total, HI word(should make n 2 for this )
#define TRISTAR_Ah_t_LO(n)         (0x0014 + 2 * n) ///< Ah total, LO word
#define TRISTAR_hourmeter_HI(n)    (0x0015 + 2 * n) ///< hourmeter, HI word(should make n 2 for this )
#define TRISTAR_hourmeter_LO(n)    (0x0016 + 2 * n) ///< hourmeter, LO word
#define TRISTAR_Alarm_LO(n)        (0x0017 + 2 * n) ///< alarm bitfield
#define TRISTAR_fault(n)           (0x0018 + 2 * n) ///< fault bitfield
#define TRISTAR_dip_switch(n)      (0x0019 + 2 * n) ///< dip swith settings at power on switch [1...8] in bits [0...7]
#define TRISTAR_control_mode(n)    (0x001A + 2 * n) ///< Control mode (0=charge,1=load,2=diversion)
#define TRISTAR_control_state(n)   (0x001B + 2 * n) ///< Control State
#define TRISTAR_pwm_duty_cycle(n)  (0x001C + 2 * n) 
#define TRISTAR_Alarm_HI(n)        (0x001D + 2 * n) ///< Alarm bitfield (continued from 0x001z)


/* TS Modbus registers have scalars used to calculate their equivalent double value */
/* You can get more info from this pdf: http://www.morningstarcorp.com/en/support/library/TS.APP.Modbus.EN.04.pdf */

static double stdScalar1 = 0.002950042724609375;
static double stdScalar2 = 0.00424652099609375;
static double stdScalar3 = 0.002034515380859375;
static double stdScalar4 = 0.00966400146484375;
static double stdScalar5 = 0.000031;
static double stdScalar6 = 0.1;


uint16_t readControlState(ModbusMaster node){

  uint8_t result;
  uint16_t data;

  result = node.readHoldingRegisters(TRISTAR_control_state(0), 1);
//Serial.println(result,HEX);
  if (result == node.ku8MBSuccess){
    data  = node.getResponseBuffer(0);
    return data;
  }
  else{
    return 0;
  }
}


uint16_t readControlMode(ModbusMaster node){

  uint8_t result;
  uint16_t data;

  result = node.readHoldingRegisters(TRISTAR_control_mode(0), 1);
//Serial.println(result,HEX);
  if (result == node.ku8MBSuccess){
    data  = node.getResponseBuffer(0);
    return data;
  }
  else{
    return 0;
  }
}

uint16_t readDipSwitch(ModbusMaster node){

  uint8_t result;
  uint16_t data;
  result = node.readHoldingRegisters(TRISTAR_dip_switch(0), 1);
    data  = node.getResponseBuffer(0);
 //   Serial.println(data);
  if (result == node.ku8MBSuccess){
    data  = node.getResponseBuffer(0);
   // Serial.println(data);
    return data;
  }
  else{
    return 0;
  }

}

uint16_t readFault(ModbusMaster node){

  uint8_t result;
  uint16_t data;
  result = node.readHoldingRegisters(TRISTAR_fault(0), 1);
    data  = node.getResponseBuffer(0);
 
  if (result == node.ku8MBSuccess){
    data  = node.getResponseBuffer(0);
   
    return data;
  }
  else{
    return 0;
  }

}

uint16_t readChargeRegRefVoltage(ModbusMaster node){

  uint8_t result;
  uint16_t data;
  result = node.readHoldingRegisters(TRISTAR_V_ref(0), 1);
    data  = node.getResponseBuffer(0);
 
  if (result == node.ku8MBSuccess){
    data  = node.getResponseBuffer(0);
   
    return (data*stdScalar1);
  }
  else{
    return 0;
  }

}


uint16_t readBatteryTemp(ModbusMaster node){

  uint8_t result;
  uint16_t data;
  result = node.readHoldingRegisters(TRISTAR_T_batt(0), 1);
    data  = node.getResponseBuffer(0);
 
  if (result == node.ku8MBSuccess){
    data  = node.getResponseBuffer(0);
   
    return data;
  }
  else{
    return 0;
  }

}

uint16_t readHeatSinkTemp(ModbusMaster node){

  uint8_t result;
  uint16_t data;
  result = node.readHoldingRegisters(TRISTAR_T_hs(0), 1);
    data  = node.getResponseBuffer(0);
 
  if (result == node.ku8MBSuccess){
    data  = node.getResponseBuffer(0);
   
    return data;
  }
  else{
    return 0;
  }

}

uint16_t readBatteryVoltageSlowFilter(ModbusMaster node){

  uint8_t result;
  uint16_t data;
  result = node.readHoldingRegisters(TRISTAR_Vb_f(0), 1);
    data  = node.getResponseBuffer(0);
 
  if (result == node.ku8MBSuccess){
    data  = node.getResponseBuffer(0);
   
    return data*stdScalar1;
  }
  else{
    return 0;
  }

}

uint16_t readLoadCurrent(ModbusMaster node){

  uint8_t result;
  uint16_t data;
  result = node.readHoldingRegisters(TRISTAR_adc_iload_f(0), 1);
    data  = node.getResponseBuffer(0);
 
  if (result == node.ku8MBSuccess){
    data  = node.getResponseBuffer(0);
   
    return (data*stdScalar4);
  }
  else{
    return 0;
  }

}

uint16_t readChargingCurrent(ModbusMaster node){

  uint8_t result;
  uint16_t data;
  result = node.readHoldingRegisters(TRISTAR_adc_ipv_f(0), 1);
    data  = node.getResponseBuffer(0);
  if (result == node.ku8MBSuccess){
    data  = node.getResponseBuffer(0);
 
    return (data*stdScalar3);
  }
  else{
    return 0;
  }

}

uint16_t readLoadVoltage(ModbusMaster node){

  uint8_t result;
  uint16_t data;
  result = node.readHoldingRegisters(TRISTAR_adc_vx_f(0), 1);
    data  = node.getResponseBuffer(0);
 	//TODO:  I know some calculation is required here. 
 	//Not sure what it is yet
  if (result == node.ku8MBSuccess){
    data  = node.getResponseBuffer(0);
   // Serial.println(data);
    return (data*stdScalar3);
  }
  else{
    return 0;
  }

}

uint16_t readBatteryVoltage(ModbusMaster node){

  uint8_t result;
  uint16_t data;
  result = node.readHoldingRegisters(TRISTAR_adc_vb_f(0), 1);
    data  = node.getResponseBuffer(0);
 	//TODO:  I know some calculation is required here. 
 	//Not sure what it is yet
  if (result == node.ku8MBSuccess){
    data  = node.getResponseBuffer(0);
   // Serial.println(data);
    return  (data*stdScalar1);
  }
  else{
    return 0;
  }

}

uint16_t readBatterySense(ModbusMaster node){

  uint8_t result;
  uint16_t data;

  result = node.readHoldingRegisters(TRISTAR_adc_vs_f(0), 1);

  if (result == node.ku8MBSuccess){
    data  = node.getResponseBuffer(0);

    return (data*stdScalar1);
  }
  else{
    return 0;
  }
}

