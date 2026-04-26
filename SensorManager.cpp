#include "SensorManager.h"
#include "config.h"
#include <DHT.h>

static DHT dht(PIN_DHT, DHT_TYPE);

void sensor_init  (SensorManager* sm)
{

  sm->lastTemp = 0.0f ;
  sm->lastHumidity= 0.0f ;
  sm->motionActive = false;
   sm->motionClearTime = 0;
  
}

void sensor_begin(void) 
{

  pinMode(PIN_PIR, INPUT);
  pinMode(PIN_DOOR,INPUT_PULLUP);


  dht.begin();
}

 static bool readPIR(SensorManager*sm)
 {
   
   const bool pirHigh = (digitalRead(PIN_PIR) ==HIGH);
   const unsigned long now = millis();
   if(pirHigh)
   {
    sm->motionActive = true;
    sm->motionClearTime = now + MOTION_CLEAR_MS;
   }
  else if (sm->motionActive && (now>sm->motionClearTime))
  {
    sm->motionActive = false;

  }
  return sm->motionActive;
 }

 static bool readDoor(void)
 {

 return(digitalRead(PIN_DOOR) == LOW);

 }

static uint16_t readLDR(void)
{
  uint16_t sum = 0;
  uint8_t i;
  for(i = 0; i<4; i++)
  {

  sum += (uint16_t)analogRead(PIN_LDR);
  delay(2); 
}
 return sum/4;

}

bool sensor_read(SensorData* out,SensorManager* sm)
 {
  float t = dht.readTemperature();
  float h = dht.readHumidity();

if (isnan(t) || isnan(h))
  {
    out->sensorError = 1;
    out->temperature = sm->lastTemp;
    out->humidity = sm->lastHumidity;
  }
else
  {
    out->sensorError = 0;
    out->temperature = round(t * 10.0f) * 0.1f;
    out->humidity = round(h * 10.0f) * 0.1f;
    sm->lastTemp = out->temperature;
    sm->lastHumidity = out->humidity;
  }
  out->motionDetected = readPIR(sm);
 out->doorOpen = readDoor();
 out->ldrValue = readLDR();
 out->isDark = (out->ldrValue < LDR_DARK_THRESHOLD);
  return !out->sensorError;
 }
  