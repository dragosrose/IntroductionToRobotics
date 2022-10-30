const int buttonPin = 5;
const int ledPin = 7;

int buttonPressCount = 0;
byte lastButtonState = LOW;

unsigned long lastReading;
unsigned int debounceTime = 50;

void setup()
{
    Serial.begin(9600);
    pinMode(buttonPin, INPUT_PULLUP);
    pinMode(ledPin, OUTPUT);
}

void loop()
{
    // Count button presses
    if (digitalRead(buttonPin) == LOW && lastButtonState == HIGH && millis() - lastReading > debounceTime)
    {
        buttonPressCount++;
        lastReading = millis();
    }
    lastButtonState = digitalRead(buttonPin);

    byte buttonIsOn = !lastButtonState;

    // // just pressed button
    // if (buttonState != lastButtonState)
    // {
    //     lastButtonState = buttonState;
    //     lastReading = millis();
    // }

    // if (millis() - lastReading >= debounceTime)
    // {
    //     // pressed for 50 ms
    //     if (buttonState == LOW)
    //     {
    //         buttonPressCount++;
    //     }

    //     // released for 50 ms
    //     else
    //     {
    //         Serial.print("Button was pressed ");
    //         Serial.print(buttonPressCount);
    //         Serial.println(" times.");
    //     }
    // }

    digitalWrite(ledPin, buttonIsOn);
    Serial.println(buttonPressCount);
    delay(1);
}