#include "utils/pitches.h"

const int buttonPin = 2;
unsigned long lastReading = 0;
int debounceTime = 50;

const int greenPedLight = 3;
const int redPedLight = 4;

const int greenCarLight = 5;
const int yellowCarLight = 6;
const int redCarLight = 7;

const int buzzer = 13;

byte state = 1;

const int stateTimer1 = 8000;
const int stateTimer2 = stateTimer1 + 3000;
const int stateTimer3 = stateTimer2 + 8000;
const int stateTimer4 = stateTimer3 + 4000;

void setup()
{

    pinMode(buttonPin, INPUT_PULLUP);

    pinMode(greenPedLight, OUTPUT);
    pinMode(redPedLight, OUTPUT);
    pinMode(greenCarLight, OUTPUT);

    pinMode(yellowCarLight, OUTPUT);
    pinMode(redCarLight, OUTPUT);

    pinMode(buzzer, OUTPUT);

    Serial.begin(9600);
}

void loop()
{
    switch (state)
    {
    case 1:
        digitalWrite(greenPedLight, LOW);
        digitalWrite(redCarLight, LOW);
        digitalWrite(redPedLight, HIGH);
        digitalWrite(greenCarLight, HIGH);

        break;
    case 2:
        if (millis() - lastReading > stateTimer1)
        {
            digitalWrite(greenCarLight, LOW);
            digitalWrite(yellowCarLight, HIGH);

            if (millis() - lastReading > stateTimer2)
                state = 3;
        }

        break;
    case 3:
        digitalWrite(yellowCarLight, LOW);
        digitalWrite(redCarLight, HIGH);
        digitalWrite(redPedLight, LOW);
        digitalWrite(greenPedLight, HIGH);

        if (millis() % 1000 < 500)
            tone(buzzer, NOTE_A4, 100);
        else
            noTone(buzzer);

        if (millis() - lastReading > stateTimer3)
            state = 4;

        break;
    case 4:
        digitalWrite(redCarLight, HIGH);
        if (millis() % 1000 < 500)
            digitalWrite(greenPedLight, LOW);
        else
            digitalWrite(greenPedLight, HIGH);

        if (millis() % 500 < 250)
            tone(buzzer, NOTE_A4, 100);
        else
            noTone(buzzer);

        if (millis() - lastReading > stateTimer4)
        {
            state = 1;
            lastReading = millis();
        }

        break;

    default:
        break;
    }

    attachInterrupt(digitalPinToInterrupt(buttonPin), buttonPressed, FALLING);
}

void buttonPressed()
{
    if (millis() - lastReading > debounceTime && state == 1)
    {
        lastReading = millis();
        state = 2;
    }
}