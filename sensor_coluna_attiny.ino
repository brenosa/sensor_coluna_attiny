#include <TinyWireM.h>
 //TinyTinyWireMM
//Endereco I2C do MPU6050
const int mpu=0x68;  
//Variaveis para armazenar valores dos sensores
int AcX,AcY,AcZ;
int timeInIncorrectPosture = 0;
void setup()
{
  pinMode(1, OUTPUT);
  TinyWireM.begin();
  TinyWireM.beginTransmission(mpu); 
  TinyWireM.write(0x6B); //  Power setting address
  TinyWireM.write(0b00000000); // Disable sleep mode (just in case)
  TinyWireM.endTransmission();
  TinyWireM.beginTransmission(mpu); 
  TinyWireM.write(0x1B); // Config register for Gyro
  TinyWireM.write(0x00000000); // 250° per second range (default)
  TinyWireM.endTransmission();
  TinyWireM.beginTransmission(mpu); //I2C address of the MPU
  TinyWireM.write(0x1C); // Accelerometer config register
  TinyWireM.write(0b00000000); // 2g range +/- (default)
  TinyWireM.endTransmission();
}
void loop()
{
  TinyWireM.beginTransmission(mpu);
  TinyWireM.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  TinyWireM.endTransmission(false);
  //Solicita os dados do sensor
  TinyWireM.requestFrom(mpu,6);  
  //Armazena o valor dos sensores nas variaveis correspondentes
  AcX=TinyWireM.read()<<8|TinyWireM.read();  //0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)     
  AcY=TinyWireM.read()<<8|TinyWireM.read();  //0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=TinyWireM.read()<<8|TinyWireM.read();  //0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L) 
 
  if( AcZ < 3000){ // incorrect posture
    timeInIncorrectPosture++; 
  } else {
    timeInIncorrectPosture =0;
  }
  if(timeInIncorrectPosture >= 10){ // 5 seconds in incorrect posture
    digitalWrite(1, HIGH);
    delay(1000);
    digitalWrite(1, LOW);
    timeInIncorrectPosture = 0;    
  }    
   
  delay(500);
}
