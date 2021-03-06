#ifndef __ACCEPTOR_H__
#define __ACCEPTOR_H__
#include "../../include/baseHeader.h"
#include "../../Thread/BaseThread.h"
#include "../../Factory/BaseFactory.h"
#include "Session.h"
#include "CommonList.h"
#include "../../Thread/Mutex.h"
#include "../../include/CServerBase.hpp"

class EpollServer;

class Acceptor : public CBaseThread
{
    public:
        Acceptor(SESSION_TYPE type=eUndefineSessionType);
        virtual ~Acceptor();
        void init(int maxAcc = 30);
        void init(void *configStruct); //预留根据配置文件初始化接收器
        bool startListen(const char *szIP, Int32 nPort);
        void *threadRoutine(void *args);

        bool createSocket();

        inline bool isListen()
        {
            return m_boIsListen;
        }

        inline int getMaxAcc(void)
        {
            return m_nMaxAcc;
        }

        inline void addSession2List(CSession *session)
        {
            AutoLock lock(&m_acceptListLock);
            m_nCurrAccept++;
            //m_acceptList.push_back(session);
            m_pWriteList->push_back(session);
        }

        bool getAcceptList(std::list<CSession *>& retList);

        inline void sessionReUse(CSession *pSession)
        {
            m_SessionFactory.reuse(pSession);
        }

        void swapSessionList()
        {
            AutoLock listLock(&m_acceptListLock);
            CommonList<CSession> *tmpList = m_pReadList;
            m_pReadList = m_pWriteList;
            m_pWriteList = tmpList;
        }

        CommonList<CSession>* getReadSessionList()
        {
            return m_pReadList;
        }

        CommonList<CSession>* getWriteSessionList()
        {
            return m_pWriteList;
        }
        
        inline void setSvrType(eSERVERTYPE type)
        {
            m_svrType = type;
        }

        inline eSERVERTYPE getSvrType()
        {
            return m_svrType;
        }

        void setSocketNoBlock(int32 socket);

        inline void setServer(EpollServer *svr)
        {
            m_ptrServer = svr;
        }

        inline EpollServer* getServer()
        {
            return m_ptrServer;
        }

private:
        Int32 m_nMaxAcc;
        char m_listenIp[32];
        Int32 m_listenPort;
        Int32 m_nServerSock;
        Int32 m_epollfd;
        eSERVERTYPE m_eAcceptType;
        bool m_boIsListen;
        Int32 m_nMaxAccept;
        Int32 m_nCurrAccept;
        CMutex m_acceptListLock;
        CommonList<CSession> m_acceptList;  // first list
        CommonList<CSession> m_acceptListSec; //second list
        CommonList<CSession> *m_pReadList; // use double list to avoid mostly mutex competition
        CommonList<CSession> *m_pWriteList; 
        CBaseFactory<CSession> m_SessionFactory;
        eSERVERTYPE m_svrType;
        EpollServer *m_ptrServer;
};

#endif // __ACCEPTOR_H
