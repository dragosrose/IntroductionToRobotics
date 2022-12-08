#include <LiquidCrystal.h>
#include <EEPROM.h>

// The number of main menu options
const byte MENU_OPTIONS = 4;

// Constants for the main menu options
const byte MENU_START = 0;
const byte MENU_HIGHSCORE = 1;
const byte MENU_SETTINGS = 2;
const byte MENU_ABOUT = 3;
const byte MENU_BACK = 4;

// Array that stores the names of the main menu options
const String menuOptionNames[MENU_OPTIONS] = {
    " 1.Start",
    " 2.Highscore",
    " 3.Settings",
    " 4.About",
};

// Array that stores the number of options for each submenu
const byte SUBMENU_OPTIONS[MENU_OPTIONS] = {
    1, // start
    1, // highscore
    5, // settings
    2, // about
};

// Array that stores the index of the currently selected option for each menu and submenu
byte currentMenuOption[MENU_OPTIONS + 1] = {
    0, // main
    0, // start
    0, // highscore
    0, // settings
    0, // about
};

// Array that stores the names of the options for each submenu
const String submenuOptionNames[MENU_OPTIONS][10] = {
    {

    },
    {

    },
    {
        " 1.Sound",
        " 2.Set name",
        " 3.LCD glow",
        " 4.LED glow",
        " 5.Back",
    },
    {
        " Simon Says, by github.com/nomoney34",
        " 2. Back",
    },
};

// Constants that define the addresses in the EEPROM where the LCD contrast and brightness values are stored
const byte EEPROM_LCD_CONTRAST_ADDRESS = 1;
const byte EEPROM_LCD_BRIGHTNESS_ADDRESS = 2;

// Stores the current player's name
String playerName = "AAA";

// Constants for the "Settings" submenu options
const byte SUBMENU_SETTINGS_SOUND = 0;
const byte SUBMENU_SETTINGS_SET_NAME = 1;
const byte SUBMENU_SETTINGS_LED_BRIGHTNESS = 2;
const byte SUBMENU_SETTINGS_MATRIX_BRIGHTNESS = 3;
const byte SUBMENU_SETTINGS_BACK = 4;

// About options
const byte SUBMENU_ABOUT_CREDITS = 0;
const byte SUBMENU_ABOUT_BACK = 1;

byte currentSubMenuOption = MENU_START;
byte currentTopOption = 0;
byte currentNumberOfOptions = MENU_OPTIONS;
byte currentTabIndex = 0;

const byte LCD_WIDTH = 16;
const byte LCD_HEIGHT = 2;

const byte JOYSTICK_X_PIN = A0;
const byte JOYSTICK_Y_PIN = A1;
const byte JOYSTICK_BUTTON_PIN = 2;
const int JOYSTICK_THRESHOLD = 512;
const int JOYSTICK_MIN = 100;
const int JOYSTICK_MAX = 900;
const int JOYSTICK_BUTTON_DEBOUNCE = 250;

unsigned long lastJoystickButtonPress = 0;
bool joyIsNeutral;

const char FULL_BLOCK = 0xFF;

enum CycleMode
{
    MENU_CYCLE,
    LETTERS_CYCLE,
};

CycleMode cycleMode = MENU_CYCLE;

const byte LETTERS = 26;
byte cycleIndex = 0;
char currentLetter = 'A';

const byte RS = 9;
const byte EN = 8;
const byte D4 = 7;
const byte D5 = 6;
const byte D6 = 5;
const byte D7 = 4;

int centerX;
int centerY;

LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);
const byte lcdContrastPin = 3;

// Properties
byte lcdContrast = 0;
byte lcdBrightness = 0;
byte ledBrightness = 0;

// Auxiliary functions
void scrollRow(String text, int row)
{
    lcd.setCursor(0, row);

    // Print the text on the specified row
    lcd.print(text);

    // Set the autoscroll mode
    lcd.autoscroll();

    // Scroll the text to the left
    for (int i = 0; i < text.length() + 4; i++)
    {
        lcd.scrollDisplayLeft();
        delay(350);
    }

    // Clear the autoscroll mode
    lcd.noAutoscroll();
}

void printText(String text, int row)
{
    // If the text length is greater than 16
    if (text.length() > 16 && currentMenuOption[currentTabIndex] - currentTopOption == row)
    {
        scrollRow(text, row);
    }
    else
    {
        // Set the cursor to the specified row
        lcd.setCursor(0, row);

        // Print the text on the specified row
        lcd.print(text);
    }
}

