#include <dht.h>

#include <Adafruit_GFX.h>
#include <gfxfont.h>

#include <Adafruit_SSD1306.h>

#include <Wire.h> 
#include <SPI.h> 

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#define SSID "hi"                                   
#define PASS ""        

ESP8266WebServer server(80);

char led_status[200];
char rBuf[50];
char gBuf[50];
char bBuf[50];
String webSite;

 #define LEDR 16 
 #define LEDG 5 
 #define LEDB 4 

 #define dht_dpin 15

//OLED
 const int d0 = 0; 
 const int d1 = 2; 
 const int res = 14; 
 const int dc = 12; 
 const int cs = 13; 

 WiFiClient client;

 Adafruit_SSD1306 display(d1,d0,dc,res,cs); 

 dht DHT;//宣告物件

void turnoff(){
digitalWrite(LEDR, LOW);
    digitalWrite(LEDG, LOW);
    digitalWrite(LEDB, LOW);
}
void buildWeb(){
    webSite += "<html>";
    webSite+="<head>\n";
    webSite+="<meta charset='UTF-8'/>\n";
  webSite+="<title>Form Control RGB_LED</title>\n";
    webSite+="<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/1.11.3/jquery.min.js\"></script>";
    webSite+="<script>";
    webSite+="$(document).ready(function(){";
    webSite+="$(\"[type = range]\").change(function(){";
    webSite+="var redval = $(\"#red\").val();";
    webSite+="$(\"#red-text\").text(redval);";
    webSite+="var greenval = $(\"#green\").val();";
    webSite+="$(\"#green-text\").text(greenval);";
    webSite+="var blueval = $(\"#blue\").val();";
    webSite+="$(\"#blue-text\").text(blueval);";
    webSite+="});";
    webSite+="});";
    webSite+="</script>";
    webSite+="</head>\n";
    webSite += "<body>";
    webSite += "<h1>RGB LED Control form</h1>";
    webSite += "<form action=\"/form1\">";
    webSite += "<p>Red Color : <span id = \"red-text\">0</span></p>";
    webSite += "<input type=\"range\" id=\"red\" name=\"R-led\" min=\"0\" max=\"255\" value=\"0\">";
webSite += "<p>Green Color : <span id = \"green-text\">0</span></p>";
webSite += "<input type=\"range\" id=\"green\" name=\"G-led\" min=\"0\" max=\"255\" value=\"0\">";
webSite += "<p>Blue Color : <span id = \"blue-text\">0</span></p>";
webSite += "<input type=\"range\" id=\"blue\" name=\"B-led\" min=\"0\" max=\"255\" value=\"0\" ><br><br>";
webSite += "<input type=\"submit\" value=\"submit\"></form>";
webSite += "</body></html>";
}

void handleRoot(){
server.send(200,"text/html",webSite);
}

void ColorPicker(){
int red_color = server.arg("R-led").toInt();
int green_color = server.arg("G-led").toInt();
    int blue_color = server.arg("B-led").toInt();
  
  analogWrite(LEDR,red_color);
    analogWrite(LEDG,green_color);
    analogWrite(LEDB,blue_color);
    server.send(200,"text/html",webSite);
}
void handleNotFound(){
server.send(404,"text/html","404 Error!");
}

void setup() {  
  
  Serial.begin(115200);
  // put your setup code here, to run once:
  turnoff();
  buildWeb();
  WiFi.begin(SSID, PASS); 

  display.begin(SSD1306_SWITCHCAPVCC,0x3D);
  display.clearDisplay();
  display.setTextWrap(false);
  
  while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");

  delay(300);             //Let system settle
  Serial.println("Humidity and temperature\n\n");   
  delay(700);             
}

server.on("/",handleRoot);
server.on("/form1",ColorPicker);
server.onNotFound(handleNotFound);

server.begin();
Serial.println(WiFi.localIP());  

}
void loop() {
// put your main code here, to run repeatedly:
server.handleClient();  


DHT.read11(dht_dpin);//設定角位
Serial.print("Humidity = ");
Serial.print(DHT.humidity);
Serial.print("% ");
Serial.print("temperature = ");
Serial.print(DHT.temperature);
Serial.println("C ");
delay(1000); //每1000ms更新一次   

}



