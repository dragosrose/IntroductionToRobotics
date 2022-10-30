const int redPotPin = A0;
const int greenPotPin = A1;
const int bluePotPin = A2;

const int redLedPin = 9;
const int greenLedPin = 10;
const int blueLedPin = 11;

const int minAnalogReadValue = 0;
const int maxAnalogReadValue = 1023;

const int minAnalogWriteValue = 0;
const int maxAnalogWriteValue = 255;

struct Color
{
    int red, green, blue;
} pot, led;

void setup()
{
    pinMode(redLedPin, OUTPUT);
    pinMode(greenLedPin, OUTPUT);
    pinMode(blueLedPin, OUTPUT);

    Serial.begin(9600);
}

long mapping(long potValue)
{
    return map(potValue, minAnalogReadValue, maxAnalogReadValue, minAnalogWriteValue, maxAnalogWriteValue);
}

void readAnalogValues()
{
    pot.red = analogRead(redPotPin);
    pot.green = analogRead(greenPotPin);
    pot.blue = analogRead(bluePotPin);
}

void digitizeValues()
{
    led.red = mapping(pot.red);
    led.green = mapping(pot.green);
    led.blue = mapping(pot.blue);
}

void writeDigitizedValues()
{
    analogWrite(redLedPin, led.red);
    analogWrite(greenLedPin, led.green);
    analogWrite(blueLedPin, led.blue);
}

void loop()
{
    readAnalogValues();
    digitizeValues();
    writeDigitizedValues();
}