
#include <AcceleroMMA7361.h>

AcceleroMMA7361 accelero;
int x;
int y;
int z;

void setup()
{

  Serial.begin(9600);
  //accelero.begin(13, 12, 11, 10, A0, A1, A2);
  accelero.begin(9, 8, 7, 6, A0, A1, A2);
  accelero.setARefVoltage(3.3);  //sets the AREF voltage to 3.3V
  accelero.setSensitivity(HIGH);  //sets the sensitivity to +/-6G
  accelero.calibrate();

}

void loop()
{
  x = accelero.getXRaw();
  
  y = accelero.getYRaw();
  
  z = accelero.getZRaw();
  
  Serial.print("nx: ");
  Serial.print(x);
  Serial.print(" ty: ");
  Serial.print(y);
  Serial.print(" tz: ");
  Serial.println(z);
  delay(500);                                   
}
