/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/Users/frank/Code/edge-test-udp/src/edge-test-udp.ino"
#include "EdgePlatform.h"
#include "Connectivity.h"
#include "Logging.h"
#include "ResetReason.h"
#include "Util.h"

void setup();
void loop();
#line 7 "/Users/frank/Code/edge-test-udp/src/edge-test-udp.ino"
STARTUP(BLE.selectAntenna(BleAntennaType::EXTERNAL));

SYSTEM_MODE(SEMI_AUTOMATIC); 
SYSTEM_THREAD(ENABLED);

STARTUP(System.enableFeature(FEATURE_DISABLE_LISTENING_MODE));
STARTUP(System.enableFeature(FEATURE_RESET_INFO));
STARTUP(System.enableFeature(FEATURE_RETAINED_MEMORY));

bool report_first_connected = false;
unsigned long last_debug_out;

int count = 0;

void setup() {

  WiFi.clearCredentials();
  WiFi.setCredentials("qqextender","qqkoffieqq-666");

  delay(2000);

  PS;
  P("start");
  
  Watchdog.init(WatchdogConfiguration().timeout(60s));
  Watchdog.start();

  logging_init();

  logging_serial1(LOG_ON, LOG_ON_DEBUG);
  logging_remote(LOG_ON, LOG_ON_DEBUG);

  Log.info("%s-%s-%d-%s", completeVersion, System.version().c_str(),(int)PLATFORM_ID, System.deviceID().c_str());

  reset_reason_log();

  Particle.publishVitals(30); 

  connectivity_init();

  last_debug_out = millis();
}


void loop() {

  Watchdog.refresh();

  connectivity_connect();

  unsigned long now = millis();

  if((now - last_debug_out) > 1000) {

    if((!report_first_connected) && connectivity_first_connected()) {

        Log.info("%s-%s-%d-%s-%s", completeVersion, System.version().c_str(),(int)PLATFORM_ID, System.deviceID().c_str(), WiFi.SSID());
        reset_reason_log();

        report_first_connected = true;
      }

    Log.info("count = %d", count++);

    last_debug_out = now;
  }
}