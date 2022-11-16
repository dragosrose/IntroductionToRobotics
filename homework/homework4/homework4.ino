// DS= [D]ata [S]torage - data
// STCP= [ST]orage [C]lock [P]in latch
// SHCP= [SH]ift register [C]lock [P]in clock

const int latchPin = 11; // STCP to 12 on Shift Register
const int clockPin = 10; // SHCP to 11 on Shift Register
const int dataPin = 12;  // DS to 14 on Shift Register

const int segD1 = 7;
const int segD2 = 6;
const int segD3 = 5;
const int segD4 = 4;

const int joyXPin = A0;
const int joyYPin = A1;
const int joySWPin = 3;

const int joyLeftThreshold = 400;
const int joyRightThreshold = 600;
const int joyUpThreshold = 900;
const int joyDownThreshold = 300;

unsigned long pressedJoySWTime = 0;
unsigned long releasedJoySWTime = 0;

const int joyShortDebounce = 100;
const int joyLongDebounce = 500;

bool joyIsNeutral = false;

int lastJoySWState = LOW;
int joySWState;

bool selected = false;

const byte regSize = 8; // 1 byte aka 8 bits

int displayDigits[] = {
    segD1, segD2, segD3, segD4};

const int displayCount = 4;
const int encodingsNumber = 16;

const int blinkDelay = 250;

int number;
const int dpByteEncoding = 0b00000001;

int byteEncodings[encodingsNumber] = {
    // A B C D E F G DP
    B11111100, // 0
    B01100000, // 1
    B11011010, // 2
    B11110010, // 3
    B01100110, // 4
    B10110110, // 5
    B10111110, // 6
    B11100000, // 7
    B11111110, // 8
    B11110110, // 9
    B11101110, // A
    B00111110, // b
    B10011100, // C
    B01111010, // d
    B10011110, // E
    B10001110  // F
};

int digits[displayCount] = {0, 0, 0, 0};
int currentDisplay = 0;

void setup()
{
    // put your setup code here, to run once:
    pinMode(latchPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    pinMode(dataPin, OUTPUT);

    for (int i = 0; i < displayCount; i++)
    {
        pinMode(displayDigits[i], OUTPUT);
        digitalWrite(displayDigits[i], LOW);
    }
    pinMode(joySWPin, INPUT_PULLUP);
    Serial.begin(9600);
}

void loop()
{
    ReadFromJoyStick();
    writeNumber(currentDisplay);
}

void ReadFromJoyStick()
{
    joySWPressed();

    int joyX = analogRead(joyXPin);
    int joyY = analogRead(joyYPin);

    if (joyY < joyLeftThreshold && joyIsNeutral && selected == false)
    {
        currentDisplay++;
        currentDisplay %= displayCount;
        joyIsNeutral = false;
        Serial.println("Left");
    }
    else if (joyY > joyRightThreshold && joyIsNeutral && selected == false)
    {
        currentDisplay--;
        currentDisplay = (displayCount + (currentDisplay % displayCount)) % displayCount;
        joyIsNeutral = false;
        Serial.println("Right");
    }
    else if (joyX > joyUpThreshold && joyIsNeutral && selected == true)
    {
        digits[currentDisplay]++;
        digits[currentDisplay] %= encodingsNumber;
        joyIsNeutral = false;
        Serial.println("Up");
    }
    else if (joyX < joyDownThreshold && joyIsNeutral && selected == true)
    {
        digits[currentDisplay]--;
        digits[currentDisplay] = (encodingsNumber + (digits[currentDisplay] % encodingsNumber)) % encodingsNumber;
        joyIsNeutral = false;
        Serial.println("Down");
    }
    else if (joyX >= joyDownThreshold && joyX < joyUpThreshold && joyY >= joyLeftThreshold && joyY < joyRightThreshold)
    {
        joyIsNeutral = true;
    }
}

void writeNumber(int currentDisplay)
{
    for (int i = 0; i < displayCount; i++)
    {
        if (i == currentDisplay)
        {
            if (selected == true)
                number = byteEncodings[digits[i]] + dpByteEncoding;
            else
            {
                if (millis() % blinkDelay < blinkDelay / 2)
                    number = byteEncodings[digits[i]] + dpByteEncoding;
                else
                    number = byteEncodings[digits[i]];
            }
        }
        else
        {
            number = byteEncodings[digits[i]];
        }

        writeReg(number);
        activateDisplay(i);
        delay(5);
    }
}

void joySWPressed()
{
    int joySWState = digitalRead(joySWPin);
    // Change "selected" state when joySW is pressed and released and when the button is long pressed (500ms) then reset the display
    if (joySWState == LOW && lastJoySWState == HIGH)
    {
        pressedJoySWTime = millis();
        Serial.println("Pressed");
    }
    else if (joySWState == HIGH && lastJoySWState == LOW)
    {
        releasedJoySWTime = millis();
        if (releasedJoySWTime - pressedJoySWTime > joyLongDebounce)
        {
            initialDisplayConfiguration();
        }
        else if (releasedJoySWTime - pressedJoySWTime > joyShortDebounce)
        {
            Serial.println("Released");
            selected = !selected;
        }
    }

    lastJoySWState = joySWState;
}

void initialDisplayConfiguration()
{
    currentDisplay = 0;
    selected = false;
    for (int i = 0; i < displayCount; i++)
    {
        digits[i] = 0;
    }
}

void writeReg(int digit)
{
    // ST_CP LOW to keep LEDs from changing while reading serial data
    digitalWrite(latchPin, LOW);
    // Shift out the bits
    shiftOut(dataPin, clockPin, MSBFIRST, digit);
    /* ST_CP on Rising to move the data from shift register
     * to storage register, making the bits available for output.
     */
    digitalWrite(latchPin, HIGH);
}

void activateDisplay(int displayNumber)
{
    // first, disable all the display digits
    for (int i = 0; i < displayCount; i++)
    {
        digitalWrite(displayDigits[i], HIGH);
    }
    // then enable only the digit you want to use now
    digitalWrite(displayDigits[displayNumber], LOW);
}
