
#include <SPI.h>
#include <WiFiNINA.h>

#include "config.h"
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
int keyIndex = 0;                 // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;
WiFiServer server(80);

void setup() {
  Serial.begin(9600);      // initialize serial communication
  pinMode(9, OUTPUT);      // set the LED pin mode

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < "1.0.0") {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);                   // print the network name (SSID);

    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, password);
    // wait 10 seconds for connection:
    delay(10000);
  }
  server.begin();                           // start the web server on port 80
  printWifiStatus();                        // you're connected now, so print out the status
}


void loop() {
  if (status != WiFi.status()) {
    // it has changed update the variable
    status = WiFi.status();

    if (status == WL_AP_CONNECTED) {
      // a device has connected to the AP
      Serial.println("Device connected to AP");
    } else {
      // a device has disconnected from the AP, and we are back in listening mode
      Serial.println("Device disconnected from AP");
    }
  }

  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {
    Serial.println("new client");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        String request = client.readStringUntil('\n');
        Serial.println(request);

        if (request.length() <= 2) {
          //  client.println("Http 200 OK\n");
          String response = makeResponse();
          client.println(response);
          delay(10);
          if (client.connected()) {
            client.stop();
          }

        }
      }
    }
  }

}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
}

String makeResponse() {
  String result = "HTTP/1.1 200 OK\n";
  result += "Content-Type: text/html\n\n";
  result += "<!doctype html>\n";
  result += "<html><head><tile>";
  result += "Hello from Arduino</title></head><br>\n";
  result += "<meta http-equiv=\"refresh\" content=\"3\">";
  result += "\n <body> \n";

  for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
    result += "analog input";
    result += analogChannel;
    result += " is ";
    result += analogRead(analogChannel);
    result += "<br />\n";

  }

  result += "</body></html>\n\n";
  return result;
}
