#include <Arduino.h>


/***********************************************************************/
    #if defined(ESP32)
    #include <WiFi.h>
    #elif defined(ESP8266)
    #include <ESP8266WiFi.h>
    #endif
    #include <Firebase_ESP_Client.h>
    #include <addons/TokenHelper.h>
    #include <addons/RTDBHelper.h>
    #define WIFI_SSID "Embedded"
    #define WIFI_PASSWORD "Embedded54321"
    #define API_KEY "AIzaSyBUwmFxX4j66PN6sKQwSIScxEcdtEj2M98"
    #define DATABASE_URL "esp8266-8abda-default-rtdb.firebaseio.com"
    #define USER_EMAIL "chamchim546@gmail.com"
    #define USER_PASSWORD "chimcham12345"
    FirebaseData fbdo;
    FirebaseAuth auth;
    FirebaseConfig config;
/***********************************************************************/
    int count = 0 ;
    String msg;
    int num = 0;
/***********************************************************************/
    void Connect_WiFi();
    void Firebase_Store(String PATH,String MSG);
    String Firebase_getString(String PATH);
/***********************************************************************/
    void setup()
    {
         Connect_WiFi(); 
    }
/***********************************************************************/
    void loop()
    {
        if(Serial.available())
        {
            char x;
            x=Serial.read();
            if(x=='A')
            {
              Firebase_Store("/LED/DATA","ON");
              digitalWrite(2,LOW);             
            }
             else if(x=='B')
            {
               Firebase_Store("/LED/DATA","OFF");
               digitalWrite(2,HIGH); 
            }
            else if(x=='C')
            {
              msg = Firebase_getString("/LED/DATA");
              Serial.println(msg);
            }
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
    
/***********************************************************************/
/***********************************************************************/