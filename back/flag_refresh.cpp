#include <EEPROM.h>
#define EEPROM_SIZE 512
void setup() 
{
EEPROM.begin(EEPROM_SIZE);
Serial.begin(9600);
}

void loop() 
{
              
  if(Serial.available())
    {
        char x = Serial.read();
        int F=0;
        if (x=='a')
        {
              EEPROM.write(510, F);
              EEPROM.commit();
              Serial.println("data stored");
        }
        if (x=='b')
        {
          int key;
          key = EEPROM.read(510);
          Serial.println(key);
          Serial.println("data REtrived");
        }

     }
}