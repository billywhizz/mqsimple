#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <imqi.hpp>
#include "mqthread.h"

#define MQ_POOL_MAX_SIZE 1000
#define MQ_MAX_MESSAGE_SIZE 1024*64

MUTEX lock;
int bInitialised = 0;

typedef struct _mqpoolentry {
	ImqChannel * pchannel;
	ImqQueueManager mgr;
	ImqQueue queue;
	int buflen;
	int handle;
	int m_iPersistent;
	char buffer[ MQ_MAX_MESSAGE_SIZE ];
	char* m_sMessage;
	std::string m_sQueueDef;
	int m_iMode;
	bool m_bConnected;
	int inuse;
	MUTEX lock;
} POOLENTRY;

POOLENTRY* tMQPool[MQ_POOL_MAX_SIZE];

extern "C" int mqacquire(char* sQueueDef, int* iHandle, int iMode);
extern "C" int mqput(int iHandle, char* sMessage, int iPersistent);
extern "C" int mqget(int iHandle, char* sMessage, int iWait);
extern "C" int mqclose(int iHandle);
extern "C" int mqrelease(int iHandle);
