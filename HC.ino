#include <ArduinoJson.h>

/////////////////////////////////////////////////////////////////////////
void printWiFiConf(void) {
 // Serial.println(WiFiConf.sta_ssid);
  //Serial.println(String(WiFiConf.sta_ssid));
 // Serial.println(WiFiConf.module_id);
}

bool loadWiFiConf() {
  if (EEPROM.read(WIFI_CONF_START + 0) == wifi_conf_format[0] &&
      EEPROM.read(WIFI_CONF_START + 1) == wifi_conf_format[1] &&
      EEPROM.read(WIFI_CONF_START + 2) == wifi_conf_format[2] &&
      EEPROM.read(WIFI_CONF_START + 3) == wifi_conf_format[3])
  {
    for (unsigned int t = 0; t < sizeof(WiFiConf); t++) {
      *((char*)&WiFiConf + t) = EEPROM.read(WIFI_CONF_START + t);
    }
    printWiFiConf();
    return true;
  } else {
    return false;
  }
}
////////////////////////////////////
//Lưu thông số vào eeprom /////////
//////////////////////////////////
void saveWiFiConf(void) {
  for (unsigned int t = 0; t < sizeof(WiFiConf); t++) {
    EEPROM.write(WIFI_CONF_START + t, *((char*)&WiFiConf + t));
  }
  EEPROM.commit();
  printWiFiConf();
}
void setDefaultModuleId(char* dst) {
  uint8_t macAddr[WL_MAC_ADDR_LENGTH];
  WiFi.macAddress(macAddr);
  sprintf(dst, "%s%02x%02x", NAME_PREF, macAddr[WL_MAC_ADDR_LENGTH - 2], macAddr[WL_MAC_ADDR_LENGTH - 1]);
}
void resetModuleId(void) {
  uint8_t macAddr[WL_MAC_ADDR_LENGTH];
  WiFi.macAddress(macAddr);
  setDefaultModuleId(WiFiConf.module_id);
}

///////////////////////////////////////////////////////////////////////
///Ham Chuyen doi user:password qua Base64 cho giao tiep qua mang///////
////////////////////////////////////////////////////////////////////////
const char PROGMEM b64_alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

/* 'Private' declarations */
void a3_to_a4(unsigned char * a4, unsigned char * a3);
void a4_to_a3(unsigned char * a3, unsigned char * a4);
unsigned char b64_lookup(char c);

int base64_encode(char *output, char *input, int inputLen) {
  int j = 0;
  i = 0;
  int encLen = 0;
  unsigned char a3[3];
  unsigned char a4[4];

  while(inputLen--) {
    a3[i++] = *(input++);
    if(i == 3) {
      a3_to_a4(a4, a3);

      for(i = 0; i < 4; i++) {
        output[encLen++] = pgm_read_byte(&b64_alphabet[a4[i]]);
      }

      i = 0;
    }
  }

  if(i) {
    for(j = i; j < 3; j++) {
      a3[j] = '\0';
    }

    a3_to_a4(a4, a3);

    for(j = 0; j < i + 1; j++) {
      output[encLen++] = pgm_read_byte(&b64_alphabet[a4[j]]);
    }

    while((i++ < 3)) {
      output[encLen++] = '=';
    }
  }
  output[encLen] = '\0';
  return encLen;
}

int base64_decode(char * output, char * input, int inputLen) {
  int j = 0;
  i = 0;
  int decLen = 0;
  unsigned char a3[3];
  unsigned char a4[4];


  while (inputLen--) {
    if(*input == '=') {
      break;
    }

    a4[i++] = *(input++);
    if (i == 4) {
      for (i = 0; i <4; i++) {
        a4[i] = b64_lookup(a4[i]);
      }

      a4_to_a3(a3,a4);

      for (i = 0; i < 3; i++) {
        output[decLen++] = a3[i];
      }
      i = 0;
    }
  }

  if (i) {
    for (j = i; j < 4; j++) {
      a4[j] = '\0';
    }

    for (j = 0; j <4; j++) {
      a4[j] = b64_lookup(a4[j]);
    }

    a4_to_a3(a3,a4);

    for (j = 0; j < i - 1; j++) {
      output[decLen++] = a3[j];
    }
  }
  output[decLen] = '\0';
  return decLen;
}

int base64_enc_len(int plainLen) {
  int n = plainLen;
  return (n + 2 - ((n + 2) % 3)) / 3 * 4;
}

int base64_dec_len(char * input, int inputLen) {
  i = 0;
  int numEq = 0;
  for(i = inputLen - 1; input[i] == '='; i--) {
    numEq++;
  }
  return ((6 * inputLen) / 8) - numEq;
}

void a3_to_a4(unsigned char * a4, unsigned char * a3) {
  a4[0] = (a3[0] & 0xfc) >> 2;
  a4[1] = ((a3[0] & 0x03) << 4) + ((a3[1] & 0xf0) >> 4);
  a4[2] = ((a3[1] & 0x0f) << 2) + ((a3[2] & 0xc0) >> 6);
  a4[3] = (a3[2] & 0x3f);
}

 void a4_to_a3(unsigned char * a3, unsigned char * a4) {
  a3[0] = (a4[0] << 2) + ((a4[1] & 0x30) >> 4);
  a3[1] = ((a4[1] & 0xf) << 4) + ((a4[2] & 0x3c) >> 2);
  a3[2] = ((a4[2] & 0x3) << 6) + a4[3];
}

unsigned char b64_lookup(char c) {
  if(c >='A' && c <='Z') return c - 'A';
  if(c >='a' && c <='z') return c - 71;
  if(c >='0' && c <='9') return c + 4;
  if(c == '+') return 62;
  if(c == '/') return 63;
  return -1;
}


