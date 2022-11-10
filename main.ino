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

int lastJoySWReading = 0;

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

int currentPin = 7;
int currentDirection = 0;
int index = 11;

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

    // Read joystick axis xValue
    int yValue = analogRead(joyYPin);
    int xValue = analogRead(joyXPin);

    // Write a program that moves through each segment of the display by joystick yValue and lights it for 0.5 seconds and then turns it off for 0.5 seconds
    if (yValue < joyLeftThreshold && joyIsNeutral)
    {
        // LEFT
        setSegment(index, LOW);
        currentDirection = 2;
        index = segmentPath[currentPin][currentDirection];
        Serial.print("LEFT: ");
        Serial.print(currentPin + 4);

        if (index == -1)
        {
            index = currentPin + 4;
        }
        else
        {
            currentPin = index - 4;
        }

        Serial.print(" -> ");
        Serial.println(index);
        joyIsNeutral = false;
    }
    else if (yValue > joyRightThreshold && joyIsNeutral)
    {
        setSegment(index, LOW);
        currentDirection = 3;
        Serial.print("RIGHT: ");
        Serial.print(currentPin + 4);

        index = segmentPath[currentPin][currentDirection];
        if (index == -1)
        {
            index = currentPin + 4;
        }
        else
        {
            currentPin = index - 4;
        }

        Serial.print(" -> ");
        Serial.println(index);

        joyIsNeutral = false;
    }

    else if (xValue > joyUpThreshold && joyIsNeutral)
    {
        setSegment(index, LOW);
        currentDirection = 0;
        Serial.print("UP: ");
        Serial.print(currentPin + 4);

        index = segmentPath[currentPin][currentDirection];
        if (index == -1)
        {
            index = currentPin + 4;
        }
        else
        {
            currentPin = index - 4;
        }

        Serial.print(" -> ");
        Serial.println(index);

        joyIsNeutral = false;
    }
    else if (xValue < joyDownThreshold && joyIsNeutral)
    {
        setSegment(index, LOW);
        currentDirection = 1;
        Serial.print("DOWN: ");
        Serial.print(currentPin + 4);

        index = segmentPath[currentPin][currentDirection];
        if (index == -1)
        {
            index = currentPin + 4;
        }
        else
        {
            currentPin = index - 4;
        }

        Serial.print(" -> ");
        Serial.println(index);

        joyIsNeutral = false;
    }
    else if (yValue > joyLeftThreshold && yValue < joyRightThreshold && xValue < joyUpThreshold && xValue > joyDownThreshold)
    {
        if (millis() % 1000 < 500)
        {
            segments[index - 4] = segmentOn;
        }
        else
        {
            segments[index - 4] = !segmentOn;
        }
        joyIsNeutral = true;
    }

    // Serial.println(xValue);

    // Serial.println(index);
    setSegment(index, segments[index - 4]);

    // attachInterrupt(digitalPinToInterrupt(joySWPin), joySWPressed, FALLING);
}

void setSegment(byte segment, bool state)
{
    digitalWrite(segmentPins[segment - 4], state ^ commonAnode);
}

void joySWPressed()
{
    Serial.println("SW Pressed");
    if (millis() - lastJoySWReading > joyShortDebounce)
    {
        lastJoySWReading = millis();
        if (millis() - lastJoySWReading > joyLongDebounce)
        {
            for (int i = 0; i < segSize; i++)
            {
                setSegment(i, LOW);
            }
        }
        else
        {
            segments[index - 4] = !segments[index - 4];
            setSegment(index, segments[index - 4]);
        }
    }
}
