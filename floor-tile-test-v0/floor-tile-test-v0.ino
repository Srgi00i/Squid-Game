#include <Adafruit_NeoPixel.h>

// Define the number of LEDs in the strip
#define NUM_DIODS 140
#define NUM_TILES 12
#define BUFF_SIZE 25
#define MAX_BRIGHT 20

    const int ledPins[NUM_TILES] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
const int analogPins[NUM_TILES] = {A0, A0, A0, A0, A0, A0, A0, A0, A0, A0, A0, A0};

const int neighborThreshold = 1;
const int loopDelay = 0;

// Create an instance of the Adafruit_NeoPixel class
Adafruit_NeoPixel strips[NUM_TILES] = {};

bool secuence[NUM_TILES] = {};
bool prevSecuence[NUM_TILES] = {};
int lastTile = 0;

int measures[NUM_TILES][BUFF_SIZE];
float means[NUM_TILES];

int gameState = 0;

float adjust[NUM_TILES] = {2.5}; // Example initial array

// int prevValues[NUM_TILES] = {0,0,0}; // Example initial array
int prevSec[NUM_TILES]; // Example initial array

const int buttonPin = 22; // Pin connected to the button
int buttonState = 0;      // Variable to store the button state

void setup()
{
    Serial.begin(115200);
    // Seed the random number generator with a unique value (using an analog input pin)
    randomSeed(analogRead(13));

    // analogReference(DEFAULT);

    // pinMode(A0, INPUT_PULLUP);

    pinMode(buttonPin, INPUT_PULLUP); // Use internal pull-up resistor

    for (int strip = 0; strip < NUM_TILES; strip++)
    {
        strips[strip] = Adafruit_NeoPixel(NUM_DIODS, ledPins[strip], NEO_GRB + NEO_KHZ800);
        // Initialize the LED strip
        strips[strip].begin();
        // Turn off all LEDs
        strips[strip].show();

        // pinMode(analogPins[strip], INPUT_PULLUP);
    }
}

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

        //::Serial.println(String(randomValue) + " <- " + String(secuence[i]));
    }

    for (int j = NUM_TILES - 1; j >= 0; j--)
    {
        if (!secuence[j])
        {
            lastTile = j;
            break;
        }
    }
}

