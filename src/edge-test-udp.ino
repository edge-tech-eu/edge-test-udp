#include "EdgePlatform.h"
#include "Connectivity.h"
#include "Logging.h"
#include "ResetReason.h"
#include "Util.h"

STARTUP(BLE.selectAntenna(BleAntennaType::EXTERNAL));

SYSTEM_MODE(SEMI_AUTOMATIC); 
SYSTEM_THREAD(ENABLED);

STARTUP(System.enableFeature(FEATURE_DISABLE_LISTENING_MODE));
STARTUP(System.enableFeature(FEATURE_RESET_INFO));
STARTUP(System.enableFeature(FEATURE_RETAINED_MEMORY));

bool report_first_connected = false;
unsigned long last_debug_out;

int count = 0;

#ifdef USE_UDP_SEND
UDP udp;
IPAddress monitor(45,76,34,89);
#endif

void setup() {

  delay(2000);

  PS;
  P("start");
  
  Watchdog.init(WatchdogConfiguration().timeout(60s));
  Watchdog.start();

  logging_init();

#ifdef USE_SERIAL_LOGGER
  logging_serial1(LOG_ON, LOG_ON_DEBUG);
#endif
#ifdef USE_REMOTE_LOGGER
  logging_remote(LOG_ON, LOG_ON_DEBUG);
#endif

#ifdef USE_UDP_SEND
  udp.begin(8888);
#endif

  Log.info("%s-%s-%d-%s", completeVersion, System.version().c_str(),(int)PLATFORM_ID, System.deviceID().c_str());
#ifdef USE_SERIAL_LOGGER
        Log.info("serial logger is on");
#endif
#ifdef USE_REMOTE_LOGGER
        Log.info("remote logger is on");
#endif
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
#ifdef USE_SERIAL_LOGGER
        Log.info("serial logger is on");
#endif
#ifdef USE_REMOTE_LOGGER
        Log.info("remote logger is on");
#endif
        reset_reason_log();

        report_first_connected = true;
      }

    Log.info("count = %d", count++);

  #ifdef USE_UDP_SEND
    String packet = String::format("count = %d", count++);
    P("XX");
    udp.sendPacket(packet, packet.length(), monitor, 8888);
    P("xx");
  #endif

    last_debug_out = now;
  }
}