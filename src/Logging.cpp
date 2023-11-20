#include "EdgePlatform.h"
#include "Logging.h"
#include "RemoteLogger.h"

// keep order of 'retained' variables
#ifdef USE_SERIAL_LOGGER
retained bool serial1LoggerEnabled = LOG_OFF;
retained bool serial1LoggerDebugEnabled = LOG_ON_DEBUG;
#endif

#ifdef USE_REMOTE_LOGGER
retained bool remoteLoggerEnabled = LOG_OFF;
retained bool remoteLoggerDebugEnabled = LOG_ON_DEBUG;
#endif

#ifdef USE_REMOTE_LOGGER
RemoteLogHandler *remoteLogHandler = NULL;
#endif

#ifdef USE_SERIAL_LOGGER
Serial1LogHandler *serial1LogHandler = NULL;
#endif

void logging_init(void) {

#ifdef USE_SERIAL_LOGGER
   logging_serial1(serial1LoggerEnabled,serial1LoggerDebugEnabled);
#endif

#ifdef USE_REMOTE_LOGGER
    logging_remote(remoteLoggerEnabled,remoteLoggerDebugEnabled);
#endif
}

#ifdef USE_SERIAL_LOGGER

void logging_serial1(bool logon, bool debug) {

    auto logManager = LogManager::instance();

    if(serial1LogHandler!=NULL) {
    
        logManager->removeHandler(serial1LogHandler);

        delete serial1LogHandler;
    }

    Serial1LogHandler* newLogHandler = NULL;
    
    if(logon) {

        newLogHandler = new Serial1LogHandler(115200, (debug?LOG_LEVEL_ALL:LOG_LEVEL_INFO));

        logManager->addHandler(newLogHandler);
    }
    
    serial1LogHandler = newLogHandler;
}
#endif

#ifdef USE_REMOTE_LOGGER
void logging_remote(bool logon, bool debug) {

    auto logManager = LogManager::instance();

    if(remoteLogHandler!=NULL) {
    
        logManager->removeHandler(remoteLogHandler);

        delete remoteLogHandler;
    }

    RemoteLogHandler* newLogHandler = NULL;
    
    if(logon) {


        newLogHandler = new RemoteLogHandler("monitor.edgetech.nl", 9999, "connectivity", (debug?LOG_LEVEL_ALL:LOG_LEVEL_INFO));

        logManager->addHandler(newLogHandler);
    }
    
    remoteLogHandler = newLogHandler;
}

#endif