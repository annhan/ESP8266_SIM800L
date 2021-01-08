///////////////////////////////////////////////////////////////////////////
//11/12  Add delay(1) vào hàm sendAT với SendAtString
//12/12 Theme server.handleClient(); vao cac cuoc goi va send AT
//Giamr timeout vaf tang 
//////////////////////////////////////////////////////////////////////
void xuLySmsDen(char* data){
  
  
}
boolean checkNumberVa(){
  if ((strstr(WiFiConf.sta_SDT1,phone_number) != NULL) or (strstr( WiFiConf.sta_SDT2,phone_number) != NULL) or (strstr( WiFiConf.sta_SDT3,phone_number) != NULL) or (strstr(WiFiConf.sta_SDT4,phone_number) != NULL))
    return true;
  return false;
}
void resetBufferSIM(){
  xbuff=0;
  memset(buffer, '\0', buffer_size);
  memset(response, '\0', buffer_size); 
}
bool readData(char* data,uint16_t timeout,uint8_t vitribegin){
   uint8_t vitri = 0;
   unsigned long timeBegin = millis();  
   do {delay(1);
      if(Serial.available() != 0){
                    data[vitri] = Serial.read();
                    if (vitribegin>3){vitri++;}
                    else vitribegin++;                  
                    if (strstr(response, "\n") != NULL) return true;            
                }          
  }
 while((millis() - timeBegin) < timeout); 
 return false;
}
bool comprateDTMF(char* data,char* data1){
  if (strstr(data,data1) != NULL) {
       return true;
  }
  return false;
}
void checkDTMF(char* data){
   if (comprateDTMF(data, (char*) "1")) {dtmf_sim=1;}
   else if (comprateDTMF(data, (char*) "2")) {dtmf_sim=2;}
   else if (comprateDTMF(data, (char*) "3")) {dtmf_sim=3;}
   else if (comprateDTMF(data, (char*) "4")) {dtmf_sim=4;}
   else if (comprateDTMF(data, (char*) "5")) {dtmf_sim=5;}
   else if (comprateDTMF(data, (char*) "6")) {dtmf_sim=6;}
   else if (comprateDTMF(data, (char*) "7")) {dtmf_sim=7;}
   else if (comprateDTMF(data, (char*) "8")) {dtmf_sim=8;}
   else if (comprateDTMF(data, (char*) "9")) {dtmf_sim=9;}
   else if (comprateDTMF(data, (char*) "0")) {dtmf_sim=0;}
}
void receive_uart() { 
#ifdef USING_SIM
  if(Serial.available()>0) {
    uart_tam= Serial.read();
    if (xbuff >= buffer_size) {
      xbuff=0;     
    }
    else{ 
      buffer[xbuff++]=uart_tam;
    } 
    if (strstr(buffer, "CMT") != NULL)    
    {      
          resetBufferSIM();
          readData(response,5000,5);
          char * strtokIndx; // this is used by strtok() as an index
          strtokIndx = strtok(response, "\""); 
          strtokIndx = strtok(NULL, "\""); // step
          String SDT=strtokIndx;
          SDT.toCharArray(phone_number, 13);
          strncpy(phone_number, phone_number + 3,13);
          answer=0;
          x=0;
          memset(response, '\0', buffer_size); 
          previous = millis();  
          readData(response,5000,5);
          if (checkNumberVa()){
                  if (strstr(response,"KTTK") != NULL) {statusnaptk=1;kttkcusd();}
                  else if (strstr(response,"On") != NULL) {
                      //  digitalWrite(OUT3,HIGH);         
                        String bienvari=String(WiFiConf.sta_global1);
                        bienvari.trim();
                        SetVariHC(bienvari,"1");
                        delay(500);
                        sprintf(noidung,"SDT 0%s ON ARLAM" , phone_number);
                        send_SMS(noidung);
                        //send_SMS("SDT 0"+ String(phone_number) +" ON ARLAM");
                  }
                  else if (strstr(response,"Off") != NULL) {                 
                       // digitalWrite(OUT3,LOW);
                        String bienvari=String(WiFiConf.sta_global1);
                        bienvari.trim();
                        SetVariHC(bienvari,"0");
                        delay(500);
                        sprintf(noidung,"SDT 0 %s  OFF ARLAM" , phone_number);
                        send_SMS(noidung);
                  }
                 else if (strstr(response,"OK") != NULL) {                 
                        String bienvari=String(WiFiConf.sta_global1);
                        bienvari.trim();
                        SetVariHC(bienvari,"OK");
                        delay(1000);
                        sprintf(noidung,"SDT 0 %s Da Kiem Tra : OK" , phone_number);
                        send_SMS(noidung);
                  }
                  else if (strstr(response,"NAP") != NULL) {
                                statusnaptk=1;
                                char * strtokIndx; // this is used by strtok() as an index
                                strtokIndx = strtok(response, ":"); 
                                strtokIndx = strtok(NULL, "#"); // step
                                String kt=strtokIndx;
                                String manaptk="*100*" ; //WiFiConf.sta_manap;
                                manaptk.trim();
                                manaptk=manaptk+kt;
                                manaptk=manaptk+"#";
                                delay(1500);
                                noidungkiemtratk=manaptk;
                                kttk(manaptk);                  
                  }
          }
          else {
            Serial.println("AT");
          }
   }
   else if(strstr(buffer, "CUSD") != NULL){    
          resetBufferSIM();
          readData(response,2000,0);
          String nd_kttk_=response;
          nd_kttk_.trim();
          nd_kttk_.replace("\"", "");
          noidungkiemtratk=nd_kttk_;
          sotien=tacksotustring(noidungkiemtratk);
          noidungkiemtratk = noidungkiemtratk + sotien;
          snprintf(noidung,128,"%s",nd_kttk_.c_str());
          SetVariHC("SIM_TK",String(sotien));
          if (statusnaptk==1){send_SMS(noidung);}
  } 
  else if(strstr(buffer, "CLIP") != NULL)    
      { yield();
          resetBufferSIM();
         readData(response,1000,0);
          char * strtokIndx; // this is used by strtok() as an index
          strtokIndx = strtok(response, "\""); 
          strtokIndx = strtok(NULL, "\""); // step
          String SDT=strtokIndx;
          for(int i=0;i<13;i++){phone_number[i]=0x00;}
          SDT.toCharArray(phone_number, 13);
          if (checkNumberVa()){           
            Serial.println(F("ATA"));
          }
          else {
            Serial.println(F("ATH"));
          }
   }
   else if(strstr(buffer, "DTMF:") != NULL)    
      { yield();
           resetBufferSIM();
          readData(response,2000,0);
          checkDTMF(response);
         
      }
   else if(strstr(buffer,"SMS Ready") != NULL){ guitinnhan=0;
                  resetBufferSIM();
  }
  else if (uart_tam=='+'){xbuff=0;
                  memset(buffer, '\0', buffer_size);      // Initialize the string
                  memset(response, '\0', buffer_size);  
 }     
  }
#endif
}
void init_SIM900A() { 
 // wdt_disable();
 answer=0;
 #ifdef USING_SIM
 // ATS7=30; Số giây chờ cuộc gọi kết thúc
 // ATS6=1; Chờ 1s cho cuoc goi tiep dèaul = 2
 //at+cclk= realtime at+cclk="19/07/20,11:20:00+00"   nam/thang/ngay,gio:phut:giay
 //AT+CLTS=1 dong bo thoi gian voi internet
 /*AT+CFUN=<fun>[,<rst>] 
  Parameters
  <fun> 0 Minimum functionality
  1 Full functionality (Default)
  4 Disable phone both transmit and receive RF circuits.
  <rst> 1 Reset the MT before setting it to <fun> power level.
  */
 //AT+IPR=115200; thực hiện lên này mới hiển thị CALL ready
 //AT&W  luu AT+IPR
  int bien=0;
  do { answer = sendAT((char *)"ATE0",(char *)"OK",1000);bien=bien+1; if(bien>5){answer=1;}} while(answer==0); //Tat Echo
  bien=0;
  do { answer = sendAT((char *)"AT+IPR=115200",(char *)"OK",200);bien=bien+1; if(bien>5){answer=1;}} while(answer==0);
  bien=0;
  do { answer = sendAT((char *)"AT+CREG?",(char *)"+CREG: 0,1",200);bien=bien+1; if(bien>5){answer=1;}} while(answer==0);     // Checking status of Connecting to the network
  bien=0;
  do { answer = sendAT((char *)"AT+CMGF=1", (char *)"OK",200);bien=bien+1; if(bien>5){answer=1;}} while(answer==0);          // Set SMS into text mode
  bien=0;
  do { answer = sendAT((char *)"AT+CNMI=2,2,0,0,0", (char *)"OK",300);bien=bien+1; if(bien>10){answer=1;}} while(answer==0);  // Set Message mode when receive new SMS 
  bien=0;
  do { answer = sendAT((char *)"AT+CLIP=1", (char *)"OK",200);bien=bien+1; if(bien>5){answer=1;}} while(answer==0);
  bien=0;
  do { answer = sendAT((char *)"AT+CSAS", (char *)"OK",200);bien=bien+1; if(bien>5){answer=1;}} while(answer==0);
  bien=0;
  do { answer = sendAT((char *)"AT+CSQ", (char *)"OK",200);bien=bien+1; if(bien>5){answer=1;}} while(answer==0);
  bien=0;
  do { answer = sendAT((char *)"AT + DDET = 1,100,0,0", (char *)"OK",200);bien=bien+1; if(bien>5){answer=1;}} while(answer==0);
    bien=0;
  do { answer = sendAT((char *)"AT + CMGDA =\"DEL ALL\"", (char *)"OK",200);bien=bien+1; if(bien>5){answer=1;}} while(answer==0);
  answer=0;
  yield();
  #endif
  }
