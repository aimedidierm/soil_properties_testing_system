#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

const char* ssid = "Nimrod";
const char* password = "nimurodi@1997";
String serverName = "http://143.110.228.128/drink-vending-machine/update.php";
void setup() {
  Serial.begin(115200); 

  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void loop() {
  // Send an HTTP POST request depending on timerDelay
  
    //Check WiFi connection status
    String req = Serial.readStringUntil('\n');
    if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;
      
      http.begin(client, serverName); 
      http.addHeader("Content-Type", "application/x-www-form-urlencoded"); 
String   httpRequestData = req;
int      httpResponseCode = http.POST(httpRequestData);
      
      if (httpResponseCode>0) {
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
      }
      // Free resources
      http.end();
    }
  }
