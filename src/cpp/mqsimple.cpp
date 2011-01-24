#include "mqsimple.h"

int parseuri(POOLENTRY *oPooledQueue)
{
	ImqString strParse(oPooledQueue->m_sQueueDef.c_str());
	ImqString strToken ;
	if(!oPooledQueue->m_bConnected)
	{
		oPooledQueue->pchannel = new ImqChannel ;
		oPooledQueue->pchannel -> setHeartBeatInterval( 1 );
		// Break down the channel definition,
		// which is of the form "channel-name/transport-type/connection-name/queuename".
		if ( strParse.cutOut( strToken, '/' ) )
		{
			oPooledQueue->pchannel -> setChannelName( strToken );
			if ( strParse.cutOut( strToken, '/' ) )
			{
				// Interpret the transport type.
				if ( strToken.upperCase( ) == (ImqString)"LU62" )
				{
					oPooledQueue->pchannel -> setTransportType( MQXPT_LU62 );
				}
				if ( strToken.upperCase( ) == (ImqString)"NETBIOS" )
				{
					oPooledQueue->pchannel -> setTransportType( MQXPT_NETBIOS );
				}
				if ( strToken.upperCase( ) == (ImqString)"SPX" )
				{
					oPooledQueue->pchannel -> setTransportType( MQXPT_SPX );
				}
				if ( strToken.upperCase( ) == (ImqString)"TCP" )
				{
					oPooledQueue->pchannel -> setTransportType( MQXPT_TCP );
				}
				// Establish the connection name.
				if ( strParse.cutOut( strToken, '/' ) )
				{
					oPooledQueue->pchannel -> setConnectionName( strToken );
					oPooledQueue->mgr.setChannelReference( oPooledQueue->pchannel );
					if ( strParse.cutOut( strToken, '/' ) )
					{
						oPooledQueue->mgr.setName(strToken);
						if (!oPooledQueue->mgr.connect())
						{
							fprintf(stderr, "ImqQueueManager::connect failed with reason code %ld\n", (long)oPooledQueue->mgr.reasonCode( ) );
							return(99);
						}
						if ( strParse.cutOut(strToken) )
						{
							oPooledQueue->queue.setConnectionReference(oPooledQueue->mgr);
							oPooledQueue->queue.setName(strToken);
							if(oPooledQueue->m_iMode == 0)
							{
								//queue.setOpenOptions(MQOO_INPUT_AS_Q_DEF + MQOO_FAIL_IF_QUIESCING + MQC.MQGMO_SYNCPOINT);
								oPooledQueue->queue.setOpenOptions(MQOO_INQUIRE + MQOO_INPUT_AS_Q_DEF + MQOO_FAIL_IF_QUIESCING);
							}
							else
							{
								//queue.setOpenOptions(MQOO_OUTPUT + MQOO_FAIL_IF_QUIESCING + MQC.MQGMO_SYNCPOINT);
								oPooledQueue->queue.setOpenOptions(MQOO_INQUIRE + MQOO_OUTPUT + MQOO_FAIL_IF_QUIESCING);
							}
							oPooledQueue->queue.open();
							fprintf(stderr, "open %d\n", oPooledQueue->handle);
							if(oPooledQueue->queue.reasonCode())
							{
								fprintf(stderr, "ImqQueue::open ended with reason code %d\n", (int)oPooledQueue->queue.reasonCode( ) );
								return(99);
							}
							if (oPooledQueue->queue.completionCode()==MQCC_FAILED)
							{
								fprintf(stderr, "unable to open queue for output\n" );
								return(99);
							}
							oPooledQueue->m_bConnected = true;
							return 0;
						}
						else
						{
							fprintf(stderr,"could not parse queue\n");
							return(99);
						}
					}
					else
					{
						fprintf(stderr,"could not parse qmanager\n");
						return(99);
					}
				}
				else
				{
					fprintf(stderr,"could not parse connection\n");
					return(99);
				}
			}
			else
			{
				fprintf(stderr,"could not parse transport\n");
				return(99);
			}
		}
		else
		{
			fprintf(stderr,"could not parse channel\n");
			return(99);
		}
	}
	else
	{
		return 0;
	}
}

