#ifndef EDGEPLATFORM_H
#define EDGEPLATFORM_H


#ifdef PLATFORM_ID

#include "Particle.h"


#define USE_SERIAL_LOGGER
// #undef USE_SERIAL_LOGGER

// #define USE_REMOTE_LOGGER
#undef USE_REMOTE_LOGGER

#define USE_UDP_SEND
// #undef USE_UDP_SEND

#define PS      Serial2.begin(115200)
#define P(x)    {Serial2.println(x);Serial2.flush();}

#define EDGE_PRODUCT_PRODUCTION     0
#define EDGE_PRODUCT_DEVELOPMENT    1

#define EDGE_PRODUCT EDGE_PRODUCT_DEVELOPMENT

// p1 = 8
#if PLATFORM_ID == 8
    #if SYSTEM_VERSION != SYSTEM_VERSION_v210
    #error SYSTEM_VERSION
    #error "Needs deviceOS v2.1.0 because of breathing green problem"
    #endif
#endif

// p2 = 32
#if PLATFORM_ID == 32
    #if SYSTEM_VERSION != SYSTEM_VERSION_v550
    #error SYSTEM_VERSION
    #error "Needs deviceOS v5.5.0"
    #endif
    #define PLATFORM_HAS_SERIAL_2
#endif

// argon = 12
#if PLATFORM_ID == PLATFORM_ARGON
    #if SYSTEM_VERSION != SYSTEM_VERSION_v550
    #error SYSTEM_VERSION
    #error "Needs deviceOS v5.5.0"
    #endif
#endif

#endif // PLATFORM_ID

#endif // EDGEPLATFORM_H