#include <iostream>
#include "AccountSvr.h"
#include "../Test/include/TestAccess.h"
#include "../Factory/BaseFactory.h"
#include "../include/baseHeader.h"
#include "../network/include/Acceptor.h"
#include "../network/include/Connector.h"
#include "../protocol/testMsg.pb.h"
#include "./include/ACCSessionHandler.hpp"
#include "../include/ServerInclude.hpp"
#include "../include/log4z.h"

#define ULIMITSVR 1

class CSession;

using namespace std;
extern void printItem(TestAccess *accessObj);

int main()
{
    using namespace zsummer::log4z;
    ILog4zManager::getRef().start();
    ILog4zManager::getRef().setLoggerLevel(LOG4Z_MAIN_LOGGER_ID,LOG_LEVEL_TRACE);
    //signal(SIGPIPE, SIG_IGN);
    //multi thread ignore SIGPIPE
    sigset_t bset, oset;
    sigemptyset(&bset);
    sigaddset(&bset, SIGPIPE);
    if (pthread_sigmask(SIG_BLOCK, &bset, &oset) != 0) 
    {
         printf("set thread signal mask fail!\n");
    }

    g_HandlerMgr = new CAccHandlerMgr;
    g_HandlerMgr->addAllHandle();

    AccountSvr *accountSvr = AccountSvr::GetInstance();
    LOGI("Hello world! ServerID is:" << accountSvr->getServerID());
    //g_ClientNetWorkObjectFactory.init(10000, 50);
    
    test_package::testMsg tmsg;
    tmsg.set_sendtime(123);
    tmsg.set_msg("protobuf hello world!!");
    
    int32 buflen = tmsg.ByteSize();
    
    char buf[buflen];
    tmsg.SerializeToArray(buf, buflen);

    test_package::testMsg after;
    after.ParseFromArray(buf, buflen);

    LOGI("after:" << after.sendtime() << "  msg:" << after.msg());
    
    accountSvr->start(); //listen start
    accountSvr->update();

    while (false)
    {
        LOGI("flash main thread");
        //conn.connect("127.0.0.1", 9997, eClient);
        sleep(1);
    }
    delete g_HandlerMgr;
    return 0;
}
