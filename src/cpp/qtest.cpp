#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mqsimple.h"

int main(int argc, char **argv)
{
	char sMessage[MQ_MAX_MESSAGE_SIZE];
	int iReturn = 0;
	int iReadHandle = 0;
	int iWriteHandle = 0;

	if(argc > 5)
	{
		char* sQueueDef = argv[1];
		int iMessages = atoi(argv[2]);
		int iWait = atoi(argv[3]);
		int iPersistent = atoi(argv[4]);
		int iMsgSize = atoi(argv[5]);
		
		// open queue for reading from pool
		iReturn = mqacquire(sQueueDef, &iReadHandle, 0);
		fprintf(stderr,"(%d) acquire read: %d\n", iReadHandle, iReturn);
		
		// open queue for writing from pool
		iReturn = mqacquire(sQueueDef, &iWriteHandle, 1);
		fprintf(stderr,"(%d) acquire write: %d\n", iWriteHandle, iReturn);

		if(iReturn == 0)
		{
			sprintf(sMessage, "%*s", iMsgSize, "");

			for(int i=1; i <= iMessages; i++)
			{
				// put message on queue
				iReturn = mqput(iWriteHandle, sMessage, iPersistent);
				fprintf(stderr,"(%d) put: %d\n", iWriteHandle, iReturn);

				// read message from queue
				iReturn = mqget(iReadHandle, sMessage, iWait);
				fprintf(stderr,"(%d) get: %d\n", iReadHandle, iReturn);
				if(iReturn==0)
				{
					fprintf(stdout,"[%s]\n", sMessage);
				}
			}

			// close handles
			iReturn = mqclose(iReadHandle);
			fprintf(stderr,"(%d) close: %d\n", iReadHandle, iReturn);
			iReturn = mqclose(iWriteHandle);
			fprintf(stderr,"(%d) close: %d\n", iWriteHandle, iReturn);

			// release handles back to pool
			iReturn = mqrelease(iReadHandle);
			fprintf(stderr,"(%d) release: %d\n", iReadHandle, iReturn);
			iReturn = mqrelease(iWriteHandle);
			fprintf(stderr,"(%d) release: %d\n", iWriteHandle, iReturn);

			// open queue for reading from pool
			iReturn = mqacquire(sQueueDef, &iReadHandle, 0);
			fprintf(stderr,"(%d) acquire read: %d\n", iReadHandle, iReturn);

			// read message from queue
			iReturn = mqget(iReadHandle, sMessage, iWait);
			fprintf(stderr,"(%d) get: %d\n", iReadHandle, iReturn);
			if(iReturn==0)
			{
				fprintf(stdout,"[%s]\n", sMessage);
			}
			
			// release read back to pool (without closing)
			iReturn = mqrelease(iReadHandle);
			fprintf(stderr,"(%d) release: %d\n", iReadHandle, iReturn);

			// open queue for reading from pool
			iReturn = mqacquire(sQueueDef, &iReadHandle, 0);
			fprintf(stderr,"(%d) acquire read: %d\n", iReadHandle, iReturn);

			// read message from queue
			iReturn = mqget(iReadHandle, sMessage, iWait);
			fprintf(stderr,"(%d) get: %d\n", iReadHandle, iReturn);
			if(iReturn==0)
			{
				fprintf(stdout,"[%s]\n", sMessage);
			}

			// close read handle
			iReturn = mqclose(iReadHandle);
			fprintf(stderr,"(%d) close: %d\n", iReadHandle, iReturn);

			// read message from queue (should reopen queue automatically)
			iReturn = mqget(iReadHandle, sMessage, iWait);
			fprintf(stderr,"(%d) get: %d\n", iReadHandle, iReturn);
			if(iReturn==0)
			{
				fprintf(stdout,"[%s]\n", sMessage);
			}
		}
	}
	else
	{
		fprintf(stderr,"usage:\nqtest \"CHANNEL/TRANSPORT/HOST(PORT)/QUEUE_MANAGER/QUEUE\" NUM_MSGS READ_TIMEOUT PERSISTENCE MESSAGE_SIZE\n");
		fprintf(stderr,"\tTRANSPORT: TCP|SPX|NETBIOS|LU62\n");
		fprintf(stderr,"\tREAD_TIMEOUT: number of milliseconds to wait for new message when reading empty queue\n");
		fprintf(stderr,"\tPERSISTENCE: 1=persistent, 0=non-persistent\n");
		fprintf(stderr,"\tMESSAGE_SIZE: Size of message to use (in bytes)\n");
		fprintf(stderr,"example:\nqtest \"QMBETFAIRCHANNEL/TCP/centos(1416)/qmbetfair/QMARKET\" 10 0 1\n");
		iReturn = 99;
	}
	return iReturn;
}
