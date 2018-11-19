/* Derived from Ed Mallon's starter logging code.
C. Fastie modified to run without sleeping anything.
added BME280 sensor, added seconds to time readout
added unixtime
*/
#include <SdFat.h>
#include  <SPI.h>
#include <Wire.h>
#include <RTClib.h>               
#include <Adafruit_BMP085.h>

SdFat SD;
#define MOSIpin 11
#define MISOpin 12
RTC_DS1307 RTC;
#define DS1307_I2C_ADDRESS 0x68
char TmeStrng[] = "0000/00/00,00:00:00";     //19 ascii characters 
int x = 0;
int y = 0;  
void setup() {  
  Serial.begin(115200); // Open serial communications and wait for port to open:                        // start the i2c interface
  Wire.begin(8);
  Wire.begin(9);
  Wire.onReceive(receiveEvent);
  RTC.begin();                         // start the RTC 
  RTC.adjust(DateTime((__DATE__), (__TIME__)));    //sets the RTC to the computer time when the sketch is loaded
  
  Serial.print("Find SD card: ");
  if (!SD.begin(10)) {
    Serial.println("Card failed");
    return;
  }
  
  Serial.println(" SD card OK");   
  //â€”â€”â€”â€”-print a header to the data fileâ€”â€”â€”-
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  if (dataFile) {                     // if the file is available, write to it:
    dataFile.println("Nano Logger");
    dataFile.close();
  }
  else {
    Serial.println("file error");     // if the file isnâ€™t open, pop up an error:
  }
  
}                                     // end of setup
void loop() {
  

  delay(500);
    
    DateTime now = RTC.now();              //this reads the time from the RTC
    sprintf(TmeStrng, "%04d/%02d/%02d,%02d:%02d:%02d", now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second()); // [added seconds]

    Serial.print("RTC utc Time: ");
    Serial.print(now.unixtime());
    Serial.println();
    Serial.print("RTC time: ");
    Serial.println(TmeStrng);
    String dataString = "";                  //this erases the previous string
    dataString += TmeStrng;
    dataString += ",";    
    dataString += String(now.unixtime());
    dataString += ",";      
    dataString += String(x);
    dataString += ",";      
    dataString += String(y);
    
File dataFile = SD.open("datalog.txt", FILE_WRITE);  // if the file is available, write to it:
if (dataFile) {
  dataFile.println(dataString);
  if(dataFile.println(dataString))
  {
    Serial.println("Storing in sd card......"); 
  }
  dataFile.close();
}
else {
  Serial.println("file error");        // if the file isnâ€™t open, pop up an error:
}
 delay(3000);                          // write data every 3 seconds
}             
void receiveEvent(int bytes)
{
  x = Wire.read();
  y = Wire.read();
  Serial.print("Receiving......");
  Serial.println(x);
  Serial.println(y);
}
