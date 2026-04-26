#include"actuator.h"
 static uint8_t relay_status;
static uint8_t Blinkstate = 0;
static unsigned long lastBlinkTime = 0;

void  actuators_begin(void)
{
  pinMode(PIN_LED_GREEN , OUTPUT);
  pinMode(PIN_LED_YELLOW , OUTPUT);
  pinMode(PIN_LED_RED , OUTPUT);
  pinMode(PIN_RELAY, OUTPUT);
  
  digitalWrite(PIN_LED_GREEN,LOW);
  digitalWrite(PIN_LED_YELLOW,LOW);
  digitalWrite(PIN_RELAY,LOW);
}


void    actuators_updateStatusLEDs(uint8_t mqttConnected, uint8_t sensorError)
{
    if (!mqttConnected || sensorError)
    {
        digitalWrite(PIN_LED_RED, HIGH);
    }
    else
    {
        digitalWrite(PIN_LED_GREEN, HIGH);
    }
}

uint8_t actuators_getRelayState(void);

void actuators_setRelay(uint8_t state)

{
    digitalWrite(PIN_RELAY, state ? HIGH : LOW);
    return relay_status;
}

uint8_t actuators_getRelayState(void);

static uint8_t relayState = 0;   // global/internal state

uint8_t actuators_getRelayState()
{
    return relayState;
}