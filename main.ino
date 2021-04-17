#include <ESP8266WiFi.h>

const char* ssid     = "xxxx";
const char* password = "xxxx";

WiFiServer server(80);
String header;

String outputState = "Desligado";
unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);
  delay(5000);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi conectado.");
  Serial.println("IP: ");
  Serial.println(WiFi.localIP());
  
  server.begin();
}

void loop() {
  
  WiFiClient client = server.available();
  
  if (client) {         

    String currentLine = "";                
    currentTime = millis();
    previousTime = currentTime;
    
    while (client.connected() && currentTime - previousTime <= timeoutTime) { 
      currentTime = millis();
      
      if (client.available()) {             
        char c = client.read();             
        header += c;
        
        if (c == '\n') {                    
          if (currentLine.length() == 0) {
            
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            if (header.indexOf("GET /on") >= 0) {
              Serial.println("LED_ON");
            } else if (header.indexOf("GET /off") >= 0) {
              Serial.println("LED_OFF");
            }
            
            break;
          } else { 
            currentLine = "";
          }
          
        } else if (c != '\r') {  
          currentLine += c;
        }
      }
    }
    
    header = "";
    client.stop();
  }
}
