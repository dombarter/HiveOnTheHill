#include <TheThingsNetwork.h>
#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <CayenneLPP.h>

Adafruit_MLX90614 mlx = Adafruit_MLX90614(); 

const char *appEui = "70B3D57ED0010496";
const char *appKey = "A6D801D81A69E4F706109071BC7D0D13";
    
#define loraSerial Serial1
#define debugSerial Serial
#define freqPlan TTN_FP_EU868
TheThingsNetwork ttn(loraSerial, debugSerial, freqPlan);
CayenneLPP lpp(51);
    
void setup() {
  loraSerial.begin(57600);
  debugSerial.begin(9600);   
  while (!debugSerial && millis() < 10000);
  debugSerial.println("Connecting To 'The Things Network'");
  ttn.join(appEui, appKey); 
  mlx.begin();
}

void loop() { 

  lpp.reset();
  lpp.addTemperature(1,mlx.readObjectTempC());
  lpp.addTemperature(2,mlx.readAmbientTempC());
  
  ttn.sendBytes(lpp.getBuffer(), lpp.getSize());

  delay(10000);
}
