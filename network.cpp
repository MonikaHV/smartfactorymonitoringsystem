#include"network.h"

static PubSubClient* mqtt =   NULL;
static byte   mac[6] = NODE_MAC;
static unsigned lastReconnectTime = 0;

static void doConnect(void)
{
  Serial.println("Trying MQTT...");

  if (mqtt->connect(CLIENT_ID, ACCESS_TOKEN, NULL))
  {
    Serial.println("MQTT Connected");
  }
  else
  {
    Serial.print("MQTT Failed , State: ");
    Serial.println(mqtt->state());
  }
}


void    network_begin(PubSubClient* mqttClient)
{

 mqtt =  mqttClient;
 //connect board to the internet;
 Ethernet.begin(mac);
 delay(2000);
 //connect to things board
 mqtt->setServer(TB_SERVER,TB_PORT);
 doConnect();

}

void network_maintain(void)
{
  Ethernet.maintain();

  if (mqtt->connected())
  {
    mqtt->loop();
  }
  else
  {
    unsigned long now = millis();
    if (now - lastReconnectTime >= RECONNECT_INTERVAL)
    {
      lastReconnectTime = now;
      doConnect();
    }
  }
}

 uint8_t network_isConnected(void)

{

  return (uint8_t) mqtt->connected();

}

uint8_t network_publish(const char* topic, const char* payload, uint8_t retain)

{

  return(uint8_t) mqtt->publish(topic, payload , retain);

}
