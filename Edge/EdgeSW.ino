#include <ArduinoJson.h>
#include <ArduinoHttpClient.h>
#include <WiFiNINA.h>
#include <Seeed_BMP280.h>
#include <Wire.h>
#include <Arduino_LSM6DS3.h>

#define START 20
#define BUZZER 21
#define LEDS 13
#define TRIG_PIN 16
#define ECHO_PIN 17
#define HALLSENSOR 15
#define ARRAY_DIMENSION 1000//max value is 3000

int offsetTime, totalTime;
bool change = true;
bool stateInt = true;

/***************** SFR05 variables *********************************/
double compressions = 0;
unsigned long duration = 0;
int past = 0, prev = 0, next = 0;

/***************** IMU variables **********************************/
double X, Y, Z;
float gx, gy, gz;
float ax, ay, az;
float angleXMax = 0, angleYMax = 0, angleZMax = 0, angleX, angleY, angleZ;
double t, d;
float angleXoffset = 0, angleYoffset = 0, angleZoffset = 0;

/***************** barometer instance and variable ****************/
BMP280 bmp280;
float pressure = 0;
double altitudeArray[ARRAY_DIMENSION]; //double altitudeArray[] = {22,32,43,54};
int arrayCounter = 0;

/***************** led variables **********************************/
bool led = HIGH;
int count = 0;
int ledTime = 0;

/***************** hall sensor variable **************************/
int magnet = 0;
float distance = 0;
float wheelCirc = 2.2608; //wheel cirocnference
float actualSpeed = 0.1;
bool state = false;
int dt = 0, t1 = 0;
double speedArray[ARRAY_DIMENSION]; //double speedArray[] = {1,2,3,4};

/***************** Wifi and Measurify Settings *******************/
char ssid[] = "Paco";
char pass[] = "slackline";
char serverAddress[] = "students.atmosphere.tools";  // server address
int port = 80;
WiFiClient wifi;
HttpClient client = HttpClient(wifi, serverAddress, port);
int status = WL_IDLE_STATUS;
StaticJsonDocument<600> doc; //document for json parsing
String token;
String feature[] = {"bike_space_travelled", "bike_fork_compression", "bike_slope_angle", "bike_lean_angle", "bike_altitude", "bike_speed"}; //features


void setup() {
  Serial.begin(9600);
  delay(3000);
  Serial.println("Checking hardware");
  
  //barometer and IMU connection check
  if (!bmp280.init())
        Serial.println("Device not connected or broken!");
  Serial.println("Barometer: OK!");
  if (!IMU.begin()) 
    Serial.println("Failed to initialize IMU!");
  Serial.println("IMU: OK!");

  
  //user interface setup
  pinMode(BUZZER, OUTPUT);
  pinMode(LEDS, OUTPUT);
  pinMode(START, INPUT_PULLUP);
  digitalWrite(START, HIGH);
  Serial.println("Buzzer: OK!");
  Serial.println("Led: OK!");
  Serial.println("Start switch: OK!");
  digitalWrite(LEDS, HIGH);
  digitalWrite(BUZZER, HIGH);
  delay(200);
  digitalWrite(BUZZER, LOW);
  delay(200);
  
  //ultrasonic sensor SFR05 setup
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  Serial.println("SFR05: OK!");
  
  //hall sensor setup
  pinMode(HALLSENSOR, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(HALLSENSOR), ISR, FALLING);
  Serial.println("Hall: OK!");
  digitalWrite(LED_BUILTIN, HIGH);
  
  //wait until user change switch state and start the ride
  stateInt = digitalRead(START);
  change = stateInt;
  while(stateInt == change){
    Serial.println("Waiting to start!");
    stateInt = digitalRead(START);
    delay(1000);
  }
  //starting IMU calibration
  Serial.println("IMU calibration");
  IMUcalibration(); //calculate the offset of the IMU attached on the bike
  digitalWrite(LED_BUILTIN, LOW);
  //buzzer start cominucation
  for(int i = 0; i < 3; i ++){
    digitalWrite(BUZZER, HIGH);
    delay(200);
    digitalWrite(BUZZER, LOW);
    delay(200);
  }
  digitalWrite(BUZZER, HIGH);
  delay(600);
  digitalWrite(BUZZER, LOW);

}

