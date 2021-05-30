#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>

#define LANE1_RED 5 // D0
#define LANE1_GREEN 16 // D1

#define LANE2_RED 0 // D2
#define LANE2_GREEN 4 // D3

#define LANE3_RED 14 // D4
#define LANE3_GREEN 2 // D5

#define LANE4_RED 13 // D6
#define LANE4_GREEN 12 // D7

void greensignal(int lane){
  if(lane==0){
    digitalWrite(LANE1_RED, LOW);
    digitalWrite(LANE1_GREEN, HIGH);
    digitalWrite(LANE2_RED, HIGH);
    digitalWrite(LANE2_GREEN, LOW);
    digitalWrite(LANE3_RED, HIGH);
    digitalWrite(LANE3_GREEN, LOW);
    digitalWrite(LANE4_RED, HIGH);
    digitalWrite(LANE4_GREEN, LOW);

  }else if(lane==1){
    digitalWrite(LANE1_RED, HIGH);
    digitalWrite(LANE1_GREEN, LOW);
    digitalWrite(LANE2_RED, LOW);
    digitalWrite(LANE2_GREEN, HIGH);
    digitalWrite(LANE3_RED, HIGH);
    digitalWrite(LANE3_GREEN, LOW);
    digitalWrite(LANE4_RED, HIGH);
    digitalWrite(LANE4_GREEN, LOW);

  }else if(lane==2){
    digitalWrite(LANE1_RED, HIGH);
    digitalWrite(LANE1_GREEN, LOW);
    digitalWrite(LANE2_RED, HIGH);
    digitalWrite(LANE2_GREEN, LOW);
    digitalWrite(LANE3_RED, LOW);
    digitalWrite(LANE3_GREEN, HIGH);
    digitalWrite(LANE4_RED, HIGH);
    digitalWrite(LANE4_GREEN, LOW);

  }else if(lane==3){
    digitalWrite(LANE1_RED, HIGH);
    digitalWrite(LANE1_GREEN, LOW);
    digitalWrite(LANE2_RED, HIGH);
    digitalWrite(LANE2_GREEN, LOW);
    digitalWrite(LANE3_RED, HIGH);
    digitalWrite(LANE3_GREEN, LOW);
    digitalWrite(LANE4_RED, LOW);
    digitalWrite(LANE4_GREEN, HIGH);
    
  }
}

#define WIFI_SSID "MAJOR PROJECT 1
"
#define WIFI_PASSWORD "Shriya123"

#define FIREBASE_HOST "traffic-control-system-84dd6-default-rtdb.firebaseio.com"

#define FIREBASE_AUTH "Zaheaty7BL8ARUwTVpiwnyoKUysvffJ9QZBcASm8"

//Define FirebaseESP8266 data object
FirebaseData fbdo;

FirebaseJson json;

int getResponse(FirebaseData &data);

String parentPath = "/traffic";
String childPath[4] = {"/lane1/vehicle-count","/lane2/vehicle-count","/lane3/vehicle-count","/lane4/vehicle-count"};

void printError(FirebaseData &data);

void setup(){
    Serial.begin(115200);
    pinMode(LANE1_RED, OUTPUT);
    pinMode(LANE1_GREEN, OUTPUT);
    pinMode(LANE2_RED, OUTPUT);
    pinMode(LANE2_GREEN, OUTPUT);
    pinMode(LANE3_RED, OUTPUT);
    pinMode(LANE3_GREEN, OUTPUT);
    pinMode(LANE4_RED, OUTPUT);
    pinMode(LANE4_GREEN, OUTPUT);
    
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED){
      Serial.print(".");
      delay(300);
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();
  
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    Firebase.reconnectWiFi(true);
  
    //Set the size of WiFi rx/tx buffers in the case where we want to work with large data.
    fbdo.setBSSLBufferSize(1024, 1024);
  
    //Set the size of HTTP response buffers in the case where we want to work with large data.
    fbdo.setResponseSize(1024);
  
    //Set database read timeout to 1 minute (max 15 minutes)
    Firebase.setReadTimeout(fbdo, 1000 * 60);
    //tiny, small, medium, large and unlimited.
    //Size and its write timeout e.g. tiny (1s), small (10s), medium (30s) and large (60s).
    Firebase.setwriteSizeLimit(fbdo, "tiny");
}



void loop(){
  int vehicle_count=0;
  for(int i=0;i<4;i++){
    if (Firebase.getInt(fbdo, parentPath+childPath[i])){
      vehicle_count = getResponse(fbdo);
    }else{
      printError(fbdo);
    }
    long timer = vehicle_count*5000;
    Serial.println("------------------------------");
    Serial.print("In the LANE-"+String(i+1)+" the vehicle-count is ");
    Serial.println(i+1);
    Serial.print("and the duration of the traffic light set is ");
    Serial.println(timer);
    Serial.println("------------------------------");
    greensignal(i);
    delay(timer);
  }
}

void printError(FirebaseData &data){
  Serial.println("------------------------------------");
  Serial.println("FAILED");
  Serial.println("REASON: " + fbdo.errorReason());
  Serial.println("------------------------------------");
}

int getResponse(FirebaseData &data){
  if (data.dataType() == "int")
    return data.intData();
  else
    return 100;
}
