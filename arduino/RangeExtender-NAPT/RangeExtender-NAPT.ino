
// NAPT example released to public domain

#if LWIP_FEATURES && !LWIP_IPV6

#define HAVE_NETDUMP 0

#ifndef STASSID
#define STASSID "qq"
#define STAPSK QQ_SECRET
#endif

#include <ESP8266WiFi.h>
#include <lwip/napt.h>
#include <lwip/dns.h>

#define NAPT 1000
#define NAPT_PORT 10

#if HAVE_NETDUMP

#include <NetDump.h>

void dump(int netif_idx, const char* data, size_t len, int out, int success) {
  (void)success;
  Serial.print(out ? F("out ") : F(" in "));
  Serial.printf("%d ", netif_idx);

  // optional filter example: if (netDump_is_ARP(data))
  {
    netDump(Serial, data, len);
    // netDumpHex(Serial, data, len);
  }
}
#endif

unsigned char connected;
unsigned char state;
int clients;
unsigned long next;
unsigned long since;
bool disconnectedState;

void setup() {

  digitalWrite(0, HIGH);
  pinMode(0, OUTPUT); 
  

  Serial.begin(115200);
  Serial.printf("\n\nNAPT Range extender\n");
  Serial.printf("Heap on start: %d\n", ESP.getFreeHeap());

#if HAVE_NETDUMP
  phy_capture = dump;
#endif

  // first, connect to STA so we can get a proper local DNS server
  WiFi.mode(WIFI_STA);
  WiFi.begin(STASSID, STAPSK);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(500);
    digitalWrite(0, LOW);
    delay(25);
    digitalWrite(0, HIGH);
  }

  connected = 1;
  state = 1;
  clients = -1;
  disconnectedState = false;
  
  
  digitalWrite(0, LOW);
  
  Serial.printf("\nSTA: %s (dns: %s / %s)\n", WiFi.localIP().toString().c_str(), WiFi.dnsIP(0).toString().c_str(), WiFi.dnsIP(1).toString().c_str());

  // By default, DNS option will point to the interface IP
  // Instead, point it to the real DNS server.
  // Notice that:
  // - DhcpServer class only supports IPv4
  // - Only a single IP can be set
  auto& server = WiFi.softAPDhcpServer();
  server.setDns(WiFi.dnsIP(0));

  WiFi.softAPConfig(  // enable AP, with android-compatible google domain
    IPAddress(172, 217, 28, 254), IPAddress(172, 217, 28, 254), IPAddress(255, 255, 255, 0));
  WiFi.softAP(STASSID "extender", STAPSK);
  Serial.printf("AP: %s\n", WiFi.softAPIP().toString().c_str());

  Serial.printf("Heap before: %d\n", ESP.getFreeHeap());
  err_t ret = ip_napt_init(NAPT, NAPT_PORT);
  Serial.printf("ip_napt_init(%d,%d): ret=%d (OK=%d)\n", NAPT, NAPT_PORT, (int)ret, (int)ERR_OK);
  if (ret == ERR_OK) {
    ret = ip_napt_enable_no(SOFTAP_IF, 1);
    Serial.printf("ip_napt_enable_no(SOFTAP_IF): ret=%d (OK=%d)\n", (int)ret, (int)ERR_OK);
    if (ret == ERR_OK) { Serial.printf("WiFi Network '%s' with same password is now NATed behind '%s'\n", STASSID "extender", STASSID); }
  }
  Serial.printf("Heap after napt init: %d\n", ESP.getFreeHeap());
  if (ret != ERR_OK) { Serial.printf("NAPT initialization failed\n"); }

  since = millis();
}

#else

void setup() {
  Serial.begin(115200);
  Serial.printf("\n\nNAPT not supported in this configuration\n");
}

#endif


void disconnect() {

  WiFi.softAPdisconnect();
  disconnectedState = true;
}

void connect() {
    WiFi.softAP(STASSID "extender", STAPSK);
    disconnectedState = false;
}

void loop() {

  bool isOnAir = (WiFi.status() == WL_CONNECTED);
  if(disconnectedState) {
    isOnAir = false;
  }
  
  if(!isOnAir && (connected == 1)) {
    connected = 0;
    digitalWrite(0, HIGH);
    Serial.printf("disconnected\n");
    next = millis() + 500;
    state=0;
  }

  if(isOnAir && (connected == 0)) {
    since = millis();
    connected = 1;
    state=1;
    digitalWrite(0, LOW);
    Serial.printf("connected\n");
  }

  if(connected==0) {
    if(state==1) {
      digitalWrite(0, LOW);
      next=millis()+25;
    } else {
      digitalWrite(0, HIGH);
      next=millis()+500;
    }

    if((millis()-since) > 30000 ) {
      Serial.printf("reconnecting\n");
      connect();
      since = millis();
      Serial.printf("clients = %d\n", clients);
    }

  } else {

    int n = WiFi.softAPgetStationNum();

    if(n!=clients) {
        Serial.printf("clients = %d\n", n);
        clients = n;
    }    

    if((millis()-since) > 300000 ) {
      Serial.printf("disconnecting\n");
      disconnect();
      since = millis();
      clients = 0;
      Serial.printf("clients = %d\n", clients);
    }
  }
  
  }
