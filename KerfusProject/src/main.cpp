#include <Arduino.h>
#include "ESP32-HUB75-MatrixPanel-I2S-DMA.h"
#include "DFRobotDFPlayerMini.h"
#include "kerfus_faces.h"
#include "WiFi.h"
#include "WiFiUdp.h"
#include "network_variables.h"

#define PANEL_RES_X 64
#define PANEL_RES_Y 32
#define PANEL_CHAIN 1  

DFRobotDFPlayerMini player;
MatrixPanel_I2S_DMA *matrix;
WiFiUDP udp;

// pins to communicate with DFPlayer Mini
static const uint8_t PIN_MP3_TX = 21; // Connects to module's RX 
static const uint8_t PIN_MP3_RX = 22; // Connects to module's TX 

const int localUdpPort = 7658;  // local port to listen on
char incomingPacket[255];  // buffer for incoming packets
int interval = 0; // random interval for blinking

void displayKerfus(const uint16_t img[PANEL_RES_Y][PANEL_RES_X]) {
    for (int row = 0; row < PANEL_RES_Y; row++) {
        for (int col = 0; col < PANEL_RES_X; col++) {
            matrix->drawPixel(col, row, img[row][col]);
        }
    }
}

void Meow(){
    displayKerfus(kerfus_meow);
    player.play(random(1,3));
    delay(1000);
    displayKerfus(kerfus_neutral);
}

void checkUDP(){
    int packetSize = udp.parsePacket();
    if (packetSize){
        int len = udp.read(incomingPacket, 255);
        if (len > 0) {
            incomingPacket[len] = '\0'; // Null-terminate the string
        }
        Serial.printf("Received packet data: %s\n", incomingPacket);

        if (String(incomingPacket) == "meow"){
            Meow();
        }
    }
}

void setup() {
    // Initialize serial port for debugging
    Serial.begin(115200);
    Serial.println("Starting...");

    // Init serial port for DFPlayer Mini
    Serial2.begin(9600, SERIAL_8N1, PIN_MP3_RX, PIN_MP3_TX);

    // Connect to WiFi
    WiFi.begin(ssid, password);
    Serial.println("Connecting to WiFi...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.printf("Connected to WiFi");
    udp.begin(localUdpPort);
    Serial.printf("Listening on UDP port %d\n", localUdpPort);

    HUB75_I2S_CFG mxconfig(
        PANEL_RES_X, PANEL_RES_Y, PANEL_CHAIN
    );

    matrix = new MatrixPanel_I2S_DMA(mxconfig);
    if (!matrix->begin()) {
        Serial.println("Matrix initialization failed!");
    }

    // Start communication with DFPlayer Mini
    if (player.begin(Serial2)) {
        Serial.println("OK");
    
        // Set volume to maximum (0 to 30).
        player.volume(2);
        // Play the first MP3 file on the SD card
        player.play(1);
    } else {
        Serial.println("Connecting to DFPlayer Mini failed!");
    }
    interval = random(3000, 8000);
    displayKerfus(kerfus_neutral);
}

void loop() {
    int time = millis();
    if (time % interval == 0){
        interval = random(3000, 8000);
        displayKerfus(kerfus_blink);
        delay(100);
        displayKerfus(kerfus_neutral);
    }
    checkUDP();
}



