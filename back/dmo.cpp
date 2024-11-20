#include <EEPROM.h>
#include<Arduino.h>
#define EEPROM_SIZE 512
#define SSID_ADDRESS 0    // Starting address for SSID
#define PASS_ADDRESS 32   // Starting address for Password

void setup() {
    Serial.begin(115200);
    EEPROM.begin(EEPROM_SIZE);

    // Example: Writing credentials
    char ssid[] = "MOTO";           // Use a character array instead of String
    

    // Example: Reading credentials
    
    // Connect to Wi-Fi here with retrieved SSID and password
}

void writeCredentials(char ssid[], char password[]) {
    // Write SSID to EEPROM
    for (int i = 0; ssid[i] != '\0'; i++) {
        EEPROM.write(SSID_ADDRESS + i, ssid[i]);
    }
    EEPROM.write(SSID_ADDRESS + strlen(ssid), '\0');  // Null terminator

    // Write Password to EEPROM
    for (int i = 0; password[i] != '\0'; i++) {
        EEPROM.write(PASS_ADDRESS + i, password[i]);
    }
    EEPROM.write(PASS_ADDRESS + strlen(password), '\0');  // Null terminator

    EEPROM.commit();
}

void readCredentials(char ssid[], char password[]) {
    // Read SSID from EEPROM
    for (int i = 0; i < 32; i++) {
        ssid[i] = EEPROM.read(SSID_ADDRESS + i);
        if (ssid[i] == '\0') {
            break;  // Stop reading when null terminator is encountered
        }
    }

    // Read Password from EEPROM
    for (int i = 0; i < 32; i++) {
        password[i] = EEPROM.read(PASS_ADDRESS + i);
        if (password[i] == '\0') {
            break;  // Stop reading when null terminator is encountered
        }
    }
}

void loop() 
{
    if(Serial.available())
    {
        char x = Serial.read();
        if (x=='a')
        {
            char ssidRead[32];
            char passwordRead[32];
            readCredentials(ssidRead, passwordRead);
            Serial.print("SSID: "); Serial.println(ssidRead);
            Serial.print("Password: "); Serial.println(passwordRead);

        }
        else if (x=='b')
        {
            char password[] = "123456789";  // Use a character array instead of String
    if (strlen(ssid) < 32 && strlen(password) < 32) {
        writeCredentials(ssid, password);
    } else {
        Serial.println("SSID or password is too long!");
    }
        }

    }
}