int sendMessage(POOLENTRY *oPooledQueue)
{
	ImqMessage msg;
	msg.useEmptyBuffer( oPooledQueue->buffer, sizeof( oPooledQueue->buffer ) );
	msg.setFormat( MQFMT_STRING );
	msg.setPersistence( oPooledQueue->m_iPersistent );
	strcpy(oPooledQueue->buffer, oPooledQueue->m_sMessage);
	oPooledQueue->buflen = (int)strlen( oPooledQueue->buffer );
	if ( oPooledQueue->buflen > 0 )
	{
		msg.setMessageLength( oPooledQueue->buflen );
		if ( ! oPooledQueue->queue.put( msg ) )
		{
			parseuri(oPooledQueue);
			fprintf(stderr, "ImqQueue::put ended with reason code %d\n",(int)oPooledQueue->queue.reasonCode( ) );
			return(99);
		}
		if(oPooledQueue->queue.completionCode() == MQCC_FAILED)
		{
			parseuri(oPooledQueue);
			fprintf(stderr, "ImqQueue::put failed with reason code %d\n",(int)oPooledQueue->queue.reasonCode( ) );
			return(99);
		}
	}
	else
	{
		return(99);
	}
	return 0;
}

int receiveMessage(POOLENTRY *oPooledQueue, char* sMessage, int iWait)
{
	ImqMessage msg;
	strcpy(sMessage, "");
	//msg.useEmptyBuffer( oPooledQueue->buffer, sizeof( oPooledQueue->buffer ) );
	ImqGetMessageOptions gmo;        // Get message options
	msg.useEmptyBuffer( oPooledQueue->buffer, sizeof( oPooledQueue->buffer ) );
	gmo.setOptions( MQGMO_WAIT | MQGMO_FAIL_IF_QUIESCING );
	gmo.setWaitInterval( iWait );
	sMessage[0] = 0;	
   	if ( ! oPooledQueue->queue.get( msg, gmo ) )
	{
		if(oPooledQueue->queue.completionCode() == MQCC_FAILED)
		{
			if ( oPooledQueue->queue.reasonCode( ) == MQRC_NO_MSG_AVAILABLE ) 
			{
				fprintf(stderr, "no more messages\n" );
				return(100);
			}
			else
			{
				fprintf(stderr, "ImqQueue::get failed with reason code %d\n",(int)oPooledQueue->queue.reasonCode( ) );
				return(99);
			}
		}
	}
	else
	{
		if ( oPooledQueue->queue.reasonCode( ) == MQRC_TRUNCATED_MSG_FAILED )
		{
			fprintf(stderr, "ImqQueue::get failed - Message Truncated %d\n",(int)oPooledQueue->queue.reasonCode( ) );
			return(99);
		}
		if ( msg.formatIs( MQFMT_STRING ) )
		{
			//fprintf(stdout, "[%s]\n", (const char*)oPooledQueue->buffer);
			oPooledQueue->buffer[ msg.dataLength( ) ] = 0 ;
			strcpy(sMessage, oPooledQueue->buffer);
		}
		else
		{
			fprintf(stderr, "Non-text message\n" );
			return 99;
		}
	}
	return 0;
}

int close(POOLENTRY *oPooledQueue)
{
	if ( ! oPooledQueue->queue.close( ) )
	{
		fprintf(stderr, "ImqQueue::close ended with reason code %d\n",(int)oPooledQueue->queue.reasonCode( ) );
		return(99);
	}
	if ( ! oPooledQueue->mgr.disconnect( ) )
	{
		fprintf(stderr, "ImqQueueManager::disconnect ended with reason code %d\n",(int)oPooledQueue->mgr.reasonCode( ) );
		return(99);
	}
	if ( oPooledQueue->pchannel )
	{
		oPooledQueue->mgr.setChannelReference( );
		delete oPooledQueue->pchannel ;
	}
	fprintf(stderr, "close %d\n", oPooledQueue->handle);
	oPooledQueue->inuse = 0;
	oPooledQueue->m_bConnected = false;
	return 0;
}

