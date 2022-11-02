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

const int second = 1000;
const int halfSecond = 500;
const int quarterSecond = 250;

const int stateTimer1 = 8 * second;
const int stateTimer2 = stateTimer1 + 3 * second;
const int stateTimer3 = stateTimer2 + 8 * second;
const int stateTimer4 = stateTimer3 + 4 * second;

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

        if (millis() % second < halfSecond)
            tone(buzzer, NOTE_A4, 100);
        else
            noTone(buzzer);

        if (millis() - lastReading > stateTimer3)
            state = 4;

        break;
    case 4:
        digitalWrite(redCarLight, HIGH);
        if (millis() % second < halfSecond)
            digitalWrite(greenPedLight, LOW);
        else
            digitalWrite(greenPedLight, HIGH);

        if (millis() % halfSecond < quarterSecond)
            tone(buzzer, NOTE_A4, 100);
        else
            noTone(buzzer);

        if (millis() - lastReading > stateTimer4)
        {
            state = 1;
            // lastReading = millis();
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