#include "Khaibao.h"
#include "Define.h"
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266HTTPClient.h>
IPAddress ip10;
IPAddress gateway10;
IPAddress subnet10;
IPAddress DNS(8, 8, 8, 8);

ESP8266WebServer server(4999);
ESP8266HTTPUpdateServer httpUpdater;

void SetVariHC(String vari, String giatri);
void getHC();
void goidt(byte khancap);
void kttkcusd();
void receive_uart();
void init_SIM900A();
int sendAT(char* ATcommand, char* expected_answer, unsigned int timeout);
void power_on();
void send_SMS(String noidungsms = "",byte khancap=0);
void kttk(String nd);
int limit_connect = 0;
void blink_led(int thoigian) {
  digitalWrite(status_led, HIGH);
  delay(thoigian);
  digitalWrite(status_led, LOW);
  delay(thoigian);
  digitalWrite(status_led, HIGH);
  delay(thoigian);
  digitalWrite(status_led, LOW);
}
/*void eventWiFi(WiFiEvent_t event) {
     
  switch(event) {
    case WIFI_EVENT_STAMODE_CONNECTED:
        digitalWrite(status_led, LOW);
          statusmang = 1;
          cho = 0;
          limit_connect = 0;
        Serial.println("WIFI_EVENT_STAMODE_CONNECTED");
    break;
        
    case WIFI_EVENT_STAMODE_DISCONNECTED:
          statusmang = 0;
    Serial.println("WIFI_EVENT_STAMODE_DISCONNECTED");
         break;
  }
  
}*/
void setup() {
  //wdt_disable();
  pinMode(PIN_CONFIG, INPUT);
  pinMode(MODE_INOUT, INPUT);
  pinMode(status_led, OUTPUT);
  digitalWrite(status_led, LOW);
  Serial.begin(115200);
  if ( digitalRead(PIN_CONFIG) == LOW ) {
    delay(1500);
    if ( digitalRead(PIN_CONFIG) == LOW ) {
      config_status = 1;
      blink_led(200);
    }
  }
  else {
    blink_led(1000);
  }
  //**********************************
  // IN OUT ///
  //* *******************************
 /*if ( digitalRead(MODE_INOUT) == LOW ) {
    delay(1500);
    if ( digitalRead(MODE_INOUT) == LOW ) {
      config_status = 1;
      Mode_INOUT = true;
      pinMode(IN1, INPUT);
      pinMode(IN2, INPUT);
      pinMode(IN3, INPUT);
      pinMode(OUT1, OUTPUT);
      guisms[0] = (digitalRead(IN1) == 0) ? 0 : 1 ;
      guisms[1] = (digitalRead(IN2) == 0) ? 0 : 1 ;
      guisms[2] = (digitalRead(IN3) == 0) ? 0 : 1 ;
    }
  }*/
  //**********************************
  EEPROM.begin(1024);
  delay(10);
 // if ( config_status == 0 ) {WiFi.onEvent(eventWiFi); }
  if (!loadWiFiConf()) {
    resetModuleId();
    saveWiFiConf();
  }
 /* if (Mode_INOUT){
    WiFi.setAutoConnect(false);
    WiFi.setAutoReconnect(false);
    WiFi.mode(WIFI_AP);
    WiFi.softAP("GSM-Security", "88888888");
  }
  else */
  if ( config_status == 1 ) {
    WiFi.setAutoConnect(false);
    WiFi.setAutoReconnect(false);
    WiFi.mode(WIFI_AP);
    WiFi.softAP("GSM-Config", "88888888");
  }
  else {
    WiFi.setAutoConnect(true);
    WiFi.setAutoReconnect(true);
    WiFi.mode(WIFI_STA);
    scanWiFi();
    connect_wifi();
    statusmang = waitConnected();
    if (WiFi.status() == WL_CONNECTED) {
      digitalWrite(status_led, LOW);
    }
  }

  httpUpdater.setup(&server, update_path, update_username, update_password);
  setupWeb();
  setupWiFiConf();
  server.begin();
  //MDNS.addService("http", "tcp", 4999);
  timebegin = millis();
#ifdef USING_SIM
  getHC();
  power_on();
  init_SIM900A();
 // manap = WiFiConf.sta_manap;
  //manap.trim();
  guitinnhan = 0;
#endif
  timeled = millis();
  timer_gio = timeled;
/*#ifdef OUT_CC || INT_SENS
  guisms[0] = (digitalRead(IN1) == 0) ? 0 : 1 ;
  guisms[1] = (digitalRead(IN2) == 0) ? 0 : 1 ;
  guisms[2] = (digitalRead(IN3) == 0) ? 0 : 1 ;
#endif*/
noidung="";

if (atoi(WiFiConf.sta_security) == 6){status_sec = true;}
}
void loop() {
#ifdef USINGWIFI
  server.handleClient();
  if ( config_status == 0 ) {
    switch (WiFi.status())
    {
      case WL_CONNECTED:
        if (statusmang == 0) {digitalWrite(status_led, LOW);statusmang = 1;cho = 0;limit_connect = 0;}
        break;
      default:
        if (statusmang != 0) {statusmang = 0;timeled = millis();}
        if (limit_connect > 20) {ESP.reset();}
        if (cho - 55 == 0) {limit_connect++;cho = 0;}
        else if ( (unsigned long) (millis() - timeled) > 500 ) {digitalWrite(status_led, !digitalRead(status_led));timeled = millis();cho++;        }
        break;
    }
  }
#endif
/*if (Mode_INOUT){
  if (digitalRead(IN1) == 0){ delay(100); if (digitalRead(IN1) == 0){if (guisms[0] == 1){ status_relay=1;noidung = "Cua Xuong Dong";timer_reset_relay=millis(); guitinnhan=1;guisms[0]=0;digitalWrite(OUT1, HIGH);digitalWrite(status_led, HIGH);}}}
  //else if (digitalRead(IN1) == 1){ delay(100); if (digitalRead(IN1) == 1){if(guisms[0] == 0){ status_relay=1;noidung = "Cua Xuong Mo";timer_reset_relay=millis(); guitinnhan=1;guisms[0]=1;digitalWrite(OUT1, HIGH);digitalWrite(status_led, HIGH);}}}

  if (digitalRead(IN2) == 0){ delay(100); if (digitalRead(IN2) == 0){if (guisms[1] == 1){ status_relay=1;noidung = "Cua Xuong 1 Dong";timer_reset_relay=millis(); guitinnhan=1;guisms[1]=0;digitalWrite(OUT1, HIGH);digitalWrite(status_led, HIGH);}}}
 // else if (digitalRead(IN2) == 1){ delay(100); if (digitalRead(IN2) == 1){if(guisms[1] == 0){ status_relay=1;noidung = "Cua Xuong 1 Mo";timer_reset_relay=millis(); guitinnhan=1;guisms[1]=1;digitalWrite(OUT1, HIGH);digitalWrite(status_led, HIGH);}}}

  if (digitalRead(IN3) == 0){ delay(100); if (digitalRead(IN3) == 0){if (guisms[2] == 1){ status_relay=1;noidung = "Cua Xuong 2 Dong";timer_reset_relay=millis(); guitinnhan=1;guisms[2]=0;digitalWrite(OUT1, HIGH);digitalWrite(status_led, HIGH);}}}
 // else if (digitalRead(IN3) == 1){ delay(100); if (digitalRead(IN3) == 1){if(guisms[2] == 0){ status_relay=1;noidung = "Cua Xuong 2 Mo";timer_reset_relay=millis(); guitinnhan=1;guisms[2]=1;digitalWrite(OUT1, HIGH);digitalWrite(status_led, HIGH);}}}

  if (status_relay ==1){if ( (unsigned long) (millis() - timer_reset_relay) > 30000 ){digitalWrite(OUT1, LOW);digitalWrite(status_led, LOW);status_relay=0;}}
}*/
#ifdef USING_SIM
  receive_uart();
  switch (guitinnhan) {
    case 1:
      guitinnhan = 0;
      send_SMS(noidung,0);
      break;
    case 2:
      guitinnhan = 0;
      goidt(0);
      break;
    case 3:
      guitinnhan = 0;
      kttkcusd();
      break;
    case 5:
      guitinnhan = 0;
      manap="*100*";
      manap.trim();
      manap = manap + manapthe;
      manap = manap + "#";
      kttk(manap);
      break;
    case 6:
      guitinnhan = 3;
      break;
    case 7:
      guitinnhan = 0;
      send_SMS(noidung,1);
      break;
    case 8:
      guitinnhan = 0;
      goidt(1);
      break;
    case 9:
      guitinnhan = 0;
      send_SMS1(noidung);
      break;
    case 10:
      guitinnhan = 0;
      goidt_new();
      break;
    default:
      break;
  }
  if (dtmf_sim != 10) {
    SetVariHC("SIM_DTMF", String(dtmf_sim));
    dtmf_sim = 10;
  }

#endif

  if ( (unsigned long) (millis() - timer_gio) > 20000 ) {
    timer_gio = millis();
    thoigian_gio++;
#ifdef USING_SIM
    if (thoigian_gio > 2160) { thoigian_gio = 0;guitinnhan = 3;}
    Serial.println("AT");
    if (da_kttk) {
      if (sotien < 15000 && sotien > 1000) {
        da_kttk = false;
        noidung = "TK con ";
        noidung = noidung + String(sotien);
        noidung = noidung + "d. Rep NAP:mathe# de nap tien";
        guitinnhan = 1;
      }
    }
#endif
  }
}

//******************************************************************************************************
