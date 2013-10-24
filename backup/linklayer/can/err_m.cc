//
// Generated file, do not edit! Created by opp_msgc 4.2 from linklayer/can/err.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "err_m.h"

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




Register_Class(ErrorFrame);

ErrorFrame::ErrorFrame(const char *name, int kind) : cMessage(name,kind)
{
    this->id_var = 0;
    this->node_var = 0;
    this->kind_var = 0;
    this->pos_var = 0;
}

ErrorFrame::ErrorFrame(const ErrorFrame& other) : cMessage(other)
{
    copy(other);
}

ErrorFrame::~ErrorFrame()
{
}

ErrorFrame& ErrorFrame::operator=(const ErrorFrame& other)
{
    if (this==&other) return *this;
    cMessage::operator=(other);
    copy(other);
    return *this;
}

void ErrorFrame::copy(const ErrorFrame& other)
{
    this->id_var = other.id_var;
    this->node_var = other.node_var;
    this->kind_var = other.kind_var;
    this->pos_var = other.pos_var;
}

void ErrorFrame::parsimPack(cCommBuffer *b)
{
    cMessage::parsimPack(b);
    doPacking(b,this->id_var);
    doPacking(b,this->node_var);
    doPacking(b,this->kind_var);
    doPacking(b,this->pos_var);
}

void ErrorFrame::parsimUnpack(cCommBuffer *b)
{
    cMessage::parsimUnpack(b);
    doUnpacking(b,this->id_var);
    doUnpacking(b,this->node_var);
    doUnpacking(b,this->kind_var);
    doUnpacking(b,this->pos_var);
}

int ErrorFrame::getId() const
{
    return id_var;
}

void ErrorFrame::setId(int id)
{
    this->id_var = id;
}

int ErrorFrame::getNode() const
{
    return node_var;
}

void ErrorFrame::setNode(int node)
{
    this->node_var = node;
}

int ErrorFrame::getKind() const
{
    return kind_var;
}

void ErrorFrame::setKind(int kind)
{
    this->kind_var = kind;
}

int ErrorFrame::getPos() const
{
    return pos_var;
}

void ErrorFrame::setPos(int pos)
{
    this->pos_var = pos;
}

class ErrorFrameDescriptor : public cClassDescriptor
{
  public:
    ErrorFrameDescriptor();
    virtual ~ErrorFrameDescriptor();

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

Register_ClassDescriptor(ErrorFrameDescriptor);

ErrorFrameDescriptor::ErrorFrameDescriptor() : cClassDescriptor("ErrorFrame", "cMessage")
{
}

ErrorFrameDescriptor::~ErrorFrameDescriptor()
{
}

bool ErrorFrameDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<ErrorFrame *>(obj)!=NULL;
}

const char *ErrorFrameDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int ErrorFrameDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 4+basedesc->getFieldCount(object) : 4;
}

unsigned int ErrorFrameDescriptor::getFieldTypeFlags(void *object, int field) const
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
    };
    return (field>=0 && field<4) ? fieldTypeFlags[field] : 0;
}

const char *ErrorFrameDescriptor::getFieldName(void *object, int field) const
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
        "kind",
        "pos",
    };
    return (field>=0 && field<4) ? fieldNames[field] : NULL;
}

int ErrorFrameDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='i' && strcmp(fieldName, "id")==0) return base+0;
    if (fieldName[0]=='n' && strcmp(fieldName, "node")==0) return base+1;
    if (fieldName[0]=='k' && strcmp(fieldName, "kind")==0) return base+2;
    if (fieldName[0]=='p' && strcmp(fieldName, "pos")==0) return base+3;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *ErrorFrameDescriptor::getFieldTypeString(void *object, int field) const
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
        "int",
        "int",
    };
    return (field>=0 && field<4) ? fieldTypeStrings[field] : NULL;
}

const char *ErrorFrameDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int ErrorFrameDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    ErrorFrame *pp = (ErrorFrame *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string ErrorFrameDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    ErrorFrame *pp = (ErrorFrame *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getId());
        case 1: return long2string(pp->getNode());
        case 2: return long2string(pp->getKind());
        case 3: return long2string(pp->getPos());
        default: return "";
    }
}

bool ErrorFrameDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    ErrorFrame *pp = (ErrorFrame *)object; (void)pp;
    switch (field) {
        case 0: pp->setId(string2long(value)); return true;
        case 1: pp->setNode(string2long(value)); return true;
        case 2: pp->setKind(string2long(value)); return true;
        case 3: pp->setPos(string2long(value)); return true;
        default: return false;
    }
}

const char *ErrorFrameDescriptor::getFieldStructName(void *object, int field) const
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
    };
    return (field>=0 && field<4) ? fieldStructNames[field] : NULL;
}

void *ErrorFrameDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    ErrorFrame *pp = (ErrorFrame *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


