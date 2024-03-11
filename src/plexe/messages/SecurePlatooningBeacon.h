#ifndef __SECUREPLATOONINGBEACON_H
#define __SECUREPLATOONINGBEACON_H

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif

// dll export symbol
#ifndef PLEXE_API
#  if defined(PLEXE_EXPORT)
#    define PLEXE_API  OPP_DLLEXPORT
#  elif defined(PLEXE_IMPORT)
#    define PLEXE_API  OPP_DLLIMPORT
#  else
#    define PLEXE_API
#  endif
#endif

#include "SecurePlatooningBeacon_m.h"

class PLEXE_API SecurePlatooningBeacon : public SecurePlatooningBeacon_Base {
private:
    void copy(const SecurePlatooningBeacon& other);

public:
    SecurePlatooningBeacon(const char* name = nullptr, short kind = 0) : SecurePlatooningBeacon_Base(name, kind) {}
    SecurePlatooningBeacon(const SecurePlatooningBeacon& other) : SecurePlatooningBeacon_Base(other) { copy(other); }
    SecurePlatooningBeacon& operator=(const SecurePlatooningBeacon& other) { 
        if (this == &other) return *this; 
        SecurePlatooningBeacon_Base::operator=(other); 
        copy(other); 
        return *this; 
    }
    virtual SecurePlatooningBeacon* dup() const override { 
        return new SecurePlatooningBeacon(*this); 
    }
    // ADD CODE HERE to redefine and implement pure virtual functions from SecurePlatooningBeacon_Base
    // override getter/setter functions for opp_string data types
    virtual void setEncryptedData(const char* encryptedData) override;
    virtual const char * getEncryptedData() const override;
    void setEncryptedData(const char* encryptedData, int length);
    void setMAC(const char* mac, int length);
    void setMAC(std::string mac);
    const char * getMAC() const;
    std::string getMACString() const;
};

#endif