void initAnimation()
{
    for (int counter = 0; counter < 3; counter++)
    {

        for (int value = 0; value < 255; value += 5)
        {
            for (int strip = 0; strip < NUM_TILES; strip++)
            {
                /* code */
                strips[strip].fill(strips[strip].Color(0, 0, value));
                strips[strip].show();
            }
        }
        delay(500);
        for (int invValue = 255; invValue >= 0; invValue -= 5)
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
    for (int counter = 0; counter < 5; counter++)
    {

        for (int value = 0; value < 255; value += 5)
        {
            for (int strip = 0; strip < NUM_TILES; strip++)
            {
                /* code */
                strips[strip].fill(strips[strip].Color(value, 0, 0));
                strips[strip].show();
            }
        }
        for (int invValue = 255; invValue >= 0; invValue -= 5)
        {
            for (int strip = 0; strip < NUM_TILES; strip++)
            {
                strips[strip].fill(strips[strip].Color(invValue, 0, 0));
                strips[strip].show();
            }
        }
    }
}

void liveAnimation()
{
    for (int counter = 0; counter < 5; counter++)
    {

        for (int value = 0; value < 255; value += 5)
        {
            for (int strip = 0; strip < NUM_TILES; strip++)
            {
                /* code */
                strips[strip].fill(strips[strip].Color(0, value, 0));
                strips[strip].show();
            }
        }
        for (int invValue = 255; invValue >= 0; invValue -= 5)
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
            strips[strip].fill(strips[strip].Color(255, 0, 0));
        }
        else
        {
            strips[strip].fill(strips[strip].Color(0, 255, 0));
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

void initialization()
{
    Serial.println(":[ Initialization ]:");

    for (int a = 0; a < NUM_TILES; a++)
    {
        prevSec[a] = secuence[a];
    }

    generateSecuence();

    bool test = true;
    for (int b = 0; b < NUM_TILES; b++)
    {
        if (prevSec[b] != secuence[b])
        {
            test = false;
        }
    }

    while (test)
    {
        for (int c = 0; c < NUM_TILES; c++)
        {
            if (prevSec[c] = secuence[c])
            {
                test = false;
            }
        }
    }

    Serial.println("Secuence:");
    for (int i = 0; i < NUM_TILES; i++)
    {
        Serial.print(String(secuence[i]) + ", ");
    }
    Serial.println("");
    Serial.println("Last tile: " + String(lastTile));

    initAnimation();

    int raw;
    for (int count = 0; count < 200; count++)
    {
        for (int t = 0; t < NUM_TILES; t++)
        {
            raw = analogRead(t);
            delay(10);
            updateArrayAndCalculateMean(t, raw);
        }
        // delay(50);
    }

    showAnimation();

    blackoutAnimation();

    // means[0] = 50;
    // means[1] = 50;
    // means[2] = 50;

    gameState = 1; // activate
}

void mainLoop()
{
    int raw = 0;

    for (int sensor = 0; sensor < NUM_TILES; sensor++)
    {
        raw = analogRead(analogPins[sensor]);
        Serial.println("+ sensor_" + String(sensor) + ": [" + String(raw) + "]");
        Serial.println("    -> mean: " + String(means[sensor]));
        delay(20);

        // if (false)
        if (raw > means[sensor] + adjust[sensor])
        // if((prevValues[sensor] != 0) && (raw > prevValues[sensor] + adjust[sensor])) // when hit detected
        {
            Serial.println("hit!!");

            if (secuence[sensor]) // hit death
            {
                gameOver();
            }
            else
            {
                // fill with green
                for (int fillId = 0; fillId <= sensor; fillId++)
                {
                    strips[fillId].fill(strips[fillId].Color(0, 255, 0));
                    strips[fillId].show();
                }
                if (sensor == lastTile)
                {
                    /*  prevValues[sensor] = raw;
                     raw = analogRead(analogPins[sensor]);

                     while (raw > prevValues[sensor] - 4)
                     {
                         prevValues[sensor] = raw;
                         delay(200);
                         raw = analogRead(analogPins[sensor]);
                     }
                     delay(1000); */

                    liveAnimation();
                    gameState = -2;
                }
            }

            break;
        }
        else
        {
            updateArrayAndCalculateMean(sensor, raw);
        }
    }
    Serial.println("---------------------------------------");
}

void gameOver()
{
    deathAnimation();

    delay(2000);

    gameState = 0;
}

// Function to calculate mean
float calculateMean(int id)
{
    int sum = 0;
    for (int i = 0; i < BUFF_SIZE; i++)
    {
        sum += measures[id][i];
    }

    return (float)sum / BUFF_SIZE;
}

// Function to update array and calculate mean
void updateArrayAndCalculateMean(int id, int newValue)
{
    // Calculate mean before updating the array
    float meanBefore = calculateMean(id);
    // Serial.print("Mean before update: ");
    // Serial.println(meanBefore);

    // Shift elements to the left and append new value
    for (int i = 0; i < BUFF_SIZE - 1; i++)
    {
        measures[id][i] = measures[id][i + 1];
    }
    measures[id][BUFF_SIZE - 1] = newValue;

    // Calculate mean after updating the array
    float meanAfter = calculateMean(id);
    // Serial.print("Mean after update: ");
    // Serial.println(meanAfter);
    means[id] = meanAfter;
}

void loop()
{
 

        for (int value = 0; value < MAX_BRIGHT; value += 1)
        {
            for (int strip = 0; strip < NUM_TILES; strip++)
            {
                /* code */
                strips[strip].fill(strips[strip].Color(value, random(0,255), random(0,155)));
                strips[strip].show();
                delay(500);
                strip = random(0,12);
            }
        }
        delay(500);
        


    
}
