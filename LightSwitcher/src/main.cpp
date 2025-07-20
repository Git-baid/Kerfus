// This is uploaded to the firmware of the ESP controlling the lightswitch
#include "WiFi.h"
#include "network_variables.h"
#include "ESP32Servo.h"

WiFiUDP udp;
Servo myServo;

const int localUdpPort = 7658;  // local port to listen on
char incomingPacket[255];  // buffer for incoming packets

const int servoPin = 15; // Pin for the servo motor

void lightsOn(){
    Serial.println("Lights ON");
    myServo.write(180);
    delay(400); // Wait for the servo to reach the position
    myServo.write(90); // Move back to the initial position
}

void lightsOff(){
    Serial.println("Lights OFF");
    myServo.write(0);
    delay(400); // Wait for the servo to reach the position
    myServo.write(90); // Move back to the initial position
}

void checkUDP(){
    int packetSize = udp.parsePacket();
    if (packetSize){
        int len = udp.read(incomingPacket, 255);
        if (len > 0) {
            incomingPacket[len] = '\0'; // Null-terminate the string
        }
        Serial.printf("Received packet data: %s\n", incomingPacket);

        if (String(incomingPacket) == "lights_on"){
            lightsOn();
        }
        else if (String(incomingPacket) == "lights_off"){
            lightsOff();
        }
    }
}

void setup(){
    // Initialize serial port for debugging
    Serial.begin(115200);
    Serial.println("Starting...");

    WiFi.begin(ssid, password);

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

    myServo.attach(servoPin); // Attach the servo to the pin
    myServo.write(90);
}

void loop(){
    checkUDP();
}
