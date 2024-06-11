#include "LedBlinking.h"
#include "Buzzer.h"
#include "Timer.h"
//#include "MotionSensor.h"
#include "ADXL345.h"
#include <WiFi.h>
#include <Adafruit_Sensor.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>

// Dane sieci Wi-Fi
const char* ssid = "...";
const char* password = "...";

// Dane bazy danych MySQL
const char* server = "...";  // Nazwa hosta serwera MySQL
char user[] = "...";                       // Użytkownik MySQL
char password_db[] = "...";                // Hasło MySQL
char database[] = "...";                   // Nazwa bazy danych

// Zmienne globalne do przechowywania poprzednich wartości akcelerometru
float prev_accelerometer_x = 0;
float prev_accelerometer_y = 0;
float prev_accelerometer_z = 0;
int isOkay = 1;
bool firstLoop = true;

/* Zmienne do obsługi czujnika ruchu
const int PIN_TO_SENSOR = 5; // GPIO5 pin connected to OUTPUT pin of sensor
int pinStateCurrent = LOW; // current state of pin
int pinStatePrevious = LOW; // previous state of pin*/
int motionDetected = 0;

WiFiClient client;
MySQL_Connection conn((Client *)&client);

void setup(void) 
{
  //Zaczynamy odczyt na porcie szeregowym z prędkością 9600 i czekamy na otwarcie połączenia
  Serial.begin(9600);
  while (!Serial);

  //Inicjalizujemy wszystkie urządzenia - buzzer, dioda, czujnik ruchu, akcelerometr
  setupBuzzer();
  setupLed();
  setupSensor();
  //setMotionSensor(PIN_TO_SENSOR);

  //Łaczymy się z Wi-Fi
  Serial.println("Łączenie z Wi-Fi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
  }
  Serial.println("\nPołączono z Wi-Fi");
  
  
  //Uzyskujemy adres IP z nazwy hosta
  IPAddress server_ip;
  if (!WiFi.hostByName(server, server_ip)) 
  {
    Serial.println("Błąd przy uzyskiwaniu adresu IP");
    return;
  }
  
  //Łączymy się z bazą danych
  Serial.println("Łączenie z bazą danych...");
  if (conn.connect(server_ip, 3306, user, password_db)) 
  {
    Serial.println("Połączono z bazą danych.");
    
    // Wybór bazy danych
    MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
    cur_mem->execute("USE ...");
    delete cur_mem;
  } 
  else 
  {
    Serial.println("Połączenie z bazą danych nieudane.");
  }
}

void loop(void) 
{
  if (conn.connected()) 
  {

    // Przypisanie wartości z akcelerometru do zmiennych
    float accelerometer_x = static_cast<float>(readSensorX().acceleration.x);
    float accelerometer_y = static_cast<float>(readSensorY().acceleration.y);
    float accelerometer_z = static_cast<float>(readSensorZ().acceleration.z);

    //Sprawdzamy który obieg pętli jest to, jeśli pierwszy to zapisujemy wartości, a jeśli kolejny to porównujemy wartości z akcelerometru
    if(firstLoop)
    {
      prev_accelerometer_x = accelerometer_x;
      prev_accelerometer_y = accelerometer_y;
      prev_accelerometer_z = accelerometer_z;
      firstLoop = false;
    }
    else
    {
      //Sprawdzamy czy wartości zmieniły się o więcej niż 0.3 wartości, 
      //jeśli tak to ustawiamy flagę isOkay na 0 i zapalamy diodę oraz uruchamiamy buzzera
      if (abs(accelerometer_x - prev_accelerometer_x) > 0.3 ||
          abs(accelerometer_y - prev_accelerometer_y) > 0.3 ||
          abs(accelerometer_z - prev_accelerometer_z) > 0.3)
      {
        isOkay = 0;
        setBuzzer();
        blinkLed();
      }
      else
      {
        isOkay = 1;
        turnOffLed();
      }

      //Zapisujemy bieżące wartości jako poprzednie do następnego porównania
      prev_accelerometer_x = accelerometer_x;
      prev_accelerometer_y = accelerometer_y;
      prev_accelerometer_z = accelerometer_z;
    }

    //Wyświetlamy wartości odczytanych z akcelerometru
    Serial.print("X: ");
    Serial.print(accelerometer_x);
    Serial.print(" Y: ");
    Serial.print(accelerometer_y);
    Serial.print(" Z: ");
    Serial.println(accelerometer_z);

    //Wyświetlamy wartość zmiennej isOkay
    Serial.print(" isOkay: ");
    Serial.println(isOkay);

    /*
    //Zapisujemy nowy odczyt, jako stary i sprawdzamy czy czujnik ruchu wykrył ruch
    pinStatePrevious = pinStateCurrent;
    pinStateCurrent = digitalRead(PIN_TO_SENSOR);

    if (pinStatePrevious == LOW && pinStateCurrent == HIGH) 
    {   
      //pin state change: LOW -> HIGH
      Serial.println("Motion detected!");
      motionDetected = 1;
    }
    else if (pinStatePrevious == HIGH && pinStateCurrent == LOW) 
    {   
      // pin state change: HIGH -> LOW
      Serial.println("Motion stopped!");
      motionDetected = 0;
    }
    */

    //Tworzymy zapytanie SQL
    char query[256];
    int motionDetected = 0;
    sprintf(query, "INSERT INTO odczyty (`TIME`, `ACC_X`, `ACC_Y`, `ACC_Z`, `MOTION`, `ISOKAY`) VALUES (NOW(), %f, %f, %f, %i, %i)", accelerometer_x, accelerometer_y, accelerometer_z, motionDetected, isOkay);

    //Wykonujemy zapytanie SQL
    MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
    cur_mem->execute(query);
    delete cur_mem;

    Serial.println("Dane wysłane do bazy danych.");
  } 
  else 
  {
    Serial.println("Nie połączono z bazą danych.");
  }

    //Ustalamy opóźnienie między wysyłaniem danych - 10000 ms - 10 sekund
    delay(10000);
}
