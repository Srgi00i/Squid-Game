#include <esp_now.h>
#include <WiFi.h>

#define BUTTON_A_PIN 23
#define BUTTON_B_PIN 19

enum Code {
    BUTTON_A,
    BUTTON_B 
};

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[] = {0xf8,0xb3,0xb7,0x2a,0xd6,0x98};

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message
{
    int code;
} struct_message;

// Create a struct_message called myData
struct_message myData;

esp_now_peer_info_t peerInfo;

// Logic parameters
int state = 0; // 0 -> rest, 1 -> Button A, 2-> Button B

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
    Serial.print("\r\nLast Packet Send Status:\t");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup()
{
    // Init Serial Monitor
    Serial.begin(9600);

    // Set device as a Wi-Fi Station
    WiFi.mode(WIFI_STA);

    // Init ESP-NOW
    if (esp_now_init() != ESP_OK)
    {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    // Once ESPNow is successfully Init, we will register for Send CB to
    // get the status of Trasnmitted packet
    esp_now_register_send_cb(OnDataSent);

    // Register peer
    memcpy(peerInfo.peer_addr, broadcastAddress, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    // Add peer
    if (esp_now_add_peer(&peerInfo) != ESP_OK)
    {
        Serial.println("Failed to add peer");
        return;
    }

    pinMode(BUTTON_A_PIN, INPUT_PULLUP);
    pinMode(BUTTON_B_PIN, INPUT_PULLUP);
}

void sendSignal(int code)
{
    // Set values to send
    myData.code = code;

    // Send message via ESP-NOW
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));

    if (result == ESP_OK)
    {
        Serial.println("Sent with success");
    }
    else
    {
        Serial.println("Error sending the data");
    }
}

void loop()
{
    if (digitalRead(BUTTON_A_PIN) == LOW)
    {
      Serial.println("Button B pressed!");
      sendSignal(BUTTON_B);
      //delay(2000);
    }
    if (digitalRead(BUTTON_B_PIN) == LOW)
    {
      Serial.println("Button A pressed!");
      sendSignal(BUTTON_A);
      //delay(2000);
    }

    //delay(50);
}