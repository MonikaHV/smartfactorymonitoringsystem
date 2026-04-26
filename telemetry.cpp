#include "telemetry.h"
#include "network.h"
#include <stdlib.h>

static char* buf = NULL;
static uint8_t len = 0;

static char* f1(float v, char* dst)
{
  return dtostrf(v, 1, 1, dst);
}

void telemetry_init(char* buffer, uint8_t bufLen)
{
  buf = buffer;    
  len = bufLen;
}

void telemetry_publishTelemetry(const SensorData* data, uint8_t relayState)
{
  char t[8], h[8];

  f1(data->temperature, t);
  f1(data->humidity, h);
  

  snprintf(buf, len,
    "{\"temperature\":%s,\"humidity\":%s,\"ldrValue\":%u,"
    "\"isDark\":%u,\"motionDetected\":%u,\"relayState\":%u,"
    "\"doorOpen\":%u,\"sensorError\":%u}",
    t, h,
    (unsigned)data->ldrValue,
    (unsigned)data->isDark,
    (unsigned)data->motionDetected,
    (unsigned)relayState,
    (unsigned)data->doorOpen,
    (unsigned)data->sensorError
  );

  Serial.println(buf);   

  network_publish(TOPIC_TELEMETRY, buf, 0);
}
