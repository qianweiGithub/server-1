#ifndef __STRICTCLIENT_H__
#define __STRICTCLIENT_H__

#include "../network/include/NetWorkObject.h"
#include "../include/baseHeader.h"
#include "../include/PackageHandler.hpp"
#include "../include/packHeader.hpp"
#include "../include/packageStruct.hpp"


class StrictClient : public NetWorkObject
{
    public:
        StrictClient();
        virtual ~StrictClient();
        int32 onRecv(PkgHeader *header, char *msgbuf, int32 buffsize);
        int32 onRecv(PkgHeader *header, MsgHeader *msghead, char *msgbuf, int32 buffsize);
        int32 processSend(uint16 sysid, uint16 msgid, char *msgbuf, int32 bufsize);
        int32 testRefectSvr(char *msgbuf, int32 bufsize);
        int32 testRefectSvr(MsgHeader *msghead, char *msgbuf, int32 bufsize);
    protected:
    private:
        uint64 m_llpkgCount;
        int32 m_nNextTick;
};

#endif // StrictClient_H