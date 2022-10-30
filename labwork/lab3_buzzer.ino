const int buzzerPin = 5;

void setup()
{
    tone(buzzerPin, 1000);
    delay(500);
    noTone(buzzerPin);
}

void loop()
{
}