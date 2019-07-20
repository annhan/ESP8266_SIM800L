//GET /set_smsnew?1=0966461323&2=0966461323&3=0966461323&4=0966461323&s=csdsdsd 0x0D0x0A0x0D0x0A
//GET /set_callnew?1=0966461323&2=0966461323&3=0966461323&4=0966461323 0x0D0x0A0x0D0x0A
///set_security?button=6
//sendsmshcl
////////////////////////////////////
//chờ kết nối mạng /////////
//////////////////////////////////
int waitConnected(void) {
  int wait = 0;
  while ( wait < 20 ) {
    if (WiFi.status() == WL_CONNECTED) {
      return (1);
    }
    //yield();
    delay(700);
    wait++;
  }
  return (0);
}

void printIP(void) {
}

void setupWiFiConf(void) {
  server.on("/wifi_conf", []() {
    String content = FPSTR(header); content += FPSTR(begin_title);
    String   content1 = F("<p>Wifi Connected: ");
    content1 += WiFiConf.sta_ssid;
    content1 += F("</br>IP: ");
    content1 += ipStr;
    content1 += F(" ( ");
    content1 += WiFiConf.module_id;
    content1 += F(".local");
    content1 += F(" )</p>");
    content1 += F("<p>");
    content1 += FPSTR(form_method_get);
    content1 += F("set_wifi_conf'>");
    content1 += FPSTR(label_html);
    content1 += F("'ssid' class=\"req\">SSID : </label><input name='ssid' class=\"txt\" id='ssid' maxlength=32 value=\"") ;
    content1 += String(WiFiConf.sta_ssid) ;
    content1 +=  F("\"");
    content1 +=  FPSTR(br_html);
    content1 += FPSTR(label_html);
    content1 += F("'pwd' class=\"req\">PASS : </label> <input type='password' class=\"txt\" name='pwd' id='pwd' value=");
    content1 += String(WiFiConf.sta_pwd);
    content1 += FPSTR(br_html);
    content1 += FPSTR(label_html);
    content1 += F("'ip' class=\"req\">IP : </label> <input name='ip' class=\"txt\" id='ip' value=");
    content1 += String(WiFiConf.sta_ip) ;
    content1 +=  FPSTR(br_html);
    content1 += FPSTR(label_html);
    content1 += F("'gateway' class=\"req\">GATEWAY : </label> <input  name='gateway' class=\"txt\" id='gateway' value=") ;
    content1 += String(WiFiConf.sta_gateway) ;
    content1 += FPSTR(br_html);

    content1 += F("<li><label for='subnet' class=\"req\">SUBNET : </label> <input  name='subnet' class=\"txt\" id='subnet' value=");
    content1 +=  String(WiFiConf.sta_subnet);
    content1 += FPSTR(br_html);
    content += "Wifi Setup";
    content += FPSTR(title_html);

    content += F("<h1>Wifi Setup</h1>");
    content += content1;
    content += F("<input type='submit' id=\"submitbtn\" value='OK' onclick='return confirm(\"Change?\");'></form>");

    content += F(" </p>");
    content += network_html;
    content += FPSTR(_bodyhtml);
    server.send(200, send_html, content);

  });
  server.on("/set_wifi_conf", []() {
    String new_ssid = server.arg("ssid");
    String new_pwd = server.arg("pwd");
    String new_ip = server.arg("ip");
    String new_gateway = server.arg("gateway");
    String new_subnet = server.arg("subnet");
    if (new_ssid.length() > 0) {
      new_ssid.toCharArray(WiFiConf.sta_ssid, sizeof(WiFiConf.sta_ssid));
      new_pwd.toCharArray(WiFiConf.sta_pwd, sizeof(WiFiConf.sta_pwd));
      new_ip.toCharArray(WiFiConf.sta_ip, sizeof(WiFiConf.sta_ip));
      new_gateway.toCharArray(WiFiConf.sta_gateway, sizeof(WiFiConf.sta_gateway));
      new_subnet.toCharArray(WiFiConf.sta_subnet, sizeof(WiFiConf.sta_subnet));
      saveWiFiConf();
      server.send(200, send_html, "OK");
    } else {
      server.send(200, send_html, "Not OK");
    }
    delay(1000);
    ESP.reset();
  });
  server.on("/sdt_conf", []() {
    String content = FPSTR(header); content += FPSTR(begin_title);
    String   content1 = F("<p>Wifi Connected: ");
    content1 += WiFiConf.sta_ssid;
    content1 += F("<p>");
    content1 += FPSTR(form_method_get);
    content1 += F("set_sdt_conf'>");
    content1 += FPSTR(label_html);
    content1 += F("'SDT1' class=\"req\">Phone 1: </label> <input  name='SDT1' class=\"txt\" id='SDT1' value=");
    content1 += String(WiFiConf.sta_SDT1);
    content1 += FPSTR(br_html);
    content1 += FPSTR(label_html);
    content1 += F("'SDT2' class=\"req\">Phone 2: </label> <input name='SDT2' class=\"txt\" id='SDT2'value=");
    content1 += String(WiFiConf.sta_SDT2);
    content1 += FPSTR(br_html);
    content1 += FPSTR(label_html);
    content1 += F("'SDT3' class=\"req\">Phone 3: </label> <input  name='SDT3' class=\"txt\" id='SDT3' value=");
    content1 += String(WiFiConf.sta_SDT3);
    content1 += FPSTR(br_html);
    content1 += FPSTR(label_html);
    content1 += F("'SDT4' class=\"req\">Phone 4: </label> <input  name='SDT4' class=\"txt\" id='SDT4' value=");
    content1 += String(WiFiConf.sta_SDT4);
    content1 += FPSTR(br_html);
    content += "mIOTLAB - Phone Conf";
    content += FPSTR(title_html);
    content += F("<h1>Phone Conf</h1>");
    content += content1;
    content += F("<input type='submit' id=\"submitbtn\" value='OK' onclick='return confirm(\"Change?\");'></form>");
    content += F("<li>Pls, Check 'x' for phone number not using");
    content += F(" </p>");
    //content += network_html;
    content += FPSTR(_bodyhtml);
    server.send(200, send_html, content);
  });
  server.on("/set_sdt_conf", []() {
    String new_SDT1 = server.arg("SDT1");
    String new_SDT2 = server.arg("SDT2");
    String new_SDT3 = server.arg("SDT3");
    String new_SDT4 = server.arg("SDT4");
    if (new_SDT1 == "") {
      new_SDT1 = "x";
    }
    if (new_SDT2 == "") {
      new_SDT2 = "x";
    }
    if (new_SDT3 == "") {
      new_SDT3 = "x";
    }
    if (new_SDT4 == "") {
      new_SDT4 = "x";
    }
    new_SDT1.toCharArray(WiFiConf.sta_SDT1, sizeof(WiFiConf.sta_SDT1));
    new_SDT2.toCharArray(WiFiConf.sta_SDT2, sizeof(WiFiConf.sta_SDT2));
    new_SDT3.toCharArray(WiFiConf.sta_SDT3, sizeof(WiFiConf.sta_SDT3));
    new_SDT4.toCharArray(WiFiConf.sta_SDT4, sizeof(WiFiConf.sta_SDT4));
    saveWiFiConf();
    server.send(200, send_html, "OK");
    //delay(500);
    //ESP.reset();
  });
  server.on("/tinnhan", []() {
    //IPAddress ip = WiFi.localIP();
    String content = FPSTR(header); content += FPSTR(begin_title);
    content += F("SMS");
    content += FPSTR(title_html);
    content += F("<h1>SMS Test</h1>");
    content += F("<p>");
    content += FPSTR(form_method_get);
    content += F("set_noidung'><li><label for='text' class=\"req\" >Content : </label><input name='text' class=\"txt\" id='text' maxlength=32  ><br /><br />");
    content += F("<li><input type='submit' id=\"submitbtn\" value='Test' onclick='return confirm(\"Send ?\");'></form>");
    content += FPSTR(_bodyhtml);
    server.send(200, send_html, content);
  });
  server.on("/set_noidung", []() {
    noidung = server.arg("text");
    guitinnhan = 1;
    server.send(200, send_html, "ok");
  });
  server.on("/cuocgoi", []() {
    String content = FPSTR(header); content += FPSTR(begin_title);
    content += F("CALL");
    content += FPSTR(title_html);
    content += F("<h1>Call Function</h1>");
    content += F("<p>");
    content += FPSTR(form_method_get);
    content += F("set_call'>");
    content += F("<li><input type='submit'  id=\"submitbtn\" value='Test' onclick='return confirm(\"Check CALL ?\");'></form>");
    content += FPSTR(_bodyhtml);
    server.send(200, send_html, content);
  });

  server.on("/set_call", []() {
    // noidung = server.arg("text");
    guitinnhan = 2;
    server.send(200, send_html, "OK");
  });
  ////////////////////////////
  // OUT COntrol ////////////
#ifdef OUT_CC
  server.on("/OUT1", []() {
    out = 1;
    server.send(200, send_html, "ok");
  });
  server.on("/OUT2", []() {
    out = 2;
    server.send(200, send_html, "ok");
  });
  server.on("/OUT3", []() {
    out = 3;
    server.send(200, send_html, "ok");
  });
#endif
  ///////////////////////
  server.on("/set_ktk", []() {
    noidung = server.arg("text");
    guitinnhan = 3;
    server.send(200, send_html, "ok");
  });
  server.on("/sendsmshcl", []() {
    getvariable_tosendSMS();
    server.send(200, send_html, "ok");
  });
  server.on("/set_noidung1", []() {
    noidung = server.arg("text");
    String new_phone = server.arg("newphone");
    new_phone.toCharArray(sdt_new, sizeof(sdt_new));
    guitinnhan = 7;
    server.send(200, send_html, "OK");
  });
  server.on("/set_smsnew", []() {
    noidung = server.arg("s");
    String new_SDT1 = server.arg("1");
    String new_SDT2 = server.arg("2");
    String new_SDT3 = server.arg("3");
    String new_SDT4 = server.arg("4");
    if (new_SDT1 == "") {
      new_SDT1 = "x";
    }
    if (new_SDT2 == "") {
      new_SDT2 = "x";
    }
    if (new_SDT3 == "") {
      new_SDT3 = "x";
    }
    if (new_SDT4 == "") {
      new_SDT4 = "x";
    }
    new_SDT1.toCharArray(sdtnew.sta_SDT1, sizeof(sdtnew.sta_SDT1));
    new_SDT2.toCharArray(sdtnew.sta_SDT2, sizeof(sdtnew.sta_SDT2));
    new_SDT3.toCharArray(sdtnew.sta_SDT3, sizeof(sdtnew.sta_SDT3));
    new_SDT4.toCharArray(sdtnew.sta_SDT4, sizeof(sdtnew.sta_SDT4));
    guitinnhan = 9;
    server.send(200, send_html, "OK");
  });
  server.on("/set_callnew", []() {
    String new_SDT1 = server.arg("1");
    String new_SDT2 = server.arg("2");
    String new_SDT3 = server.arg("3");
    String new_SDT4 = server.arg("4");
    if (new_SDT1 == "") {
      new_SDT1 = "x";
    }
    if (new_SDT2 == "") {
      new_SDT2 = "x";
    }
    if (new_SDT3 == "") {
      new_SDT3 = "x";
    }
    if (new_SDT4 == "") {
      new_SDT4 = "x";
    }
    new_SDT1.toCharArray(sdtnew.sta_SDT1, sizeof(sdtnew.sta_SDT1));
    new_SDT2.toCharArray(sdtnew.sta_SDT2, sizeof(sdtnew.sta_SDT2));
    new_SDT3.toCharArray(sdtnew.sta_SDT3, sizeof(sdtnew.sta_SDT3));
    new_SDT4.toCharArray(sdtnew.sta_SDT4, sizeof(sdtnew.sta_SDT4));
    guitinnhan = 10;
    server.send(200, send_html, "OK");
  });
  server.on("/set_rsSim", []() {
    guitinnhan = 4;
    server.send(200, send_html, "OK");
  });

  server.on("/set_call2", []() {
    String new_phone = server.arg("newphone");
    new_phone.toCharArray(sdt_new, sizeof(sdt_new));
    guitinnhan = 8;
    server.send(200, send_html, "OK");
  });
  server.on("/set_naptk", []() {
    manapthe = server.arg("Recharge");
    manapthe.trim();
    guitinnhan = 5;
    server.send(200, send_html, "OK");
  });
  //////////////
  /*
    server.on("/module_id", []() {
      IPAddress ip = WiFi.localIP();
      String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
      char defaultId[sizeof(WiFiConf.module_id)];
      setDefaultModuleId(defaultId);
      String content = FPSTR(header); content += FPSTR(begin_title);
      content += WiFiConf.module_id;
      content += F(".local - Module ID");
      content += title_html;
      content += F("<h1>Module ID cho Wifi</h1>");
      content += F("<p>Module ID: ");
      content += WiFiConf.module_id;
      content += F("</br>IP: ");
      content += ipStr;
      content += F("</p>");
      content += F("<p>");
      content += F("<form method='get' action='set_module_id'><label for='module_id' class=\"req\">New Module ID: </label><input name='module_id' class=\"txt\" id='module_id' maxlength=32 value='");
      content += WiFiConf.module_id;
      content += F("'><input type='submit'  id=\"submitbtn\" onclick='return confirm(\"Are you sure you want to change the Module ID?\");'></form>");
      content += F(" Empty will reset to default ID '");
      content += defaultId;
      content += F("'</p>");
      content += FPSTR(_bodyhtml);
      server.send(200, send_html, content);
    });

    server.on("/set_module_id", []() {
      String new_id = server.arg("module_id");;
      if (new_id.length() > 0) {
        new_id.toCharArray(WiFiConf.module_id, sizeof(WiFiConf.module_id));
      } else {
        resetModuleId();
      }
      saveWiFiConf();
      server.send(200, send_html, "OK");
      delay(1000);
      ESP.reset();
    });*/

  server.on("/Reboot", HTTP_GET, []() {
    String content = FPSTR(header); content += FPSTR(begin_title);
    content += F("Reset");
    content += FPSTR(title_html);
    content += F("</br>");
    content += F("</p>");
    content += F("<p>");
    content += F("<form method='get' action='set_Reset'>");
    content += F("<input type='submit'  id=\"submitbtn\" value='Reset' onclick='return confirm(\"REBOOT NOW ?\");'></form>");
    content += F("</p>");
    content += FPSTR(_bodyhtml);
    server.send(200, send_html, content);
  });

 /* server.on("/mang_didong", []() {
    String content = FPSTR(header); content += FPSTR(begin_title);
    content += F("USSD");
    content += FPSTR(title_html);
    content += F("<h1>Cellular</h1>");
    content += FPSTR(form_method_get);
    content += F("set_mang_didong'>");
    content += FPSTR(label_html);
    content += F("'manap' class=\"req\">Recharge code :&nbsp;</label><input name='manap' class=\"txt\" id='manap' maxlength=32 value=");
    content += String(WiFiConf.sta_manap);
    content += FPSTR(br_html);
    content += FPSTR(label_html);
    content += F("'makttk' class=\"req\">Check : </label> <input  name='makttk' class=\"txt\" id='pwdhc2' value=");
    content += String(WiFiConf.sta_makttk) ;
    content += FPSTR(br_html);
    content += F("<input type='submit'  id=\"submitbtn\" value='OK' onclick='return confirm(\"Change Setting ?\");'></form>");
    content += F("<li>Recharge code:*100*");
    content += F("<li>Balance Code:*101#");
    content += FPSTR(_bodyhtml);
    server.send(200, send_html, content);
  });*/

  /*server.on("/set_mang_didong", []() {
    String new_naptk = server.arg("manap");
    String new_makttk = server.arg("makttk");
    new_naptk.toCharArray(WiFiConf.sta_manap, sizeof(WiFiConf.sta_manap));
    new_makttk.toCharArray(WiFiConf.sta_makttk, sizeof(WiFiConf.sta_makttk));
    saveWiFiConf();
    server.send(200, send_html, "OK");
  });*/

  server.on("/hc2_conf", []() {
    String content = FPSTR(header); content += FPSTR(begin_title);
      //  content1 = ipStr;
    //content1 += F(" ( ");
    //content1 += WiFiConf.module_id;
    //String content1 += F(".local");
    //content1 += F(" )</p>");
    String content1= F("<p>");
    content1 += FPSTR(form_method_get);
    content1 += F("set_hc2_conf'>");
    content1 += F("<div class=\"row\">");
    content1 += F("<li><select name='button' class=\"dropbtn\" >");
    String id_check = "";
    for (int i = 0; i < 2; i++) {
      switch (i) {
        case 0:  id_check = "HC2"; break;
        case 1:  id_check = "HCL"; break;
      }
      if (atoi(WiFiConf.choose_hc) != i)
        content1 += "<option value=\"" + String(i) + "\">" +  id_check + "</option>";
      else
        content1 += "<option value=\"" + String(i) + "\" selected>" +  id_check + "</option>";
    }
    content1 += F("</select>");
    content1 += F("</div>");
    content1 += FPSTR(label_html);
    content1 += F("'iphc2' class=\"req\">IP HC2:</label><input name='iphc2' class=\"txt\" id='iphc2' maxlength=32 value=");
    content1 += String(WiFiConf.sta_iphc2);
    content1 += FPSTR(br_html);
    content1 += FPSTR(label_html);
    content1 += F("'userhc2' class=\"req\">User HC2: </label> <input name='userhc2' class=\"txt\"  id='userhc2' value=");
    content1 += String(WiFiConf.sta_userhc) ;
    content1 += FPSTR(br_html);
    content1 += FPSTR(label_html);
    content1 += F("'pwdhc2' class=\"req\">PASS HC2: </label> <input type='password' class=\"txt\" name='pwdhc2' id='pwdhc2' value=");
    content1 += String(WiFiConf.sta_passhc) ;
    content1 += FPSTR(br_html);
    content1 += FPSTR(label_html);
    content1 += F("'global1' class=\"req\">Global 1:</label> <input name='global1' class=\"txt\" id='global1'value=");
    content1 += String(WiFiConf.sta_global1);
    content1 += FPSTR(br_html);
    content1 += FPSTR(label_html);
    content1 += F("'global2' class=\"req\">Global 2:</label> <input  name='global2' class=\"txt\" id='global2' value=");
    content1 +=  String(WiFiConf.sta_global2);
    content1 += FPSTR(br_html);
    content += F("HC2");
    content += FPSTR(title_html);
    content += F("<h1>HC2 Setting</h1>");
    content += content1;
    content += F("<input type='submit' id=\"submitbtn\" value='OK' onclick='return confirm(\"Change?\");'></form>");
    content += FPSTR(form_method_get);
    content += F("getHC'>");
    content += F("<input type='submit' value='Check'></form>");
    content += F(" </p>");
    //content += F("<li>Information HC2");
    content += F("<li>SIM_TK - Tai khoang SIM");
    content += F("<li>Global1 - Trang Thai ON,OFF,OK");
    content += F("<li>SIM_DTMF - Status DTMF");
    //content += F("<li>SIM_CALL - Phone number quitclaim calling");
    content += FPSTR(_bodyhtml);
    server.send(200, send_html, content);
  });
  server.on("/set_hc2_conf", []() {
    String data1 = server.arg(F("button"));
    data1.toCharArray(WiFiConf.choose_hc, sizeof(WiFiConf.choose_hc));
    String new_IPHC = server.arg("iphc2");
    String new_userhc = server.arg("userhc2");
    String new_pwdhc = server.arg("pwdhc2");
    String new_global1 = server.arg("global1");
    String new_global2 = server.arg("global2");
    //String new_global3 = server.arg("global3");
    if (new_IPHC.length() > 0) {
      new_IPHC.toCharArray(WiFiConf.sta_iphc2, sizeof(WiFiConf.sta_iphc2));
      new_userhc.toCharArray(WiFiConf.sta_userhc, sizeof(WiFiConf.sta_userhc));
      new_pwdhc.toCharArray(WiFiConf.sta_passhc, sizeof(WiFiConf.sta_passhc));
      new_global1.toCharArray(WiFiConf.sta_global1, sizeof(WiFiConf.sta_global1));
      new_global2.toCharArray(WiFiConf.sta_global2, sizeof(WiFiConf.sta_global2));
      saveWiFiConf();
    }
    server.send(200, send_html, "OK");
  });
  server.on("/getHC", []() {
    getHC();
    server.send(200, send_html, "OK");
  });
  /*server.on("/set_language", []() {
    String new_language = server.arg("language");
    if (new_language == "Vietnamese") {
      String lan = "1";
      lan.toCharArray(WiFiConf.sta_language, sizeof(WiFiConf.sta_language));
    }
    else {
      String lan = "0";
      lan.toCharArray(WiFiConf.sta_language, sizeof(WiFiConf.sta_language));
    }
    saveWiFiConf();
    server.send(200, send_html, "OK");
    delay(1000);
    ESP.reset();
    });*/
  server.on("/set_Reset", HTTP_GET, []() {
    server.send(200, send_html, "OK-Reboot");
    
    ESP.reset();
  });
 server.on("/set_security", HTTP_GET, []() {
    String data1 = server.arg(F("button"));
    data1.toCharArray(WiFiConf.sta_security, sizeof(WiFiConf.sta_security));
    saveWiFiConf();
    server.send(200, send_html, "OK-Reboot");
    delay(1500);
    ESP.reset();
  });
  server.on("/Reset1", HTTP_GET, []() {

    String content = FPSTR(header); content += FPSTR(begin_title);
    //  String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
    content += F("Reset default");
    content += FPSTR(title_html);
    content += F("</br>");
    content += F("<p>");
    content += FPSTR(form_method_get);
    content += F("set_Reset1'>");
    content += F("<input type='submit'  id=\"submitbtn\" value='Reset' onclick='return confirm(\"Reset NOW ?\");'></form>");
    content += F("</p>");
    content += FPSTR(_bodyhtml);
    server.send(200, send_html, content);
  });
  server.on("/set_Reset1", HTTP_GET, []() {
    String new_IPHC = "192.168.1.10";
    String new_userhc = "";
    String new_pwdhc = "";
    String new_global1 = "";
    String new_global2 = "";
    String new_ssid = "";
    String new_pwd = "";
    String new_ip = "192.168.1.220";
    String new_gateway = "192.168.1.1";
    String new_subnet = "255.255.255.0";
    String new_sdt1 = "x";
    String new_sdt2 = "x";
    String new_sdt3 = "x";
    String new_sdt4 = "x";

    new_ssid.toCharArray(WiFiConf.sta_ssid, sizeof(WiFiConf.sta_ssid));
    new_pwd.toCharArray(WiFiConf.sta_pwd, sizeof(WiFiConf.sta_pwd));
    new_ip.toCharArray(WiFiConf.sta_ip, sizeof(WiFiConf.sta_ip));
    new_gateway.toCharArray(WiFiConf.sta_gateway, sizeof(WiFiConf.sta_gateway));
    new_subnet.toCharArray(WiFiConf.sta_subnet, sizeof(WiFiConf.sta_subnet));
    new_IPHC.toCharArray(WiFiConf.sta_iphc2, sizeof(WiFiConf.sta_iphc2));
    new_userhc.toCharArray(WiFiConf.sta_userhc, sizeof(WiFiConf.sta_userhc));
    new_pwdhc.toCharArray(WiFiConf.sta_passhc, sizeof(WiFiConf.sta_passhc));
    new_global1.toCharArray(WiFiConf.sta_global1, sizeof(WiFiConf.sta_global1));
    new_global2.toCharArray(WiFiConf.sta_global2, sizeof(WiFiConf.sta_global2));

    new_sdt1.toCharArray(WiFiConf.sta_SDT1, sizeof(WiFiConf.sta_SDT1));
    new_sdt2.toCharArray(WiFiConf.sta_SDT2, sizeof(WiFiConf.sta_SDT2));
    new_sdt3.toCharArray(WiFiConf.sta_SDT3, sizeof(WiFiConf.sta_SDT3));
    new_sdt4.toCharArray(WiFiConf.sta_SDT4, sizeof(WiFiConf.sta_SDT4));

    String lan = "0";
    lan.toCharArray(WiFiConf.sta_language, sizeof(WiFiConf.sta_language));
    resetModuleId();
    saveWiFiConf();
    server.send(200, send_html, "Rebooting");
    ESP.reset();
  });
}

