/////////////////////////////////////////////
//  WIFI FORMAT ////////////////////////////
///////////////////////////////////////////
#define WIFI_CONF_FORMAT {0, 0, 0, 1}
#define WIFI_CONF_START 0
#define NAME_PREF "GSM-"
const uint8_t wifi_conf_format[] = WIFI_CONF_FORMAT;
struct WiFiConfStruct {
  uint8_t format[4];
  char sta_ssid[32];
  char sta_pwd[64];
  char sta_ip[15];
  char sta_gateway[15];
  char sta_subnet[15];
  char sta_iphc2[20];
  char sta_userhc[64];
  char sta_passhc[64];
  char sta_SDT1[15];
  char sta_SDT2[15];
  char sta_SDT3[15];
  char sta_SDT4[15];
  char sta_global1[20];
  char sta_global2[20];
  char sta_language[3];
  char sta_manap[10];
  char sta_makttk[10];
  char choose_hc[4];
  char sta_security[3];
  char module_id[32];
} WiFiConf = {
  WIFI_CONF_FORMAT,
  "An Nhan",
  "xxxyyy",
  "192.168.1.210",
  "192.168.1.1",
  "255.255.255.0",
  "",
  "",
  "",
  "x",
  "x",
  "x",
  "x",
  "",
  "",
  "1",
  "*100*",
  "*101#",
  "0",
  "0",
  ""
};
struct sdt_sendnew {
  char sta_SDT1[15];
  char sta_SDT2[15];
  char sta_SDT3[15];
  char sta_SDT4[15];
} sdtnew = {
  "x",
  "x",
  "x",
  "x"
};
