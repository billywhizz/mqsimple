using System;
using System.Xml;
using System.Security;
using System.Runtime.InteropServices;
using System.Text;

namespace com.devknowledge.components.mqsimple
{
	[SuppressUnmanagedCodeSecurityAttribute]
	public class MQSimple
	{
		private int iHandle = -1;
		private StringBuilder sb = new StringBuilder(1024 * 64);
		
		[DllImport ("mqsimple", CharSet=CharSet.Ansi)]
		private static extern int mqacquire(String sQueueDef, ref int iHandle, int iMode);
		[DllImport ("mqsimple", CharSet=CharSet.Ansi)]
		private static extern int mqrelease(int iHandle);
		[DllImport ("mqsimple", CharSet=CharSet.Ansi)]
		private static extern int mqclose(int iHandle);
		[DllImport ("mqsimple", CharSet=CharSet.Ansi)]
		private static extern int mqput(int iHandle, String sMessage, int iPersistent);
		[DllImport ("mqsimple", CharSet=CharSet.Ansi)]
		private static extern int mqget(int iHandle, StringBuilder sMessage, int iWait);

		public MQSimple()
		{
			iHandle = -1;
		}
		
		public int Handle
		{
			get
			{
				return iHandle;
			}
		}
		
		public int AcquireRead(string sQueueDef)
		{
			if(iHandle != -1) return 96;
			int iReturn = mqacquire(sQueueDef, ref iHandle, 0);
			if(iReturn == 0)
			{
				return iReturn;
			}
			else
			{
				return iReturn;
			}
		}
		
		public int AcquireWrite(string sQueueDef)
		{
			if(iHandle != -1) return 96;
			int iReturn = mqacquire(sQueueDef, ref iHandle, 1);
			return iReturn;
		}

		public int Release()
		{
			int iReturn = mqrelease(iHandle);
			// reset handle so object cannot be closed after it has been released.
			iHandle = -1;
			return iReturn;
		}
		
		public int Close()
		{
			if(iHandle == -1) return 97;
			int iReturn = mqclose(iHandle);
			if(iReturn == 0)
			{
				iReturn = mqrelease(iHandle);
				iHandle = -1;
				return iReturn;
			}
			else
			{
				return iReturn;
			}
		}
		
		public int Put(string sMessage, bool bPersistent)
		{
			if(iHandle == -1) return 97;
			int iReturn = mqput(iHandle, sMessage, bPersistent?1:0);
			return iReturn;
		}
		
		public int Get(ref string sMessage, int iWait)
		{
			if(iHandle == -1) return 97;
			int iReturn = mqget(iHandle, sb, iWait);
			if(iReturn == 0)
			{
				sMessage = sb.ToString();
				return iReturn;
			}
			else
			{
				return iReturn;
			}
		}
	}
}
