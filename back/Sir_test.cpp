    #include <Arduino.h>
    #include <Preferences.h>

/***********************************************************************/
    #if defined(ESP32)
    #include <WiFi.h>
    #elif defined(ESP8266)
    #include <ESP8266WiFi.h>
    #endif
    #include <Firebase_ESP_Client.h>
    #include <addons/TokenHelper.h>
    #include <addons/RTDBHelper.h>
    String ssid = "S 20 FE";
    String password = "123456710";
    #define WIFI_SSID "S 20 FE"
    #define WIFI_PASSWORD "123456710"
    #define API_KEY "AIzaSyDt2Q1wUwFogJFAPLstSVgF9xT5UijgKYY"
    #define DATABASE_URL "https://wifi-crd-change-default-rtdb.firebaseio.com"
    #define USER_EMAIL "davidabhram47nrk@gmail.com"
    #define USER_PASSWORD "wifi12345"
    #include <EEPROM.h>
    #define EEPROM_SIZE 1
    FirebaseData fbdo;
    FirebaseAuth auth;
    FirebaseConfig config;
    Preferences preferences;
/***********************************************************************/
    int count = 0 ;
    String msg;
    int num = 0;
    String WIFI_SSID_1;
    String WIFI_PASSWORD_1;
    int F = 0;
/***********************************************************************/
    int Extract_integer_from_string(String x);
    void Connect_WiFi_base();
    void Connect_WiFi_New();
    void Firebase_Store(String PATH,String MSG);
    String Firebase_getString(String PATH);
    
    void write_Credentials(const String& ssid, const String& password);
    void read_Credentials(String &ssid, String &password);
    void read_F(int &F);
    void write_F(int F);
/***********************************************************************/
void setup()
    {
      Serial.begin(9600); 
      Serial.println("EEPROM TEST");
      EEPROM.begin(EEPROM_SIZE);
       
          
    }
/***********************************************************************/
void loop()
    {
      char x;
      if(Serial.available())
      {
        x=Serial.read();
        if(x=='a')
        {
          F=Serial.parseInt();
          EEPROM.write(0, F);
          EEPROM.commit();
          Serial.println("Stored data  = "+String(F));
        }
        else if(x=='b')
        {
             EEPROM.put(0, "myData");
        F= EEPROM.read(0);
        Serial.println(F);
        }
        else if(x=='c')
        {
           char gratitude[] = "Thank You Espressif!";
            EEPROM.writeString(1, gratitude);
            Serial.println("Stored data  = ");
        }
        else if(x=='d')
        {
          char gratitude[] = "Welcome to soften";
            EEPROM.writeString(1, gratitude);
            Serial.println("Stored data  = ");
        }
        else if(x=='e')
        {
          Serial.println(EEPROM.readString(1));
        }
      }
      delay(100);
    }
/***********************************************************************/
void Connect_WiFi_New()
{
          WiFi.disconnect();
          delay(800); 
          Serial.println("Connecting to New vWi-Fi"); 
          Serial.println ("SSID => " + ssid+"\nPass=>"+password);
          WiFi.begin(ssid, password);
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
          Firebase_Store("/STATUS","CONNECTED to new");
}
/***********************************************************************/
    void Connect_WiFi_base()
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
          Firebase_Store("/STATUS","CONNECTED_toBase");
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
void write_Credentials(const String& ssid, const String& password) 
{
    preferences.putString("ssid", ssid);
    preferences.putString("password", password);
}
/***********************************************************************/
void read_Credentials(String &ssid, String &password) 
{
    ssid = preferences.getString("ssid", "defaultSSID");       // "defaultSSID" if not found
    password = preferences.getString("password", "defaultPW"); // "defaultPW" if not found
}
/***********************************************************************/
void write_F(int F) 
{
    preferences.putInt("F", F);              // Store the integer F
}
/***********************************************************************/
void read_F(int &F) 
{
    F = preferences.getInt("F", 0);  // Default value 0 if not found
}
/***********************************************************************/