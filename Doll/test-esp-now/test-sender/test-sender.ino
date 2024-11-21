#include <esp_now.h>
#include <WiFi.h>

#define BUTTON_A_PIN 23
#define BUTTON_B_PIN 19

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[] = {0xc8,0xf0,0x9e,0xf8,0x76,0x04};

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message
{
    //char a[32];
    int code;
    //float c;
    //bool d;
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
    //strcpy(myData.a, "THIS IS A CHAR");
    myData.code = code;
    //myData.c = 1.2;
    //myData.d = false;

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
      sendSignal(2);
      delay(2000);
    }
    if (digitalRead(BUTTON_B_PIN) == LOW)
    {
      Serial.println("Button A pressed!");
      sendSignal(1);
      delay(2000);
    }

    delay(50);
}