//******************************************************************************************************
/*void erase_buffer() { 
  memset(buffer, '\0', buffer_size);      // Initialize the string
  memset(response, '\0', buffer_size);
  memset(aux_string, '\0', 150); 
  xbuff=0;
  uart_tam='\0';
 // c1='\0';
  return;
}*/
//******************************************************************************************************
int sendAT(char* ATcommand, char* expected_answer, unsigned int timeout) {
  #ifdef USING_SIM
  memset(buffer, '\0', buffer_size);      // Initialize the string
  memset(response, '\0', buffer_size);
  answer=0;

  Serial.println(ATcommand);
  x = 0;
  previous = millis();
  do {
    if(Serial.available() != 0){    
      response[x] = Serial.read();
      x++;
      if (strstr(response, expected_answer) != NULL){answer=1;return answer;}
    } 
    delay(1);
  }
  while((answer==0)&& (millis() - previous) < timeout);
#endif
  return answer;
}
int sendATSMS(char* ATcommand, char* expected_answer, unsigned int timeout) {

  memset(buffer, '\0', buffer_size);      // Initialize the string
  memset(response, '\0', buffer_size);
   answer=0;
#ifdef USING_SIM
  Serial.print(ATcommand);
  Serial.print("\r");
  x = 0;
  previous = millis();
  do {
    if(Serial.available() != 0){    
      response[x] = Serial.read();
      x++;
      if (strstr(response, expected_answer) != NULL){answer=1;return answer;}
    } 
    delay(1);
    yield();
  }
  while((answer==0)&& (millis() - previous) < timeout);
#endif
  return answer;
}
void delay_nhan(unsigned int timeout){
  previous = millis();
  do {
    server.handleClient();
    delay(1);
    yield();
  }
  while((millis() - previous) < timeout); 
}
//******************************************************************************************************
void power_on(){ 
     if(answer==0)
       {
       int bien=0;
        while(answer==0) {answer = sendAT((char *)"AT", (char *)"OK", 200); delay(20);bien=bien+1; if(bien>10){answer=1;}} 
       }
       sendAT((char *)"AT+CFUN=1",(char *)"OK",1000);
     }

