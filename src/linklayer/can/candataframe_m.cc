//
// Generated file, do not edit! Created by opp_msgc 4.3 from linklayer/can/candataframe.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "candataframe_m.h"

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




Register_Class(CanDataFrame);

CanDataFrame::CanDataFrame(const char *name, int kind) : cMessage(name,kind)
{
    this->node_var = 0;
    this->canID_var = 0;
    this->length_var = 0;
    this->rtr_var = 0;
    for (unsigned int i=0; i<8; i++)
        this->data_var[i] = 0;
    this->startTime_var = 0;
    this->period_var = 0;
}

CanDataFrame::CanDataFrame(const CanDataFrame& other) : cMessage(other)
{
    copy(other);
}

CanDataFrame::~CanDataFrame()
{
}

CanDataFrame& CanDataFrame::operator=(const CanDataFrame& other)
{
    if (this==&other) return *this;
    cMessage::operator=(other);
    copy(other);
    return *this;
}

void CanDataFrame::copy(const CanDataFrame& other)
{
    this->node_var = other.node_var;
    this->canID_var = other.canID_var;
    this->length_var = other.length_var;
    this->rtr_var = other.rtr_var;
    for (unsigned int i=0; i<8; i++)
        this->data_var[i] = other.data_var[i];
    this->startTime_var = other.startTime_var;
    this->period_var = other.period_var;
}

void CanDataFrame::parsimPack(cCommBuffer *b)
{
    cMessage::parsimPack(b);
    doPacking(b,this->node_var);
    doPacking(b,this->canID_var);
    doPacking(b,this->length_var);
    doPacking(b,this->rtr_var);
    doPacking(b,this->data_var,8);
    doPacking(b,this->startTime_var);
    doPacking(b,this->period_var);
}

void CanDataFrame::parsimUnpack(cCommBuffer *b)
{
    cMessage::parsimUnpack(b);
    doUnpacking(b,this->node_var);
    doUnpacking(b,this->canID_var);
    doUnpacking(b,this->length_var);
    doUnpacking(b,this->rtr_var);
    doUnpacking(b,this->data_var,8);
    doUnpacking(b,this->startTime_var);
    doUnpacking(b,this->period_var);
}

const char * CanDataFrame::getNode() const
{
    return node_var.c_str();
}

void CanDataFrame::setNode(const char * node)
{
    this->node_var = node;
}

int CanDataFrame::getCanID() const
{
    return canID_var;
}

void CanDataFrame::setCanID(int canID)
{
    this->canID_var = canID;
}

int CanDataFrame::getLength() const
{
    return length_var;
}

void CanDataFrame::setLength(int length)
{
    this->length_var = length;
}

bool CanDataFrame::getRtr() const
{
    return rtr_var;
}

void CanDataFrame::setRtr(bool rtr)
{
    this->rtr_var = rtr;
}

unsigned int CanDataFrame::getDataArraySize() const
{
    return 8;
}

char CanDataFrame::getData(unsigned int k) const
{
    if (k>=8) throw cRuntimeError("Array of size 8 indexed by %lu", (unsigned long)k);
    return data_var[k];
}

void CanDataFrame::setData(unsigned int k, char data)
{
    if (k>=8) throw cRuntimeError("Array of size 8 indexed by %lu", (unsigned long)k);
    this->data_var[k] = data;
}

simtime_t CanDataFrame::getStartTime() const
{
    return startTime_var;
}

void CanDataFrame::setStartTime(simtime_t startTime)
{
    this->startTime_var = startTime;
}

int CanDataFrame::getPeriod() const
{
    return period_var;
}

void CanDataFrame::setPeriod(int period)
{
    this->period_var = period;
}

class CanDataFrameDescriptor : public cClassDescriptor
{
  public:
    CanDataFrameDescriptor();
    virtual ~CanDataFrameDescriptor();

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

Register_ClassDescriptor(CanDataFrameDescriptor);

CanDataFrameDescriptor::CanDataFrameDescriptor() : cClassDescriptor("CanDataFrame", "cMessage")
{
}

CanDataFrameDescriptor::~CanDataFrameDescriptor()
{
}

bool CanDataFrameDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<CanDataFrame *>(obj)!=NULL;
}

const char *CanDataFrameDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int CanDataFrameDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 7+basedesc->getFieldCount(object) : 7;
}

unsigned int CanDataFrameDescriptor::getFieldTypeFlags(void *object, int field) const
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
        FD_ISARRAY | FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<7) ? fieldTypeFlags[field] : 0;
}

const char *CanDataFrameDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "node",
        "canID",
        "length",
        "rtr",
        "data",
        "startTime",
        "period",
    };
    return (field>=0 && field<7) ? fieldNames[field] : NULL;
}

int CanDataFrameDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='n' && strcmp(fieldName, "node")==0) return base+0;
    if (fieldName[0]=='c' && strcmp(fieldName, "canID")==0) return base+1;
    if (fieldName[0]=='l' && strcmp(fieldName, "length")==0) return base+2;
    if (fieldName[0]=='r' && strcmp(fieldName, "rtr")==0) return base+3;
    if (fieldName[0]=='d' && strcmp(fieldName, "data")==0) return base+4;
    if (fieldName[0]=='s' && strcmp(fieldName, "startTime")==0) return base+5;
    if (fieldName[0]=='p' && strcmp(fieldName, "period")==0) return base+6;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *CanDataFrameDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "string",
        "int",
        "int",
        "bool",
        "char",
        "simtime_t",
        "int",
    };
    return (field>=0 && field<7) ? fieldTypeStrings[field] : NULL;
}

const char *CanDataFrameDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int CanDataFrameDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    CanDataFrame *pp = (CanDataFrame *)object; (void)pp;
    switch (field) {
        case 4: return 8;
        default: return 0;
    }
}

std::string CanDataFrameDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    CanDataFrame *pp = (CanDataFrame *)object; (void)pp;
    switch (field) {
        case 0: return oppstring2string(pp->getNode());
        case 1: return long2string(pp->getCanID());
        case 2: return long2string(pp->getLength());
        case 3: return bool2string(pp->getRtr());
        case 4: return long2string(pp->getData(i));
        case 5: return double2string(pp->getStartTime());
        case 6: return long2string(pp->getPeriod());
        default: return "";
    }
}

bool CanDataFrameDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    CanDataFrame *pp = (CanDataFrame *)object; (void)pp;
    switch (field) {
        case 0: pp->setNode((value)); return true;
        case 1: pp->setCanID(string2long(value)); return true;
        case 2: pp->setLength(string2long(value)); return true;
        case 3: pp->setRtr(string2bool(value)); return true;
        case 4: pp->setData(i,string2long(value)); return true;
        case 5: pp->setStartTime(string2double(value)); return true;
        case 6: pp->setPeriod(string2long(value)); return true;
        default: return false;
    }
}

const char *CanDataFrameDescriptor::getFieldStructName(void *object, int field) const
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
        NULL,
        NULL,
    };
    return (field>=0 && field<7) ? fieldStructNames[field] : NULL;
}

void *CanDataFrameDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    CanDataFrame *pp = (CanDataFrame *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


