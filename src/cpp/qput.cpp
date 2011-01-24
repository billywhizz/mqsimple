#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mqsimple.h"

int main(int argc, char **argv)
{
	char sMessage[MQ_MAX_MESSAGE_SIZE];
	int iReturn = 0;
	int iWriteHandle = 0;
	int iExit = 0;
	
	if(argc > 2)
	{
		char* sQueueDef = argv[1];
		int iPersistent = atoi(argv[2]);
		
		size_t result = fread(sMessage, 1, MQ_MAX_MESSAGE_SIZE, stdin);
		if(feof(stdin))
		{
			// open queue for reading from pool
			iReturn = mqacquire(sQueueDef, &iWriteHandle, 1);
			fprintf(stderr,"(%d) acquire write: %d\n", iWriteHandle, iReturn);
			
			if(iReturn == 0)
			{
				// write message to queue
				iReturn = mqput(iWriteHandle, sMessage, iPersistent);
				fprintf(stderr,"(%d) put: %d\n", iWriteHandle, iReturn);
				if(iReturn!=0)
				{
					iExit = 99;
				}
				// close handles
				iReturn = mqclose(iWriteHandle);
				fprintf(stderr,"(%d) close: %d\n", iWriteHandle, iReturn);
				// release handles back to pool
				iReturn = mqrelease(iWriteHandle);
				fprintf(stderr,"(%d) release: %d\n", iWriteHandle, iReturn);
			}
		}
		else
		{
			fprintf(stderr,"message too large: %d", result);
		}
	}
	return iExit;
}
