//include required libraries
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <DHT.h>

#include "config.h"
#include "SensorManager.h"
#include "actuator.h"
#include"network.h"
#include"telemetry.h"
#include"RPC.h"
static char sharedBuf[200];

static EthernetClient ethClient;
static PubSubClient mqttClient(ethClient);
unsigned long lastTelemetry=0;

#define PIN_LED_RED 4

void setup() 
{
  pinMode(PIN_LED_RED, OUTPUT);
  digitalWrite(PIN_LED_RED, HIGH); // OFF (Active LOW)

  Serial.begin(9600);
  Serial.print("Node1 is booting...");

  sensor_begin();
  actuators_begin();

  telemetry_init(sharedBuf, sizeof(sharedBuf));
  rpc_init(&mqttClient, sharedBuf, sizeof(sharedBuf));

  mqttClient.setCallback(rpc_mqttCallback);
  network_begin(&mqttClient);
}

void loop() 
{
  network_maintain();

//  REQUIRED: process MQTT messages
  mqttClient.loop();

  SensorData data;
  sensors_read(&data);

  if(network_isConnected())
  {
    unsigned long now = millis();
    if (now - lastTelemetry >= TELEMETRY_INTERVAL)
    {
      lastTelemetry = now;
      telemetry_publishTelemetry(&data, actuators_getRelayState);
    }
    actuators_updateStatusLEDs(network_isConnected(),data.sensorError);
  }
}