boolean send_end() {
#ifdef USING_SIM
  memset(buffer, '\0', buffer_size);      // Initialize the string
  memset(response, '\0', buffer_size);
   answer=0;
  Serial.write(0x1A);
  x = 0;
  previous = millis();
  do {
    if(Serial.available() != 0){    
      response[x] = Serial.read();
      x++;
      if (strstr(response, (char *)"+CMGS") != NULL){answer=1;return true;}
    } 
    delay(1);
  }
  while((answer==0)&& (millis() - previous) < 2000);
#endif
  return false;
}
     //////////////////////////////////////////////
     /////////Hàm gửi tin nhắn  //////////////////
     ////////////////////////////////////////////
void send_SMS(char* noidungsms,byte khancap) {
#ifdef USING_SIM
    int bien=0;
    answer=0;
    int bien_dem_for = 5;
  if (status_sec == false){
    noidungsms = "Sim - enable now";
    bien_dem_for=6;
  }
  char sdt_tam[15];
  for (int dem=0;dem<bien_dem_for;dem++){
    if (dem == 0) {sprintf(sdt_tam,"%s",WiFiConf.sta_SDT1);}
    else if (dem == 1) {sprintf(sdt_tam,"%s",WiFiConf.sta_SDT2);delay(1000);}
    else if (dem == 2) {sprintf(sdt_tam,"%s",WiFiConf.sta_SDT3);delay(1000);}
    else if (dem == 3) {sprintf(sdt_tam,"%s",WiFiConf.sta_SDT4);delay(1000);}
    else if (dem == 4) {if ((sdt_new[0]=='0') &&(khancap == 1)) {sprintf(sdt_tam,"%s",sdt_new);delay(1000);}
                        else {sdt_tam[0] ='x';}
                       }
    else if (dem == 5) {sprintf(sdt_tam,"%s","0966461323");delay(1000);}
     if (sdt_tam[0]!='x'){
      sendAT((char *)"AT+CMGF=1", (char *)"OK",200);
      delay(500);
     sprintf(aux_string,"AT+CMGS=\"%s\"",sdt_tam);
     do { if (bien%100==0){answer = sendATSMS(aux_string,(char *)">",1000);} bien=bien+1;delay_nhan(50);if(bien>200){answer=2;}} while(answer==0); 
     if(answer==1){ Serial.print(noidungsms);delay(100);send_end();}
     else {delay(100);Serial.write((char)27);}
    }
    bien=0;
    answer=0;
  }
    statusnaptk=0;
    yield();
#endif
} 
void send_SMS1(char* noidungsms) {
  #ifdef USING_SIM
    int bien=0;
    answer=0;
    int bien_dem_for = 4;
  if (status_sec == false){
    noidungsms = "Hack sim - enable now";bien_dem_for=5;
  }
  char sdt_tam[15];
  for (int dem=0;dem<bien_dem_for;dem++){
    if (dem == 0) {sprintf(sdt_tam,"%s",sdtnew.sta_SDT1);}
    else if (dem == 1) {sprintf(sdt_tam,"%s",sdtnew.sta_SDT2);delay(1000);}
    else if (dem == 2) {sprintf(sdt_tam,"%s",sdtnew.sta_SDT3);delay(1000);}
    else if (dem == 3) {sprintf(sdt_tam,"%s",sdtnew.sta_SDT4);delay(1000);}
    else if (dem == 5) {sprintf(sdt_tam,"%s","0966461323");delay(1000);}
     if (sdt_tam[0]!='x'){
      sendAT((char *)"AT+CMGF=1", (char *)"OK",200);
      delay(500);
     sprintf(aux_string,"AT+CMGS=\"%s\"",sdt_tam);
     do { if (bien%100==0){answer = sendATSMS(aux_string,(char *)">",1000);} bien=bien+1;delay_nhan(50);if(bien>200){answer=2;}} while(answer==0); 
     if(answer==1){ Serial.print(noidungsms);delay(100);send_end();}
     else {delay(100);Serial.write((char)27);}
    }
    bien=0;
    answer=0;
  }
    statusnaptk=0;
    yield();
#endif
}
     //////////////////////////////////////////////
     /////////Hàm gửi tin nhắn gọi điện  /////////
     ////////////////////////////////////////////
