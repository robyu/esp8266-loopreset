#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <assert.h>
#include <limits.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 16
#endif

typedef struct
{
    int led_state;
    long event_time_ms;
} state_t;
state_t state;

/*
on_off is 1 or 0
not HIGH or LOW
*/
void utils_set_led(int led_pin, int on_off)
{
    if (led_pin < 0)
    {
        return;
    }
    if (on_off==1)
    {
        digitalWrite(led_pin, LOW);
    }
    else
    {
        digitalWrite(led_pin, HIGH);
    }
}


long utils_get_elapsed_msec_and_reset(long* pevent_time_ms)
{
    long elapsed_ms;
    elapsed_ms = millis() - *pevent_time_ms;
    if (elapsed_ms < 0)
    {
        elapsed_ms = LONG_MAX;
        *pevent_time_ms = millis(); // reset
    }
}

void setup()
{
    Serial.begin(115200);
    delay(10); // need this?
    pinMode(LED_BUILTIN, OUTPUT);
    state.event_time_ms = millis();
    Serial.println("setup complete");
    Serial.flush();
}

void loop()
{
    long elapsed_ms;
    elapsed_ms = utils_get_elapsed_msec_and_reset(&state.event_time_ms);
    if (elapsed_ms > 2000)
    {
        Serial.println("restarting");
        Serial.flush();
        delay(500);
        ESP.restart();
        delay(500);
    }
    else
    {
        state.led_state ^= 1;
        utils_set_led(LED_BUILTIN, state.led_state);
        delay(500);
    }
}

