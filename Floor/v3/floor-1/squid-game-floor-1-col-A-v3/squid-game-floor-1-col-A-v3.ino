#include <Adafruit_NeoPixel.h>

#define NUM_DIODS 140       // number of LEDs in the strip
#define NUM_TILES 12        // number of tiles     
#define MAX_BRIGHT 30       // max brightness for the LEDs (DO NOT TOUCH)
#define STEP_ANIMATION 10   // size of the step for dimming in animations

const bool debug = true;   // activates debug mode (flags, all green, ...)

const int neighborThreshold = 1;
const int loopDelay = 500;

const int ledPins[NUM_TILES] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
const int digitalPins[NUM_TILES] = {23, 25, 27, 29, 31, 33, 35, 37, 39, 41, 43, 45};

// Create an instance of the Adafruit_NeoPixel class
Adafruit_NeoPixel strips[NUM_TILES] = {};

bool secuence[NUM_TILES] = {};
int lastTile = -1;
int lastStep = -1;

int gameState = 0;
bool enableList[NUM_TILES] = {true, true, true, true, true, true, true, true, true, true, true, true};

// Animation functions

void initAnimation()
{
    for (int counter = 0; counter < 3; counter++)
    {
        
            for (int value = 0; value < MAX_BRIGHT; value += STEP_ANIMATION)
            {
                for (int strip = 0; strip < NUM_TILES; strip++)
                {
                    strips[strip].fill(strips[strip].Color(0, 0, value));
                    strips[strip].show();
                }
            }

            delay(500);

            for (int invValue = MAX_BRIGHT; invValue >= 0; invValue -= STEP_ANIMATION)
            {
                for (int strip = 0; strip < NUM_TILES; strip++)
                {
                    strips[strip].fill(strips[strip].Color(0, 0, invValue));
                    strips[strip].show();
                }
            }
    }
}

void deathAnimation()
{
    for (int counter = 0; counter < 3; counter++)
    {

        for (int value = 0; value < MAX_BRIGHT; value += STEP_ANIMATION)
        {
            for (int strip = 0; strip < NUM_TILES; strip++)
            {
                /* code */
                strips[strip].fill(strips[strip].Color(value, 0, 0));
                strips[strip].show();
            }
        }
        for (int invValue = MAX_BRIGHT; invValue >= 0; invValue -= STEP_ANIMATION)
        {
            for (int strip = 0; strip < NUM_TILES; strip++)
            {
                strips[strip].fill(strips[strip].Color(invValue, 0, 0));
                strips[strip].show();
            }
        }
    }
    for (int value = 0; value < MAX_BRIGHT; value += STEP_ANIMATION)
    {
        for (int strip = 0; strip < NUM_TILES; strip++)
        {
            /* code */
            strips[strip].fill(strips[strip].Color(value, 0, 0));
            strips[strip].show();
        }
    }
}

void liveAnimation()
{
    for (int counter = 0; counter < 3; counter++)
    {

        for (int value = 0; value < MAX_BRIGHT; value += STEP_ANIMATION)
        {
            for (int strip = 0; strip < NUM_TILES; strip++)
            {
                /* code */
                strips[strip].fill(strips[strip].Color(0, value, 0));
                strips[strip].show();
            }
        }
        for (int invValue = MAX_BRIGHT; invValue >= 0; invValue -= STEP_ANIMATION)
        {
            for (int strip = 0; strip < NUM_TILES; strip++)
            {
                strips[strip].fill(strips[strip].Color(0, invValue, 0));
                strips[strip].show();
            }
        }
    }
}

void showAnimation()
{
    for (int strip = 0; strip < NUM_TILES; strip++)
    {
        if (secuence[strip])
        {
            strips[strip].fill(strips[strip].Color(MAX_BRIGHT, 0, 0));
        }
        else
        {
            strips[strip].fill(strips[strip].Color(0, MAX_BRIGHT, 0));
        }
        
        strips[strip].show();
    }

    delay(5000);
}

void blackoutAnimation()
{
    for (int strip = 0; strip < NUM_TILES; strip++)
    {
        strips[strip].fill(strips[strip].Color(0, 0, 0));

        strips[strip].show();
    }
}

