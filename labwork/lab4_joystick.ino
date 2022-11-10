const int joyXPin = A0;
const int joyYPin = A1;
const int joySWPin = 12;

bool swState;
int xValue, yValue;

void setup()
{
    pinMode(joySWPin, INPUT_PULLUP);
    Serial.begin(9600);
}

void loop()
{
    swState = digitalRead(joySWPin);
    xValue = analogRead(joyXPin);
    yValue = analogRead(joyYPin);

    Serial.print("Switch: ");
    Serial.print(swState);
    Serial.print("  |   ");
    Serial.print("X: ");
    Serial.print(xValue);
    Serial.print(" | ");
    Serial.print("Y: ");
    Serial.println(yValue);
}