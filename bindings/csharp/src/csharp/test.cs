using System;
using System.Xml;
using System.Runtime.InteropServices;
using System.Text;
using com.devknowledge.components.mqsimple;

class LibTest
{
	[STAThread]
	static int Main(String[] args)
	{
		int iReturn = 0;

		if(args.Length == 5)
		{
			string sQueueDef = args[0];
			int iMessages = Convert.ToInt32(args[1]);
			int iWait = Convert.ToInt32(args[2]);
			int iPersistent = Convert.ToInt32(args[3]);
			int iMessageSize = Convert.ToInt32(args[4]);
			bool bPersistent = iPersistent==1?true:false;
			
			string sMessage = new string(' ', iMessageSize);
	
			MQSimple oMQSimpleRead = new MQSimple();
			MQSimple oMQSimpleWrite = new MQSimple();
	
			iReturn = oMQSimpleRead.AcquireRead(sQueueDef);
			System.Console.Write("({0}) acquire read: {1}\n", oMQSimpleRead.Handle, iReturn);
			iReturn = oMQSimpleWrite.AcquireWrite(sQueueDef);
			System.Console.Write("({0}) acquire write: {1}\n", oMQSimpleWrite.Handle, iReturn);

			if(iReturn == 0)
			{
				for(int i=1; i <= iMessages; i++)
				{
					// put message on queue
					iReturn = oMQSimpleWrite.Put(sMessage, bPersistent);
					System.Console.Write("({0}) put: {1}\n", oMQSimpleWrite.Handle, iReturn);
	
					// read message from queue
					iReturn = oMQSimpleRead.Get(ref sMessage, iWait);
					System.Console.Write("({0}) get: {1}\n", oMQSimpleRead.Handle, iReturn);
					if(iReturn==0)
					{
						System.Console.Write("[{0}]\n", sMessage);
					}
				}
	
				// close handles
				iReturn = oMQSimpleRead.Close();
				System.Console.Write("({0}) close: {1}\n", oMQSimpleRead.Handle, iReturn);
				iReturn = oMQSimpleWrite.Close();
				System.Console.Write("({0}) close: {1}\n", oMQSimpleWrite.Handle, iReturn);
	
				// release handles back to pool
				iReturn = oMQSimpleRead.Release();
				System.Console.Write("({0}) release: {1}\n", oMQSimpleRead.Handle, iReturn);
				iReturn = oMQSimpleWrite.Release();
				System.Console.Write("({0}) release: {1}\n", oMQSimpleWrite.Handle, iReturn);
	
				// open queue for reading from pool
				iReturn = oMQSimpleRead.AcquireRead(sQueueDef);
				System.Console.Write("({0}) acquire read: {1}\n", oMQSimpleRead.Handle, iReturn);
	
				// read message from queue
				iReturn = oMQSimpleWrite.Get(ref sMessage, iWait);
				System.Console.Write("({0}) get: {1}\n", oMQSimpleRead.Handle, iReturn);
				if(iReturn==0)
				{
					System.Console.Write("[{0}]\n", sMessage);
				}
				
				// release read back to pool (without closing)
				iReturn = oMQSimpleRead.Release();
				System.Console.Write("({0}) release: {1}\n", oMQSimpleRead.Handle, iReturn);
	
				// open queue for reading from pool
				iReturn = oMQSimpleRead.AcquireRead(sQueueDef);
				System.Console.Write("({0}) acquire read: {1}\n", oMQSimpleRead.Handle, iReturn);
	
				// read message from queue
				iReturn = oMQSimpleWrite.Get(ref sMessage, iWait);
				System.Console.Write("({0}) get: {1}\n", oMQSimpleRead.Handle, iReturn);
				if(iReturn==0)
				{
					System.Console.Write("[{0}]\n", sMessage);
				}
	
				// close read handle
				iReturn = oMQSimpleRead.Close();
				System.Console.Write("({0}) close: {1}\n", oMQSimpleRead.Handle, iReturn);
	
				// read message from queue (should reopen queue automatically)
				iReturn = oMQSimpleWrite.Get(ref sMessage, iWait);
				System.Console.Write("({0}) get: {1}\n", oMQSimpleRead.Handle, iReturn);
				if(iReturn==0)
				{
					System.Console.Write("[{0}]\n", sMessage);
				}
			}
		}
		else
		{
			System.Console.Write("usage:\nqtest \"CHANNEL/TRANSPORT/HOST(PORT)/QUEUE_MANAGER/QUEUE\" NUM_MSGS READ_TIMEOUT PERSISTENCE MESSAGE_SIZE\n");
			System.Console.Write("\tTRANSPORT: TCP|SPX|NETBIOS|LU62\n");
			System.Console.Write("\tREAD_TIMEOUT: number of milliseconds to wait for new message when reading empty queue\n");
			System.Console.Write("\tPERSISTENCE: 1=persistent, 0=non-persistent\n");
			System.Console.Write("\tMESSAGE_SIZE: Size of message to use (in bytes)\n");
			System.Console.Write("example:\nqtest \"QMBETFAIRCHANNEL/TCP/centos(1416)/qmbetfair/QMARKET\" 10 0 1\n");
			iReturn = 99;
		}
		return iReturn;
	}
}
