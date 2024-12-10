#include <esp_now.h>
#include <WiFi.h>

#define SIG_PIN_A 21
#define SIG_PIN_B 23

enum Code {
    BUTTON_A,
    BUTTON_B
};

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message
{
    Code code;
} struct_message;

// Create a struct_message called myData
struct_message myData;

int actDuration = 200;

void buttonFunctionA()
{
  Serial.println("Executing function A...");

  digitalWrite(SIG_PIN_A, HIGH);
  delay(actDuration);
  digitalWrite(SIG_PIN_A, LOW);
}

void buttonFunctionB()
{
  Serial.println("Executing function B...");

  digitalWrite(SIG_PIN_B, HIGH);
  delay(actDuration);
  digitalWrite(SIG_PIN_B, LOW);
}

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len)
{
    memcpy(&myData, incomingData, sizeof(myData));
    Serial.print("Bytes received: ");
    Serial.println(len);
    Serial.print("Code: ");
    Serial.println(myData.code);
    Serial.println();

    // Button A
    if (myData.code == BUTTON_A)
    {
      buttonFunctionA();
    }
    // Button B
    else if (myData.code == BUTTON_B)
    {
      buttonFunctionB();
    }
    
}

void setup()
{
    // Initialize Serial Monitor
    Serial.begin(9600);

    // Set device as a Wi-Fi Station
    WiFi.mode(WIFI_STA);

    // Init ESP-NOW
    if (esp_now_init() != ESP_OK)
    {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    // Once ESPNow is successfully Init, we will register for recv CB to
    // get recv packer info
    esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));

    pinMode(SIG_PIN_A, OUTPUT);
    pinMode(SIG_PIN_B, OUTPUT);
    digitalWrite(SIG_PIN_A, LOW);
    digitalWrite(SIG_PIN_B, LOW);
}

void loop()
{
}