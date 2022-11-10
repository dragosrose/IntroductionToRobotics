const int pinA = 4;
const int pinB = 5;
const int pinC = 6;
const int pinD = 7;
const int pinE = 8;
const int pinF = 9;
const int pinG = 10;
const int pinDP = 11;

int state = 1;

const int joyXPin = A0;
const int joyYPin = A1;
const int joySWPin = 3;

const int joyLeftThreshold = 400;
const int joyRightThreshold = 600;
const int joyUpThreshold = 900;
const int joyDownThreshold = 300;

const int joyShortDebounce = 100;
const int joyLongDebounce = 500;

unsigned long pressedJoySWTime = 0;
unsigned long releasedJoySWTime = 0;

int lastJoySWState = LOW;
int joySWState;

const int segSize = 8;

bool commonAnode = false;

bool joyIsNeutral = false;

byte segmentOn = HIGH;

const int numOfDirections = 4;
const int segmentPins[segSize] = {pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP};
const int segmentPath[segSize][numOfDirections] = {
    // UP, DOWN, LEFT, RIGHT
    {-1, pinG, pinF, pinB},
    {pinA, pinG, pinF, -1},
    {pinG, pinD, pinE, pinDP},
    {pinG, -1, pinE, pinC},
    {pinG, pinD, -1, pinC},
    {pinA, pinG, -1, pinB},
    {pinA, pinD, -1, -1},
    {-1, -1, pinC, -1},

};

const int pinOffset = 4;
// So I can use both the index in the segmentPins array and the index in the segmentPath array

int currentPin = 7;
// DP in the segmentPath array

int currentDirection = 0;
int index = 11;
// DP in the segmentPins array
int programState = 1;

byte segments[segSize] = {0, 0, 0, 0, 0, 0, 0, 0};

void setup()
{
    for (int i = 0; i < segSize; i++)
    {
        pinMode(segmentPins[i], OUTPUT);
    }
    pinMode(joySWPin, INPUT_PULLUP);
    if (commonAnode)
    {
        segmentOn = !segmentOn;
    }

    Serial.begin(9600);
}

void loop()
{
    displaySegments();
    joySWPressed();
    // Read joystick axis xValue
    int yValue = analogRead(joyYPin);
    int xValue = analogRead(joyXPin);

    if (programState == 1)
        canvasMovement(xValue, yValue);
    else
        lockState(xValue, yValue);
}

void setSegments(int pin, bool state)
{
    segments[pin - pinOffset] = state;
}

void displaySegments()
{
    for (int i = 0; i < segSize; i++)
    {
        digitalWrite(segmentPins[i], segments[i] ^ commonAnode);
    }
}

void displaySegmentByIndex(byte segment, bool state)
{
    digitalWrite(segmentPins[segment - pinOffset], state ^ commonAnode);
}

void lockState(int xValue, int yValue)
{
    if (yValue < joyLeftThreshold && joyIsNeutral)
    {
        setSegments(index, LOW);
        joyIsNeutral = false;
    }
    else if (yValue > joyRightThreshold && joyIsNeutral)
    {
        setSegments(index, HIGH);
        joyIsNeutral = false;
    }
    else if (yValue > joyLeftThreshold && yValue < joyRightThreshold)
    {
        displaySegmentByIndex(index, segments[index - pinOffset]);
        joyIsNeutral = true;
    }
}

void canvasMovement(int xValue, int yValue)
{
    if (yValue < joyLeftThreshold && joyIsNeutral)
    {
        // LEFT
        currentDirection = 2;
        index = segmentPath[currentPin][currentDirection];
        Serial.print("LEFT: ");
        Serial.print(currentPin + pinOffset);

        if (index == -1)
        {
            index = currentPin + pinOffset;
        }
        else
        {
            currentPin = index - pinOffset;
        }

        Serial.print(" -> ");
        Serial.println(index);
        joyIsNeutral = false;
    }
    else if (yValue > joyRightThreshold && joyIsNeutral)
    {
        currentDirection = 3;
        Serial.print("RIGHT: ");
        Serial.print(currentPin + pinOffset);

        index = segmentPath[currentPin][currentDirection];
        if (index == -1)
        {
            index = currentPin + pinOffset;
        }
        else
        {
            currentPin = index - pinOffset;
        }

        Serial.print(" -> ");
        Serial.println(index);

        joyIsNeutral = false;
    }

    else if (xValue > joyUpThreshold && joyIsNeutral)
    {
        currentDirection = 0;
        Serial.print("UP: ");
        Serial.print(currentPin + pinOffset);

        index = segmentPath[currentPin][currentDirection];
        if (index == -1)
        {
            index = currentPin + pinOffset;
        }
        else
        {
            currentPin = index - pinOffset;
        }

        Serial.print(" -> ");
        Serial.println(index);

        joyIsNeutral = false;
    }
    else if (xValue < joyDownThreshold && joyIsNeutral)
    {
        currentDirection = 1;
        Serial.print("DOWN: ");
        Serial.print(currentPin + pinOffset);

        index = segmentPath[currentPin][currentDirection];
        if (index == -1)
        {
            index = currentPin + pinOffset;
        }
        else
        {
            currentPin = index - pinOffset;
        }

        Serial.print(" -> ");
        Serial.println(index);

        joyIsNeutral = false;
    }
    else if (yValue > joyLeftThreshold && yValue < joyRightThreshold && xValue < joyUpThreshold && xValue > joyDownThreshold)
    {
        if (millis() % 1000 < 500)
        {
            displaySegmentByIndex(index, HIGH);
        }
        else
        {
            displaySegmentByIndex(index, LOW);
        }
        joyIsNeutral = true;
    }
}

void joySWPressed()
{
    joySWState = digitalRead(joySWPin);

    if (lastJoySWState == HIGH && joySWState == LOW)
    {
        pressedJoySWTime = millis();
    }
    else if (lastJoySWState == LOW && joySWState == HIGH)
    {
        releasedJoySWTime = millis();

        long pressDuration = releasedJoySWTime - pressedJoySWTime;

        if (pressDuration > joyShortDebounce)
        {

            if (pressDuration > joyLongDebounce)
            {
                Serial.println("Long Press");
                for (int i = pinOffset; i < segSize + pinOffset; i++)
                {
                    setSegments(i, LOW);
                }
                programState = 1;
                currentPin = 7;
                currentDirection = 0;
                index = 11;
            }
            else
            {
                Serial.println("Short Press");
                programState == 1 ? programState = 2 : programState = 1;
            }
        }
    }

    lastJoySWState = joySWState;
}
