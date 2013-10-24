//
// Generated file, do not edit! Created by opp_msgc 4.2 from linklayer/can/arb.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "arb_m.h"

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// Another default rule (prevents compiler from choosing base class' doPacking())
template<typename T>
void doPacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doPacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}

template<typename T>
void doUnpacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doUnpacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}




Register_Class(ArbMsg);

ArbMsg::ArbMsg(const char *name, int kind) : cMessage(name,kind)
{
    this->id_var = 0;
    this->node_var = 0;
    this->rtr_var = 0;
    this->remotesent_var = 0;
    this->signInTime_var = 0;
}

ArbMsg::ArbMsg(const ArbMsg& other) : cMessage(other)
{
    copy(other);
}

ArbMsg::~ArbMsg()
{
}

ArbMsg& ArbMsg::operator=(const ArbMsg& other)
{
    if (this==&other) return *this;
    cMessage::operator=(other);
    copy(other);
    return *this;
}

void ArbMsg::copy(const ArbMsg& other)
{
    this->id_var = other.id_var;
    this->node_var = other.node_var;
    this->rtr_var = other.rtr_var;
    this->remotesent_var = other.remotesent_var;
    this->signInTime_var = other.signInTime_var;
}

void ArbMsg::parsimPack(cCommBuffer *b)
{
    cMessage::parsimPack(b);
    doPacking(b,this->id_var);
    doPacking(b,this->node_var);
    doPacking(b,this->rtr_var);
    doPacking(b,this->remotesent_var);
    doPacking(b,this->signInTime_var);
}

void ArbMsg::parsimUnpack(cCommBuffer *b)
{
    cMessage::parsimUnpack(b);
    doUnpacking(b,this->id_var);
    doUnpacking(b,this->node_var);
    doUnpacking(b,this->rtr_var);
    doUnpacking(b,this->remotesent_var);
    doUnpacking(b,this->signInTime_var);
}

int ArbMsg::getId() const
{
    return id_var;
}

void ArbMsg::setId(int id)
{
    this->id_var = id;
}

int ArbMsg::getNode() const
{
    return node_var;
}

void ArbMsg::setNode(int node)
{
    this->node_var = node;
}

bool ArbMsg::getRtr() const
{
    return rtr_var;
}

void ArbMsg::setRtr(bool rtr)
{
    this->rtr_var = rtr;
}

bool ArbMsg::getRemotesent() const
{
    return remotesent_var;
}

void ArbMsg::setRemotesent(bool remotesent)
{
    this->remotesent_var = remotesent;
}

simtime_t ArbMsg::getSignInTime() const
{
    return signInTime_var;
}

void ArbMsg::setSignInTime(simtime_t signInTime)
{
    this->signInTime_var = signInTime;
}

class ArbMsgDescriptor : public cClassDescriptor
{
  public:
    ArbMsgDescriptor();
    virtual ~ArbMsgDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(ArbMsgDescriptor);

ArbMsgDescriptor::ArbMsgDescriptor() : cClassDescriptor("ArbMsg", "cMessage")
{
}

ArbMsgDescriptor::~ArbMsgDescriptor()
{
}

bool ArbMsgDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<ArbMsg *>(obj)!=NULL;
}

const char *ArbMsgDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int ArbMsgDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 5+basedesc->getFieldCount(object) : 5;
}

unsigned int ArbMsgDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<5) ? fieldTypeFlags[field] : 0;
}

const char *ArbMsgDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "id",
        "node",
        "rtr",
        "remotesent",
        "signInTime",
    };
    return (field>=0 && field<5) ? fieldNames[field] : NULL;
}

int ArbMsgDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='i' && strcmp(fieldName, "id")==0) return base+0;
    if (fieldName[0]=='n' && strcmp(fieldName, "node")==0) return base+1;
    if (fieldName[0]=='r' && strcmp(fieldName, "rtr")==0) return base+2;
    if (fieldName[0]=='r' && strcmp(fieldName, "remotesent")==0) return base+3;
    if (fieldName[0]=='s' && strcmp(fieldName, "signInTime")==0) return base+4;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *ArbMsgDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "int",
        "bool",
        "bool",
        "simtime_t",
    };
    return (field>=0 && field<5) ? fieldTypeStrings[field] : NULL;
}

const char *ArbMsgDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int ArbMsgDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    ArbMsg *pp = (ArbMsg *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string ArbMsgDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    ArbMsg *pp = (ArbMsg *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getId());
        case 1: return long2string(pp->getNode());
        case 2: return bool2string(pp->getRtr());
        case 3: return bool2string(pp->getRemotesent());
        case 4: return double2string(pp->getSignInTime());
        default: return "";
    }
}

bool ArbMsgDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    ArbMsg *pp = (ArbMsg *)object; (void)pp;
    switch (field) {
        case 0: pp->setId(string2long(value)); return true;
        case 1: pp->setNode(string2long(value)); return true;
        case 2: pp->setRtr(string2bool(value)); return true;
        case 3: pp->setRemotesent(string2bool(value)); return true;
        case 4: pp->setSignInTime(string2double(value)); return true;
        default: return false;
    }
}

const char *ArbMsgDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldStructNames[] = {
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
    };
    return (field>=0 && field<5) ? fieldStructNames[field] : NULL;
}

void *ArbMsgDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    ArbMsg *pp = (ArbMsg *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


