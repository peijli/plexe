#ifndef SECUREPLATOONINGBEACONING_H_
#define SECUREPLATOONINGBEACONING_H_

#include "BaseProtocol.h"
#include "SimplePlatooningBeaconing.h"

namespace plexe {
    class SecurePlatooningBeaconing : public SimplePlatooningBeaconing {
    protected:
        virtual void handleSelfMsg(cMessage* msg);
    public:
        SecurePlatooningBeaconing();
        virtual ~SecurePlatooningBeaconing();
        virtual void initialize(int stage);
    };
} // namespace plexe

#endif /* SECUREPLATOONINGBEACONING_H_ */