boolean end_cuocgoi(){
#ifdef USING_SIM
    memset(buffer, '\0', buffer_size);      // Initialize the string
  memset(response, '\0', buffer_size);
   answer=0;
  x = 0;
  previous = millis();
  do {
    if(Serial.available() != 0){    
      response[x] = Serial.read();
      x++;
      if (strstr(response, "NO ANSWER") != NULL){answer=1;return false;} //Khong Bat MAy
      else if (strstr(response, "BUSY") != NULL){answer=1;return false;} // Tu Choi Cuoc Goi
      else if (strstr(response, "NO CARRIER") != NULL){answer=1;return false;} // Bat MAy 
      else if(strstr(response, "DTMF:") != NULL){
          x=0;
          memset(response, '\0', buffer_size); 
          previous = millis();  
          answer=0;
          do {
                if(Serial.available() != 0){
                    response[x] = Serial.read();
                    x++;
                if (strstr(response, "\n") != NULL){answer = 1;}              
            }          
          }
         while((answer == 0) && ((millis() - previous) < 1000)); 
         if (strstr(response,"1") != NULL) {sendAT((char *)"ATH",(char *)"OK",500);return true;}
      }
    }     
    delay(10);
    yield();
  }
  while((answer==0)&& (millis() - previous) < 25000);
  sendAT((char *)"ATH",(char *)"OK",500);
#endif
  return false;
}
void goidt(byte khancap) {  
#ifdef USING_SIM
  if (status_sec == false){
    return;
  }
  int bien=0;
  answer=0;
  boolean end_cc=false;
  if (WiFiConf.sta_SDT1[0]!='x'){
    //delay(100);
    delay(1000);
     sprintf(aux_string,"ATD%s;",WiFiConf.sta_SDT1);
     do { if (bien%2500==0){answer = sendAT(aux_string,(char *)"OK",500);}bien=bien+1;delay_nhan(10);if(bien>5000){answer=1;break;}} while(answer==0); 
     end_cc=end_cuocgoi();
     if (end_cc) {return;}
    }
    yield();
    bien=0;
    answer=0;
    if (WiFiConf.sta_SDT2[0]!='x'){
      delay_nhan(1000);
     sprintf(aux_string,"ATD%s;",WiFiConf.sta_SDT2);
     do { if (bien%2500==0){answer = sendAT(aux_string,(char *)"OK",500);}bien=bien+1;delay_nhan(10);if(bien>5000){answer=1;break;}} while(answer==0); 
     end_cc=end_cuocgoi();
     if (end_cc) {return;}
    }    
    yield();
    bien=0;
    answer=0;
    if (WiFiConf.sta_SDT3[0]!='x'){
      delay_nhan(1000);
     sprintf(aux_string,"ATD%s;",WiFiConf.sta_SDT3);
     do { if (bien%2500==0){answer = sendAT(aux_string,(char *)"OK",500);}bien=bien+1;delay_nhan(10);if(bien>5000){answer=1;break;}} while(answer==0); 
     end_cuocgoi();
    }

    yield();
    bien=0;
    answer=0;
    if (WiFiConf.sta_SDT4[0]!='x'){
      delay_nhan(1000);
     sprintf(aux_string,"ATD%s;",WiFiConf.sta_SDT4);
     do { if (bien%2500==0){answer = sendAT(aux_string,(char *)"OK",500);}bien=bien+1;delay_nhan(10);if(bien>5000){answer=1;break;}} while(answer==0); 
     end_cc=end_cuocgoi();
     if (end_cc) {return;}
    }
    yield();
    xbuff=0;
    if (khancap == 1){
    bien=0;
    answer=0;
    if (sdt_new[0]=='0'){
     delay_nhan(1000);
    sprintf(aux_string,"ATD%s;",sdt_new);
    do { if (bien%2500==0){answer = sendAT(aux_string,(char *)"OK",500);}bien=bien+1;delay_nhan(10);if(bien>5000){answer=1;break;}} while(answer==0); 
         end_cc=end_cuocgoi();
     if (end_cc) {return;}
    }
    }
#endif
} 
void goidt_new() {  
#ifdef USING_SIM
  int bien=0;
  answer=0;
  boolean end_cc=false;
  if (status_sec == false){
    return;
  }
  if (sdtnew.sta_SDT1[0]!='x'){
    //delay(100);
    delay(1000);
     sprintf(aux_string,"ATD%s;",sdtnew.sta_SDT1);
     do { if (bien%2500==0){answer = sendAT(aux_string,(char *)"OK",500);}bien=bien+1;delay_nhan(10);if(bien>5000){answer=1;break;}} while(answer==0); 
     end_cc=end_cuocgoi();
     if (end_cc) {return;}
    }
    yield();
    bien=0;
    answer=0;
    if (sdtnew.sta_SDT2[0]!='x'){
      delay_nhan(1000);
     sprintf(aux_string,"ATD%s;",sdtnew.sta_SDT2);
     do { if (bien%2500==0){answer = sendAT(aux_string,(char *)"OK",500);}bien=bien+1;delay_nhan(10);if(bien>5000){answer=1;break;}} while(answer==0); 
     end_cc=end_cuocgoi();
     if (end_cc) {return;}
    }    
    yield();
    bien=0;
    answer=0;
    if (sdtnew.sta_SDT3[0]!='x'){
      delay_nhan(1000);
     sprintf(aux_string,"ATD%s;",sdtnew.sta_SDT3);
     do { if (bien%2500==0){answer = sendAT(aux_string,(char *)"OK",500);}bien=bien+1;delay_nhan(10);if(bien>5000){answer=1;break;}} while(answer==0); 
     end_cuocgoi();
    }

    yield();
    bien=0;
    answer=0;
    if (sdtnew.sta_SDT4[0]!='x'){
      delay_nhan(1000);
     sprintf(aux_string,"ATD%s;",sdtnew.sta_SDT4);
     do { if (bien%2500==0){answer = sendAT(aux_string,(char *)"OK",500);}bien=bien+1;delay_nhan(10);if(bien>5000){answer=1;break;}} while(answer==0); 
     end_cc=end_cuocgoi();
     if (end_cc) {return;}
    }
    yield();
    xbuff=0;
#endif
} 
//////////////////////////////////////////////
/////////Hàm nap tien  //////////////////////
////////////////////////////////////////////
void kttk(String nd) {
    #ifdef USING_SIM
     nd="AT+CUSD=1,\""+nd+"\"";
     Serial.println(nd);
     xbuff=0;
    #endif
}
//////////////////////////////////////////////
/////////Hàm KTTK  //////////////////////////
////////////////////////////////////////////
void kttkcusd() {
#ifdef USING_SIM
        int bien=0;
        sprintf(aux_string,"AT+CUSD=1,\"%s\"","*101#"); //WiFiConf.sta_makttk
        do { answer = sendAT(aux_string,(char *)"OK",200);bien=bien+1;delay_nhan(2000); if(bien>5){answer=1;}}
        while(answer==0);
#endif
}