void loop() {
  offsetTime = millis();
  
  while(stateInt != change){
    stateInt = digitalRead(START);
    delay(50);
  }
  delay(100);
  while((stateInt == change) && (arrayCounter < ARRAY_DIMENSION)){
  
    stateInt = digitalRead(START); //check switch state
    
    /***************** SFR05 distance detection *****************/
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    duration = pulseIn(ECHO_PIN, HIGH);
    (next == prev)?past = past:past = prev;
    prev = next;
    if(((duration/29/2)-next) >= 2){
      next = (duration/29/2);
    }
    if(!duration)
     Serial.println("Warning: no pulse from sensor"); 
    else if((next > prev) && (prev < past)){
        compressions ++;
    }
    /*Serial.print(next);
    Serial.print('\t');
    Serial.println(compressions);*/
    
    /***************** IMU angles *****************/
    if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable()) {
      IMU.readAcceleration(gx, gy, gz);
      IMU.readGyroscope(ax, ay, az);
      t = pow(IMU.gyroscopeSampleRate(), -1);
    }
    
    X = atan(gy / sqrt(pow(gx, 2) + pow(gz, 2))) * 57.2957795130823;
    Y = atan(gx / sqrt(pow(gy, 2) + pow(gz, 2))) * 57.2957795130823;
    Z = atan(gz / sqrt(pow(gy, 2) + pow(gx, 2))) * 57.2957795130823;

    //complementary filter
    angleX = (0.98 * (angleX + t * ax) + 0.02 * X) - angleXoffset;
    angleY = (0.98 * (angleY + t * ay) + 0.02 * Y) - angleYoffset;
    angleZ = (0.98 * (angleZ + t * ay) + 0.02 * Z) - angleZoffset;
    
    (angleX > angleXMax)?angleXMax = angleX:angleXMax = angleXMax;
    (angleY > angleYMax)?angleYMax = angleY:angleYMax = angleYMax;
    (angleZ > angleZMax)?angleZMax = angleZ:angleZMax = angleZMax;
    
    Serial.print(angleX);
    Serial.print("\t");
    Serial.print(angleY);
    Serial.print("\t");
    Serial.println(angleZ);
    
    /***************** Led blinking and barometer and speed values*****************/
    
    if(((millis() - ledTime) >= 1000) && (arrayCounter < ARRAY_DIMENSION)) { //if one second passed, so blink and put values into array
      digitalWrite(LEDS, led);
      led = !led;
      //Serial.println(led);
      /***************** barometer altitude calculation *****************/
      pressure = bmp280.getPressure();
      altitudeArray[arrayCounter] = bmp280.calcAltitude(pressure);
      //Serial.println(altitudeArray[arrayCounter]);
      /***************** speed array filling ****************************/
      speedArray[arrayCounter] = actualSpeed;
      arrayCounter ++;
      ledTime = millis();
    }
    delay(10);
  }
  Serial.println(arrayCounter);
  digitalWrite(LEDS, HIGH);
  totalTime = (millis() - offsetTime)/1000;
  for(int i = 0; i < 4; i ++){
    digitalWrite(BUZZER, HIGH);
    delay(100);
    digitalWrite(BUZZER, LOW);
    delay(100);
  }
  /***************** Send data from Arduino to Measurify  *****************/

  //WiFi connection and Measurify login
  WiFiConnection(); //create a connection betwin Arduino Nano and local wifi
  token = POSTlogin(); //make a post login request to measurify server
  delay(3000);
  Serial.println();
  
  Serial.println("Inviando i dati a measurify...");
  POSTsamples(token, arrayCounter, speedArray, altitudeArray, totalTime, distance/1000, angleXMax, angleYMax, compressions + 0.1);
  //POSTsamples(token, 4, speedArray, altitudeArray, 189, 34, 32, 78, 12.3);
  digitalWrite(BUZZER, HIGH);
  delay(1000);
  digitalWrite(BUZZER, LOW);
  /*Serial.println("Finito!");
  Serial.print("Distanza: ");
  Serial.println(distance);
  Serial.print("Compressioni: ");
  Serial.println(compressions);
  Serial.print("Angle max: ");
  Serial.println(angleXMax);
  Serial.println("Altitude and speed array value");
  for(int i = 0; i < 500; i ++){
    Serial.print(altitudeArray[i]);
    Serial.print('\t');
    Serial.println(speedArray[i]);
  }*/
  while(true);
}

///////// POST login request /////////