// Auxiliar functions

void generateSecuence()
{
    int trueCounter = 0;
    int falseCounter = 0;
    bool randomValue;

    for (int i = 0; i < NUM_TILES; i++)
    {
        randomValue = random(0, 2);

        if (!randomValue)
        {
            if (falseCounter < neighborThreshold + 1)
            {
                secuence[i] = false;
                falseCounter += 1;
                trueCounter = 0;
            }
            else
            {
                if (trueCounter < neighborThreshold)
                {
                    secuence[i] = false;
                    trueCounter = 0;
                    falseCounter += 1;
                }
                else
                {
                    secuence[i] = true;
                    trueCounter += 1;
                    falseCounter = 0;
                }
            }
        }
        else
        {
            if (trueCounter < neighborThreshold)
            {
                secuence[i] = true;
                trueCounter += 1;
                falseCounter = 0;
            }
            else
            {
                secuence[i] = false;
                falseCounter += 1;
                trueCounter = 0;
            }
        }
    }

    // Aditional secuence conditions

    //

    for (int j = NUM_TILES - 1; j >= 0; j--)
    {
        if (!secuence[j])
        {
            lastTile = j;
            break;
        }
    }
}

void gameOver()
{
    deathAnimation();

    delay(2000);

    gameState = -2;
}

// Setup functions

void setup()
{
    Serial.begin(115200);

    // Seed the random number generator with a unique value (using an analog input pin)
    randomSeed(analogRead(0));

    for (int strip = 0; strip < NUM_TILES; strip++)
    {
        strips[strip] = Adafruit_NeoPixel(NUM_DIODS, ledPins[strip], NEO_GRB + NEO_KHZ800);
        // Initialize the LED strip
        strips[strip].begin();
        // Turn off all LEDs
        strips[strip].show();

        pinMode(digitalPins[strip], INPUT);
    }
}

void initialization()
{
    Serial.println(":[ Initialization ]:");
    lastStep = 0;
    lastTile = 0;

    generateSecuence();

    Serial.println("Secuence:");
    for (int i = 0; i < NUM_TILES; i++)
    {
        enableList[i] = true;
        Serial.print(String(secuence[i]) + ", ");
    }
    Serial.println("");
    Serial.println("Last tile: " + String(lastTile));

    initAnimation();

    if (debug)
    {
        for (int t = 0; t < NUM_TILES; t++)
        {
            secuence[t] = false;
        }
        lastTile = 11;
    }   

    showAnimation();

    blackoutAnimation();

    // Activate game
    gameState = 1;
    
}

void mainLoop()
{
    int inputSignal = 0;

    for (int sensor = lastStep; sensor < NUM_TILES; sensor++)
    {
        inputSignal = digitalRead(digitalPins[sensor]);
        Serial.println("+ sensor_" + String(sensor) + ": [" + String(inputSignal) + "]");

        // if (sensor == 10) // test one tile only
        if (enableList[sensor] && (inputSignal == HIGH))
        {
            Serial.println("hit!!");
            enableList[sensor] = false;
            lastStep = sensor;

            if (secuence[sensor]) // Stepped on death
            {
                gameOver();
            }
            else
            {
                // Fill with green
                for (int fillId = 0; fillId <= sensor; fillId++)
                {
                    strips[fillId].fill(strips[fillId].Color(0, MAX_BRIGHT, 0));
                    strips[fillId].show();
                }
                // Victory
                if (sensor == lastTile)
                {

                    liveAnimation();

                    if (debug)
                        gameState = 0;
                    else
                        gameState = -2;
                }
            }

            break;
        }
    }
    Serial.println("---------------------------------------");
}

void loop()
{

    if (gameState == 0) // initialization
    {
        initialization();
    }
    else if (gameState == 1) // activate
    {
        mainLoop();
    }
    else if (gameState == 2) // game over
    {
        gameState = 0;
    }
    else if (gameState == -1) // debug
    {
        initAnimation();
    }

    delay(loopDelay);
}
