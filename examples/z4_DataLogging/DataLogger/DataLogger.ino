#include <DS3231_Simple.h>

DS3231_Simple Clock;
#ifdef ARDUINO_ARCH_ESP32
  #define SAMPLEPIN A5
#else
  #define SAMPLEPIN A0
#endif

void setup() {
  
  
  Serial.begin(115200);  
  Serial.println();
  
  Clock.begin();
    
  // Erase the contents of the EEPROM
  Clock.setEepromAddress(0x50);
  Clock.formatEEPROM();
  
  // First we will disable any existing alarms
  Clock.disableAlarms();
  
  // And now add the alarm to happen every second
  Clock.setAlarm(DS3231_Simple::ALARM_EVERY_SECOND); 
  
  Serial.println(F("Logging value of analogRead(A1), enter any character to dump the log."));
  
}

static uint32_t timeout=0;
void loop() 
{ 
/*
  if(Clock.checkAlarms())
  {
    // Time to log a data point
    Clock.writeLog(analogRead(SAMPLEPIN));
    Serial.print('.');
  }
*/
  if(millis()-timeout>1000){ //every second do a reading
    Clock.writeLog(analogRead(SAMPLEPIN));
    timeout=millis();
    }
    
  if(Serial.available())
  {
    while(Serial.available()) Serial.read();
    dumpLog();
  }
}

void dumpLog()
{
  unsigned int loggedData;
  DateTime     loggedTime;
  
  // Note that reading a log entry also deletes the log entry
  // so you only get one-shot at reading it, if you want to do
  // something with it, do it before you discard it!
  unsigned int x = 0;
  while(Clock.readLog(loggedTime,loggedData))
  {
    if(x == 0)
    {
      Serial.println();
      Serial.print(F("Date,analogRead("));
      Serial.print(SAMPLEPIN,DEC);
      Serial.println(F(")"));
    }
    
    x++;
    Clock.printTo(Serial,loggedTime);
    Serial.print(',');
    Serial.println(loggedData);
  }
  Serial.println();
  Serial.print(F("# Of Log Entries Found: "));
  Serial.println(x);
  Serial.println();
}