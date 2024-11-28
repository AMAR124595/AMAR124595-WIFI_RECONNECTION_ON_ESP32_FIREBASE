    #include <Arduino.h>
    #include <EEPROM.h>
    #define EEPROM_SIZE 512
    #define SSID_ADDRESS 0    // Starting address for SSID
    #define PASS_ADDRESS 32   // Starting address for Password


/***********************************************************************/
    #if defined(ESP32)
    #include <WiFi.h>
    #elif defined(ESP8266)
    #include <ESP8266WiFi.h>
    #endif
    #include <Firebase_ESP_Client.h>
    #include <addons/TokenHelper.h>
    #include <addons/RTDBHelper.h>
    #define WIFI_SSID "S 20 FE"
    #define WIFI_PASSWORD "123456710"
    #define API_KEY "....................."
    #define DATABASE_URL "...................................."
    #define USER_EMAIL "....................."
    #define USER_PASSWORD "............."

    FirebaseData fbdo;
    FirebaseAuth auth;
    FirebaseConfig config;
/***********************************************************************/
    int count = 0 ;
    String msg;
    int num = 0;
    String WIFI_SSID_1;
    String WIFI_PASSWORD_1;
    int F = 0;
/***********************************************************************/
    int Extract_integer_from_string(String x);
    void Connect_WiFi();
    void Firebase_Store(String PATH,String MSG);
    String Firebase_getString(String PATH);
    void Connect_WiFi_1();
    void read_Credentials(char ssid[], char password[]);
    void write_Credentials(char ssid[], char password[]);
    //void read_F(int &F);
   // void write_F(int F);
/***********************************************************************/
    void setup()
    { 
      Serial.begin(9600);
      WiFi.disconnect();
      EEPROM.begin(EEPROM_SIZE);
      int key = 0;
      
      key = EEPROM.read(510);
      Serial.println(key);
      Serial.println("data retrived");
      delay(1000);
      if(key == 1)
            {
              
              Connect_WiFi_1();
            }
      else
            {
              Connect_WiFi();
              delay(1000);
              EEPROM.write(510, F);
              EEPROM.commit();
            }
    }
/***********************************************************************/
    void loop()
    {
      
      String CONNECT;
      WIFI_SSID_1 = Firebase_getString("/SSID");
      WIFI_PASSWORD_1 = Firebase_getString("/PASSWORD");
      Serial.println(WIFI_SSID_1);
      Serial.println(WIFI_PASSWORD_1);
      char ssid[32];
      char password[32];
          
          // Copy the content of the Strings into the char arrays
          WIFI_SSID_1.toCharArray(ssid, sizeof(ssid));
          WIFI_PASSWORD_1.toCharArray(password, sizeof(password));
          
          // Check the length of the SSID and password (excluding the null terminator)
          if (strlen(ssid) < 32 && strlen(password) < 32) 
          {
              write_Credentials(ssid, password);
              Serial.println("Data stored");
          } 
          else 
          {
              Serial.println("SSID or password is too long!");
          }

      CONNECT = Firebase_getString("/CONNECT");
      if(CONNECT == "ON")
      {
        esp_restart();
      }
    }
/***********************************************************************/
    void Connect_WiFi()
    {
          Serial.begin(9600);
          delay(100); 
          WiFi.disconnect();
          delay(800); 
          Serial.println("Connecting to Wi-Fi"); 
          WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
          Serial.print("Connecting to Wi-Fi");
          delay(100);
          while (WiFi.status() != WL_CONNECTED)
          {
            Serial.print(".");
            delay(300);
          }
          Serial.println();
          Serial.print("Connected with IP: ");
          Serial.println(WiFi.localIP());
          Serial.println();
          Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);
          config.api_key = API_KEY;
          auth.user.email = USER_EMAIL;
          auth.user.password = USER_PASSWORD;
          config.database_url = DATABASE_URL;
          config.token_status_callback = tokenStatusCallback;
          #if defined(ESP8266)
            fbdo.setBSSLBufferSize(2048 /* Rx buffer size in bytes from 512 - 16384 */, 2048 /* Tx buffer size in bytes from 512 - 16384 */);
          #endif
          Firebase.begin(&config, &auth);
          Firebase.reconnectWiFi(true);
          Firebase.setDoubleDigits(5);
          config.timeout.serverResponse = 10 * 1000;
          Firebase_Store("/STATUS","CONNECTED");
          F = 1;
    }
/***********************************************************************/
    void Firebase_Store(String PATH,String MSG)
    {
          Serial.print("Uploading data \" ");
          Serial.print(MSG);
          Serial.print(" \"  to the location \" ");
          Serial.print(PATH);
          Serial.println(" \"");
          Firebase.RTDB.setString(&fbdo, PATH, MSG);
          delay(50);
    }
/***********************************************************************/
    String Firebase_getString(String PATH)
    {
      String msg = (Firebase.RTDB.getString(&fbdo, PATH) ? fbdo.to<const char *>() : fbdo.errorReason().c_str());
      delay(50);
      return msg;
    }
/***********************************************************************/
  int Extract_integer_from_string(String x)
{
    int num=0,i;
    for(i=0;x[i]!='\0';i++)
    {
        num = num * 10 + (x[i] - 48);
    }
    return num;
   
}  
/***********************************************************************/
void write_Credentials(char ssid[], char password[]) 
{
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
/***********************************************************************/
void read_Credentials(char ssid[], char password[]) 
{
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
/***********************************************************************/
/*void write_F(int F) 
{
    preferences.putInt("F", F);              // Store the integer F
}*/
/***********************************************************************/
/*void read_F(int &F) 
{
    F = preferences.getInt("F", 0);  // Default value 0 if not found
}*/
/***********************************************************************/
void Connect_WiFi_1()
{
  Serial.begin(9600);
  char ssidRead[32];
  char passwordRead[32];
  read_Credentials(ssidRead, passwordRead);
  Serial.print("SSID: "); 
  Serial.println(ssidRead);
  Serial.print("Password: "); 
  Serial.println(passwordRead);

          Serial.begin(9600);
          delay(100); 
          WiFi.disconnect();
          delay(800); 
          Serial.println("Connecting to Wi-Fi_1"); 
          WiFi.begin(ssidRead, passwordRead);
          Serial.print("Connecting to Wi-Fi_NEW");
          delay(100);
          int X=0;
          while (WiFi.status() != WL_CONNECTED)
          { 
            Serial.print(".");
            delay(300);
            if(X==150)
            {
                Connect_WiFi();
            }
            Serial.print(X);
            X++;
          }
          Serial.println();
          Serial.print("Connected with IP: ");
          Serial.println(WiFi.localIP());
          Serial.println();
          Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);
          config.api_key = API_KEY;
          auth.user.email = USER_EMAIL;
          auth.user.password = USER_PASSWORD;
          config.database_url = DATABASE_URL;
          config.token_status_callback = tokenStatusCallback;
          #if defined(ESP8266)
            fbdo.setBSSLBufferSize(2048 /* Rx buffer size in bytes from 512 - 16384 */, 2048 /* Tx buffer size in bytes from 512 - 16384 */);
          #endif
          Firebase.begin(&config, &auth);
          Firebase.reconnectWiFi(true);
          Firebase.setDoubleDigits(5);
          config.timeout.serverResponse = 10 * 1000;
          Firebase_Store("/STATUS","CONNECTED_1");
}
/***********************************************************************/