String POSTlogin() {
  
  Serial.println("Making POST login request");
  Serial.println();
  String postData = "{\"username\":\"smartbike-monitor-user-username\",\"password\":\"smartbike-monitor-user-password\"}"; //body

  client.beginRequest();
  client.post("/v1/login");
  client.sendHeader("Content-Type", "application/json");
  client.sendHeader("Content-Length", postData.length());
  client.sendHeader("Host", "students.atmosphere.tools");
  client.sendHeader("Connection: keep-alive");
  client.beginBody();
  client.print(postData);
  client.endRequest();
  
  // read the status code and body of the response
  int statusCode = client.responseStatusCode();
  String responseToken = client.responseBody();
  
  DeserializationError error = deserializeJson(doc, responseToken);
  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
  }

  String token = doc["token"]; //get token from body response
  
  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(responseToken);

  return token;
}


///////// POST measure request /////////

void POSTsamples(String token, int arrayCounter, double speedArray[], double altitudeArray[], int totalTime, float distance, float angleXMax, float angleYMax, double compressions) {
  
  Serial.println("Making POST measure request");
  String samples = "[[";
  for(int i = 0; i < arrayCounter; i ++){
    if(i != 0)
      samples += ",";
    samples += String(speedArray[i]);
  }
  samples += "],[";
  for(int i = 0; i < arrayCounter; i ++){
    if(i != 0)
      samples += ",";
    samples += String(altitudeArray[i]);
  }
  samples += "]," + String(totalTime) + "," + String(distance) + "," + String(angleXMax) + "," + String(angleYMax) + "," + String(compressions) + "]";
  
  String postData = "{\"thing\": \"mybike\", \"feature\": \"bike_run\", \"device\": \"bike-monitor\", \"samples\": [ { \"values\":" + samples + "} ]}";
  Serial.println(postData);
  client.beginRequest();
  client.post("/v1/measurements");
  client.sendHeader("Content-Type", "application/json");
  client.sendHeader("Content-Length", postData.length());
  client.sendHeader("Host", "students.atmosphere.tools");
  client.sendHeader("Connection: keep-alive");
  client.sendHeader("Authorization", token);
  client.beginBody();
  client.print(postData);
  client.endRequest();

  // read the status code and body of the response
  int statusCode = client.responseStatusCode();
  String responseAPI = client.responseBody();

  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(responseAPI);
}

///////// GET measure request /////////

void GETmeasure(String token) {
  Serial.println("Making GET measure request");

  client.beginRequest();
  client.get("/v1/measurements?filter={\"thing\":\"mybike\"}&limit=10&page=1");
  client.sendHeader("Host", "students.atmosphere.tools");
  client.sendHeader("Connection: keep-alive");
  client.sendHeader("Authorization", token);
  client.endRequest();

  // read the status code and body of the response
  int statusCode = client.responseStatusCode();
  String responseAPI = client.responseBody();

  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(responseAPI);
}

///////// create a WiFi connection /////////

void WiFiConnection() {
  
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid); // print the network name (SSID);

    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);
  }

  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}

///////// Interrupt Service Routin hall effect sensor /////////

void ISR() {
  /***************** wheel cycle detection *****************/
  !state;
  distance += wheelCirc; //adding circonference value to total space travelled
  dt = millis() - t1; //calculate time betwenn wheel rotations
  actualSpeed = (wheelCirc / dt)*3600; //calculate speed Km/h
  t1 = millis();
}

void IMUcalibration(){

  for(int i = 1; i <= 30; i ++){
    if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable()) {
        IMU.readAcceleration(gx, gy, gz);
        IMU.readGyroscope(ax, ay, az);
        t = pow(IMU.gyroscopeSampleRate(), -1);
      }
      
      X = atan(gy / sqrt(pow(gx, 2) + pow(gz, 2))) * 57.2957795130823;
      Y = atan(gx / sqrt(pow(gy, 2) + pow(gz, 2))) * 57.2957795130823;
      Z = atan(gz / sqrt(pow(gy, 2) + pow(gx, 2))) * 57.2957795130823;
      
      angleXoffset += 0.98 * (angleX + t * ax) + 0.02 * X;
      angleYoffset += 0.98 * (angleY + t * ay) + 0.02 * Y;
      angleZoffset += 0.98 * (angleZ + t * ay) + 0.02 * Z;
      delay(10);
  }
  angleXoffset = angleXoffset/30;
  angleYoffset = angleYoffset/30;
  angleZoffset = angleZoffset/30;
}
