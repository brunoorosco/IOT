#include <Arduino.h>
#include <ESP8266WiFi.h>

const char* ssid = "WIFI_EDUC_126"; //endereço do wifi
const char* password = "educ126634#"; //senha da rede escola

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  delay(10);

  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  digitalWrite(D1, 0);
  digitalWrite(D2, 0);
  digitalWrite(D3, 0);

  // Connect to WiFi network
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
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }

  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();

  // Match the request
  int val;
  String m;
  if (req.indexOf("/D1/0") != -1){
    m = "D1 is Low";
    digitalWrite(D1, 0);
  }
  else if (req.indexOf("/D1/1") != -1){
    m = "D1 is High";
    digitalWrite(D1, 1);
  }
  else if (req.indexOf("/D2/0") != -1){
    m = "D2 is Low";
    digitalWrite(D2, 0);
  }
  else if (req.indexOf("/D2/1") != -1){
    m = "D2 is High";
    digitalWrite(D2, 1);
  }
  else if (req.indexOf("/D3/0") != -1){
    m = "D3 is Low";
    digitalWrite(D3, 0);
  }
  else if (req.indexOf("/D3/1") != -1){
    m = "D3 is High";
    digitalWrite(D3, 1);
  }
  else {
    Serial.println("invalid request");
    client.stop();
    return;
  }



  client.flush();

  // Prepare the response
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<head><meta http-equiv=\"Access-Control-Allow-Origin\" content=\"*\"></head>\n<html>\r\n" +m;
  s += "</html>\n";

  // Send the response to the client
  client.print(s);
  delay(1);
  Serial.println("Client disonnected");

  // The client will actually be disconnected
  // when the function returns and 'client' object is detroyed
}
