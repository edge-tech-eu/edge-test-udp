#include "EdgePlatform.h"
#include "RemoteLogger.h"

const uint16_t RemoteLogHandler::kLocalPort = 8888;

RemoteLogHandler::RemoteLogHandler(String host, uint16_t port, String app, LogLevel level, 
    const LogCategoryFilters &filters, String system) : LogHandler(level, filters), m_host(host), m_port(port), m_app(app),
                                       m_system(system)  {
    m_inited = false;

    LogManager::instance()->addHandler(this);
}

IPAddress RemoteLogHandler::resolve(const char *host) {

    P("RR");
    IPAddress adr = Network.resolve(host);
    P("rr");

    return(adr);
}

void RemoteLogHandler::log(String message) {
    //String time = Time.format(Time.now(), TIME_FORMAT_ISO8601_FULL);
    String packet = String::format("[%s] [%s] %s", m_system.c_str(), m_app.c_str(), message.c_str());
    P("SS");
    int ret = m_udp.sendPacket(packet, packet.length(), m_address, m_port);
    P("ss");
    if (ret < 1) {
        m_inited = false;
    }
}

RemoteLogHandler::~RemoteLogHandler() {
    LogManager::instance()->removeHandler(this);
}

bool RemoteLogHandler::lazyInit() {
    
    P("CC");
    bool connected = Particle.connected();
    P("cc");

     if(connected) {

        if (!m_inited) {
            
            P("UU");
            uint8_t ret = m_udp.begin(kLocalPort);
            P("uu");
            m_inited = ret != 0;

            if (!m_inited) {
                return false;
            }
        }

        if (!m_address) {
            m_address = resolve(m_host);

            if (!m_address) {
                return false;
            }
        }
    }

    return true;
}

const char* RemoteLogHandler::extractFileName(const char *s) {
    const char *s1 = strrchr(s, '/');
    if (s1) {
        return s1 + 1;
    }
    return s;
}

const char* RemoteLogHandler::extractFuncName(const char *s, size_t *size) {
    const char *s1 = s;
    for (; *s; ++s) {
        if (*s == ' ') {
            s1 = s + 1; // Skip return type
        } else if (*s == '(') {
            break; // Skip argument types
        }
    }
    *size = s - s1;
    return s1;
}

void RemoteLogHandler::logMessage(const char *msg, LogLevel level, const char *category, const LogAttributes &attr) {

    //if(Particle.connected()) {

        if (!lazyInit()) {
        return;
        }

        String s;

        if (attr.has_time) {
            s.concat(String::format("%010u ", (unsigned)attr.time));
            //char timestring[15];
            //sprintf(timestring, "%010u ", (unsigned)attr.time);
            //s.concat(timestring);
        }

        if (category) {
            s.concat("[");
            s.concat(category);
            s.concat("] ");
        }

        // Source file
        if (attr.has_file) {
            s = extractFileName(attr.file); // Strip directory path
            s.concat(s); // File name
            if (attr.has_line) {
                s.concat(":");
                s.concat(String(attr.line)); // Line number
            }
            if (attr.has_function) {
                s.concat(", ");
            } else {
                s.concat(": ");
            }
        }

        // Function name
        if (attr.has_function) {
            size_t n = 0;
            s = extractFuncName(attr.function, &n); // Strip argument and return types
            s.concat(s);
            s.concat("(): ");
        }

        // Level
        s.concat(levelName(level));
        s.concat(": ");

        // Message
        if (msg) {
            s.concat(msg);
        }

        // Additional attributes
        if (attr.has_code || attr.has_details) {
            s.concat(" [");
            // Code
            if (attr.has_code) {
                s.concat(String::format("code = %p" , (intptr_t)attr.code));
            }
            // Details
            if (attr.has_details) {
                if (attr.has_code) {
                    s.concat(", ");
                }
                s.concat("details = ");
                s.concat(attr.details);
            }
            s.concat(']');
        }

        log(s);

    //} else {

    //    m_inited = false;
    //}
}