POOLENTRY* getQueue(char* sQueueDef, int iMode, int* iHandle)
{
	if(bInitialised == 0)
	{
		MUTEX_INIT(&lock);
		bInitialised = 1;
	}
	POOLENTRY *oQueue;
	*iHandle = -1;
	MUTEX_LOCK(&lock);
	for(int i=0; i<MQ_POOL_MAX_SIZE; i++)
	{
		if(tMQPool[i] != 0)
		{
			int iCompare = strncmp(tMQPool[i]->m_sQueueDef.c_str(), sQueueDef, strlen(sQueueDef));
			if(iCompare==0 && tMQPool[i]->m_iMode == iMode && tMQPool[i]->inuse == 0)
			{
				*iHandle = i;
				tMQPool[i]->inuse = 1;
				MUTEX_UNLOCK(&lock);
				return tMQPool[i];
			}
		}
		else
		{
			*iHandle = i;
		}
	}
	if(*iHandle == -1)
	{
		MUTEX_UNLOCK(&lock);
		return NULL;
	}
	else
	{
		int iReturn = 0;
		oQueue = new POOLENTRY;
		oQueue->m_sQueueDef = std::string(sQueueDef);
		fprintf(stderr, "Queue %s\n", oQueue->m_sQueueDef.c_str());
		oQueue->m_iMode = iMode;
		oQueue->inuse = 1;
		oQueue->m_bConnected = false;
		oQueue->handle = *iHandle;
		MUTEX_INIT(&oQueue->lock);
		iReturn = parseuri(oQueue);
		if(iReturn!=0)
		{
			fprintf(stderr,"parse failed\n");
			MUTEX_UNLOCK(&lock);
			return NULL;
		}
		else
		{
			oQueue->m_bConnected = true;
		}
		tMQPool[*iHandle] = oQueue;
		MUTEX_UNLOCK(&lock);
		return oQueue;
	}
}

extern "C" int mqacquire(char* sQueueDef, int* iHandle, int iMode)
{
	POOLENTRY *oPooledQueue = getQueue(sQueueDef, iMode, iHandle);
	if(oPooledQueue == NULL) return 99;
	if(!oPooledQueue->m_bConnected) parseuri(oPooledQueue);
	return 0;
}

extern "C" int mqclose(int iHandle)
{
	POOLENTRY *oPooledQueue = tMQPool[iHandle];
	if(oPooledQueue == 0) return 99;
	if(oPooledQueue->m_bConnected) return close(oPooledQueue);
	return 0;
}

extern "C" int mqrelease(int iHandle)
{
	POOLENTRY *oPooledQueue = tMQPool[iHandle];
	if(oPooledQueue == 0) return 99;
	oPooledQueue->inuse = 0;
	return 0;
}

extern "C" int mqput(int iHandle, char* sMessage, int iPersistent)
{
	if(strlen(sMessage) > MQ_MAX_MESSAGE_SIZE)
	{
		return 99;
	}
	else
	{
		POOLENTRY *oPooledQueue = tMQPool[iHandle];
		if(oPooledQueue == 0) return 99;
		if(!oPooledQueue->m_bConnected) parseuri(oPooledQueue);
		oPooledQueue->m_sMessage = sMessage;
		oPooledQueue->m_iPersistent = iPersistent;
		return sendMessage(oPooledQueue);
	}
}

extern "C" int mqget(int iHandle, char* sMessage, int iWait)
{
	POOLENTRY *oPooledQueue = tMQPool[iHandle];
	if(oPooledQueue == 0) return 99;
	if(!oPooledQueue->m_bConnected) parseuri(oPooledQueue);
	//oPooledQueue->m_sMessage = sMessage;
	return receiveMessage(oPooledQueue, sMessage, iWait);
}
