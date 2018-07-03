#include <TheThingsNetwork.h>
#include <Wire.h>
#include <math.h>
#include <Adafruit_MLX90614.h>

Adafruit_MLX90614 mlx = Adafruit_MLX90614(); 

const char *appEui = "70B3D57ED001042F";
const char *appKey = "13215CA3CA18A4327A2B68471F7C6601";
    
#define loraSerial Serial1
#define debugSerial Serial
#define freqPlan TTN_FP_EU868
#define LED_GREEN 4
TheThingsNetwork ttn(loraSerial, debugSerial, freqPlan);
    
void setup() {
  loraSerial.begin(57600);
  debugSerial.begin(9600);

  pinMode(LED_GREEN,OUTPUT);
    
  while (!debugSerial && millis() < 10000);

  debugSerial.println("Connecting To 'The Things Network'");
  ttn.join(appEui, appKey); 

  ttn.onMessage(message);

  mlx.begin();
}

void loop() {

  int objTemp = 00; //codes for ambient and object
  int ambTemp = 01;
  
  float tempReading = mlx.readObjectTempC();
  int integerPart = (int)tempReading;
  int decimalPart = ((tempReading - (float)integerPart)*100)+1;

  byte object[3];
  object[0] = objTemp;
  object[1] = integerPart;
  object[2] = decimalPart;

  ttn.sendBytes(object, sizeof(object)); //sending object temperature

  delay(5000);

  tempReading = mlx.readAmbientTempC();
  integerPart = (int)tempReading;
  decimalPart = ((tempReading - (float)integerPart)*100)+1;

  byte ambient[3];
  ambient[0] = ambTemp;
  ambient[1] = integerPart;
  ambient[2] = decimalPart;
  
  ttn.sendBytes(ambient, sizeof(ambient)); //sending ambient temperature 

  delay(5000);
}

void message(const uint8_t *payload, size_t size, port_t port)
{
    if(payload[0] == 00){
       digitalWrite(LED_GREEN,LOW);
    } 
    if(payload[0] == 01){
      digitalWrite(LED_GREEN,HIGH);  
    }
}
