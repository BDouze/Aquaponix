
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Ucglib.h>

// Connect DS18B20 data pin to the following uC pin with a 4.7 k pullup resistor. 

#define ONE_WIRE_BUS 32 
//#define PIN_LED_EXT 4
#define PIN_LED_EXT 4

OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire);
byte isLedOn = 1;


// Waveshare 0.95inch OLED Module RGB Color 65K Coloreful 96*64 Pixel SPI Interface Curved/horizontal Pinheader with Chip Driver SSD1331 
// Pin 23 -> MOSI / DIN / SDI
// Pin 18 -> CLK/SCK
// Pin 19 -> RST
// Pin 13 -> D/C
// Pin 5  -> CS
// Ucglib_SSD1331_18x96x64_UNIVISION_SWSPI ucg(sclk, data, cd, cs, [reset])
// Ucglib_SSD1331_18x96x64_UNIVISION_HWSPI ucg(cd, cs, [reset])

Ucglib_SSD1331_18x96x64_UNIVISION_SWSPI ucg(/*sclk=*/ 18, /*data=*/ 23, /*cd=*/ 13 , /*cs=*/ 5, /*reset=*/ 19);
//Ucglib_SSD1331_18x96x64_UNIVISION_HWSPI ucg(/*cd=*/ 9 , /*cs=*/ 10, /*reset=*/ 8);

void setup() {
  // Open serial port for debug messages
  Serial.begin(115200);
  
  // Setup the i/o pins
  pinMode(PIN_LED_EXT, OUTPUT);

  //  pinMode (32, INPUT);

  sensors.begin(); 

  delay(500);
  //ucg.begin(UCG_FONT_MODE_TRANSPARENT);
  ucg.begin(UCG_FONT_MODE_SOLID);
  delay(500);
  ucg.clearScreen();
  

}

void switchLed() {
  if (isLedOn) {
    isLedOn = 0;
    digitalWrite(PIN_LED_EXT, LOW);     
  } else {
    isLedOn = 1;
    digitalWrite(PIN_LED_EXT, HIGH);     
  }
  
}

void loop() {
  switchLed();
  
  Serial.print(" Requesting temperatures..."); 
  sensors.requestTemperatures(); // Send the command to get temperature readings 
  float temperature = sensors.getTempCByIndex(0);
  Serial.print("DONE. "); 

  Serial.print("Temperature is: "); 
  Serial.print(temperature); 
  Serial.println(" °C"); 


  ucg.clearScreen();
  ucg.setRotate180();
  ucg.setFont(ucg_font_ncenR12_tr);
  ucg.setColor(255, 255, 255);
  //ucg.setColor(0, 255, 0);
  //ucg.setColor(1, 255, 0,0);
  
  ucg.setPrintPos(0,25);
  ucg.print(temperature);
  ucg.print(" C");
 
  delay(1000);
}




