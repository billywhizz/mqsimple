#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mqsimple.h"

int main(int argc, char **argv)
{
	char sMessage[MQ_MAX_MESSAGE_SIZE];
	int iReturn = 0;
	int iReadHandle = 0;
	int iExit = 0;
	
	if(argc > 2)
	{
		char* sQueueDef = argv[1];
		int iWait = atoi(argv[2]);
		
		// open queue for reading from pool
		iReturn = mqacquire(sQueueDef, &iReadHandle, 0);
		fprintf(stderr,"(%d) acquire read: %d\n", iReadHandle, iReturn);
		
		if(iReturn == 0)
		{
			// read message from queue
			iReturn = mqget(iReadHandle, sMessage, iWait);
			fprintf(stderr,"(%d) get: %d\n", iReadHandle, iReturn);
			if(iReturn==0)
			{
				fprintf(stdout,"[%s]", sMessage);
			}
			else
			{
				iExit = 99;
			}
			// close handles
			iReturn = mqclose(iReadHandle);
			fprintf(stderr,"(%d) close: %d\n", iReadHandle, iReturn);
			// release handles back to pool
			iReturn = mqrelease(iReadHandle);
			fprintf(stderr,"(%d) release: %d\n", iReadHandle, iReturn);
		}
	}
	return iExit;
}
