#include "EdgePlatform.h"
#include "Logging.h"
#include "RemoteLogger.h"

// keep order of 'retained' variables
retained bool serialLoggerEnabled = LOG_OFF;
retained bool serialLoggerDebugEnabled = LOG_ON_DEBUG;
retained bool serial1LoggerEnabled = LOG_OFF;
retained bool serial1LoggerDebugEnabled = LOG_ON_DEBUG;
#ifdef PLATFORM_HAS_SERIAL_2
retained bool serial2LoggerEnabled = LOG_OFF;
retained bool serial2LoggerDebugEnabled = LOG_ON_DEBUG;
#endif
retained bool remoteLoggerEnabled = LOG_OFF;
retained bool remoteLoggerDebugEnabled = LOG_ON_DEBUG;

RemoteLogHandler *remoteLogHandler = NULL;
#ifdef PLATFORM_HAS_SERIAL_2
Serial2LogHandler *serial2LogHandler = NULL;
#endif
SerialLogHandler *serialLogHandler = NULL;
Serial1LogHandler *serial1LogHandler = NULL;


void logging_init(void) {

    logging_serial(serialLoggerEnabled,serialLoggerDebugEnabled);
    logging_serial1(serial1LoggerEnabled,serial1LoggerDebugEnabled);
#ifdef PLATFORM_HAS_SERIAL_2
    logging_serial2(serial2LoggerEnabled,serial2LoggerDebugEnabled);
#endif
    logging_remote(remoteLoggerEnabled,remoteLoggerDebugEnabled);
}

void logging_serial(bool logon, bool debug) {

    auto logManager = LogManager::instance();

    if(serialLogHandler!=NULL) {
    
        logManager->removeHandler(serialLogHandler);

        delete serialLogHandler;
    }

    SerialLogHandler* newLogHandler = NULL;
    
    if(logon) {

        newLogHandler = new SerialLogHandler(115200, (debug?LOG_LEVEL_ALL:LOG_LEVEL_INFO));

        logManager->addHandler(newLogHandler);
    }
    
    serialLogHandler = newLogHandler;
}

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

#ifdef PLATFORM_HAS_SERIAL_2
void logging_serial2(bool logon, bool debug) {

    auto logManager = LogManager::instance();

    if(serial2LogHandler!=NULL) {
    
        logManager->removeHandler(serial2LogHandler);

        delete serial2LogHandler;
    }

    Serial2LogHandler* newLogHandler = NULL;
    
    if(logon) {

        newLogHandler = new Serial2LogHandler(115200, (debug?LOG_LEVEL_ALL:LOG_LEVEL_INFO));

        logManager->addHandler(newLogHandler);
    }
    
    serial2LogHandler = newLogHandler;
}
#endif

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