// Functions for the menu options
void printMenu()
{
    clearDisplay();
    printText(menuOptionNames[currentTopOption], 0);
    printText(menuOptionNames[currentTopOption + 1], 1);
    printText(">", currentMenuOption[currentTabIndex] - currentTopOption);
}

// Start game
void startGame()
{
    clearDisplay();
    lcd.print("Game started");
    delay(1000);
    clearDisplay();
    printMenu();
}

// Highscore list
void printHighscore()
{
    clearDisplay();
    lcd.print("Highscore");
    delay(1000);
    clearDisplay();
    printMenu();
}

// Settings
void printSettings()
{
    clearDisplay();
    printText(submenuOptionNames[currentTabIndex - 1][currentTopOption], 0);
    printText(submenuOptionNames[currentTabIndex - 1][currentTopOption + 1], 1);
    printText(">", currentMenuOption[currentTabIndex] - currentTopOption);
}

void printSetttingsName()
{
    clearDisplay();
    printText("Set name", 0);
    printText(playerName, 1);

    lcd.setCursor(cycleIndex, 1);
    lcd.blink();
}

void printLCDCustomContrast()
{
    clearDisplay();
    printText("LCD contrast", 0);
    printText(String(lcdContrast), 1);
}

void printLCDCustomBrightness()
{
    clearDisplay();
    printText("LCD brightness", 0);
    printText(String(lcdBrightness), 1);
}

void printLEDCustomBrightness()
{
    clearDisplay();
    printText("LED brightness", 0);
    printText(String(ledBrightness), 1);
}
// About
void printAbout()
{
    clearDisplay();
    printText(submenuOptionNames[currentTabIndex - 1][currentTopOption], 0);
    printText(submenuOptionNames[currentTabIndex - 1][currentTopOption + 1], 1);
    currentMenuOption[currentTabIndex] = 1;
    printText(">", currentMenuOption[currentTabIndex] - currentTopOption);
}

// Functions for the joystick
void (*menuFunctions[MENU_OPTIONS + 1])() = {
    printMenu,
    startGame,
    printHighscore,
    printSettings,
    printAbout,
};

void setup()
{
    Serial.begin(9600);
    initJoystick();
    initDisplay();
    printMenu();
}

void loop()
{
    int x = readJoystickX();
    int y = readJoystickY();
    bool buttonPressed = readJoystickButton();

    if (y < JOYSTICK_MIN && joyIsNeutral)
    {
        handleUpArrow(currentNumberOfOptions, menuFunctions[currentTabIndex]);
        joyIsNeutral = false;
    }
    else if (y > JOYSTICK_MAX && joyIsNeutral)
    {
        handleDownArrow(currentNumberOfOptions, menuFunctions[currentTabIndex]);
        joyIsNeutral = false;
    }
    else if (y >= JOYSTICK_MIN && y <= JOYSTICK_MAX)
    {
        joyIsNeutral = true;
    }

    if (buttonPressed && millis() - lastJoystickButtonPress > JOYSTICK_BUTTON_DEBOUNCE)
    {
        lastJoystickButtonPress = millis();
        handleButtonPressed();
    }

    delay(100);
}

void initJoystick()
{
    pinMode(JOYSTICK_BUTTON_PIN, INPUT_PULLUP);
    pinMode(JOYSTICK_X_PIN, INPUT);
    pinMode(JOYSTICK_Y_PIN, INPUT);

    centerX = analogRead(JOYSTICK_X_PIN);
    centerY = analogRead(JOYSTICK_Y_PIN);
}

void initDisplay()
{
    lcd.begin(LCD_WIDTH, LCD_HEIGHT);
    lcdContrast = EEPROM.read(0);
    pinMode(lcdContrastPin, OUTPUT);
    analogWrite(lcdContrastPin, lcdContrast);
    clearDisplay();
}

void clearDisplay()
{
    lcd.clear();
    lcd.setCursor(0, 0);
}

int readJoystickX()
{
    int x = analogRead(JOYSTICK_X_PIN);
    return x;
}

int readJoystickY()
{
    int y = analogRead(JOYSTICK_Y_PIN);
    return y;
}

bool readJoystickButton()
{
    return digitalRead(JOYSTICK_BUTTON_PIN) == LOW;
}

