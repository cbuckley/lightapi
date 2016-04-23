#include <WebServer.h>

/* Light API */
#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#include <Ethernet.h>
#include "WS2812_Definitions.h"

#define PIN 4
#define LED_COUNT 300
char r = 0,g = 0,b = 0;
Adafruit_NeoPixel leds = Adafruit_NeoPixel(LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);

WebServer webserver("/set", 80);
static uint8_t mac[6] = { 0x02, 0xAA, 0xBB, 0xCC, 0x00, 0x22 };

void setup()
{
  leds.begin();  // Call this to start up the LED strip.
  //clearLEDs();   // This function, defined below, turns all LEDs off...
  leds.show();   // ...but the LEDs don't actually update until you call this.
  Serial.begin(9600);
  while (!Serial) {
    ;
  }
  webserver.setDefaultCommand(&rgbCmd);
  webserver.begin();
  Ethernet.begin(mac);
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());

}


//WS2812B
void loop()
{
  webserver.processConnection();
}


void rgbCmd(WebServer &server, WebServer::ConnectionType type, char *, bool)
{
  if (type == WebServer::POST)
  {
    bool repeat;
    char name[16], value[16];
    do
    {
      repeat = server.readPOSTparam(name, 16, value, 16);
      if (strcmp(name, "red") == 0)
      {
        r = strtoul(value, NULL, 10);
      }
      if (strcmp(name, "green") == 0)
      {
        g = strtoul(value, NULL, 10);
      }
      if (strcmp(name, "blue") == 0)
      {
        b = strtoul(value, NULL, 10);
      }
      
    } while (repeat);
    server.httpSeeOther("/set");
    colorAll(leds.Color(r, g, b)); // Blue
    int ledpattern[3][3] = {{r,g,b},{r,g,b},{r,g,b}};

    return;
  }
  server.httpSuccess();
   if (type == WebServer::GET)
  {
    P(message) = 
      "<!DOCTYPE html><html><head>"
        "<title>RGB Test</title>"
        "<link href='http://ajax.googleapis.com/ajax/libs/jqueryui/1.8.16/themes/base/jquery-ui.css' rel=stylesheet />"
        "<script src='http://ajax.googleapis.com/ajax/libs/jquery/1.6.4/jquery.min.js'></script>"
        "<script src='http://ajax.googleapis.com/ajax/libs/jqueryui/1.8.16/jquery-ui.min.js'></script>"
        "<style> body { background: white; } #red, #green, #blue { margin: 10px; } #red { background: #f00; } #green { background: #0f0; } #blue { background: #00f; } </style>"
        "<script>"
      

        "function changeRGB(event, ui) { var id = $(this).attr('id'); if (id == 'red') $.post('/set', { red: ui.value } ); if (id == 'green') $.post('/set', { green: ui.value } ); if (id == 'blue') $.post('/set', { blue: ui.value } ); } "
        "$(document).ready(function(){ $('#red, #green, #blue').slider({min: 0, max:255, change:changeRGB}); });"
      
        "</script>"
      "</head>"
      "<body style='font-size:62.5%;'>"
        "<div id=red></div>"
        "<div id=green></div>"
        "<div id=blue></div>"
      "</body>"
      "</html>";
    server.printP(message);
  }
}
void colorPixels(int **) {
  for(
}
void colorAll(uint32_t c) {
  for(uint16_t i=0; i<leds.numPixels(); i++) {
    leds.setPixelColor(i, c);
  }
  leds.show();
}
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<leds.numPixels(); i++) {
    leds.setPixelColor(i, c);
    leds.show();
    delay(wait);
  }
}
