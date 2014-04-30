#include "CanID.h"

namespace FiCo4OMNeT {

CanID::CanID(int id, cModule *node, simtime_t signInTime, bool rtr) :
        id(id), node(node), signInTime(signInTime), rtr(rtr) {
}

int CanID::getId() {
    return id;
}

cModule* CanID::getNode() {
    return node;
}

bool CanID::getRtr() {
    return rtr;
}

simtime_t CanID::getSignInTime() {
    return signInTime;
}

}