void handleUpArrow(int MENU_OPTIONS, void (*printTab)())
{
    switch (cycleMode)
    {
    case MENU_CYCLE:
        currentMenuOption[currentTabIndex] = (currentMenuOption[currentTabIndex] + MENU_OPTIONS - 1) % MENU_OPTIONS;
        if (currentMenuOption[currentTabIndex] == MENU_OPTIONS - 1)
            currentTopOption = MENU_OPTIONS - 2;
        if (currentMenuOption[currentTabIndex] < currentTopOption)
            currentTopOption = currentMenuOption[currentTabIndex];
        printTab();
        break;
    case LETTERS_CYCLE:
        currentLetter++;
        if (currentLetter > 'Z')
            currentLetter = 'A';
        playerName[cycleIndex] = currentLetter;
        printSetttingsName();
        break;
    }
}

void handleDownArrow(int MENU_OPTIONS, void (*printTab)())
{
    switch (cycleMode)
    {
    case MENU_CYCLE:
        currentMenuOption[currentTabIndex] = (currentMenuOption[currentTabIndex] + 1) % MENU_OPTIONS;
        if (currentMenuOption[currentTabIndex] == 0)
            currentTopOption = 0;
        if (currentMenuOption[currentTabIndex] > currentTopOption + 1)
            currentTopOption = currentMenuOption[currentTabIndex] - 1;
        printTab();
        break;
    case LETTERS_CYCLE:
        currentLetter--;
        if (currentLetter < 'A')
            currentLetter = 'Z';

        playerName[cycleIndex] = currentLetter;
        printSetttingsName();
        break;
    }
}

void handleButtonPressed()
{
    switch (currentTabIndex)
    {
    case 0:
        switch (currentMenuOption[currentTabIndex])
        {
        case MENU_START:
            startGame();
            break;
        case MENU_HIGHSCORE:
            printHighscore();
            break;
        case MENU_SETTINGS:
            currentTabIndex = MENU_SETTINGS + 1;
            currentTopOption = 0;
            currentNumberOfOptions = SUBMENU_OPTIONS[MENU_SETTINGS];
            currentMenuOption[currentTabIndex] = 0;
            clearDisplay();
            printSettings();
            break;
        case MENU_ABOUT:
            currentTabIndex = MENU_ABOUT + 1;
            currentTopOption = 0;
            currentNumberOfOptions = SUBMENU_OPTIONS[MENU_ABOUT];
            currentMenuOption[currentTabIndex] = 0;
            clearDisplay();
            printAbout();
            break;
        }
        break;
    case MENU_SETTINGS + 1:
        switch (currentMenuOption[currentTabIndex])
        {
        case SUBMENU_SETTINGS_SET_NAME:
            if (cycleMode == MENU_CYCLE)
            {
                cycleMode = LETTERS_CYCLE;
                currentLetter = playerName[cycleIndex];
                clearDisplay();
                printSetttingsName();
            }
            else
            {
                if (cycleIndex == 2)
                {
                    cycleIndex = 0;
                    cycleMode = MENU_CYCLE;
                    printText("Name set!", 1);
                    delay(1000);
                    currentTabIndex = MENU_SETTINGS + 1;
                    currentTopOption = 0;
                    currentNumberOfOptions = SUBMENU_OPTIONS[MENU_SETTINGS];
                    currentMenuOption[currentTabIndex] = 0;
                    lcd.noBlink();
                    clearDisplay();
                    printSettings();
                }
                else
                {
                    cycleIndex++;
                    currentLetter = playerName[cycleIndex];
                    clearDisplay();
                    printSetttingsName();
                }
            }

            break;

        case SUBMENU_SETTINGS_BACK:
            currentTabIndex = MENU_START;
            currentNumberOfOptions = MENU_OPTIONS;
            currentTopOption = 0;
            currentMenuOption[currentTabIndex] = MENU_START;
            clearDisplay();
            printMenu();
            break;
        default:
            break;
        }
        break;
    case MENU_ABOUT + 1:
        switch (currentMenuOption[currentTabIndex])
        {

        case SUBMENU_ABOUT_BACK:
            currentTabIndex = MENU_START;
            currentNumberOfOptions = MENU_OPTIONS;
            currentTopOption = 0;
            currentMenuOption[currentTabIndex] = MENU_START;
            clearDisplay();
            printMenu();
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}
