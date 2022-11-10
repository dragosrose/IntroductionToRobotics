const int pinA = 4;
const int pinB = 5;
const int pinC = 6;
const int pinD = 7;
const int pinE = 8;
const int pinF = 9;
const int pinG = 10;
const int pinDP = 11;

const int joyXPin = A0;
const int joyYPin = A1;
const int joySWPin = 12;

const int joyLeftThreshold = 400;
const int joyRightThreshold = 600;

int currentDigit = 0;

const int segSize = 8;
int index = 0;

bool commonAnode = false;

bool joyIsNeutral = false;
bool joySWState = HIGH;
bool joyPrevSWState = HIGH;
bool dpState = false;

byte segmentOn = HIGH;

int segments[segSize] = {pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP};

const int numberOfDigits = 10;
// bool digitMatrix[numberOfDigits][segSize - 1] = {
//     {true, true, true, true, true, true, false},     // 0
//     {false, true, true, false, false, false, false}, // 1
//     {true, true, false, true, true, false, true},    // 2
//     {true, true, true, true, false, false, true},    // 3
//     {false, true, true, false, false, true, true},   // 4
//     {true, false, true, true, false, true, true},    // 5
//     {true, false, true, true, true, true, true},     // 6
//     {true, true, true, false, false, false, false},  // 7
//     {true, true, true, true, true, true, true},      // 8
//     {true, true, true, true, false, true, true}      // 9
// };

byte digitMatrix[numberOfDigits][segSize - 1] = {
    {1, 1, 1, 1, 1, 1, 0}, // 0
    {0, 1, 1, 0, 0, 0, 0}, // 1
    {1, 1, 0, 1, 1, 0, 1}, // 2
    {1, 1, 1, 1, 0, 0, 1}, // 3
    {0, 1, 1, 0, 0, 1, 1}, // 4
    {1, 0, 1, 1, 0, 1, 1}, // 5
    {1, 0, 1, 1, 1, 1, 1}, // 6
    {1, 1, 1, 0, 0, 0, 0}, // 7
    {1, 1, 1, 1, 1, 1, 1}, // 8
    {1, 1, 1, 1, 0, 1, 1}  // 9
};

void setup()
{
    for (int i = 0; i < segSize; i++)
    {
        pinMode(segments[i], OUTPUT);
    }
    pinMode(joySWPin, INPUT_PULLUP);
    if (commonAnode)
    {
        segmentOn = !segmentOn;
    }
}

void loop()
{
    // for (int i = 0; i < numberOfDigits; i++)
    // {
    //     setDigit(i);
    //     delay(1000);
    // }
    // digitalWrite(pinDP, segmentOn ^ commonAnode);

    int joyY = analogRead(joyYPin);
    if (joyY < joyLeftThreshold && joyIsNeutral)
    {
        currentDigit = constrain(currentDigit - 1, 0, 9);
        joyIsNeutral = false;
    }

    if (joyY > joyRightThreshold && joyIsNeutral)
    {
        currentDigit = constrain(currentDigit + 1, 0, 9);
        joyIsNeutral = false;
    }
    if (joyLeftThreshold < joyY && joyY < joyRightThreshold)
    {
        joyIsNeutral = true;
    }

    joySWState = digitalRead(joySWPin);
    if (joySWState != joyPrevSWState)
    {
        if (joySWState == LOW)
        {
            dpState = !dpState;
        }
    }
    joyPrevSWState = joySWState;

    setDigit(currentDigit, dpState);
}

void setDigit(byte digit, bool dpState)
{
    for (int i = 0; i < segSize - 1; i++)
    {
        digitalWrite(segments[i], digitMatrix[digit][i] ^ commonAnode);
    }

    digitalWrite(pinDP, dpState ^ commonAnode);
}