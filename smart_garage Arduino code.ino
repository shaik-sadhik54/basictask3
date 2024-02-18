#include <Servo.h>
#include <ESP8266WiFi.h>

Servo doorServo; // Create a servo object to control the door

const int laserPin = 2; // Pin connected to the laser sensor
const int openButtonPin = 3; // Pin connected to the button for opening the door
const int closeButtonPin = 4; // Pin connected to the button for closing the door
const int voiceCommandPin = 5; // Pin connected to the microphone for voice commands

int openAngle = 0; // Angle to open the door
int closeAngle = 90; // Angle to close the door

bool doorOpen = false; // Flag to track the door status

// Wi-Fi credentials
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

WiFiServer server(80); // Create a server object

void setup() {
  doorServo.attach(9); // Attach the servo to pin 9
  pinMode(laserPin, INPUT);
  pinMode(openButtonPin, INPUT_PULLUP);
  pinMode(closeButtonPin, INPUT_PULLUP);
  pinMode(voiceCommandPin, INPUT_PULLUP);
  
  Serial.begin(9600); // Start serial communication
  delay(10);

  // Connect to Wi-Fi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
}

void loop() {
  // Check for incoming client connections
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  while (!client.available()) {
    delay(1);
  }

  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  // Handle commands received from the smartphone application
  if (request.indexOf("/open") != -1) {
    openDoor();
  } else if (request.indexOf("/close") != -1) {
    closeDoor();
  }

  // Check if an obstacle is detected by the laser sensor
  if (digitalRead(laserPin) == LOW) {
    Serial.println("Obstacle detected! Stop the door!");
    // You can add code here to stop the door from closing or opening
  }

  // Check for voice commands
  if (digitalRead(voiceCommandPin) == LOW) {
    processVoiceCommand();
  }
}

void openDoor() {
  if (!doorOpen) {
    doorServo.write(openAngle); // Rotate the servo to open the door position
    Serial.println("Door opening...");
    doorOpen = true;
    // Send notification that the door is opening
    sendNotification("Garage door is opening.");
    delay(1000); // Adjust delay as needed for the door to open completely
  }
}

void closeDoor() {
  if (doorOpen) {
    doorServo.write(closeAngle); // Rotate the servo to close the door position
    Serial.println("Door closing...");
    doorOpen = false;
    // Send notification that the door is closing
    sendNotification("Garage door is closing.");
    delay(1000); // Adjust delay as needed for the door to close completely
  }
}

void processVoiceCommand() {
  // Add voice command processing logic here
  Serial.println("Voice command detected...");
  // For example, you can use a voice recognition library to process commands
}

void sendNotification(String message) {
  // Add code to send notifications via IoT network or other means
  Serial.println("Sending notification: " + message);
  // For example, you can use Wi-Fi or GSM modules to send notifications
}
