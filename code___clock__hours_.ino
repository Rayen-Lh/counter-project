/*********
  Rayen Lahmar
  Complete project details web part
*********/

// Load Wi-Fi library
#include <WiFi.h>
#include <ds3231.h>
#include "Wire.h";
  #include "RTClib.h"
struct ts t;
RTC_DS3231 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// Replace with your network credentials
const char* ssid = "Airbox-CC7D";
const char* password = "CPUR2021";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;
// Assign input variables to GPIO pins
const int input12 = 12;
const int input26 = 26;
const int input25 = 25;
int i = 0 ;
int count[8] ={0,0,0,0,0,0,0,0}, ncomp = 0 , tt ;
int prod[26]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
String Lmt ="";
int lhour = 0;
unsigned long chrono = 0;
unsigned long attente = 60000ul;
// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);
  // Initialize the output variables as outputs
  pinMode(input12, INPUT);
  pinMode(input26, INPUT);
  pinMode(input25, INPUT);

    if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

   if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date &amp; time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date &amp; time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("..");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();

  DS3231_init(DS3231_CONTROL_INTCN);

  t.hour = 0;
  t.min = 0;
  t.sec = 0;
  t.mday = 1;
  t.mon = 1;
  t.year = 2022;

  DS3231_set(t);
}

void loop() {
   DS3231_get(&t);
  int state1 = digitalRead(input12);
  int state2 = digitalRead(input26);
  int state3 = digitalRead(input25);

  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();


            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta http-equiv='refresh' content='0.4'>");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<body><h1>TPS Web Server</h1>");
            //liste des réfs
            client.println("<body>");
            //style du tableau et du body de page html

            
            client.println("<style>");
            client.println("table, th, td {border:1px solid black;text-align: center;} body { background-color:cyan;text-align: center;} table {width:100%;}");
            client.println("</style>");
            client.println("<p> nombre des pieces semi finies</p>");
            client.println("<table style='widht:100%'>");
            client.println("<tr><th>premiere heure</th>");
            client.println("<td>");
            client.println(count[0]);
            client.println("</td></tr>");
            client.println("<tr><th>deuxieme heure</th>");
            client.println("<td>");
            client.println(count[1]);
            client.println("</td></tr>");
            client.println("<tr><th>troisieme heure</th>");
            client.println("<td>");
            client.println(count[2]);
            client.println("</td></tr>");
            client.println("<tr><th>quaterieme heure</th>");
            client.println("<td>");
            client.println(count[3]);
            client.println("</td></tr>");
            client.println("<tr><th>cinqieme heure</th>");
            client.println("<td>");
            client.println(count[4]);
            client.println("</td></tr>");
            client.println("<tr><th>sixieme heure</th>");
            client.println("<td>");
            client.println(count[5]);
            client.println("</td></tr>");
            client.println("<tr><th>septieme heure</th>");
            client.println("<td>");
            client.println(count[6]);
            client.println("</td></tr>");
            client.println("<tr><th>huitieme heure</th>");
            client.println("<td>");
            client.println(count[7]);
            client.println("</td></tr>");
            client.println("<tr><th>total articles</th>");
            client.println("<td>");
            client.println(tt);
            client.println("</td></tr>");
            client.println("</table>");
            client.println(); 
            client.println("</body></html>");
            // Web Page Heading
              
            // If the sensors got a result it will display it on the botton
              
                
            if ((state1 == 1 && state2 == 0 && state3 == 0) || (state1 == 0 && state2 == 1 && state3 == 0) ||
                (state1 == 1 && state2 == 1 && state3 == 0) || (state1 == 0 && state2 == 0 && state3 == 1) || (state1 == 1 && state2 == 0 && state3 == 1) ||
                (state1 == 0 && state2 == 1 && state3 == 1) || (state1 == 1 && state2 == 1 && state3 == 1)) {

                        if (millis() - chrono > 500) {
                chrono = millis();
                i++;
                tt++;
              if ( t.hour == 8 ) {
                t.hour = 7 ;
              }
              if ( t.hour != lhour ) {
                i = 0;
                lhour = t.hour;
              }
                delay(100);
              }
              count[t.hour] = i ;
                }
            //End of HTML page
            client.println("</body></html>");

            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }

}
