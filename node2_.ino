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
#include "rpc.h"

static char sharedBuf[200];

static EthernetClient ethClient;
static PubSubClient mqttClient(ethClient);
unsigned long lastTelemetry=0;

static SensorManager sensors;

void setup() 
{
  Serial.begin(9600);
  Serial.print("Node2 is booting...");

  // put your setup code here, to run once:
   // initilase sensor 
   sensor_begin();
   sensor_init(&sensors);
   //initialise actutors
   actuators_begin();

   telemetry_init(sharedBuf, sizeof(sharedBuf));
   rpc_init(&mqttClient, sharedBuf, sizeof(sharedBuf));
   // coonect board to internet and mqtt
   network_begin(&mqttClient);
   


}

void loop() 
{
  // put your main code here, to run repeatedly:
   network_maintain();
  //read data from the sensors 
    SensorData data;
    sensor_read(&data, &sensors);

  if(data.humidity>=HUMIDITY_CRIT && !actuators_getRelayState )
  {

    actuators_setRelay("true");


  }


  //publish to the cloud 
  if(network_isConnected())
  {
     unsigned long now = millis();
     if (now-lastTelemetry>=TELEMETRY_INTERVAL)
     {
      lastTelemetry=now;
      telemetry_publishTelemetry(&data, actuators_getRelayState);

     }
     
   actuators_updateStatusLEDs(network_isConnected(), data.sensorError);
  }
} 
  