void setupWeb(void) {
  server.on("/", []() {
    //IPAddress ip = WiFi.localIP();
    //String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
    String content = FPSTR(header);
    content += FPSTR(begin_title);
    content += F("Main page");
    content += FPSTR(title_html);
    content += F("<h1>GSM</h1>");
    content += FPSTR(fieldset);
    content += FPSTR(legend_style);
    content += F("Config");
    content += FPSTR(legend_end);
    content += FPSTR(fieldset);
    content += FPSTR(legendhref);
    content += F("wifi_conf'>Wifi Conf</a>");
    content += FPSTR(legend_end);
    content += F("<li>Setting Wifi");
    if (status_sec == false){
    content += F("<li>Security: Fail");
    }
    else {content += F("<li>Security: OK");}
    content += FPSTR(_fieldset);
    content += FPSTR(fieldset);
    content += FPSTR(legendhref);
    content += F("hc2_conf'>HC2 Conf</a>");
    content += FPSTR(legend_end);
    content += F("<li>Status : ");
    content += SerialHC2;
    content += F("<li>HC2 response :");
    content += noidung;
    content += FPSTR(_fieldset);
    content += FPSTR(fieldset);
    content += FPSTR(legendhref);
    content += F("sdt_conf'>Phone Conf</a>");
    content += FPSTR(legend_end);
    content += F("<li>Phone 1:");
    content += String(WiFiConf.sta_SDT1);
    content += F("<li>Phone 2:");
    content += String(WiFiConf.sta_SDT2);
    content += F("<li>Phone 3:");
    content += String(WiFiConf.sta_SDT3);
    content += F("<li>Phone 4:");
    content += String(WiFiConf.sta_SDT4);
    content += F("<li>Res: ");
    content += noidungkiemtratk ;
    content += FPSTR(_fieldset);
    //content += FPSTR(fieldset);
    //content += FPSTR(legendhref);
    //content += F("mang_didong'>Cellular Conf</a>");
    //content += FPSTR(legend_end);
    //content += F("<li>Recharge code :");
    //content += String(WiFiConf.sta_manap);
    //content += F("<li>Balance Check : ");
   // content += String(WiFiConf.sta_makttk) ;
    //content += F("<li>USSD res: ");
   // content += noidungkiemtratk ;
   // content += FPSTR(_fieldset);
    content += FPSTR(_fieldset);
    content += FPSTR(fieldset);
    content += FPSTR(legend_style);
    content += F("Test");
    content += FPSTR(legend_end);
    content += FPSTR(fieldset);
    content += FPSTR(legendhref);
    content += F("tinnhan'>SMS Test</a>");
    content += FPSTR(legend_end);
    content += F("<li>Des: send SMS");
    content += FPSTR(_fieldset);
    content += FPSTR(fieldset);
    content += FPSTR(legendhref);
    content += F("cuocgoi'>Call Test</a>");
    content += FPSTR(legend_end);
    content += F("<li>Des: call");
    content += FPSTR(_fieldset);
    content += FPSTR(_fieldset);
    
    content += FPSTR(fieldset);
    content += FPSTR(legend_style);
    content += F("Function");
    content += FPSTR(legend_end);  
    content += FPSTR(fieldset);
    content += FPSTR(legendhref);
    content += F("firmware'>Firmware</a>");
    content += FPSTR(legend_end);
    content += F("<li>Status : V2.8 - 19/07/2019");
    content += FPSTR(_fieldset);
    content += FPSTR(fieldset);
    content += FPSTR(legendhref);
    content += F("Reboot'>Reboot</a>");
    content += FPSTR(legend_end);
    //content += F("<li>Des: parameter is not change.");
    content += FPSTR(_fieldset);
    content += FPSTR(fieldset);
    content += FPSTR(legendhref);
    content += F("Reset1'>Factory Reset</a>");
    content += FPSTR(legend_end);
    content += FPSTR(_fieldset);
    content += FPSTR(_fieldset);
    content += FPSTR(_bodyhtml);
    server.send(200, send_html, content);
  });
}


// Set Language:  content += F("<h1>GSM </h1><form method='get' action='set_language'><input type=\"submit\" name=\"language\" value=\"Vietnamese\"> &nbsp &nbsp &nbsp <input type=\"submit\" name=\"language\" value=\"English\"></form>");
