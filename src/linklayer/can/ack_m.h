//
// Generated file, do not edit! Created by opp_msgc 4.3 from linklayer/can/ack.msg.
//

#ifndef _ACK_M_H_
#define _ACK_M_H_

#include <omnetpp.h>

// opp_msgc version check
#define MSGC_VERSION 0x0403
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of opp_msgc: 'make clean' should help.
#endif



/**
 * Class generated from <tt>linklayer/can/ack.msg</tt> by opp_msgc.
 * <pre>
 * message AckMsg{
 * 	bool ack; 	
 * 	int id;		
 * }
 * </pre>
 */
class AckMsg : public ::cMessage
{
  protected:
    bool ack_var;
    int id_var;

  private:
    void copy(const AckMsg& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const AckMsg&);

  public:
    AckMsg(const char *name=NULL, int kind=0);
    AckMsg(const AckMsg& other);
    virtual ~AckMsg();
    AckMsg& operator=(const AckMsg& other);
    virtual AckMsg *dup() const {return new AckMsg(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual bool getAck() const;
    virtual void setAck(bool ack);
    virtual int getId() const;
    virtual void setId(int id);
};

inline void doPacking(cCommBuffer *b, AckMsg& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, AckMsg& obj) {obj.parsimUnpack(b);}


#endif // _ACK_M_H_