////////////////////////////////////
//do wifi /////////
//////////////////////////////////
void scanWiFi(void) {
  int founds = WiFi.scanNetworks();
  network_html = F("<ol>");
  for (i = 0; i < founds; ++i)
  {
    network_html += F("<li>");
    network_html += WiFi.SSID(i);
    network_html += F(" (");
    network_html += WiFi.RSSI(i);
    network_html += F(")");
    network_html += (WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*";
    network_html += F("</li>");
  }
  network_html += F("</ol>");
}
//*******************
// Hafm tach IP.Gateway
//*******************
void parseBytes(const char* str, char sep, byte* bytes, int maxBytes, int base) {
    for (i = 0; i < maxBytes; i++) {
        bytes[i] = strtoul(str, NULL, base);  // Convert byte
        str = strchr(str, sep);               // Find next separator
        if (str == NULL || *str == '\0') {
            break;                            // No more separators, exit
        }
        str++;                                // Point to next character after separator
    }
}
void parseBytes1(const char* str, char sep, int address, int maxBytes, int base) {
  for (int i = 0; i < maxBytes; i++) {
    if (address == 1) ip10[i] = strtoul(str, NULL, base);  // Convert byte
    else if (address == 2) gateway10[i] = strtoul(str, NULL, base);  // Convert byte
    else if (address == 3) subnet10[i] = strtoul(str, NULL, base);  // Convert byte
   // Serial.println(bytes[i]);
    str = strchr(str, sep);               // Find next separator
    if (str == NULL || *str == '\0') {
      break;                            // No more separators, exit
    }
    str++;                                // Point to next character after separator
  }
}

///////////////////////////////////////////
//Set Varuable toi HC2 ////////////////////
//////////////////////////////////////////
void SetVariHC(String vari,String giatri) {
  if (atoi(WiFiConf.choose_hc) == 0){
      WiFiClient client;
      HTTPClient http_hc;
      http_hc.begin(client,"http://" + String(WiFiConf.sta_iphc2) + "/api/globalVariables/"+vari);
      char* user=WiFiConf.sta_userhc;
      char* pass=WiFiConf.sta_passhc;
      http_hc.setAuthorization(user, pass);
      http_hc.addHeader("Content-Type", "application/json");
      http_hc.PUT("{\"value\":\"" + giatri + "\",\"invokeScenes\":true}");
     // http.writeToStream(&Serial);
      http_hc.end();
  }
}
void SetVariHC2Save(String vari,String giatri) { // Theo ma hoa
  if (atoi(WiFiConf.choose_hc) == 0){
      int vitricat=0;    
      char tamchar[128];
      sprintf(tamchar, "%s:%s|", WiFiConf.sta_userhc, WiFiConf.sta_passhc);
      for (byte tam=0;tam<sizeof(tamchar);tam++){
            if (tamchar[tam]=='|'){
                  vitricat=tam;
                  break;
            }
      }
      int encodedLen = base64_enc_len(vitricat-1);
      char encoded[encodedLen];
      base64_encode(encoded, tamchar, vitricat);
      WiFiClient client;
      HTTPClient http;
      http.begin(client,"http://" + String(WiFiConf.sta_iphc2) + "/api/globalVariables/"+vari);
      http.setAuthorization(encoded);
      http.addHeader("Content-Type", "application/json");
      http.PUT("{\"value\":\"" + giatri + "\",\"invokeScenes\":true}");
      http.writeToStream(&Serial);
      http.end();
  }
}
////////////////////////////////////
//Get thông số Hc2 /////////
//////////////////////////////////
void getHC() {
  if (atoi(WiFiConf.choose_hc) == 0){
  WiFiClient client;
  if (!client.connect(WiFiConf.sta_iphc2,80)) {  
     SerialHC2=F("connection failed");      
    return;
  }
  String url = F("/api/settings/info");
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: "+String(WiFiConf.sta_iphc2)+"\r\n" + 
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      //Serial.println(">>> Client Timeout !");
      client.stop();
      SerialHC2=F("HC2 not connected");
      return;
    }
  }
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
     if (line.length()>70) {
      line.replace("{","");
      line.replace("}","");
      line.replace("\""," ");
      line.remove(76);
      SerialHC2=line;}
    
  }
  }
}
bool processJson(char* message) {
  DynamicJsonDocument jsonBuffer(200);
  DeserializationError error = deserializeJson(jsonBuffer, message);
  bool trave = false;
  //JsonObject& root = jsonBuffer.parseObject(message);
  if (error) {return false;} 
  if (jsonBuffer.containsKey("value")) { 
    strlcpy(noidung, jsonBuffer["value"],sizeof(noidung));
    //noidung = jsonBuffer["value"].as<String>();
    trave =  true;
  }
  jsonBuffer.clear();
  return trave;
}
void getvariable_tosendSMS() {
      WiFiClient client;
      HTTPClient http_hc; //" + String(WiFiConf.sta_iphc2) + "
      http_hc.begin(client,"http://" + String(WiFiConf.sta_iphc2) + "/api/globalVariables/noidungtinnhan");
      char* user=WiFiConf.sta_userhc;
      char* pass=WiFiConf.sta_passhc;
      http_hc.setAuthorization(user, pass);
      http_hc.addHeader("Content-Type", "application/json");
      http_hc.GET();
      String payload = http_hc.getString();
      http_hc.end();
      char message[payload.length() + 1];
      payload.toCharArray(message, sizeof(message));
      if (processJson(message)) {
      guitinnhan = 1;
      }
}
