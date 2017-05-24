#include "stdafx.h"
#include "GPS.h"
#include "BinaryMSG.h"
#include "Serial.h"
#include "WaitReadLog.h"
#include "Redir.h"
#include "MaskedBitmap.h" 
#include "Monitor_1PPS.h"
#include "PositionRateDlg.h"
#include "SysRestartDlg.h"
#include "SnrBarChart.h" 
#include "Pic_Earth.h"
#include "Pic_Scatter.h"
#include "Config1ppsPulseWidthDlg.h"
#include "Timing_start.h"
#include "Config_Proprietary.h"
#include "ConAntennaDetection.h"
#include "Con1PPS_OutputMode.h"
#include "ConfigNmeaIntervalDlg.h"
#include "ConDauDlg.h"
#include "ConSrePorDlg.h"
#include "DrMultiHzDlg.h"
#include "Config1ppsFrequenceOutput.h"
#include "ConfigEricssonIntervalDlg.h"
#include "GetAlmanac.h"
#include "CommonConfigDlg.h"
#include "GPSDlg.h"
#include "DrTestDlg.h"

struct CommandEntry
{
	U08 cmdId;
	U08 cmdSubId;
	U16 cmdSize;
	U08 cmdAck;
	U08 cmdAckSub;
};

static CommandEntry cmdTable[] =
{
	//QuerySwVerSysCmd
	{ 0x02, 0x01, 2, 0x80, 0x00 },
	//QuerySwCrcSysCmd
	{ 0x03, 0x01, 2, 0x81, 0x00 },
	//QueryCustomerIDCmd,
	{ 0x0D, 0xFF, 1, 0x92, 0x00 },
	//QueryPositionRateCmd
	{ 0x10, 0xFF, 1, 0x86, 0x00 },
	//QueryPowerModeCmd
	{ 0x15, 0xFF, 1, 0xB9, 0x00 },
	//QueryDatalogLogStatusCmd,
	{ 0x17, 0xFF, 1, 0x94, 0x00 },
	//QueryBinaryMeasurementDataOutCmd,
	{ 0x1F, 0xFF, 1, 0x89, 0x00 },
	//QueryRtcmMeasurementDataOutCmd,
	{ 0x21, 0xFF, 1, 0x8A, 0x00 },
	//QueryBasePositionCmd,
	{ 0x23, 0xFF, 1, 0x8B, 0x00 },
	//QueryDatumCmd
	{ 0x2D, 0xFF, 1, 0xAE, 0x00 },
	//QueryDopMaskCmd
	{ 0x2E, 0xFF, 1, 0xAF, 0x00 },
	//QueryElevationAndCnrMaskCmd,
	{ 0x2F, 0xFF, 1, 0xB0, 0x00 },
	//QueryAgpsStatusCmd,
	{ 0x34, 0xFF, 8, 0xB2, 0x00 },
	//QueryPositionPinningCmd
	{ 0x3A, 0xFF, 1, 0xB4, 0x00 },
	//Query1ppsModeCmd
	{ 0x3F, 0xFF, 1, 0xB6, 0x00 },
	//QueryNoisePowerCmd,
	{ 0x40, 0xFF, 1, 0xB7, 0x00 },
	//QueryTimingCmd
	{ 0x44, 0xFF, 1, 0xC2, 0x00 },
	//QueryCableDelayCmd,
	{ 0x46, 0xFF, 1, 0xBB, 0x00 },
	//QueryAntennaDetectionCmd,
	{ 0x48, 0xFF, 1, 0xBC, 0x00 },
	//QueryProprietaryMessageCmd
	{ 0x4A, 0xFF, 1, 0xBF, 0x00 },
	//QueryGnssNmeaTalkIdCmd,
	{ 0x4F, 0xFF, 1, 0x93, 0x00 },
	//GetGpAlmanacCmd,
	{ 0x50, 0xFF, 2, 0xBE, 0x00 },
	//QuerySbasCmd,
	{ 0x62, 0x02, 2, 0x62, 0x80 },
	//QueryQzssCmd,
	{ 0x62, 0x04, 2, 0x62, 0x81 },
	//QuerySbas2Cmd,
	{ 0x62, 0x06, 2, 0x62, 0x82 },
	//QuerySbasDefaultCmd,
	{ 0x62, 0x07, 2, 0x62, 0x83 },
	//QuerySagpsCmd,
	{ 0x63, 0x02, 2, 0x63, 0x80 },
	//QueryGnssBootStatusCmd,
	{ 0x64, 0x01, 2, 0x64, 0x80 },
	//QueryNmeaIntervalV8Cmd,
	{ 0x64, 0x03, 2, 0x64, 0x81 },
	//QueryNmeaBinaryOutputDestinationCmd,
	{ 0x64, 0x05, 2, 0x64, 0x82 },
	//QueryInterferenceDetectControlCmd,
	{ 0x64, 0x07, 2, 0x64, 0x83 },
	//QueryNoisePowerControlCmd,
	{ 0x64, 0x09, 2, 0x64, 0x84 },
	//QueryParameterSearchEngineNumberCmd,
	{ 0x64, 0x0b, 2, 0x64, 0x85 },
	//QueryV8PowerSaveParameters,
	{ 0x64, 0x0D, 2, 0x64, 0x86 },
	//QueryPositionFixNavigationMaskCmd,
	{ 0x64, 0x12, 2, 0x64, 0x88 },
	//QueryRefTimeSyncToGpsTimeCmd,
	{ 0x64, 0x16, 2, 0x64, 0x8A },
	//QueryNavigationModeV8Cmd,
	{ 0x64, 0x18, 2, 0x64, 0x8B },
	//QueryGnssNavSolCmd,
	{ 0x64, 0x1A, 2, 0x64, 0x8C }, 
	//QueryTimeStampingCmd,
	{ 0x64, 0x1E, 2, 0x64, 0x8D },
	//QueryGpsTimeCmd,
	{ 0x64, 0x20, 2, 0x64, 0x8E },
  //QueryPstiCmd
	{ 0x64, 0x22, 3, 0x64, 0x8F },
	//QuerySearchEngineSleepCriteriaCmd,
	{ 0x64, 0x26, 2, 0x64, 0x91 },
	//QueryDatumIndexCmd,
	{ 0x64, 0x28, 2, 0x64, 0x92 },
	//QueryConstellationCapabilityCmd
	{ 0x64, 0x29, 2, 0x64, 0x93 },
	//QuerySignalDisturbanceStatusCmd,
	{ 0x64, 0x2B, 2, 0x64, 0x94 },
	//QuerySignalDisturbanceDataCmd,
	{ 0x64, 0x2C, 2, 0x64, 0x95 },
	//QueryGeofenceCmd,
	{ 0x64, 0x35, 3, 0x64, 0x99 },
	//QueryGeofenceResultCmd,
	{ 0x64, 0x31, 2, 0x64, 0x97 },
	//QueryGeofenceCmdEx,
	{ 0x64, 0x35, 3, 0x64, 0x99 },
	//QueryGeofenceResultCmdEx,
	{ 0x64, 0x36, 2, 0x64, 0x9A },
	//QueryVeryLowSpeedCmd,
	{ 0x64, 0x38, 3, 0x64, 0x9B },
  //QueryPstiCmd2
	{ 0x64, 0x3C, 3, 0x64, 0x9D },
	//QuerySha1StringCmd
	{ 0x64, 0x7E, 2, 0x64, 0xFF },
	//QueryVersionExtensionCmd
	{ 0x64, 0x7D, 2, 0x64, 0xFE },
	//Query1ppsFreqencyOutputCmd,
	{ 0x65, 0x04, 2, 0x65, 0x81 },
	//QueryGnssKnumberSlotCnr2Cmd,
	{ 0x66, 0x7F, 2, 0x66, 0xFF },
	//GetBdAlmanacCmd,
	{ 0x67, 0x04, 3, 0x67, 0x81 },
	//QueryDgpsCmd,
	{ 0x69, 0x02, 2, 0x69, 0x80 },
	//QuerySmoothModeCmd,
	{ 0x69, 0x04, 2, 0x69, 0x81 },
	//QueryRtkModeCmd,
	{ 0x6A, 0x02, 2, 0x6A, 0x80 },
	//QueryRtkParametersCmd,
	{ 0x6A, 0x04, 2, 0x6A, 0x81 },
	//QueryRtkModeCmd2,
	{ 0x6A, 0x07, 2, 0x6A, 0x83 },
	//QueryDrMultiHzCmd,
	{ 0x6F, 0x02, 2, 0x6F, 0x80 },
	//QueryRegisterCmd,
	{ 0x71, 0xFF, 5, 0xC0, 0x00 },
	//QueryRegisterCmd16,
	{ 0x73, 0xFF, 5, 0xC1, 0x00 },
	//InsdrAccelerometerSelfTestCmd,
	{ 0x79, 0x01, 2, 0x79, 0x80 },
	//InsdrGyroscopeSelfTestCmd,
	{ 0x79, 0x02, 2, 0x79, 0x81 },
	//InsdrAccumulateAngleStartCmd,
	{ 0x79, 0x03, 2, 0x79, 0x82 },
	//InsdrAccumulateAngleStopCmd,
	{ 0x79, 0x04, 2, 0x79, 0x86 },
	//QueryNmeaInterval2V8Cmd,
	{ 0x7A, 0x01, 3, 0x7A, 0x01 },
	//QueryDofunUniqueIdCmd,
	{ 0x7A, 0x02, 3, 0x7A, 0x02 },
	//QueryEricssonIntervalCmd,
	{ 0x7A, 0x04, 3, 0x7A, 0x04 },
	//QuerySerialNumberCmd,
	{ 0x7A, 0x05, 3, 0x7A, 0x05 },
	//QueryUartPassCmd,
	{ 0x7A, 0x08, 3, 0x7A, 0x08 },
	//ReadSup800UserDataCmd,
	{ 0x7A, 0x09, 8, 0x7A, 0x09 },
	//QueryPstmDeviceAddressCmd
	{ 0x7A, 0x0A, 3, 0x7A, 0x0A },
	//QueryPstnLatLonDigitsCmd
	{ 0x7A, 0x0A, 3, 0x7A, 0x0A },
	//QueryChannelDopplerCmd,
	{ 0x7B, 0xFF, 2, 0xFE, 0x00 },
	//QueryChannelClockOffsetCmd,
	{ 0x7C, 0xFF, 9, 0xFF, 0x00 },
	//QueryDrHwParameterCmd,
	{ 0x7E, 0xFF, 1, 0xF1, 0x00 },
	//QueryDrInfoCmd,
	{ 0x7F, 0xFF, 1, 0xF0, 0x00 },
};

enum SqBinaryCmd
{
	QuerySwVerSysCmd = 0,
	QuerySwCrcSysCmd,
	QueryCustomerIDCmd,
	QueryPositionRateCmd,
	QueryPowerModeCmd,
	QueryDatalogLogStatusCmd,
	QueryBinaryMeasurementDataOutCmd,
	QueryRtcmMeasurementDataOutCmd,
  QueryBasePositionCmd,
	QueryDatumCmd,
	QueryDopMaskCmd,
	QueryElevationAndCnrMaskCmd,
	QueryAgpsStatusCmd,
	QueryPositionPinningCmd,
	Query1ppsModeCmd,
	QueryNoisePowerCmd,
	QueryTimingCmd,
	QueryCableDelayCmd,
	QueryAntennaDetectionCmd,
	QueryProprietaryMessageCmd,
	QueryGnssNmeaTalkIdCmd,
  GetGpAlmanacCmd,
	QuerySbasCmd,
	QueryQzssCmd,
  QuerySbas2Cmd,
  QuerySbasDefaultCmd,
	QuerySagpsCmd,
	QueryGnssBootStatusCmd,
	QueryNmeaIntervalV8Cmd,
	QueryNmeaBinaryOutputDestinationCmd,
	QueryInterferenceDetectControlCmd,
	QueryNoisePowerControlCmd,
	QueryParameterSearchEngineNumberCmd,
	QueryV8PowerSaveParameters,
	QueryPositionFixNavigationMaskCmd,
	QueryRefTimeSyncToGpsTimeCmd,
	QueryNavigationModeV8Cmd,
	QueryGnssNavSolCmd,
	QueryTimeStampingCmd,
	QueryGpsTimeCmd,
  QueryPstiCmd,
	QuerySearchEngineSleepCriteriaCmd,
	QueryDatumIndexCmd,
	QueryConstellationCapabilityCmd,
	QuerySignalDisturbanceStatusCmd,
	QuerySignalDisturbanceDataCmd,
	QueryGeofenceCmd,
	QueryGeofenceResultCmd,
	QueryGeofenceCmdEx,
	QueryGeofenceResultCmdEx,
	QueryVeryLowSpeedCmd,
  QueryPstiCmd2,
	QuerySha1StringCmd,
	QueryVersionExtensionCmd,
	Query1ppsFreqencyOutputCmd,
	QueryGnssKnumberSlotCnr2Cmd,
  GetBdAlmanacCmd,
	QueryDgpsCmd,
	QuerySmoothModeCmd,
	QueryRtkModeCmd,
	QueryRtkParametersCmd,
	QueryRtkModeCmd2,
	QueryDrMultiHzCmd,
	QueryRegisterCmd,
	QueryRegisterCmd16,
	InsdrAccelerometerSelfTestCmd,
	InsdrGyroscopeSelfTestCmd,
	InsdrAccumulateAngleStartCmd,
	InsdrAccumulateAngleStopCmd,
	QueryNmeaInterval2V8Cmd,
	QueryDofunUniqueIdCmd,
	QueryEricssonIntervalCmd,
	QuerySerialNumberCmd,
	QueryUartPassCmd,
	ReadSup800UserDataCmd,
	QueryPstmDeviceAddressCmd,
	QueryPstnLatLonDigitsCmd,
	QueryChannelDopplerCmd,
	QueryChannelClockOffsetCmd,
	QueryDrHwParameterCmd,
	QueryDrInfoCmd,
};

bool CGPSDlg::SaveEphemeris(U08* buff, U08 id)
{	
	if(Cal_Checksum(buff) == id)
	{
		int len = buff[3] - 1;
		m_ephmsFile.Write(&buff[5], len);     
		return true;
	}
	return false;
}

bool CGPSDlg::SaveEphemeris2(U08* buff, WORD id)
{	
	if(CalCheckSum2(buff) == id)
	{	
		int len = buff[3] - 2;	//id and sub id are two bytes.
		m_ephmsFile.Write(&buff[6], len);     
		return true;
	}
	return false;
}

CGPSDlg::CmdErrorCode CGPSDlg::GetCommandReturnType(U08* buff, int tail, bool showMsg)
{	
	if(buff[0]==0xa0 && buff[1]==0xa1 &&
		(buff[tail-1]==0x0d) && (buff[tail]==0x0a) && (buff[4]==0x83) && (buff[5]==0x0))
	{ //ACK0 (V6 Firmware will return this before ACK) - 0xa0, 0xa1, l1, l2, 0x83, 0x00, ... ,0x0d, 0x0a
		return Ack0;
	}
	if((buff[0]==0xa0) && (buff[1]==0xa1) &&
		(buff[tail-1]==0x0d) && (buff[tail]==0x0a) && (buff[4]==0x83))
	{ //ACK - 0xa0, 0xa1, l1, l2, 0x83, 0xcommandId, ... ,0x0d, 0x0a
		return Ack;
	}
	if((buff[0]==0xa0) && (buff[1]==0xa1) &&
		(buff[tail-1]==0x0d) && (buff[tail]==0x0a) && (buff[4]==0x84))
	{ //NACK - 0xa0, 0xa1, l1, l2, 0x84, 0xcommandId, ... ,0x0d, 0x0a
		if(showMsg)
		{
			add_msgtolist("Received NACK...");
		}
		return NACK;
	}
	if((buff[0]==0xa0) && (buff[1]==0xa1) &&
		(buff[tail-1]==0x0d) && (buff[tail]==0x0a) && (buff[4]==0x85))
	{ //FormatError (V8 FW after 2016 will return this when command format error
    //- 0xa0, 0xa1, l1, l2, 0x85, 0xcommandId, ... ,0x0d, 0x0a
		if(showMsg)
		{
			add_msgtolist("Received FormatError...");
		}
		return FormatError;
	}
	return Timeout;
}

bool CGPSDlg::SendToTarget(U08* message, U16 length, LPCSTR promptMessage, int timeout)
{	
  //For old style, timeout is boolean type.
  if(timeout == 1)
  {
    timeout = 1000;
  }
  else if(timeout == 0)
  {
    timeout = 2000;
  }

	if(m_bShowBinaryCmdData)
	{
		add_msgtolist("In : " + theApp.GetHexString(message, length));	
	}

	ClearQue();
  if(NULL == m_serial) 
  {
    return false;
  }
  else
  {
	  m_serial->SendData(message, length);
  }

	static U08 buffer[COM_BUFFER_SIZE] = { 0 };
	ScopeTimer t;
	while(NULL != m_serial)
	{	
    if((int)t.GetDuration() > timeout)
    {
      break;
    }
	  int retLen = m_serial->GetBinary(buffer, sizeof(buffer) - 1, timeout);
		if(!ReadOK(retLen))
		{
			Sleep(10);
			continue;
		}
		buffer[retLen] = 0;
    if(!PreprocessInputLine(buffer, retLen))
    {
      continue;
    }
	
		CmdErrorCode ack = GetCommandReturnType(buffer, retLen - 1, (promptMessage != NULL));	
		if(ack == Ack)
		{
			if(m_bShowBinaryCmdData)
			{
				add_msgtolist("Ack: " + theApp.GetHexString(buffer, retLen));	
			}

			if(strlen(promptMessage))
			{
				add_msgtolist(promptMessage);	
			}
			return true;
		}
		else if(ack == NACK)
		{
			return false;
		}
		else if(ack == FormatError)
		{
			if(m_bShowBinaryCmdData)
			{
				add_msgtolist("FormatError: " + theApp.GetHexString(buffer, retLen));	
			}
			ShowFormatError(message, buffer);
			return false;
		}
	}	//while(1)	
  return false;
}

bool CGPSDlg::SendToTargetOld(U08* message, U16 length, const char* Msg, bool quick)
{	
	time_t start,end;
	start = clock();
	if(m_bShowBinaryCmdData)
	{
		add_msgtolist("In : " + theApp.GetHexString(message, length));	
	}

	DWORD timeout = (quick) ? 2000 : 10000;
	if(NULL == m_serial) return false;
	ClearQue();
	m_serial->SendData(message, length);	
	ScopeTimer t;
	while(1)
	{		
		U08 buff[1024] = {0};
		if(NULL == m_serial) return false;
		m_serial->GetBinaryAck(buff, sizeof(buff), timeout - t.GetDuration());

		U08 len = buff[2] <<8 | buff[3];
		int k1 = len + 5;
		int k2 = len + 6;		
		CmdErrorCode ack = GetCommandReturnType(buff, k2, (Msg!=NULL));	
		if(ack == Ack)
		{
			if(m_bShowBinaryCmdData)
			{
				add_msgtolist("Ack: " + theApp.GetHexString(buff, buff[2] <<8 | buff[3] + 7));	
			}

			if(strlen(Msg))
			{
				add_msgtolist(Msg);	
			}
			return true;
		}
		else if(ack == NACK)
		{
			return false;
		}
		else if(ack == FormatError)
		{
			if(m_bShowBinaryCmdData)
			{
				add_msgtolist("FormatError: " + theApp.GetHexString(buff, buff[2] <<8 | buff[3] + 7));	
			}
			ShowFormatError(message, buff);
			return false;
		}

		end=clock();
		if(quick)
		{
			if(TIMEOUT_METHOD_QUICK(start, end))
				return false;
		}
		else
		{
			if(TIMEOUT_METHOD(start, end))
				return false;
		}
	}		
}

bool CGPSDlg::SendToTargetNoAck(U08* message, U16 length)
{		
	if(NULL == m_serial) return false;
	ClearQue();
	m_serial->SendData(message, length, true);	
	return true;
}

bool CGPSDlg::SendToTargetNoWait(U08* message, U16 length, LPCSTR Msg)
{		
	time_t start,end;
	start = clock();		    
	if(m_bShowBinaryCmdData)
	{
		add_msgtolist("In : " + theApp.GetHexString(message, length));	
	}

	if(NULL == m_serial) return false;
	ClearQue();
	m_serial->SendData(message, length);	
	while(1)
	{	
		U08 buff[1024] = {0};

		if(NULL == m_serial) return false;
		m_serial->GetBinary(buff, sizeof(buff));

		U08 len = buff[2] << 8 | buff[3];
		int k1 = len + 5;
		int k2 = len + 6;		
		CmdErrorCode ack = GetCommandReturnType(buff, k2);		
		if(ack == Ack)
		{
			if(m_bShowBinaryCmdData)
			{
				add_msgtolist("Ack: " + theApp.GetHexString(buff, k2+1));	
			}

			if(strlen(Msg))
			{
				add_msgtolist(Msg);	
			}
			return true;
		}
		else if(ack == NACK)
		{
			return false;
		}
		
		end=clock();
		if(TIMEOUT_METHOD(start, end))
		{			
			return false;
		}	
	}
}

bool CGPSDlg::CheckGPS(U08* message, U16 length, char* Msg)
{	
	if(NULL == m_serial) return false;
	m_serial->SendData(message, length, true);	  
	time_t start,end;
	start = clock();		    
	while(1)
	{		
		U08 buff[1024] = {0};
		if(NULL == m_serial) return false;
		m_serial->GetBinary(buff, sizeof(buff), SCAN_TIME_OUT_MS);			
		U08 len = buff[2] << 8 | buff[3];
		int k1 = len + 5;
		int k2 = len + 6;			
		if(GetCommandReturnType(buff, k2) != Timeout)
		{
			if(Msg[0] != 0)
			{
				add_msgtolist(Msg);	
			}		
			return true;
		}
		end = clock();
		if((end-start) > SCAN_TIME_OUT_MS)
		{				
			return false;
		}
	}
}

UINT LogReadBatchControlThread(LPVOID pParam)
{
	CGPSDlg::gpsDlg->LogReadBatchControl();
	return 0;
}

void CGPSDlg::OnDatalogLogReadBatch()
{
	if(!CheckConnect())
	{
		return;
	}

	SetInputMode(NoOutputMode);
	CString fileName("Data.log");	
	CFileDialog dlgFile(false, _T("log"), fileName, OFN_HIDEREADONLY, _T("ALL Files (*.*)|*.*||"), this);
	INT_PTR nResult = dlgFile.DoModal();
	fileName = dlgFile.GetPathName();			

	CFileException ef;
	try
	{
		if(nResult == IDOK)
		{  				
			datalogFilename = fileName;
			this->m_nDownloadBaudIdx = g_setting.boostBaudIndex;
			::AfxBeginThread(LogReadBatchControlThread, 0);
		}		
		else
		{
			SetMode();
			CreateGPSThread();
		}
	}
	catch(CFileException *fe)
	{
		fe->ReportError();
		fe->Delete();
		return;
	}	
}

bool CGPSDlg::DataLogReadOffsetCtrl(int startId, int totalSector, int offset, U08 *buffer, long bufferSize, long *receiveBytes)
{
	WRL = new CWaitReadLog;
	AfxBeginThread(WaitLogRead, 0);	
	WaitForSingleObject(waitlog, INFINITE);

	memset(buffer, 0, bufferSize);
	for(int i=startId; i<totalSector; i+=offset)
	{
		int step = ((i+offset)>totalSector) ? (totalSector - i) : offset;
		int tryCount = 0;
		while(1)
		{
			++tryCount;
			if(tryCount > 60)
			{
				return false;
			}			
			
			long tmp_count = 0;
			if(DatalogReadAll(i, step, &buffer[*receiveBytes], step * 4096, &tmp_count))
			{
				*receiveBytes += tmp_count;
				CString displayMessage;
				displayMessage.Format("Retrieve Log data #%d sector #%d bytes\n", i, *receiveBytes);
				WRL->msg.SetWindowText(displayMessage);
				Sleep(150);
				break;
			}
			else
			{
				Utility::LogFatal(__FUNCTION__, "[DataLog] DatalogReadAll fail", __LINE__);
				Sleep(150);
			}
		}
	}
	VerifyDataLogFormat(buffer, receiveBytes);
	return true;
}

bool CGPSDlg::DatalogReadAll(int startId, int offset, U08 *datalog, long size, long *receiveBytes)
{
	U08 cmd[5] = {0x1d, startId >> 8, startId & 0xff, offset >> 8, offset & 0xff};
	U08 message[12] = {0};
	int len = SetMessage2(message, cmd, sizeof(cmd));
	Sleep(50);

	int i = 0;
	for(; i<30; ++i)
	{
		if(SendToTarget(message, len, "", true))
		{
			break;
		}
	}

	if(i==30)
	{
		return false;
	}

	U08 ptr_tmp = 0;
	bool isFinish = false;
	DWORD res_c = 0;
	*receiveBytes = 0;
	U08 res_buff[0x1000] = {0};
	while(1)
	{		
		U08 buff[0x2000] = {0};
		//Read 8k one time.
		if(NULL == m_serial) return false;
		DWORD readBytes = m_serial->ReadData(buff, sizeof(buff), true);
		if(readBytes <= 0 || readBytes > sizeof(buff))
		{
			continue;
		}
		if(*receiveBytes > 8000)
		{
			int a = 0;
		}
		bool isEnd = VerifyDataLogBuffer(buff, datalog, &ptr_tmp, readBytes, receiveBytes);
		if(isEnd)
		{
			if(ptr_tmp > 0)
			{
				memcpy(res_buff, buff, ptr_tmp);
			}
			isFinish = true;
			break;
		}

		if(*receiveBytes > size + 10)
		{
			Utility::LogFatal(__FUNCTION__, "[DataLog] receiveBytes fail", __LINE__);
			isFinish = false;
			break;
		}

		if(*receiveBytes + readBytes > 8000)
		{
			int a = 0;
		}
		memcpy(&datalog[*receiveBytes], buff, readBytes);

		*receiveBytes += readBytes;
	}

	if(!isFinish)
	{
		Utility::LogFatal(__FUNCTION__, "[DataLog] isFinish fail", __LINE__);
		return false;
	}

	res_c = ptr_tmp;
	int count = 0;
	bool chk_sum_right = false;

	while(1)
	{
		if(res_c > 0)
		{
			if(!strncmp((char*)res_buff,"CHECKSUM=",9) && res_c >= 12)
			{
				U08 chk = (U08)res_buff[9];

				U16 id;
				if(res_buff[10] == 10 && res_buff[11] == 13)
					id = startId;	
				else
					id = (U08)res_buff[11]<<8|(U08)res_buff[10];

				U08 chk_sum = 0;
				for(int i=0; i <*receiveBytes; i++)
					chk_sum = datalog[i]^chk_sum;
				TRACE("chk=%d,chk_sum=%d,*receiveBytes=%d\n", chk, chk_sum, *receiveBytes);
				if(chk == chk_sum && id == startId)
				{
					return true;
				}
				Utility::LogFatal(__FUNCTION__, "[DataLog] chk_sum fail", __LINE__);
				return false;
			}
			Utility::LogFatal(__FUNCTION__, "[DataLog] CHECKSUM fail", __LINE__);
		}
		U08 buff[1024] = {0};
		if(NULL == m_serial) return false;
		DWORD readBytes = m_serial->ReadData(&res_buff[res_c], sizeof(buff), true);
		res_c += readBytes;
		count++;
		if(count > 10000)
		{
			Utility::LogFatal(__FUNCTION__, "[DataLog] count fail", __LINE__);
			return false;
		}
	} //while(1)
	return false;
}
/*
BOOL CGPSDlg::OpenDataLogFile(UINT nOpenFlags)
{
	return dataLogFile.Open(datalogFilename, nOpenFlags);
}
*/
void CGPSDlg::LogReadBatchControl()
{
	if(!dataLogFile.Open(datalogFilename, CFile::modeReadWrite | CFile::modeCreate))
	{
		return;
	}

	U16 endPos, totalBytes;
	if(!QueryDataLogBoundary(&endPos, &totalBytes))
	{
		dataLogFile.Close();
		Utility::LogFatal(__FUNCTION__, "[DataLog] QueryDataLogBoundary fail", __LINE__);
		return;
	}

	BoostBaudrate(FALSE);
	long bufferSize = (endPos + 1) * 0x1000;
	U08* buffer = (U08*)malloc(bufferSize);
	long receiveBytes = 0;
	if(DataLogReadOffsetCtrl(0, endPos, 2, buffer, bufferSize, &receiveBytes))
	{
		dataLogFile.Write(buffer, receiveBytes);
		WRL->msg.SetWindowText("Data Log reading complete.");
		CGPSDlg::gpsDlg->add_msgtolist("Data Log reading successfully");
	}
	else
	{
		WRL->msg.SetWindowText("Data Log reading fails!");
		CGPSDlg::gpsDlg->add_msgtolist("Data Log reading fails!");
	}
	WRL->IsFinish = true;
	free(buffer);
	
	CGPSDlg::gpsDlg->target_only_restart(0);
	if(NULL == m_serial) return;
	CGPSDlg::gpsDlg->m_serial->ResetPort(g_setting.GetBaudrateIndex());
	CGPSDlg::gpsDlg->m_BaudRateCombo.SetCurSel(g_setting.GetBaudrateIndex());
	dataLogFile.Close();
}

DWORD ReadDataLogSector(CSerial* serial, void* buffer, DWORD bufferSize)
{
	DWORD totalSize = 0;
	char* bufferIter = (char*)buffer;
	DWORD nBytesRead = 0;
	do
	{ 
		nBytesRead = serial->ReadData(bufferIter, 1);
		if(nBytesRead <= 0)
		{
			return totalSize;
		}

		if((*bufferIter == 0x0a) && (*(bufferIter - 1) == 0x0d))
		{	//When read 0d, 0a, check pack size to make sure until tail.
			DWORD len = *(bufferIter - totalSize + 3);
			if(totalSize == len + 6)
			{
				break;
			}
		}
		bufferIter += nBytesRead;
		totalSize += nBytesRead;
	}
	while(totalSize < bufferSize);
	return totalSize;	
}

void CGPSDlg::VerifyDataLogFormat(U08 *datalog, long *size)
{
	long count = 0;
	U08* buff = new U08[*size];
	for(long i=0; i<*size; i+=0x1000)
	{
		U08 *bufferIter = &datalog[i];
		long tmp_count = 0;
		while(1)
		{
			U08 type = bufferIter[0] & 0xE0;
			if(type == 0x40 || type == 0x60)
			{
				bufferIter += 18;
				tmp_count += 18;
			}
			else if(type == 0x80)
			{
				bufferIter += 8;
				tmp_count += 8;
			}
			else if(type == 0x20 || type == 0xC0)
			{
				bufferIter += 20;
				tmp_count += 20;
			}
			else
			{
				break;
			}
		}
		memcpy(&buff[count], &datalog[i], tmp_count);
		count += tmp_count;
	}
	memcpy(datalog, buff, count);
	*size = count;
	delete [] buff;
}


bool CGPSDlg::VerifyDataLogBuffer(U08 *buff, U08 *datalog, U08 *ptr_last, int size, long *receive_count)
{
	bool isEnd = false;
	for(int i=0; i<size; ++i)
	{
		if(i < size - 3 && 
			buff[i] == 'E' && buff[i+1] == 'N' && buff[i+2] == 'D' && buff[i+3] == '\0')
		{
			isEnd = true;
			memcpy(&datalog[*receive_count], buff, i);
			*receive_count += i;
			if(size > i + 3)
			{
				memcpy(buff, &buff[i+4], size - (i+4));
				*ptr_last = size - (i + 4);
			}
			Utility::Log(__FUNCTION__, "[DataLog] VerifyDataLogBuffer type 1", __LINE__);
			break;
		}
		else if(i < size - 2 && *receive_count >= 1 && 
			datalog[*receive_count-1] == 'E' && buff[i] == 'N' && buff[i+1] == 'D' && buff[i+2] == '\0')
		{
			isEnd = true;
			*receive_count -= 1;
			if(size > i + 3)
			{
				memcpy(buff, &buff[i+3], size - (i + 3));
				*ptr_last = size - (i + 3);
			}
			Utility::Log(__FUNCTION__, "[DataLog] VerifyDataLogBuffer type 2", __LINE__);
			break;					
		}
		else if(i < size - 1 && *receive_count >= 2 && 
			datalog[*receive_count-2] == 'E' && datalog[*receive_count-1] == 'N' && buff[i] == 'D' && buff[i+1] == '\0')
		{
			isEnd = true;
			*receive_count -= 2;
			if(size > i + 2)
			{
				memcpy(buff, &buff[i+2], size - (i + 2));
				*ptr_last = size - (i + 2);
			}
			Utility::Log(__FUNCTION__, "[DataLog] VerifyDataLogBuffer type 3", __LINE__);
			break;					
		}
		else if(i < size && *receive_count >= 3 && 
			datalog[*receive_count-3] == 'E' && datalog[*receive_count-2] == 'N' && datalog[*receive_count-1] == 'D' && buff[i] == '\0')
		{
			isEnd = true;
			*receive_count -= 3;
			if(size > i + 1)
			{
				memcpy(buff, &buff[i+1], size - (i + 1));
				*ptr_last = size - (i + 1);
			}
			Utility::Log(__FUNCTION__, "[DataLog] VerifyDataLogBuffer type 4", __LINE__);
			break;					
		}
	}
	return isEnd;
}

U08 CGPSDlg::MinihomerQuerytag()
{
	U08 msg[1] ,checksum=0;
	CString temp;
	U32 data = 0;
	U08 buff[100];
	int k1,k2;
	time_t start,end;

	msg[0]=0x7D; //msgid

	int len = CGPSDlg::gpsDlg->SetMessage(msg,sizeof(msg));

	CGPSDlg::gpsDlg->ClearQue();
	if(CGPSDlg::gpsDlg->SendToTarget(CGPSDlg::m_inputMsg,len, "Query miniHomer tag successfully", true))
	{
		start = clock();
		while(1)
		{
			memset(buff, 0, 100);
			if(NULL == CGPSDlg::gpsDlg->m_serial) return false;
			CGPSDlg::gpsDlg->m_serial->GetBinary(buff, sizeof(buff));			
			len = buff[2]<<8|buff[3];		
			k1=len+5;
			k2=len+6;
			if((buff[0]==0xa0) && (buff[1]==0xa1) && (buff[4]==0xD1) && (buff[k2-1]==0x0d)&&(buff[k2]==0x0a))
			{
				for(int i=0;i<(int) buff[3];i++)
					checksum^=buff[i+4];			
				if(checksum == buff[k2-2])
				{
					U08 size = buff[5];
					
					if(buff[5] == 0xFF)
					{
						CGPSDlg::gpsDlg->add_msgtolist("No Tag");
					}else
					{
						temp.Append("Tag = ");
						for (int i=0;i<size;i++)
						{
							temp.AppendFormat("0x%02X ",buff[6+i]);
						}
						CGPSDlg::gpsDlg->add_msgtolist(temp);
					}

					break;
				}
			}
			end=clock();	
			if(CGPSDlg::gpsDlg->TIMEOUT_METHOD_QUICK(start,end))
				break;	
		}		
	}
	else
		CGPSDlg::gpsDlg->add_msgtolist("Query DR Info Fail.");

	CGPSDlg::gpsDlg->SetMode();
	CGPSDlg::gpsDlg->CreateGPSThread();
	return TRUE;	
}

UINT MinihomerQuerytagThread(LPVOID pParam)
{	
	CGPSDlg::gpsDlg->MinihomerQuerytag();
	return TRUE;	
}

void CGPSDlg::OnMinihomerQuerytag()
{
	if(!CheckConnect())
	{
		return;
	}
	AfxBeginThread(MinihomerQuerytagThread, 0);	
}

UINT ActivateminiHomerThread(LPVOID pParam)
{	
	CGPSDlg::gpsDlg->activate_minihomer();
	return TRUE;	
}

void CGPSDlg::set_minihomerid(U08* id,int id_len)
{
	U08 msg[11], checksum=0;
	CString temp;
	U32 data = 0;

	msg[0]=0x74; // set device_id;
	memcpy(&msg[1],id,id_len);

	int len = SetMessage(msg,sizeof(msg));

	ClearQue();
	if(SendToTarget(CGPSDlg::m_inputMsg,len, "Set miniHomer Device ID successfully", true) != 1)
		add_msgtolist("Set miniHomer Device ID Fail.");	
}

void CGPSDlg::set_minihomerkey(U08* key,int key_len)
{
	U08 msg[65] ,checksum=0;
	CString temp;
	U32 data = 0;

	msg[0]=0x75; // set device_id;
	memcpy(&msg[1],key,key_len);

	int len = SetMessage(msg,sizeof(msg));

	ClearQue();
	if(SendToTarget(CGPSDlg::m_inputMsg,len, "Set miniHomer Device Key successfully", true) != 1)
		add_msgtolist("Set miniHomer Device Key Fail.");	
}

void CGPSDlg::activate_minihomer()
{
	CRedirector m_redir;
	char cmd_path[1024];
	GetCurrentDirectory(1024, cmd_path);
	strcat_s(cmd_path, sizeof(cmd_path), "\\Create_miniHomer_Activate_Code.exe -c -s");

	m_redir.Close();
	m_redir.Open(cmd_path);

	m_redir.Wait();

	TRACE("%s",m_redir.std_output);
	m_redir.Close();

	U08 id[10];
	U08 key[64];

	CString retval = m_redir.std_output;
		
	int start = retval.Find("id=")+3;
	for (int i=0;i<10;i++)
	{
		id[i] = Utility::GetOctValue(retval[start+i*2], retval[start+i*2+1]);
	}

	start = retval.Find("signature=")+10;
	for (int i=0;i<64;i++)
	{
		key[i] = Utility::GetOctValue(retval[start+i*2], retval[start+i*2+1]);
	}
	
	set_minihomerid(id,sizeof(id));
	set_minihomerkey(key,sizeof(key));

	SetMode();
	CreateGPSThread();
	return;	
}

void CGPSDlg::OnMinihomerActivate()
{
	if(!CheckConnect())
	{
		return;
	}
	AfxBeginThread(ActivateminiHomerThread, 0);	
}	

inline const char *go_next_dot(const char *buff)
{
	while(1)
	{
		if(*buff == 0 || *buff=='*') return NULL;
		if(*buff == ',') return buff+1;
		buff++;
	}
}

void CGPSDlg::parse_sti_20_message(const char *buff,int len) // for timing module
{
	const char *ptr = buff;
	CString temp;

	ptr = go_next_dot(ptr);
	if(ptr == NULL) return;
	ptr = go_next_dot(ptr);
	if(ptr == NULL) return;
	ptr = go_next_dot(ptr);
	if(ptr == NULL) return;
	ptr = go_next_dot(ptr);
	if(ptr == NULL) return;
	ptr = go_next_dot(ptr);
	if(ptr == NULL) return;

	temp.Format("%d",atoi(ptr));
	m_odo_meter.SetWindowText(temp);

	ptr = go_next_dot(ptr);
	if(ptr == NULL) return;

	ptr = go_next_dot(ptr);
	if(ptr == NULL) return;

	temp.Format("%d",atoi(ptr));
	m_backward_indicator.SetWindowText(temp);

	ptr = go_next_dot(ptr);
	if(ptr == NULL) return;
	ptr = go_next_dot(ptr);
	if(ptr == NULL) return;
	ptr = go_next_dot(ptr);
	if(ptr == NULL) return;
	ptr = go_next_dot(ptr);
	if(ptr == NULL) return;

	temp.Format("%.2f", atof(ptr));
	GetDlgItem(IDC_GYRO_DATA)->SetWindowText(temp);
}

void CGPSDlg::parse_sti_03_message(const char *buff,int len) // for timing module
{
	const char *ptr = buff;

	ptr = go_next_dot(ptr);
	if(ptr == NULL) return;

	ptr = go_next_dot(ptr);
	if(ptr == NULL) return;

	CString temp;
	temp.Format("%d",atoi(ptr));
	m_odo_meter.SetWindowText(temp);
}

#if (SHOW_RTK_BASELINE || _TAB_LAYOUT_)
void CGPSDlg::parse_sti_31_message(const char *buff,int len) // for timing module
{
	const char *ptr = buff;

	ptr = go_next_dot(ptr);
	if(ptr == NULL) return;
	ptr = go_next_dot(ptr);
	if(ptr == NULL) return;

	CString temp;
	temp.Format("%.3f",atof(ptr));
#if (SHOW_RTK_BASELINE)
	m_bootStatus.SetWindowText(temp);
#else
	m_baselineLength.SetWindowText(temp);
#endif
}
#endif

#if(MORE_INFO || _TAB_LAYOUT_)
void CGPSDlg::parse_sti_30_message(const char *buff,int len) // for timing module
{
	const char *ptr = buff;
	memset(&m_psti030, 0, sizeof(&m_psti030));

	ptr = go_next_dot(ptr);
	if(ptr == NULL) return;
	ptr = go_next_dot(ptr);
	if(ptr == NULL) return;
	ptr = go_next_dot(ptr);
	if(ptr == NULL) return;
	ptr = go_next_dot(ptr);
	if(ptr == NULL) return;
	ptr = go_next_dot(ptr);
	if(ptr == NULL) return;
	ptr = go_next_dot(ptr);
	if(ptr == NULL) return;
	ptr = go_next_dot(ptr);
	if(ptr == NULL) return;
	ptr = go_next_dot(ptr);
	if(ptr == NULL) return;
	ptr = go_next_dot(ptr);
	if(ptr == NULL) return;
	ptr = go_next_dot(ptr);
	if(ptr == NULL) return;
	ptr = go_next_dot(ptr);
	if(ptr == NULL) return;
	ptr = go_next_dot(ptr);
	if(ptr == NULL) return;
	ptr = go_next_dot(ptr);
	if(ptr == NULL) return;
	ptr = go_next_dot(ptr);
	if(ptr == NULL) return;
	m_psti030.rtkAge = (F32)atof(ptr);

	ptr = go_next_dot(ptr);
	if(ptr == NULL) return;
	m_psti030.rtkRatio = (F32)atof(ptr);

	PostMessage(UWM_UPDATE_PSTI030, (WPARAM)&m_psti030, 0);
}
#endif
#if(_TAB_LAYOUT_)
void CGPSDlg::parse_sti_32_message(LPCSTR buff, int len) // for timing module
{
	const char *ptr = buff;
	memset(&m_psti032, 0, sizeof(&m_psti032));

	ptr = go_next_dot(ptr);
	if(ptr == NULL) return;
	ptr = go_next_dot(ptr);
	if(ptr == NULL) return;
	ptr = go_next_dot(ptr);
	if(ptr == NULL) return;
	ptr = go_next_dot(ptr);
	if(ptr == NULL) return;
	ptr = go_next_dot(ptr);
	ptr = go_next_dot(ptr);
	if(ptr == NULL) return;

	if(ptr == NULL) return;
	m_psti032.eastProjection = (F32)atof(ptr);

	ptr = go_next_dot(ptr);
	if(ptr == NULL) return;
	m_psti032.northProjection = (F32)atof(ptr);

	ptr = go_next_dot(ptr);
	if(ptr == NULL) return;
	m_psti032.upProjection = (F32)atof(ptr);

	ptr = go_next_dot(ptr);
	if(ptr == NULL) return;
	m_psti032.baselineLength = (F32)atof(ptr);

	ptr = go_next_dot(ptr);
	m_psti032.baselineCourse = (F32)atof(ptr);

	PostMessage(UWM_UPDATE_PSTI032, (WPARAM)&m_psti032, 0);
}
#endif

#if(_MODULE_SUP_800_)
void CGPSDlg::parse_sti_04_001_message(const char *buff,int len) // for timing module
{
	const char *ptr = buff;

	ptr = go_next_dot(ptr);
	if(ptr == NULL) return;

	ptr = go_next_dot(ptr);
	if(ptr == NULL) return;

	ptr = go_next_dot(ptr);
	if(ptr == NULL) return;

	this->m_psti004001Copy.Valide = atoi(ptr);

	ptr = go_next_dot(ptr);
	if(ptr == NULL) return;

	this->m_psti004001Copy.Pitch = atof(ptr);

	ptr = go_next_dot(ptr);
	if(ptr == NULL) return;

	this->m_psti004001Copy.Roll = atof(ptr);

	ptr = go_next_dot(ptr);
	if(ptr == NULL) return;

	this->m_psti004001Copy.Yaw = atof(ptr);

	ptr = go_next_dot(ptr);
	if(ptr == NULL) return;

	this->m_psti004001Copy.Pressure = atoi(ptr);

	ptr = go_next_dot(ptr);
	if(ptr == NULL) return;

	this->m_psti004001Copy.Temperature = atof(ptr);
}
#endif

void CGPSDlg::parse_sti_message(const char *buff,int len)
{
	const char *ptr = buff;
	int psti_id;

	ptr = go_next_dot(ptr);
	if(ptr == NULL) 
		return;

	psti_id = atoi(ptr);
	if(psti_id == 50)  // glonass k-number
	{
		parse_psti_50(ptr);
	} 
	else if(psti_id == 0)
	{
		parse_sti_0_message(buff, len);
	}
	else if(psti_id == 20)		// for dr
	{
		parse_sti_20_message(buff, len);
	}
	else if(psti_id == 3)		// for jamming interference
	{
		parse_sti_03_message(buff, len);
	}
#if (SHOW_RTK_BASELINE || _TAB_LAYOUT_)
	else if(psti_id == 31)		// for jamming interference
	{
		parse_sti_31_message(buff, len);
	}
#endif
#if (MORE_INFO || _TAB_LAYOUT_)
	else if(psti_id == 30)		// for jamming interference
	{
		parse_sti_30_message(buff, len);
	}
#endif
#if (_TAB_LAYOUT_)
	else if(psti_id == 32)		// for jamming interference
	{
		parse_sti_32_message(buff, len);
	}
#endif
#if(_MODULE_SUP_800_)
	else if(psti_id == 4)		// for SUP800
	{
		ptr = go_next_dot(ptr);
		int psti_sub_id = atoi(ptr);

		if(psti_sub_id==1)
		{
			parse_sti_04_001_message(buff,len);
		}
	}
#endif
}

#if(TIMING_MODE)
void CGPSDlg::parse_sti_0_message(const char *buff,int len) // for timing module
{
	int mode,survery_len;
	float error;
	int set_std,now_std;
	const char *ptr = buff;

	ptr = go_next_dot(ptr);
	if(ptr == NULL) return;
	ptr = go_next_dot(ptr);
	if(ptr == NULL) return;
	mode = atoi(ptr);
	ptr = go_next_dot(ptr);
	if(ptr == NULL) return;
	survery_len = atoi(ptr);
	ptr = go_next_dot(ptr);
	if(ptr == NULL) return;
	error = (float)atof(ptr);

	ptr = go_next_dot(ptr);
	if(ptr == NULL) return;
	set_std = atoi(ptr);
	ptr = go_next_dot(ptr);
	if(ptr == NULL) return;
	now_std = atoi(ptr);

	if(dia_monitor_1pps != NULL)
		dia_monitor_1pps->Show1PPSTiming(mode,survery_len,error,set_std,now_std);
}
#else
void CGPSDlg::parse_sti_0_message(const char *buff,int len) // for timing module
{
	int mode,survery_len;
	float error;
	const char *ptr = buff;


	ptr = go_next_dot(ptr);
	if(ptr == NULL) return;
	ptr = go_next_dot(ptr);
	if(ptr == NULL) return;
	mode = atoi(ptr);
	ptr = go_next_dot(ptr);
	if(ptr == NULL) return;
	survery_len = atoi(ptr);
	ptr = go_next_dot(ptr);
	if(ptr == NULL) return;
	error = (float)atof(ptr);

	if(dia_monitor_1pps != NULL)
		dia_monitor_1pps->Show1PPSTiming(mode,survery_len,error);
}
#endif

void CGPSDlg::OnMonitoring1Pps()
{
	if(dia_monitor_1pps == NULL)
	{
		dia_monitor_1pps = new CMonitor_1PPS();
		dia_monitor_1pps->Create(IDD_1PPS_MONITOR);
		dia_monitor_1pps->ShowWindow(SW_SHOW);
	}
	else
	{
		dia_monitor_1pps->SetFocus();
	}
}

void CGPSDlg::close_minitor_1pps_window()
{
	dia_monitor_1pps->DestroyWindow();
	dia_monitor_1pps = NULL;
}

void CGPSDlg::parse_psti_50(const char *buff)		// gnss
{
	const char *ptr = buff;
	U08 psti_num, seq_num;
	U08 total_gnss;

	ptr = go_next_dot(ptr);

	psti_num = atoi(ptr);
	ptr = go_next_dot(ptr);
	seq_num = atoi(ptr);
	ptr = go_next_dot(ptr);
	total_gnss = atoi(ptr);
	ptr = go_next_dot(ptr);

	if(seq_num == 1)
	{
		memset(&m_gnssTemp,0,sizeof(GNSS_T));
	}

	while(ptr && *ptr != ',')
	{ 
		m_gnssTemp.sate[m_gnssTemp.gnss_in_view].k_num = atoi(ptr);
		ptr=go_next_dot(ptr);
		m_gnssTemp.sate[m_gnssTemp.gnss_in_view].slot_num = atoi(ptr);
		ptr=go_next_dot(ptr);
		m_gnssTemp.sate[m_gnssTemp.gnss_in_view].snr = atoi(ptr);
		ptr=go_next_dot(ptr);
		m_gnssTemp.gnss_in_view++;
	}

	if(seq_num == psti_num && m_gnssTemp.gnss_in_view == total_gnss)
	{
		memcpy(&m_gnss, &m_gnssTemp,sizeof(GNSS_T));
		memset(&m_gnssTemp,0,sizeof(GNSS_T));	
	}

	ptr = NULL;
}

int position_update_rate;
int position_update_attr;
UINT Configurepositionrate(LPVOID param)
{
	U08 msg[3];

	msg[0] = 0xE;
	msg[1] = position_update_rate;
	msg[2] = position_update_attr;

	int len = CGPSDlg::gpsDlg->SetMessage(msg, sizeof(msg));
	CGPSDlg::gpsDlg->ClearQue();
	if(CGPSDlg::gpsDlg->SendToTarget(CGPSDlg::m_inputMsg,len, "Configure Position Rate successfully", true))
	{
		Sleep(200);
#if(_MODULE_SUP_800_)
    // 20160810, request from Andrew, SUP800 fw can't support baud rate more than 230400
		if(position_update_rate>20 && g_setting.GetBaudrate()<230400)				//Boost to 230400 when update rate > 20Hz.
		{
			CGPSDlg::gpsDlg->ConfigBaudrate(6, position_update_attr);	
		}
#else
    if(position_update_rate > 40 && g_setting.GetBaudrate() < 961600)				//Boost to 961600 when update rate > 40Hz.
		{
			CGPSDlg::gpsDlg->ConfigBaudrate(8, position_update_attr);	
		}
		else if(position_update_rate>25 && g_setting.GetBaudrate()<460800)				//Boost to 460800 when update rate > 25Hz.
		{
			CGPSDlg::gpsDlg->ConfigBaudrate(7, position_update_attr);	
		}
		else if(position_update_rate>20 && g_setting.GetBaudrate()<230400)				//Boost to 230400 when update rate > 20Hz.
		{
			CGPSDlg::gpsDlg->ConfigBaudrate(6, position_update_attr);	
		}
#endif
		else if(position_update_rate>10 && g_setting.GetBaudrate()<115200)				//Boost to 115200 when update rate > 10Hz.
		{
			CGPSDlg::gpsDlg->ConfigBaudrate(5, position_update_attr);	
		}
		else if(position_update_rate>2 && g_setting.GetBaudrate()<38400)			//Boost to 38400 when update rate > 2Hz.
		{
			CGPSDlg::gpsDlg->ConfigBaudrate(3, position_update_attr);	
		}
		else if(position_update_rate>1 && g_setting.GetBaudrate()<9600)			//Boost to 9600 when update rate > 1Hz.
		{
			CGPSDlg::gpsDlg->ConfigBaudrate(1, position_update_attr);	
		}
		//else
		//{
        //Andrew request 20160901, doesn't need restart in this
		  	//CGPSDlg::gpsDlg->SendRestartCommand(1);
		//}
	}

	CGPSDlg::gpsDlg->SetMode();  
	CGPSDlg::gpsDlg->CreateGPSThread();
	return 0;	
}

UINT ConfigureDrMultiHz(LPVOID param)
{
	U08 msg[4] = {0};

	msg[0] = 0x6F;
	msg[1] = 0x01;
	msg[2] = position_update_rate;
	msg[3] = position_update_attr;

	int len = CGPSDlg::gpsDlg->SetMessage(msg, sizeof(msg));

	CGPSDlg::gpsDlg->ClearQue();
	if(CGPSDlg::gpsDlg->SendToTarget(CGPSDlg::m_inputMsg,len, "Configure DR Multi-Hz successfully", true))
	{
		Sleep(200);
		if(position_update_rate > 10 && g_setting.GetBaudrate()<115200)
		{
			CGPSDlg::gpsDlg->ConfigBaudrate(5, position_update_attr);		//Boost to 115200 when update rate > 10Hz.
		}
		else if(position_update_rate>2 && g_setting.GetBaudrate()<38400)	//Boost to 115200 when update rate > 2Hz.
		{
			CGPSDlg::gpsDlg->ConfigBaudrate(3, position_update_attr);		//Boost to 115200 when update rate > 10Hz.
		}
		else if(position_update_rate>1 && g_setting.GetBaudrate()<9600)	//Boost to 115200 when update rate > 1Hz.
		{
			CGPSDlg::gpsDlg->ConfigBaudrate(1, position_update_attr);		//Boost to 115200 when update rate > 10Hz.
		}
		else
		{
      //Andrew request 20160901, doesn't need restart in this
			//CGPSDlg::gpsDlg->SendRestartCommand(1);
		}
	}
	CGPSDlg::gpsDlg->SetMode();  
	CGPSDlg::gpsDlg->CreateGPSThread();
	return 0;	
}

void CGPSDlg::OnConfigurePositionUpdateRate()
{
	if(!CheckConnect())
	{
		return;
	}
	SetInputMode(NoOutputMode);
	CPositionRateDlg *dlg = new CPositionRateDlg(this);

	if(dlg->DoModal() == IDOK)
	{
		position_update_rate = dlg->rate;
		position_update_attr = dlg->attr;
		//com_baudrate = g_setting.GetBaudrate();
#if(CHECK_SAEE_MULTIHZ_ON)
		if(position_update_rate > 1)
		{
			U08 data = 0;
			U32 data2 = 0;
			if(Ack == QuerySagps(Return, &data))
			{
				if(data != 2)
				{	//AGPS not disable or ROM version.
					if(Ack == QueryGnssBootStatus(Return, &data2))
					{
						if((LOBYTE(data2) != 1) && HIBYTE(data2) != 0)
						{	// Not boot from ROM.
							add_msgtolist("Configure Position Rate Cancel...");	
							::AfxMessageBox("Please disable SAEE before using multi-hz update rate.");

							SetMode();  
							CreateGPSThread();
							return;
						}
					}
				}
			}
		}
#endif
		AfxBeginThread(Configurepositionrate, 0);
	}
	else
	{
		SetMode();  
		CreateGPSThread();
	}
}

void CGPSDlg::OnBinaryConfigDrMultiHz()
{
	if(!CheckConnect())
	{
		return;
	}
	SetInputMode(NoOutputMode);

	DrMultiHzDlg *dlg = new DrMultiHzDlg(this);

	if(dlg->DoModal() == IDOK)
	{
		position_update_rate = dlg->rate;
		position_update_attr = dlg->attr;
		//com_baudrate = g_setting.baudrate;
		AfxBeginThread(ConfigureDrMultiHz, 0);
	}
	else
	{
		SetMode();  
		CreateGPSThread();
	}

}

void CGPSDlg::OnBnClickedECompassCalibration()
{
	if(!CheckConnect())
	{
		return;
	}

	//WaitEvent();
	ClearQue();

	U08 message[8];
	U08 msg[1];

	msg[0] = 0x54; //msgid
	int msg_len = SetMessage2(message, msg, sizeof(msg));

	int res = SendToTarget(message, msg_len, "E-Compass Calibration successfully", true);
	if(res)
	{
		GetDlgItem(IDC_ECOM_COUNTER)->SetWindowText("20");
		SetTimer(ECOM_CALIB_TIMER, 1000, NULL);
	}
	else
	{
		::AfxMessageBox("No E-Compass for calibration.");
	}
	CreateGPSThread();
}

void CGPSDlg::OnBinarySystemRestart()
{	
	if(!CheckConnect())
	{
		return;
	}

	SetInputMode(NoOutputMode);
	CSysRestartDlg dlg;
	dlg.ReStartType = 0;
	INT_PTR nResult = dlg.DoModal();	
	if(nResult == IDOK)
	{		
		m_ttffCount = 0;
		m_initTtff = false;
		SetTTFF(0);
		DeleteNmeaMemery();
	}
	else
	{
		SetMode();  
		CreateGPSThread();
	}
}

void CGPSDlg::OnBnClickedHotstart()
{
	if(!CheckConnect())
	{
		return;
	}

	SetInputMode(NoOutputMode);	
	m_ttffCount      = 0;	
	m_initTtff = false;
	if( IS_DEBUG == FALSE)
	{
		target_only_restart(1);
		//CreateGPSThread();
	}
	else
	{
		CSysRestartDlg* dlg = new CSysRestartDlg;
		dlg->ReStartType = 1;
		INT_PTR nResult = dlg->DoModal();	
		if(nResult == IDOK)
		{		
			SetTTFF(0);
		}
		else
		{
			SetMode();  
			Sleep(500);
			CreateGPSThread();
		}
		delete dlg;	dlg= NULL;
	}
	DeleteNmeaMemery();
	m_ttffCount      = 0;	
	m_initTtff = false;
	//m_ttff.SetWindowText("0");
	SetTTFF(0);

	ClearGlonass();
}

//warm start
void CGPSDlg::OnBnClickedWarmstart()
{
	if(!CheckConnect())
		return;
	SetInputMode(NoOutputMode);	

	m_ttffCount      = 0;	
	m_initTtff = false;
	if(IS_DEBUG == FALSE)
	{
		target_only_restart(2);
	}
	else
	{
		CSysRestartDlg* dlg = new CSysRestartDlg;
		dlg->ReStartType = 2;
		INT_PTR nResult = dlg->DoModal();	
		if(nResult == IDOK)
		{		
			SetTTFF(0);
		}
		else
		{
			SetMode();  
			Sleep(500);
			CreateGPSThread();
		}
		delete dlg;	dlg= NULL;	
	}
	DeleteNmeaMemery();
	ClearGlonass();
}

void CGPSDlg::target_only_restart(int mode)
{
	SendRestartCommand(mode);
	SetMode(); 
	CreateGPSThread();
}

void CGPSDlg::SendRestartCommand(int mode)
{
	SYSTEMTIME	now; 
	GetSystemTime(&now);

	U08 msg[15] = {0};
	msg[0] = 0x1; //msgid
	msg[1] = mode; //mode

	if(mode!=1 && mode!=5)	
	{
		msg[2] = HIBYTE(now.wYear);
		msg[3] = LOBYTE(now.wYear);	
		msg[4] = (U08)now.wMonth;
		msg[5] = (U08)now.wDay;
		msg[6] = (U08)now.wHour;
		msg[7] = (U08)now.wMinute;
		msg[8] = (U08)now.wSecond;
		
		if(mode!=4)
		{
			if(m_gpggaMsgBk.Latitude == 0.0)
			{
				m_gpggaMsgBk.Latitude = warmstart_latitude;
			}
			if(m_gpggaMsgBk.Longitude == 0.0)
			{
				m_gpggaMsgBk.Longitude = warmstart_longitude;
			}
			if(m_gpggaMsgBk.Altitude == 0.0)
			{
				m_gpggaMsgBk.Altitude = (F32)warmstart_altitude;
			}

			S16 lat = (S16)(m_gpggaMsgBk.Latitude / 100) * 100;
			lat += (S16)((m_gpggaMsgBk.Latitude - lat)* 100 / 60 + 0.5);
			if(m_gpggaMsgBk.Latitude_N_S == 'S')
			{
				lat *= -1;
			}
			msg[9] = HIBYTE(lat);
			msg[10]= LOBYTE(lat);	

			S16 lon = (S16)((m_gpggaMsgBk.Longitude / 100) * 100);
			lon += (S16)((m_gpggaMsgBk.Longitude - lon)* 100 / 60 + 0.5);
			if(m_gpggaMsgBk.Longitude_E_W == 'W')
			{
				lon *= -1;
			}
			msg[11]= HIBYTE(lon);
			msg[12]= LOBYTE(lon);

			msg[13]= HIBYTE((int)m_gpggaMsgBk.Altitude);
			msg[14]= LOBYTE((int)m_gpggaMsgBk.Altitude);
		}
	}

	int len = CGPSDlg::gpsDlg->SetMessage(msg,sizeof(msg));
	SendToTarget(CGPSDlg::m_inputMsg, len, "System Restart successfully", true);
}

void CGPSDlg::Restart(U08* messages, BOOL restoreConnection /* = TRUE */)
{
	m_CloseBtn.ShowWindow(0);
	//WaitEvent();
	ClearQue();
  bool bAck = (restoreConnection) 
    ? SendToTarget(messages, 22, "System Restart successfully", true)
    : SendToTargetNoAck(messages, 22);

  if(bAck)
  {
    DeleteNmeaMemery();
    ClearInformation();
    m_initTtff = false;
    m_ttffCount = 0;
    SetTTFF(0);
    ClearGlonass();
  }

  if(restoreConnection)
  {
	  SetMode(); 
	  CreateGPSThread();	
  }
	gpsSnrBar->Invalidate(FALSE);
	bdSnrBar->Invalidate(FALSE);
#if(!SUPPORT_BDL2_GSV2)
	gaSnrBar->Invalidate(FALSE);
#endif

	pic_earth->Invalidate(FALSE);
	pic_scatter->Invalidate(FALSE);
	
	m_CloseBtn.ShowWindow(1);
  PostMessage(UWM_TEST_XN112_START, (bAck) ? 1 : 0, 0);
}

UINT RestartThread(LPVOID pParam)
{
	BinaryData binData(15);
	*(binData.GetBuffer(0)) = 1;
	*(binData.GetBuffer(1)) = CGPSDlg::gpsDlg->GetRestartMode();
	BinaryCommand binCmd(binData);
	CGPSDlg::gpsDlg->Restart(binCmd.GetBuffer());

	return 0;
}

void CGPSDlg::OnBnClickedColdstart()
{
#if defined(PRODUCTION_OLIVER20161128)
  OnBinaryConfigureClockOffset();
  return;
#elif defined(XN120_TESTER)
  DoXn120Tester();
  return;
#endif
	if(!CheckConnect())
	{
		return;
	}
	SetInputMode(NoOutputMode);
	m_restartMode = 3;	
	AfxBeginThread(RestartThread, 0);
}

UINT AFX_CDECL Configure1ppsPulseWidthThread(LPVOID param)
{
	CGPSDlg::gpsDlg->ExecuteConfigureCommand(CGPSDlg::gpsDlg->m_inputMsg, 14, "Configure 1PPS Pulse Width successfully");
	return 0;
}

void CGPSDlg::OnConfigure1PpsPulseWidth()
{
	if(!CheckConnect())
	{
		return;
	}

	SetInputMode(NoOutputMode);
	CConfig1ppsPulseWidthDlg dlg;
	INT_PTR nResult = dlg.DoModal();
	if(nResult == IDOK) 
	{
    U08 msg[7] = {0};
		msg[0] = 0x65;
		msg[1] = 0x01;
		msg[2] = dlg.m_nPulseWidth >> 24 & 0xFF;
		msg[3] = dlg.m_nPulseWidth >> 16 & 0xFF;
		msg[4] = dlg.m_nPulseWidth >> 8 & 0xFF;
		msg[5] = dlg.m_nPulseWidth & 0xFF;
		msg[6] = (U08)dlg.m_nAttribute;

		int len = SetMessage(msg, sizeof(msg));
    AfxBeginThread(Configure1ppsPulseWidthThread, 0);
	}
	else
	{
		SetMode();  
		CreateGPSThread();
	}
}

UINT AFX_CDECL Query1ppsPulseWidth(LPVOID param)
{
	U08 msg[2];
	msg[0] = 0x65;
	msg[1] = 0x02;

	int len = CGPSDlg::gpsDlg->SetMessage(msg, sizeof(msg));

	//CGPSDlg::gpsDlg->WaitEvent();
	CGPSDlg::gpsDlg->ClearQue();

	U08 buff[100] = {0};
	if(CGPSDlg::gpsDlg->SendToTarget(CGPSDlg::m_inputMsg, len, "Query 1PPS Pulse Width successfully", true))
	{
		ScopeTimer timer;

		while(1)
		{
			memset(buff, 0, 100);
			if(NULL == CGPSDlg::gpsDlg->m_serial) return FALSE;
			DWORD res = CGPSDlg::gpsDlg->m_serial->GetBinaryAck(buff, sizeof(buff));	

			if(ReadOK(res))
			{
				if(Cal_Checksum(buff) == 0x65 && buff[5] == 0x80)
				{
					if(CGPSDlg::gpsDlg->GetShowBinaryCmdData())
					{
						CGPSDlg::gpsDlg->add_msgtolist("Return: " + theApp.GetHexString(buff,  buff[2] <<8 | buff[3] + 7));	
					}
					CString strTxt;
					UINT32 nPulseWidth = 0;
					nPulseWidth = buff[6] << 24 & 0xff000000 | 
								buff[7] << 16 & 0xff0000 | 
								buff[8] << 8 &0xff00 | 
								buff[9] & 0xff;
					strTxt.Format("1PPS Pulse Width : %dus", nPulseWidth);
					CGPSDlg::gpsDlg->add_msgtolist(strTxt);
					break;
				}
			}
			if(timer.GetDuration() > TIME_OUT_MS)
			{
				AfxMessageBox("Timeout: GPS device no response.");
				break;
			}
		} //while(1)
	} //if(CGPSDlg::gpsDlg->SendToTarget(messages,len,"Query Position Pinning successfully"))

	CGPSDlg::gpsDlg->SetMode();  
	CGPSDlg::gpsDlg->CreateGPSThread();
	return TRUE;	
}

void CGPSDlg::OnQuery1PpsPulseWidth()
{
	if(!CheckConnect())
	{
		return;
	}
	//Utility::Log(__FUNCTION__, "start QuerySBAS thread", __LINE__);
	AfxBeginThread(Query1ppsPulseWidth, 0);	
}

long int g_1ppsFrequencyOutput = 0;
U08 g_1ppsFrequencyOutputAttr = 0;
UINT Config1ppsFrequencyOutputThread(LPVOID param)
{
	U08 msg[7];

	msg[0] = 0x65;
	msg[1] = 0x03;
	msg[2] = (U08)HIBYTE(HIWORD(g_1ppsFrequencyOutput));
	msg[3] = (U08)LOBYTE(HIWORD(g_1ppsFrequencyOutput));
	msg[4] = (U08)HIBYTE(LOWORD(g_1ppsFrequencyOutput));
	msg[5] = (U08)LOBYTE(LOWORD(g_1ppsFrequencyOutput));
	msg[6] = g_1ppsFrequencyOutputAttr;

	int len = CGPSDlg::gpsDlg->SetMessage(msg, sizeof(msg));
	CGPSDlg::gpsDlg->ExecuteConfigureCommand(CGPSDlg::m_inputMsg, len, "Configure 1PPS Frequency Output successfully");
	return 0;	
}

void CGPSDlg::OnConfig1ppsFrequencyOutput()
{
	CConfig1ppsFrequenceOutput dlg;

	if(!CheckConnect())	
	{
		return;
	}

	if(dlg.DoModal()!=IDOK)
	{
		SetMode();  
		CreateGPSThread();
		return;
	}

	g_1ppsFrequencyOutput = dlg.freqOutput;
	g_1ppsFrequencyOutputAttr = dlg.freqOutputAttr;
	AfxBeginThread(Config1ppsFrequencyOutputThread, 0);
}

void CGPSDlg::OnLineAssistance()
{
	if(!CheckConnect())	
	{
    return;
  }

	CTiming_start dlg;
	dlg.DoModal();
	SetMode();  
	CreateGPSThread();
}

UINT AFX_CDECL ConfigureNoisePowerControlThread(LPVOID param)
{
	CGPSDlg::gpsDlg->ExecuteConfigureCommand(CGPSDlg::gpsDlg->m_inputMsg, 11, "Configure Noise Power Control successfully");
	return 0;
}

U32 m_propri_enable;
int m_propri_attr;
UINT ConfigProprietaryMessageThread(LPVOID param)
{
	U08 msg[6] = { 0 };

	msg[0] = 0x49;
	msg[1] = m_propri_enable>>24 & 0xFF;
	msg[2] = m_propri_enable>>16 & 0xFF;
	msg[3] = m_propri_enable>>8 & 0xFF;
	msg[4] = m_propri_enable & 0xFF;
	msg[5] = m_propri_attr;

	int len = CGPSDlg::gpsDlg->SetMessage(msg,sizeof(msg));
	CGPSDlg::gpsDlg->ExecuteConfigureCommand(CGPSDlg::m_inputMsg, len, "Configure Proprietary NMEA successfully");

	return 0;	
}

void CGPSDlg::OnConfigProprietaryMessage()
{
	if(!CheckConnect())	
	{
		return;
	}

	CConfig_Proprietary dlg;
	if(dlg.DoModal()==IDOK)
	{
		m_propri_enable = dlg.enable;
		m_propri_attr = dlg.attr;
		AfxBeginThread(ConfigProprietaryMessageThread, 0);
	}
	else
	{
		SetMode();  
		CreateGPSThread();
	}
}

int m_antenna_control,m_antenna_attr;
UINT configy_antenna_detection_thread(LPVOID param)
{
	U08 msg[3];

	memset(msg,0,sizeof(msg));

	msg[0] = 0x47;
	msg[1] = m_antenna_control;
	msg[2] = m_antenna_attr;

	int len = CGPSDlg::gpsDlg->SetMessage(msg,sizeof(msg));
	CGPSDlg::gpsDlg->ExecuteConfigureCommand(CGPSDlg::m_inputMsg, len, "Configure Antenna Detection successfully");

	return 0;	
}

void CGPSDlg::OnBinaryConfigureantennadetection()
{
	if(!CheckConnect())	
	{
    return;
  }
   
  CConAntennaDetection dlg;
	if(dlg.DoModal()==IDOK)
	{
		m_antenna_control = dlg.antenna_control;
		m_antenna_attr = dlg.attr;
		AfxBeginThread(configy_antenna_detection_thread, 0);
	}
	else
	{
		SetMode();  
		CreateGPSThread();
	}
}

U08 CGPSDlg::QueryChanelFreq(int chanel,U16 *prn,double *freq)
{
	CmdErrorCode ack;
	U32 v;
	long int nco_freq;
	int clk_base_reg = 0x60005A24;
	int clk_struct_size = 0x5b0;
	m_regAddress = clk_base_reg+4 + chanel*clk_struct_size;
	ack = QueryRegister(Return, &v);

	if(ack != Ack) 
	{
	return 0;
	}
	//return 0;

	*prn = v>>16;

	m_regAddress = 0x2000401C + chanel*0x100;
	ack = QueryRegister(Return, &v);
  if(ack != Ack) 
  {
    return 0;
  }
  v = v<<9;
  memcpy(&nco_freq,&v,4);
  nco_freq = nco_freq / 512;
  *freq = nco_freq * 0.030487 - 32051.25;

  return 1;
}

U08 CGPSDlg::PredictClockOffset(double *clk_offset)
{
	U08 ack;
	U16 prn;
	double nco_freq;
	int i;

	for(i=0;i<12;i++)
	{ 
		ack = QueryChanelFreq(i,&prn,&nco_freq);
		//return ack; // Test
		if(ack == 1)
		{       
			if(prn == 1)
			{
				*clk_offset = nco_freq - 173.3276;
				return 1;
			}     
		}
	}
	return 0;
}

void CGPSDlg::OnClockOffsetPredict()
{
	if(!CheckConnect())
	{
		return;
	}
	SetInputMode(NoOutputMode);
	int ChannelTable[GSA_MAX_SATELLITE] = {0};
	QueryChannelDoppler(Return, ChannelTable);
	QueryClockOffset(Display, ChannelTable);
	SetMode();
	CreateGPSThread();
}

void CGPSDlg::OnClockOffsetPredictOld()
{
	if(!CheckConnect())
	{
		return;
	}
	SetInputMode(NoOutputMode);
	QueryChannelDoppler(Display, NULL);
	SetMode();
	CreateGPSThread();
}

CGPSDlg::CmdErrorCode CGPSDlg::GetBinaryResponse(BinaryData* ackCmd, U08 cAck, U08 cAckSub, DWORD timeOut, bool silent, bool noWaitAck, int cmdSize, int cmdLen)
{
	ScopeTimer t;
	bool alreadyAck = noWaitAck;
	while(NULL != m_serial)
	{
		ackCmd->Clear();
		if(CGPSDlg::gpsDlg->CheckTimeOut(t.GetDuration(), timeOut, silent))
		{	//Time Out
			return Timeout;
		}

    DWORD len = m_serial->GetBinaryAck(ackCmd->GetBuffer(), ackCmd->Size(), timeOut - t.GetDuration());
		if(!ReadOK(len))
		{	
			continue;
		}

		int cmdSize = MAKEWORD((*ackCmd)[3], (*ackCmd)[2]);
		if(cmdSize != len - 7)
		{	//Packet Size Error
			continue;
		}
		if( (*ackCmd)[0] != 0xa0 || (*ackCmd)[1] != 0xa1 ||
			(*ackCmd)[len-2] != 0x0d || (*ackCmd)[len-1] != 0x0a)
		{	//Format Error
			continue;
		}
		if((*ackCmd)[4] == 0x83 && (*ackCmd)[5] == 0x0)
		{	//ACK0
			continue;
		}
		if((*ackCmd)[4] == 0x84)
		{	//NACK
			if(m_bShowBinaryCmdData)
			{
				add_msgtolist("NACK: " + theApp.GetHexString(ackCmd->Ptr(), len));	
			}
			add_msgtolist("Received NACK...");
			return NACK;
		}
		if((*ackCmd)[4] == 0x85)
		{	//FormatError
			if(m_bShowBinaryCmdData)
			{
				add_msgtolist("FormatError: " + theApp.GetHexString(ackCmd->Ptr(), len));	
			}

			if(cmdSize > 0 && cmdLen > 0)
			{
				CString txt;
				txt.Format("Format Error! Viewer / FW Length: %d/%d", ConvertLeonU16(ackCmd->Ptr(5 + cmdSize)), cmdLen);
				add_msgtolist(txt);
			}
			add_msgtolist("Received FormatError...");
			return FormatError;
		}

		if( (*ackCmd)[4] == 0x83)
		{	//Get ACK
			if(m_bShowBinaryCmdData)
			{
				add_msgtolist("Ack: " + theApp.GetHexString(ackCmd->Ptr(), len));	
			}
			alreadyAck = true;
			continue;
		}

		if(alreadyAck && Cal_Checksum((*ackCmd).GetBuffer()) && (*ackCmd)[4]==cAck && cAckSub==0x00)
		{
			if(m_bShowBinaryCmdData)
			{
				add_msgtolist("Out: "  +theApp.GetHexString(ackCmd->Ptr(), len));	
			}
			return Ack;
		}
		if(alreadyAck && Cal_Checksum((*ackCmd).GetBuffer()) && (*ackCmd)[4]==cAck && (*ackCmd)[5]==cAckSub)
		{
			if(m_bShowBinaryCmdData)
			{
				add_msgtolist("Out: "  +theApp.GetHexString(ackCmd->Ptr(), len));	
			}
			return Ack;
		}
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::ExcuteBinaryCommand(U08 ackId, U08 ackSubId, BinaryCommand* cmd, BinaryData* ackCmd, DWORD timeOut, bool silent)
{
	U08* pCmd = cmd->GetBuffer();
	int inSize = cmd->Size();
	if(m_bShowBinaryCmdData)
	{
		add_msgtolist("In : " + theApp.GetHexString(pCmd, inSize));	
	}
	ackCmd->Alloc(1024);
	if(NULL == CGPSDlg::gpsDlg->m_serial) 
		return Timeout;
	m_serial->ClearQueue();
	m_serial->SendData(pCmd, inSize);

	return GetBinaryResponse(ackCmd, ackId, ackSubId, timeOut, silent);

}

CGPSDlg::CmdErrorCode CGPSDlg::ExcuteBinaryCommand(int cmdIdx, BinaryCommand* cmd, BinaryData* ackCmd, DWORD timeOut, bool silent)
{
	CommandEntry binMsg = cmdTable[cmdIdx];
  return CGPSDlg::ExcuteBinaryCommand(binMsg.cmdAck, binMsg.cmdAckSub, cmd, ackCmd, timeOut, silent);
}

CGPSDlg::CmdErrorCode CGPSDlg::ExcuteBinaryCommandNoWait(int cmdIdx, BinaryCommand* cmd)
{
	CommandEntry binMsg = cmdTable[cmdIdx];
	U08* pCmd = cmd->GetBuffer();
	int inSize = cmd->Size();
	if(m_bShowBinaryCmdData)
	{
		add_msgtolist("In : " + theApp.GetHexString(pCmd, inSize));	
	}

	m_serial->SendData(pCmd, inSize);
	return Timeout;
}

CGPSDlg::CmdErrorCode (CGPSDlg::*queryFunction)(CGPSDlg::CmdExeMode, void*) = NULL;
UINT GenericQueryThread(LPVOID param)
{
	(*CGPSDlg::gpsDlg.*queryFunction)(CGPSDlg::Display, NULL);
	if(NULL == CGPSDlg::gpsDlg->m_serial) return FALSE;
	CGPSDlg::gpsDlg->SetMode();  
	CGPSDlg::gpsDlg->CreateGPSThread();
	return TRUE;	
}

void CGPSDlg::GenericQuery(QueryFunction pfn)
{
	if(!CheckConnect())
	{
		return;
	}

	SetInputMode(NoOutputMode);
	queryFunction = pfn;
	::AfxBeginThread(GenericQueryThread, 0);
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryPositionRate(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QueryPositionRateCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryPositionRateCmd].cmdId);

	BinaryData ackCmd;
	if(Ack == ExcuteBinaryCommand(QueryPositionRateCmd, &cmd, &ackCmd))
	{
		if(nMode==Return)
		{
			*((U08*)outputData) = ackCmd[5];
			return Ack;
		}

		CString strMsg = "Query Position Rate successfully";
		add_msgtolist(strMsg);
		strMsg.Format("Position Rate = %d Hz", ackCmd[5]);
		add_msgtolist(strMsg);
	}
	return Timeout;
}

const TEL el[] = 
{
	{0,0},
	{6377563.396,	299.3249646},
	{6377340.189,	299.3249646},
	{6378160,	    298.25},
	{6377483.865,	299.1528128},
	{6377397.155,	299.1528128},
	{6378206.4,  	294.9786982},
	{6378249.145,	293.465},
	{6377276.345,	300.8017},
	{6377298.556,	300.8017},
	{6377301.243,	300.8017},
	{6377295.664,	300.8017},
	{6377304.063,	300.8017},
	{6377309.613,	300.8017},
	{6378155,	    298.3},
	{6378200,	    298.3},
	{6378270,	    297},
	{6378160,	    298.247},
	{6378388,	    297},
	{6378245,	    298.3},
	{6378137,	    298.257222101},
	{6378160,	    298.25},
	{6378135,	    298.26},
	{6378137,	    298.257223563}
};

const TDRL datum[] =
{
	{   0,    0,    0, el[23].a, el[23].I_F,23},
	{-118,  -14,  218, el[7].a,  el[7].I_F ,23},   
	{-134,   -2,  210, el[7].a,  el[7].I_F,7 },
	{-165,  -11,  206, el[7].a,  el[7].I_F ,7},	
	{-123,  -20,  220, el[7].a,  el[7].I_F ,7},
	{-166,  -15,  204, el[7].a,  el[7].I_F ,7},	
	{-128,  -18,  224, el[7].a,  el[7].I_F ,7},
	{-161,  -14,  205, el[7].a,  el[7].I_F ,7},
	{ -43, -163,   45, el[19].a, el[19].I_F,19},
	{-150, -250,   -1, el[18].a, el[18].I_F,18},	
	{-143, -236,    7, el[18].a, el[18].I_F,18},
	{-115,	118,  426, el[6].a,  el[6].I_F ,6},
	{-491,  -22,  435, el[3].a,  el[3].I_F ,3},  
	{-270,	 13,   62, el[7].a,  el[7].I_F ,7},	
	{-138, -105, -289, el[7].a, el[7].I_F,7},
	{-153,	 -5, -292, el[7].a, el[7].I_F ,7},	
	{-125, -108, -295, el[7].a, el[7].I_F,7}, 
	{-161,  -73, -317, el[7].a, el[7].I_F ,7},	
	{-143,	-90, -294, el[7].a, el[7].I_F,7},
	{-134, -105, -295, el[7].a, el[7].I_F ,7},	
	{-169,	-19, -278, el[7].a, el[7].I_F,7},
	{-147,  -74, -283, el[7].a, el[7].I_F ,7},	
	{-142,	-96, -293, el[7].a, el[7].I_F,7},
	{-160,   -6, -302, el[7].a, el[7].I_F ,7},	
	{-157,	 -2, -299, el[7].a, el[7].I_F,7},
	{-175,  -23, -303, el[7].a, el[7].I_F ,7},
	{-205,	107,   53, el[18].a, el[18].I_F,18},   
	{ 145,	 75, -272, el[18].a, el[18].I_F,18},	
	{-320,	550, -494, el[18].a, el[18].I_F,18},	
	{ 114, -116, -333,  el[18].a, el[18].I_F,18},	
	{ 124, -234,  -25, el[18].a, el[18].I_F,18},   
	{-133,  -48,  148, el[3].a,  el[3].I_F ,3},	
	{-134,	-48,  149, el[3].a, el[3].I_F,3},
	{ -79, -129,  145, el[7].a,  el[7].I_F,7},
	{-127, -769,  472, el[18].a, el[18].I_F,18},   
	{ -73,  213,  296, el[6].a, el[6].I_F,6},
	{-173,	253,   27, el[18].a, el[18].I_F,18},   
	{ 307,  304, -318, el[18].a, el[18].I_F,18},
	{-384,	664,  -48, el[5].a,  el[5].I_F,5},
	{-104, -129,  239, el[18].a, el[18].I_F,18},	
	{-148,	136,   90, el[18].a, el[18].I_F,18},
	{ 298, -304, -375, el[18].a, el[18].I_F,18},
	{-136, -108, -292, el[7].a, el[7].I_F,7},
	{  -2,  151,  181, el[6].a, el[6].I_F,6},
	{-263,	  6,  431, el[7].a, el[7].I_F,7},
	{ 175,  -38,  113, el[18].a, el[18].I_F,18},	
	{-134,	229,  -29, el[18].a, el[18].I_F,18},
	{-206,  172,   -6, el[18].a, el[18].I_F,18},
	{ -83,	 37,  124, el[7].a, el[7].I_F,7},     
	{ 260,   12, -147, el[7].a, el[7].I_F,7},
	{-377,	681,  -50, el[5].a, el[5].I_F,5},
	{ 230, -199, -752, el[18].a, el[18].I_F,18},	
	{ 211,	147,  111, el[18].a, el[18].I_F,18},
	{ 374,  150,  588, el[5].a, el[5].I_F,5},
	{-104, -101, -140, el[18].a, el[18].I_F,18},   
	{-130, -117, -151, el[18].a, el[18].I_F,18},
	{ -86,  -96, -120, el[18].a, el[18].I_F,18},   
	{ -86,  -96, -120, el[18].a, el[18].I_F,18},
	{ -87,	-95, -120, el[18].a, el[18].I_F,18},   
	{ -84,  -95, -130, el[18].a, el[18].I_F,18},
	{-117, -132, -164, el[18].a, el[18].I_F,18},   
	{ -97, -103, -120, el[18].a, el[18].I_F,18},
	{ -97,	-88, -135, el[18].a, el[18].I_F,18},   
	{-107,  -88, -149, el[18].a, el[18].I_F,18},
	{ -87,	-98, -121, el[18].a, el[18].I_F,18},   
	{ -87,  -96, -120, el[18].a, el[18].I_F,18},
	{-103, -106, -141, el[18].a, el[18].I_F,18},   
	{ -84, -107, -120, el[18].a, el[18].I_F,18},
	{-112,	-77, -145, el[18].a, el[18].I_F,18},   
	{ -86,  -98, -119, el[18].a, el[18].I_F,18},
	{  -7,	215,  225, el[7].a,  el[7].I_F,7},
	{-133, -321,   50, el[18].a,  el[18].I_F,18},	
	{  84,	-22,  209, el[18].a,  el[18].I_F,18},
	{-104,  167,  -38, el[18].a,  el[18].I_F ,18},
	{-100, -248,  259, el[6].a,  el[6].I_F,6},
	{-403,  684,   41, el[5].a,  el[5].I_F,5},
	{ 252, -209, -751, el[18].a,  el[18].I_F,18},  
	{-333, -222,  114, el[18].a,  el[18].I_F,18},
	{ 653, -212,  449, el[4].a,  el[4].I_F,4},
	{ -73,   46,  -86, el[18].a,  el[18].I_F,18},	
	{-156, -271, -189, el[18].a,  el[18].I_F,18},
	{-637, -549, -203, el[18].a,  el[18].I_F,18},
	{ 282,  726,  254, el[8].a,  el[8].I_F,8},
	{ 295,  736,  257, el[10].a,  el[10].I_F,10},
	{ 283,	682,  231, el[13].a,  el[13].I_F,13},
	{ 217,  823,  299, el[8].a,  el[8].I_F,8},	
	{ 182,	915,  344, el[8].a,  el[8].I_F,8},
	{ 198,  881,  317, el[8].a,  el[8].I_F,8},	
	{ 210,	814,  289, el[8].a,  el[8].I_F,8},
	{ -24,  -15,    5, el[17].a,  el[17].I_F,17},
	{ 506, -122,  611, el[2].a,  el[2].I_F,2},
	{-794,	119, -298, el[18].a,  el[18].I_F,18},	
	{ 208, -435, -229, el[18].a,  el[18].I_F,18},
	{ 189,	-79, -202, el[18].a,  el[18].I_F,18},
	{ -97,	787,   86,  el[8].a,  el[8].I_F,8},
	{ 145, -187,  103, el[18].a,  el[18].I_F,18},
	{ -11,	851,	5, el[12].a,  el[12].I_F,12},
	{ 647, 1777,-1124, el[18].a,  el[18].I_F,18},
	{   0,	  0,	0,  el[20].a,  el[20].I_F,20},
	{  42,  124,  147, el[6].a,  el[6].I_F,6},
	{-130,	 29,  364, el[7].a,  el[7].I_F,7},    
	{ -90,   40,   88, el[7].a,  el[7].I_F,7},
	{-133,	-77,  -51, el[6].a,  el[6].I_F,6},    
	{-133,  -79,  -72, el[6].a,  el[6].I_F,6},
	{ -74, -130,   42, el[7].a,  el[7].I_F,7},    
	{  41, -220, -134, el[7].a,  el[7].I_F,7},
	{ 639,	405,   60, el[5].a,  el[5].I_F,5},    
	{  31,  146,   47, el[7].a,  el[7].I_F,7},
	{ 912,	-58, 1227, el[18].a,  el[18].I_F,18},
	{ -81,  -84,  115, el[7].a,  el[7].I_F,7},	
	{ -92,  -93,  122, el[7].a,  el[7].I_F,7},
	{ 174,  359,  365, el[7].a,  el[7].I_F},	
	{-247, -148,  369, el[7].a,  el[7].I_F,7},
	{-243, -192,  477, el[7].a,  el[7].I_F},	
	{-249, -156,  381, el[7].a,  el[7].I_F,7},
	{ -10,  375,  165, el[18].a,  el[18].I_F,18},
	{  -5,	135,  172, el[6].a,  el[6].I_F,6},    
	{  -2,  152,  149, el[6].a,  el[6].I_F,6},
	{   2,	204,  105, el[6].a,  el[6].I_F,6},    
	{  -4,  154,  178, el[6].a,  el[6].I_F,6},
	{   1,	140,  165, el[6].a,  el[6].I_F,6},    
	{  -7,  162,  188, el[6].a,  el[6].I_F,6},
	{  -9,	157,  184, el[6].a,  el[6].I_F,6},    
	{ -22,  160,  190, el[6].a,  el[6].I_F,6},
	{   4,	159,  188, el[6].a,  el[6].I_F,6},    
	{  -7,	139,  181, el[6].a,  el[6].I_F,6},
	{   0,	125,  201, el[6].a,  el[6].I_F,6},    
	{  -9,	152,  178, el[6].a,  el[6].I_F,6},
	{  11,	114,  195, el[6].a,  el[6].I_F,6},    
	{  -3,	142,  183, el[6].a,  el[6].I_F,6},
	{   0,	125,  194, el[6].a,  el[6].I_F,6},    
	{ -10,	158,  187, el[6].a,  el[6].I_F,6},
	{  -8,	160,  176, el[6].a,  el[6].I_F,6},    
	{  -9,	161,  179, el[6].a,  el[6].I_F,6},
	{  -8,	159,  175, el[6].a,  el[6].I_F,6},    
	{ -12,	130,  190, el[6].a,  el[6].I_F,6},
	{   0,	  0,    0, el[20].a, el[20].I_F,20},   
	{  -2,	  0,    4, el[20].a,  el[20].I_F,20},
	{   0,	  0,	0, el[20].a, el[20].I_F,20},   
	{   0,	  0,    0, el[20].a,  el[20].I_F,20},
	{   1,	  1,   -1, el[20].a, el[20].I_F,20},   
	{   0,	  0,    0, el[20].a,  el[20].I_F,20},
	{-186,	-93,  310, el[7].a,  el[7].I_F,7},  
	{-425, -169,   81,  el[18].a,  el[18].I_F,18},
	{-130,	110,  -13,  el[15].a,  el[15].I_F,15},
	{  89, -279, -183,  el[6].a,  el[6].I_F,6},  
	{  45, -290, -172,  el[6].a,  el[6].I_F,6},
	{  65, -290, -190,  el[6].a,  el[6].I_F,6},	
	{  61, -285, -181,  el[6].a,  el[6].I_F,6}, 
	{  58, -283, -182,  el[6].a,  el[6].I_F,6},
	{-346,	 -1,  224,  el[7].a,  el[7].I_F,7},
	{ 371, -112,  434,  el[1].a,  el[1].I_F,1}, 	
	{ 371, -111,  434,  el[1].a,  el[1].I_F,1},
	{ 375, -111,  431,  el[1].a,  el[1].I_F,1},	
	{ 384, -111,  425,  el[1].a,  el[1].I_F,1}, 
	{ 370, -108,  434,  el[1].a,  el[1].I_F,1}, 	
	{-307,  -92,  127,  el[18].a,  el[18].I_F,18}, 
	{ 185,  165,   42,  el[18].a,  el[18].I_F,18}, 
	{-106, -129,  165,  el[7].a,  el[7].I_F,7},
	{-148,	 51, -291,  el[18].a,  el[18].I_F,18}, 
	{-499, -249,  314,  el[18].a,  el[18].I_F,18}, 
	{-270,  188, -388,  el[18].a,  el[18].I_F,18}, 
	{-270,	183, -390,  el[18].a,  el[18].I_F,18}, 
	{-305,	243, -442,  el[18].a,  el[18].I_F,18}, 
	{-282,	169, -371,  el[18].a,  el[18].I_F,18}, 
	{-278,	171, -367,  el[18].a,  el[18].I_F,18}, 
	{-298,	159, -369,  el[18].a,  el[18].I_F,18}, 
	{-288,	175, -376,  el[18].a,  el[18].I_F,18}, 
	{-279,	175, -379,  el[18].a,  el[18].I_F,18}, 
	{-295,	173, -371,  el[18].a,  el[18].I_F,18}, 
	{  16,	196,   93,  el[18].a,  el[18].I_F,18}, 
	{  11,	 72, -101,  el[6].a,  el[6].I_F,6},
	{  28, -130,  -95,  el[19].a,  el[19].I_F,19}, 
	{-128, -283,   22,  el[18].a,  el[18].I_F,18}, 
	{ 164,	138, -189,  el[18].a,  el[18].I_F,18}, 
	{  94, -948,-1262,  el[18].a,  el[18].I_F,18}, 
	{-225,	-65,	9,  el[18].a,  el[18].I_F,18},
	{  28, -121,  -77,  el[19].a,  el[19].I_F,19},
	{  23, -124,  -82,  el[19].a,  el[19].I_F,19},
	{  26, -121,  -78,   el[19].a,  el[19].I_F,19}, 
	{  24, -124,  -82,    el[19].a,  el[19].I_F,19}, 
	{  15, -130,  -84,    el[19].a,  el[19].I_F,19}, 
	{  24, -130,  -92,    el[19].a,  el[19].I_F,19}, 
	{  28, -121,  -77,   el[19].a,  el[19].I_F,19}, 
	{ 589,	 76,  480,   el[5].a,  el[5].I_F,5}, 
	{ 170,   42,   84,   el[18].a,  el[18].I_F,18}, 
	{-203,	141,   53,   el[18].a,  el[18].I_F,5}, 
	{-355,   21,   72,   el[18].a,  el[18].I_F,18}, 
	{ 616,	 97, -251,   el[4].a,  el[4].I_F,4}, 
	{-289, -124,   60,   el[18].a,  el[18].I_F,18}, 
	{ -88,	  4,  101,   el[7].a,  el[7].I_F,7}, 
	{ -62,   -1,  -37,   el[21].a,  el[21].I_F,21}, 
	{ -61,    2,  -48,  el[21].a,  el[21].I_F,21}, 
	{ -60,   -2,  -41,   el[21].a,  el[21].I_F,21}, 
	{ -75,	 -1,  -44,   el[21].a,  el[21].I_F,21}, 
	{ -44,    6,  -36,   el[21].a,  el[21].I_F,21}, 
	{ -48,    3,  -44,  el[21].a,  el[21].I_F,21}, 
	{ -47,   26,  -42,   el[21].a,  el[21].I_F,21}, 
	{ -53,	  3,  -47,   el[21].a,  el[21].I_F,21}, 
	{ -57,    1,  -41,   el[21].a,  el[21].I_F,21}, 
	{ -61,	  2,  -33,   el[21].a,  el[21].I_F,21}, 
	{ -58,    0,  -44,   el[21].a,  el[21].I_F,21}, 
	{ -45,	 12,  -33,   el[21].a,  el[21].I_F,21}, 
	{ -45,    8,  -33,   el[21].a,  el[21].I_F,21}, 
	{   7,	-10,  -26,  el[14].a,  el[14].I_F,14}, 
	{-189, -242,  -91,  el[18].a,  el[18].I_F,18}, 
	{-679,	669,  -48,   el[9].a,  el[7].I_F,7},
	{-148,  507,  685, el[5].a,  el[5].I_F,5},	
	{-148,	507,  685,  el[5].a,  el[5].I_F,5},
	{-158,  507,  676,  el[5].a,  el[5].I_F,5},	
	{-147,	506,  687,  el[5].a,  el[5].I_F,5},
	{-632,  438, -609,  el[18].a,  el[18].I_F,18},
	{  51,	391,  -36,  el[7].a,  el[7].I_F,7},
	{-123, -206,  219, el[7].a,  el[7].I_F,7},
	{ 276,	-57,  149,  el[18].a,  el[18].I_F,18},
	{ 102,   52,  -38, el[16].a,  el[16].I_F,16},
	{   0,	  0,	0,  el[22].a,  el[22].I_F,22},
	{-155,  171,   37,  el[18].a,  el[18].I_F,18},
	{-265,	120, -358,  el[18].a,  el[18].I_F,22},     	
};

CGPSDlg::CmdErrorCode CGPSDlg::QueryDatum(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QueryDatumCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryDatumCmd].cmdId);

	BinaryData ackCmd;
	if(Ack == ExcuteBinaryCommand(QueryDatumCmd, &cmd, &ackCmd))
	{
		U16 datumIdx = MAKEWORD(ackCmd[6], ackCmd[5]);
		CString strMsg = "Query Datum successfully";
		add_msgtolist(strMsg);
		strMsg.Format("DeltaX: %d, DeltaY: %d, DeltaZ: %d", 
			datum[datumIdx].DeltaX, datum[datumIdx].DeltaY, datum[datumIdx].DeltaZ);
		add_msgtolist(strMsg);
		strMsg.Format("Semi_Major_Axis: %.3f", datum[datumIdx].Semi_Major_Axis);
		add_msgtolist(strMsg);
		strMsg.Format("Inversed_Flattening: %.9f", datum[datumIdx].Inversd_Flattening);
		add_msgtolist(strMsg);
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QuerySha1String(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QuerySha1StringCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QuerySha1StringCmd].cmdId);
	cmd.SetU08(2, cmdTable[QuerySha1StringCmd].cmdSubId);

	if(NoWait==nMode)
	{
		ExcuteBinaryCommandNoWait(QuerySha1StringCmd, &cmd);
		return Timeout;
	}
	BinaryData ackCmd;
	if(Ack == ExcuteBinaryCommand(QuerySha1StringCmd, &cmd, &ackCmd))
	{
		CString strMsg, strSha;
		const int Sha1Length = 32;
		char* p = strSha.GetBuffer(Sha1Length + 1);
		memcpy(p, ackCmd.GetBuffer(6), Sha1Length);
		p[Sha1Length] = 0;
		strSha.ReleaseBuffer();
		strMsg.Format("SHA1: %s", strSha);
		add_msgtolist(strMsg);
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryConstellationCapability(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QueryConstellationCapabilityCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryConstellationCapabilityCmd].cmdId);
	cmd.SetU08(2, cmdTable[QueryConstellationCapabilityCmd].cmdSubId);

	if(NoWait==nMode)
	{
		ExcuteBinaryCommandNoWait(QueryConstellationCapabilityCmd, &cmd);
		return Timeout;
	}
	BinaryData ackCmd;
  CmdErrorCode err =ExcuteBinaryCommand(QueryConstellationCapabilityCmd, &cmd, &ackCmd);
	if(err != Ack)
	{
    return err;
  }

  if(Return == nMode)
  {	
    *((BinaryData*)outputData) = ackCmd;
	  return err;
  }

	CString strMsg;
	U16 mode = MAKEWORD(ackCmd[7], ackCmd[6]);
	add_msgtolist("Constellation Capability : ");

	if(mode & 0x0001)
	{
		strMsg += "GPS, ";
	}
	if(mode & 0x0002)
	{
		strMsg += "GLONASS, ";
	}
	if(mode & 0x0004)
	{
		strMsg += "Galileo, ";
	}
	if(mode & 0x0008)
	{
		strMsg += "Beidou, ";
	}
	add_msgtolist(strMsg);

	UINT32 freq = ackCmd[8] << 24 & 0xff000000 | 
				ackCmd[9] << 16 & 0xff0000 | 
				ackCmd[10] << 8 &0xff00 | 
				ackCmd[11] & 0xff;
	strMsg.Format("GPS Ref. Freq. : %d Hz", freq);
	add_msgtolist(strMsg);

	freq = ackCmd[12] << 24 & 0xff000000 | 
				ackCmd[13] << 16 & 0xff0000 | 
				ackCmd[14] << 8 &0xff00 | 
				ackCmd[15] & 0xff;
	strMsg.Format("GLONASS Ref. Freq. : %d Hz", freq);
	add_msgtolist(strMsg);

	freq = ackCmd[16] << 24 & 0xff000000 | 
				ackCmd[17] << 16 & 0xff0000 | 
				ackCmd[18] << 8 &0xff00 | 
				ackCmd[19] & 0xff;
	strMsg.Format("Galileo Ref. Freq. : %d Hz", freq);
	add_msgtolist(strMsg);

	freq = ackCmd[20] << 24 & 0xff000000 | 
				ackCmd[21] << 16 & 0xff0000 | 
				ackCmd[22] << 8 &0xff00 | 
				ackCmd[23] & 0xff;
	strMsg.Format("Beidou Ref. Freq. : %d Hz", freq);
	add_msgtolist(strMsg);

	return Ack;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryVersionExtension(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QueryVersionExtensionCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryVersionExtensionCmd].cmdId);
	cmd.SetU08(2, cmdTable[QueryVersionExtensionCmd].cmdSubId);

	if(NoWait==nMode)
	{
		ExcuteBinaryCommandNoWait(QueryVersionExtensionCmd, &cmd);
		return Ack;
	}
	BinaryData ackCmd;
	if(Ack == ExcuteBinaryCommand(QueryVersionExtensionCmd, &cmd, &ackCmd))
	{
		CString strMsg, strSha;
		const int Sha1Length = 32;
		char* p = strSha.GetBuffer(Sha1Length + 1);
		memcpy(p, ackCmd.GetBuffer(6), Sha1Length);
		p[Sha1Length] = 0;
		strSha.ReleaseBuffer();

		if(nMode==Return)
		{
			*((CString*)outputData) = strSha;
			return Ack;
		}

		strMsg.Format("Version Extension: %s", strSha);
		add_msgtolist(strMsg);
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::SendZenlandInitCmd(CmdExeMode nMode, void* outputData)
{
  //0xF2 0x0E 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x66 0x77 0x01 0x86 0x72 0x06 0x00 0x00
  U08 cmd[] = { 0xF2, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                0x00, 0x66, 0x77, 0x01, 0x86, 0x72, 0x06, 0x00, 0x00 };
	BinaryData ackCmd;

  m_serial->ClearQueue();
  m_serial->SendData(cmd, sizeof(cmd), true);
  ackCmd.Alloc(32);
	DWORD len = m_serial->GetZenlaneResponse1(ackCmd.GetBuffer(), ackCmd.Size(), 2000);
  CString strMsg = "Initialization successfully";
  if(ackCmd[0] == 0xF2 && ackCmd[6] == 0x12 && ackCmd[10]==0x4b && ackCmd[11]==0x43 && ackCmd[12]==0x41)
  {
    add_msgtolist(strMsg);
    return Ack;
  }
  else
  {
    strMsg = "Initialization no response";
    add_msgtolist(strMsg);
  }

	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::SendZenlandQueryCmd(CmdExeMode nMode, void* outputData)
{
  //0xE7 0x0E 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x66 0x77 0x01 0x86 0x4F 0X06 0x00 0x00
  U08 cmd[] = { 0xE7, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                0x00, 0x66, 0x77, 0x01, 0x86, 0x4F, 0x06, 0x00, 0x00 };
	BinaryData ackCmd;

    m_serial->ClearQueue();
	  m_serial->SendData(cmd, sizeof(cmd), true);
    ackCmd.Alloc(32);
		DWORD len = m_serial->GetZenlaneResponse1(ackCmd.GetBuffer(), ackCmd.Size(), 2000);
    CString strMsg = "Send query command...";
    add_msgtolist(strMsg);
    //if(ackCmd[6] == 0x12 && ackCmd[10]==0x4b && ackCmd[11]==0x43 && ackCmd[12]==0x41)
    //{
    //  //add_msgtolist(strMsg);
    //  return Ack;
    //}
    //else
    //{
    //  //strMsg = "Query no response";
    //  //add_msgtolist(strMsg);
    //}
		return Ack;
}

CGPSDlg::CmdErrorCode CGPSDlg::QuerySoftwareVersionSystemCode(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QuerySwVerSysCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QuerySwVerSysCmd].cmdId);
	cmd.SetU08(2, cmdTable[QuerySwVerSysCmd].cmdSubId);

	if(NoWait==nMode)
	{
		ExcuteBinaryCommandNoWait(QuerySwVerSysCmd, &cmd);
		return Ack;
	}

	BinaryData ackCmd;
  CmdErrorCode err = ExcuteBinaryCommand(QuerySwVerSysCmd, &cmd, &ackCmd);
	if(err != Ack)
	{
		return err;
	}

  CString strMsg;
	if(Return == nMode)
	{	//Return command data
    CString* strOutput = (CString*)outputData;
	  strMsg.Format("%s%d.%d.%d\r\n", "Kernel Version ", ackCmd[7], ackCmd[8], ackCmd[9]);
	  *strOutput = strMsg;
	  strMsg.Format("%s%d.%d.%d\r\n", "Software Version ", ackCmd[11], ackCmd[12], ackCmd[13]);
	  *strOutput += strMsg;
	  strMsg.Format("%s%d.%d.%d\r\n", "Revision ", ackCmd[15] + 2000, ackCmd[16], ackCmd[17]);
	  *strOutput += strMsg;
		return err;
	}

	CString strExt;
	if(Ack==QueryVersionExtension(Return, &strExt))
	{
	}

	strMsg = "Query Version successfully";
	add_msgtolist(strMsg);
	strMsg.Format("%s%d.%d.%d%s", "Kernel Version ", ackCmd[7], ackCmd[8], ackCmd[9], strExt);
	add_msgtolist(strMsg);
	strMsg.Format("%s%d.%d.%d", "Software Version ", ackCmd[11], ackCmd[12], ackCmd[13]);
	add_msgtolist(strMsg);
	strMsg.Format("%s%d.%d.%d", "Revision ", ackCmd[15] + 2000, ackCmd[16], ackCmd[17]);
	add_msgtolist(strMsg);
	m_versionInfo = ackCmd;
	return Ack;
}

/*
CGPSDlg::CmdErrorCode CGPSDlg::QuerySoftwareCrcRomCode(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QuerySwCrcRomCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QuerySwCrcRomCmd].cmdId);
	cmd.SetU08(2, cmdTable[QuerySwCrcRomCmd].cmdSubId);

	BinaryData ackCmd;
	if(Ack == ExcuteBinaryCommand(QuerySwCrcRomCmd, &cmd, &ackCmd, 5000))
	{
		CString strMsg = "Query CRC successfully";
		add_msgtolist(strMsg);
		strMsg.Format("%s%02x%02x", "Rom CRC: ", ackCmd[6], ackCmd[7]);
		add_msgtolist(strMsg);
	}
	return Timeout;
}
*/
CGPSDlg::CmdErrorCode CGPSDlg::QuerySoftwareCrcSystemCode(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QuerySwCrcSysCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QuerySwCrcSysCmd].cmdId);
	cmd.SetU08(2, cmdTable[QuerySwCrcSysCmd].cmdSubId);

	BinaryData ackCmd;
  //QuerySoftwareCrcSystemCode need long timeout
	if(Ack == ExcuteBinaryCommand(QuerySwCrcSysCmd, &cmd, &ackCmd, 5000))
	{
		if(nMode==Return)
		{
			*((U16*)outputData) = (ackCmd[6]<<8) | ackCmd[7];
			return Ack;
		}

		CString strMsg = "Query CRC successfully";
		add_msgtolist(strMsg);
		strMsg.Format("%s%02x%02x", "System CRC: ", ackCmd[6], ackCmd[7]);
		add_msgtolist(strMsg);
	}
	return Ack;
}
/*
CGPSDlg::CmdErrorCode CGPSDlg::QueryWaasStatus(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QueryWaasStatusCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryWaasStatusCmd].cmdId);
	cmd.SetU08(2, cmdTable[QueryWaasStatusCmd].cmdSubId);

	BinaryData ackCmd;
	if(Ack == ExcuteBinaryCommand(QueryWaasStatusCmd, &cmd, &ackCmd))
	{
		CString strMsg = "Query Waas Status successfully";
		add_msgtolist(strMsg);
		strMsg.Format("WAAS status: %s", (ackCmd[5]) ? "Enable" : "Disable");
		add_msgtolist(strMsg);
	}
	return Timeout;
}
*/
CGPSDlg::CmdErrorCode CGPSDlg::QueryPositionPinning(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QueryPositionPinningCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryPositionPinningCmd].cmdId);

	BinaryData ackCmd;
	if(Ack == ExcuteBinaryCommand(QueryPositionPinningCmd, &cmd, &ackCmd))
	{
		CString strMsg = "Query Position Pinning successfully";
		add_msgtolist(strMsg);
		strMsg = "Position Pinning: ";
		if(0==ackCmd[5])
		{
			strMsg += "Default";
		}
		else if(1==ackCmd[5])
		{
			strMsg += "Enable";
		}
		else if(2==ackCmd[5])
		{
			strMsg += "Disable";
		}
		add_msgtolist(strMsg);
		strMsg.Format("Pinning Speed: %d", MAKEWORD(ackCmd[7], ackCmd[6]));
		add_msgtolist(strMsg);
		strMsg.Format("Pinning Cnt: %d", MAKEWORD(ackCmd[9], ackCmd[8]));
		add_msgtolist(strMsg);
		strMsg.Format("Unpinning Speed: %d", MAKEWORD(ackCmd[11], ackCmd[10]));
		add_msgtolist(strMsg);
		strMsg.Format("Unpinning Cnt: %d", MAKEWORD(ackCmd[13], ackCmd[12]));
		add_msgtolist(strMsg);
		strMsg.Format("Unpinning Distance: %d", MAKEWORD(ackCmd[15], ackCmd[14]));
		add_msgtolist(strMsg);
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::Query1ppsMode(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[Query1ppsModeCmd].cmdSize);
	cmd.SetU08(1, cmdTable[Query1ppsModeCmd].cmdId);

	BinaryData ackCmd;
	if(Ack == ExcuteBinaryCommand(Query1ppsModeCmd, &cmd, &ackCmd))
	{
		CString strMsg = "Query GPS Measurement Mode successfully";
		add_msgtolist(strMsg);
		strMsg = "GPS Measurement Mode: ";
		if(0==ackCmd[5])
		{
			strMsg += "Not sync to UTC second";
		}
		else if(1==ackCmd[5])
		{
			strMsg += "Sync to UTC second when 3D fix";
		}
		else if(2==ackCmd[5])
		{
			strMsg += "On when 1 SV";
		}
		add_msgtolist(strMsg);
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryPowerMode(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QueryPowerModeCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryPowerModeCmd].cmdId);

	BinaryData ackCmd;
	if(Ack == ExcuteBinaryCommand(QueryPowerModeCmd, &cmd, &ackCmd))
	{
		CString strMsg = "Query Power Mode successfully";
		add_msgtolist(strMsg);
		strMsg = "Power Mode: ";
		if(1==ackCmd[5])
		{
			strMsg += "Power Save";
		}
		else
		{
			strMsg += "Normal";
		}
		add_msgtolist(strMsg);
	}
	return Timeout;
}
/*
CGPSDlg::CmdErrorCode CGPSDlg::QueryPowerSavingParameters(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QueryPowerSavingParametersCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryPowerSavingParametersCmd].cmdId);

	BinaryData ackCmd;
	if(Ack == ExcuteBinaryCommand(QueryPowerSavingParametersCmd, &cmd, &ackCmd))
	{
		CString strMsg = "Query Power Saving Parameters successfully";
		add_msgtolist(strMsg);
		strMsg.Format("2 PSE no fix cycle time:%d", MAKEWORD(ackCmd[7], ackCmd[6]));
		add_msgtolist(strMsg);
		strMsg.Format("2 PSE fix cycle time:%d", MAKEWORD(ackCmd[9], ackCmd[8]));
		add_msgtolist(strMsg);
		strMsg.Format("4 PSE no fix cycle time:%d", MAKEWORD(ackCmd[11], ackCmd[10]));
		add_msgtolist(strMsg);
		strMsg.Format("4 PSE fix cycle time:%d", MAKEWORD(ackCmd[13], ackCmd[12]));
		add_msgtolist(strMsg);
		strMsg.Format("First boot up full power time:%d", MAKEWORD(ackCmd[15], ackCmd[14]));
		add_msgtolist(strMsg);
		strMsg.Format("Power save entering wait:%d", MAKEWORD(ackCmd[17], ackCmd[16]));
		add_msgtolist(strMsg);
		strMsg.Format("Cold start full power time:%d", MAKEWORD(ackCmd[19], ackCmd[18]));
		add_msgtolist(strMsg);
		strMsg.Format("Tunnel full power time:%d", MAKEWORD(ackCmd[21], ackCmd[20]));
		add_msgtolist(strMsg);
		strMsg.Format("SV no diff time:%d", MAKEWORD(ackCmd[23], ackCmd[22]));
		add_msgtolist(strMsg);
		strMsg = "Power Mode: ";
		if(1==ackCmd[24])
		{
			strMsg += "Power Save";
		}
		else
		{
			strMsg += "Normal";
		}
		add_msgtolist(strMsg);
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryV8PowerSavingParametersRom(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QueryV8RomPowerSaveParameters].cmdSize);
	cmd.SetU08(1, cmdTable[QueryV8RomPowerSaveParameters].cmdId);
	cmd.SetU08(2, cmdTable[QueryV8RomPowerSaveParameters].cmdSubId);

	BinaryData ackCmd;
	if(Ack == ExcuteBinaryCommand(QueryV8RomPowerSaveParameters, &cmd, &ackCmd))
	{
		CString strMsg = "Query Power Saving Parameters successfully";
		add_msgtolist(strMsg);
		strMsg.Format("2 PSE no fix cycle time:%d", MAKEWORD(ackCmd[7], ackCmd[6]));
		add_msgtolist(strMsg);
		strMsg.Format("2 PSE fix cycle time:%d", MAKEWORD(ackCmd[9], ackCmd[8]));
		add_msgtolist(strMsg);
		strMsg.Format("4 PSE no fix cycle time:%d", MAKEWORD(ackCmd[11], ackCmd[10]));
		add_msgtolist(strMsg);
		strMsg.Format("4 PSE fix cycle time:%d", MAKEWORD(ackCmd[13], ackCmd[12]));
		add_msgtolist(strMsg);
		strMsg.Format("First boot up full power time:%d", MAKEWORD(ackCmd[15], ackCmd[14]));
		add_msgtolist(strMsg);
		strMsg.Format("Power save entering wait:%d", MAKEWORD(ackCmd[17], ackCmd[16]));
		add_msgtolist(strMsg);
		strMsg.Format("Cold start full power time:%d", MAKEWORD(ackCmd[19], ackCmd[18]));
		add_msgtolist(strMsg);
	}
	return Timeout;
}
*/
CGPSDlg::CmdErrorCode CGPSDlg::QueryV8PowerSavingParameters(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QueryV8PowerSaveParameters].cmdSize);
	cmd.SetU08(1, cmdTable[QueryV8PowerSaveParameters].cmdId);
	cmd.SetU08(2, cmdTable[QueryV8PowerSaveParameters].cmdSubId);

	BinaryData ackCmd;
	if(Ack == ExcuteBinaryCommand(QueryV8PowerSaveParameters, &cmd, &ackCmd))
	{
		CString strMsg = "Query Power Saving Parameters successfully";
		add_msgtolist(strMsg);
		strMsg.Format("2 PSE no fix cycle time:%d", MAKEWORD(ackCmd[7], ackCmd[6]));
		add_msgtolist(strMsg);
		strMsg.Format("2 PSE fix cycle time:%d", MAKEWORD(ackCmd[9], ackCmd[8]));
		add_msgtolist(strMsg);
		strMsg.Format("4 PSE no fix cycle time:%d", MAKEWORD(ackCmd[11], ackCmd[10]));
		add_msgtolist(strMsg);
		strMsg.Format("4 PSE fix cycle time:%d", MAKEWORD(ackCmd[13], ackCmd[12]));
		add_msgtolist(strMsg);
		strMsg.Format("First boot up full power time:%d", MAKEWORD(ackCmd[15], ackCmd[14]));
		add_msgtolist(strMsg);
		strMsg.Format("Power save entering wait:%d", MAKEWORD(ackCmd[17], ackCmd[16]));
		add_msgtolist(strMsg);
		strMsg.Format("Cold start full power time:%d", MAKEWORD(ackCmd[19], ackCmd[18]));
		add_msgtolist(strMsg);
		strMsg.Format("Reserved:%d", MAKEWORD(ackCmd[21], ackCmd[20]));
		add_msgtolist(strMsg);
		strMsg.Format("SV no diff time:%d", MAKEWORD(ackCmd[23], ackCmd[22]));
		add_msgtolist(strMsg);
		strMsg = "Power Mode: ";
		if(1==ackCmd[24])
		{
			strMsg += "Power Save";
		}
		else
		{
			strMsg += "Normal";
		}
		add_msgtolist(strMsg);
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryProprietaryMessage(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QueryProprietaryMessageCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryProprietaryMessageCmd].cmdId);

	BinaryData ackCmd;
	if(Ack == ExcuteBinaryCommand(QueryProprietaryMessageCmd, &cmd, &ackCmd))
	{
		add_msgtolist("Query Proprietary Message successfully");
		DWORD enable = MAKELONG(MAKEWORD(ackCmd[8], ackCmd[7]), MAKEWORD(ackCmd[6], ackCmd[5]));
		if(enable & 0x00000001)
		{
			add_msgtolist("epe enable");
		}
		else
		{
			add_msgtolist("epe disable");
		}
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryTiming(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QueryTimingCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryTimingCmd].cmdId);

	BinaryData ackCmd;
	CmdErrorCode err = ExcuteBinaryCommand(QueryTimingCmd, &cmd, &ackCmd, (nMode == Display) ? 3000 : 1000);
	if(err != Ack)
	{
		return err;
	}
 
	if(Return == nMode)
	{	//Return command data
    *((BinaryData*)outputData) = ackCmd;
		return err;
	}

	CString strMsg = "Query Timing successfully";
	add_msgtolist(strMsg);

	_1PPS_Timing_T t;
	t.Timing_mode = ackCmd[5];
	t.Survey_Length = MAKELONG(MAKEWORD(ackCmd[9], ackCmd[8]), MAKEWORD(ackCmd[7], ackCmd[6]));
	t.Standard_deviation = MAKELONG(MAKEWORD(ackCmd[13], ackCmd[12]), MAKEWORD(ackCmd[11], ackCmd[10]));
	for(int i=0; i<sizeof(t.latitude); ++i)
	{
		((BYTE*)(&t.latitude))[7 - i] = ackCmd[14 + i];
		((BYTE*)(&t.longitude))[7 - i] = ackCmd[22 + i];
		if(i < sizeof(t.altitude))
		{
			((BYTE*)(&t.altitude))[3 - i] = ackCmd[30 + i];
		}
	}
	t.RT_Timing_mode = ackCmd[34];
	t.RT_Survey_Length = MAKELONG(MAKEWORD(ackCmd[38], ackCmd[37]), MAKEWORD(ackCmd[36], ackCmd[35]));

	if(t.Timing_mode==0)
	{
		add_msgtolist("Timing Mode: PVT Mode");
	}
	else if(t.Timing_mode==1)
	{
		add_msgtolist("Timing Mode: Survey Mode");
		strMsg.Format("Survey Length: %d",t.Survey_Length);
		add_msgtolist(strMsg);
		strMsg.Format("Standard deviation: %d",t.Standard_deviation);
		add_msgtolist(strMsg);
	}
	else if(t.Timing_mode==2)
	{
		add_msgtolist("Timing Mode: Static Mode");
		strMsg.Format("Saved Latitude: %12.9lf",t.latitude);
		add_msgtolist(strMsg);
		strMsg.Format("Saved Longitude: %12.9lf",t.longitude);
		add_msgtolist(strMsg);
		strMsg.Format("Saved Altitude: %3.2f",t.altitude);
		add_msgtolist(strMsg);
	}

	if(t.RT_Timing_mode==0)
	{
		add_msgtolist("Run-time Timing Mode: PVT Mode");
	}
	else if(t.RT_Timing_mode==1)
	{
		add_msgtolist("Run-time Timing Mode: Survey Mode");
		strMsg.Format("Run-time Survey Length: %d",t.RT_Survey_Length);
		add_msgtolist(strMsg);
	}
	else if(t.RT_Timing_mode==2)
	{
		add_msgtolist("Run-time Timing Mode: Static Mode");
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryDopMask(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QueryDopMaskCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryDopMaskCmd].cmdId);

	BinaryData ackCmd;
	if(Ack == ExcuteBinaryCommand(QueryDopMaskCmd, &cmd, &ackCmd))
	{
		CString strMsg = "Query DOP Mask successfully";
		add_msgtolist(strMsg);

		strMsg.Format("DOP Mode: %d", ackCmd[5]);
		add_msgtolist(strMsg);
		if(ackCmd[5]==1)
		{
			strMsg.Format("PDOP: %.1f", MAKEWORD(ackCmd[7], ackCmd[6]) / 10.0);
			add_msgtolist(strMsg);
			strMsg.Format("HDOP: %.1f", MAKEWORD(ackCmd[9], ackCmd[8]) / 10.0);
			add_msgtolist(strMsg);
		}
		else if(ackCmd[5] == 2)
		{
			strMsg.Format("PDOP: %.1f", MAKEWORD(ackCmd[7], ackCmd[6]) / 10.0);
			add_msgtolist(strMsg);
		}
		else if(ackCmd[5] == 3)
		{
			strMsg.Format("HDOP: %.1f", MAKEWORD(ackCmd[9], ackCmd[8]) / 10.0);
			add_msgtolist(strMsg);
		}
		else if(ackCmd[5] == 4)
		{
			strMsg.Format("GDOP: %.1f", MAKEWORD(ackCmd[11], ackCmd[10]) / 10.0);
			add_msgtolist(strMsg);
		}
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryElevationAndCnrMask(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QueryElevationAndCnrMaskCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryElevationAndCnrMaskCmd].cmdId);

	BinaryData ackCmd;
	if(Ack == ExcuteBinaryCommand(QueryElevationAndCnrMaskCmd, &cmd, &ackCmd))
	{
		CString strMsg = "Query Elevation and CNR Mask successfully";
		add_msgtolist(strMsg);
		strMsg.Format("Elevation and CNR Mask Mode: %d", ackCmd[5]);
		add_msgtolist(strMsg);
		if(ackCmd[5]==1)
		{
			strMsg.Format("Elevation Mask: %d", ackCmd[6]);
			add_msgtolist(strMsg);
			strMsg.Format("CNR Mask: %d", ackCmd[7]);
			add_msgtolist(strMsg);
		}
		else if(ackCmd[5] == 2)
		{
			strMsg.Format("Elevation Mask: %d", ackCmd[6]);
			add_msgtolist(strMsg);
		}
		else if(ackCmd[5] == 3)
		{
			strMsg.Format("CNR Mask: %d", ackCmd[7]);
			add_msgtolist(strMsg);
		}
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryAntennaDetection(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QueryAntennaDetectionCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryAntennaDetectionCmd].cmdId);

	BinaryData ackCmd;
	if(Ack == ExcuteBinaryCommand(QueryAntennaDetectionCmd, &cmd, &ackCmd))
	{
		CString strMsg = "Query Antenna Detection successfully";
		add_msgtolist(strMsg);
		//20150709 Modify for fw spec, just only ON/OFF.
		//if(ackCmd[5] & 0x01)
		//{
		//	add_msgtolist("Short Circuit Detection On");
		//}
		//else
		//{
		//	add_msgtolist("Short Circuit Detection Off");
		//}
		//if(ackCmd[5] & 0x02)
		//{
		//	add_msgtolist("Antenna Detection On");
		//}
		//else
		//{
		//	add_msgtolist("Antenna Detection Off");
		//}
		if(ackCmd[5])
		{
			add_msgtolist("Antenna Detection On");
		}
		else
		{
			add_msgtolist("Antenna Detection Off");
		}

		strMsg.Format("Antenna Mode Status:0x%02X", ackCmd[6]);
		add_msgtolist(strMsg);
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryNoisePower(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QueryNoisePowerCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryNoisePowerCmd].cmdId);

	BinaryData ackCmd;
	if(Ack == ExcuteBinaryCommand(QueryNoisePowerCmd, &cmd, &ackCmd))
	{
		CString strMsg = "Query Noise Power successfully";
		add_msgtolist(strMsg);
		strMsg.Format("Noise Power: %d", MAKELONG(MAKEWORD(ackCmd[8], ackCmd[7]), MAKEWORD(ackCmd[6], ackCmd[5])));
		add_msgtolist(strMsg);
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryDrInfo(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QueryDrInfoCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryDrInfoCmd].cmdId);

	BinaryData ackCmd;
	if(Ack == ExcuteBinaryCommand(QueryDrInfoCmd, &cmd, &ackCmd))
	{
		CString strMsg = "Query DR Info successfully";
		add_msgtolist(strMsg);
		//data = buff[7]<<24 | buff[8]<<16 | buff[9]<<8 | buff[10];
		F32 gyroData;
		for(int i=0; i<sizeof(gyroData); ++i)
		{
			((BYTE*)(&gyroData))[sizeof(gyroData) - i - 1] = ackCmd[7 + i];

		}
		strMsg.Format("Temperature = %f",(F32)MAKEWORD(ackCmd[6], ackCmd[5]) / 10.0);
		add_msgtolist(strMsg);
		strMsg.Format("Gyro Data = %f",gyroData);
		add_msgtolist(strMsg);
		strMsg.Format("Odo Data = %d", MAKEWORD(ackCmd[12], ackCmd[11]));
		add_msgtolist(strMsg);
		strMsg.Format("Odo backward indicator =  %s", (ackCmd[13]) ? "TRUE" : "FALSE");
		add_msgtolist(strMsg);

	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryDrHwParameter(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QueryDrHwParameterCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryDrHwParameterCmd].cmdId);

	BinaryData ackCmd;
	if(Ack == ExcuteBinaryCommand(QueryDrHwParameterCmd, &cmd, &ackCmd))
	{
		CString strMsg = "Query DR HW Parameter successfully";
		add_msgtolist(strMsg);

		F32 gyroOutputUnit, gyroReference, gyroAdcReference, gyroAdcGain, defaultOdoScale;
		for(int i=0; i<sizeof(gyroOutputUnit); ++i)
		{
			((BYTE*)(&gyroOutputUnit))[sizeof(F32) - i - 1] = ackCmd[5 + i];
			((BYTE*)(&gyroReference))[sizeof(F32) - i - 1] = ackCmd[9 + i];
			((BYTE*)(&gyroAdcReference))[sizeof(F32) - i - 1] = ackCmd[14 + i];
			((BYTE*)(&gyroAdcGain))[sizeof(F32) - i - 1] = ackCmd[21 + i];
			((BYTE*)(&defaultOdoScale))[sizeof(F32) - i - 1] = ackCmd[25 + i];

		}
		strMsg.Format("Gyro output unit = %f mV/dps", gyroOutputUnit);
		add_msgtolist(strMsg);
		strMsg.Format("Gyro reference voltage = %f", gyroReference);
		add_msgtolist(strMsg);
		strMsg.Format("Gyro ADC type = %s", (ackCmd[13]) ? "unipolar" : "differential");
		add_msgtolist(strMsg);
		strMsg.Format("Gyro ADC reference voltage = %f", gyroAdcReference);
		add_msgtolist(strMsg);
		strMsg.Format("Gyro ADC sampling freq = %d", MAKEWORD(ackCmd[19], ackCmd[18]));
		add_msgtolist(strMsg);
		strMsg.Format("Gyro ADC NBITS = %d", ackCmd[20]);
		add_msgtolist(strMsg);
		strMsg.Format("Gyro ADC gain = %f", gyroAdcGain);
		add_msgtolist(strMsg);
		strMsg.Format("Default ODO scale = %f", defaultOdoScale);
		add_msgtolist(strMsg);
		strMsg.Format("%s", (ackCmd[29]) ? "ODO has backward" : "ODO no backward");
		add_msgtolist(strMsg);
		strMsg.Format("%s", (ackCmd[30]) ? "ODO invert FW/BW" : "ODO not invert FW/BW");
		add_msgtolist(strMsg);
		strMsg.Format("%s", (ackCmd[31]) ? "ODO in period mode" : "ODO in hz mode");
		add_msgtolist(strMsg);
		strMsg.Format("ODO period in = %d us", MAKEWORD(ackCmd[33], ackCmd[32]));
		add_msgtolist(strMsg);
		strMsg.Format("DR firmware version = %d.%d.%d", ackCmd[35], ackCmd[36], ackCmd[37]);
		add_msgtolist(strMsg);
		strMsg.Format("%s", (ackCmd[38]) ? "DR enable temperature" : "DR not enable temperature");
		add_msgtolist(strMsg);
		strMsg.Format("%s", (ackCmd[39]) ? "DR enable tunnel table" : "DR not enable tunnel table");
		add_msgtolist(strMsg);
		strMsg = "DR table region = ";
		if(ackCmd[43] & 0x01)
		{
			strMsg.Append("Taiwan,");
		}
		if(ackCmd[43] & 0x02)
		{
			strMsg.Append("Korea,");
		}
		if(ackCmd[43] & 0x04)
		{
			strMsg.Append("Japan,");
		}
		if(ackCmd[43] & 0x08)
		{
			strMsg.Append("China,");
		}
		add_msgtolist(strMsg);

	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QuerySbas(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QuerySbasCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QuerySbasCmd].cmdId);
	cmd.SetU08(2, cmdTable[QuerySbasCmd].cmdSubId);

	BinaryData ackCmd;
  CmdErrorCode err = ExcuteBinaryCommand(QuerySbasCmd, &cmd, &ackCmd, (nMode == Display) ? 3000 : 1000);
	if(err != Ack)
	{
    return err;
  }

  if(Return == nMode)
	{	
    *((BinaryData*)outputData) = ackCmd;
		return err;
  }

	CString strMsg = "Query SBAS successfully";
	add_msgtolist(strMsg);
	strMsg.Format("SBAS system: %s", ((ackCmd[6]) ? "Enable" : "Disable"));
	add_msgtolist(strMsg);
	if(ackCmd[7]==0)
	{
		strMsg.Format("Ranging : %s", "Disable");
	}
	else if(ackCmd[7]==1)
	{
		strMsg.Format("Ranging : %s", "Enable");
	}
	else
	{
		strMsg.Format("Ranging : %s", "Auto");
	}
	add_msgtolist(strMsg);
	strMsg.Format("Ranking URA Mask: %d", ackCmd[8]);
	add_msgtolist(strMsg);
	strMsg.Format("Correction: %s", ((ackCmd[9]) ? "Enable" : "Disable"));
	add_msgtolist(strMsg);
	strMsg.Format("Number of tracking channels: %d", ackCmd[10]);
	add_msgtolist(strMsg);
	strMsg.Format("All: %s", ((ackCmd[11] & 0x80) ? "Enable" : "Disable"));
	add_msgtolist(strMsg);	
	if(!(ackCmd[11] & 0x80))
	{
		strMsg.Format("WAAS: %s", ((ackCmd[11] & 0x01) ? "Enable" : "Disable"));
		add_msgtolist(strMsg);
		strMsg.Format("EGNOS: %s", ((ackCmd[11] & 0x02) ? "Enable" : "Disable"));
		add_msgtolist(strMsg);
		strMsg.Format("MSAS: %s", ((ackCmd[11] & 0x04) ? "Enable" : "Disable"));
		add_msgtolist(strMsg);
		strMsg.Format("GAGAN: %s", ((ackCmd[11] & 0x08) ? "Enable" : "Disable"));
		add_msgtolist(strMsg);
	}
	return Ack;
}

CGPSDlg::CmdErrorCode CGPSDlg::QuerySbas2(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QuerySbas2Cmd].cmdSize);
	cmd.SetU08(1, cmdTable[QuerySbas2Cmd].cmdId);
	cmd.SetU08(2, cmdTable[QuerySbas2Cmd].cmdSubId);

	BinaryData ackCmd;
  CmdErrorCode err = ExcuteBinaryCommand(QuerySbas2Cmd, &cmd, &ackCmd, (nMode == Display) ? 3000 : 1000);
	if(err != Ack)
	{
    return err;
  }

  if(Return == nMode)
	{	
    *((BinaryData*)outputData) = ackCmd;
		return err;
  }

	CString strMsg = "Query SBAS Advanced successfully";
	add_msgtolist(strMsg);
	strMsg.Format("SBAS system: %s", ((ackCmd[6]) ? "Enable" : "Disable"));
	add_msgtolist(strMsg);
	if(ackCmd[7] == 0)
	{
		strMsg.Format("Ranging : %s", "Disable");
	}
	else if(ackCmd[7] == 1)
	{
		strMsg.Format("Ranging : %s", "Enable");
	}
	else
	{
		strMsg.Format("Ranging : %s", "Auto");
	}
	add_msgtolist(strMsg);
	strMsg.Format("Ranking URA Mask: %d", ackCmd[8]);
	add_msgtolist(strMsg);
	strMsg.Format("Correction: %s", ((ackCmd[9]) ? "Enable" : "Disable"));
	add_msgtolist(strMsg);
	strMsg.Format("Number of tracking channels: %d", ackCmd[10]);
	add_msgtolist(strMsg);
	strMsg.Format("All: %s", ((ackCmd[11] & 0x80) ? "Enable" : "Disable"));
	add_msgtolist(strMsg);	
	if(!(ackCmd[11] & 0x80))
	{
		strMsg.Format("WAAS: %s (%s)", (ackCmd[11] & 0x01) ? "Enable" : "Disable",
      (ackCmd[12] & 0x01) ? "Modify default PRN" : "Default search PRN");
		add_msgtolist(strMsg);
		if(ackCmd[12] & 0x01)
    {
      CString txt;
      const int MaxTableSize = 3;
      strMsg.Format("WAAS search PRN (%d): ", ackCmd[13]);
      for(int i = 0; i < MaxTableSize && i < ackCmd[13]; ++i)
      {
        if(ackCmd[14 + i] != 0)
        {
          txt.Format("%d", ackCmd[14 + i]);
          if(i != 0)
          {
            strMsg += ", ";
          }
          strMsg += txt;
        }
      }
		  add_msgtolist(strMsg);
    }

		strMsg.Format("EGNOS: %s(%s)", ((ackCmd[11] & 0x02) ? "Enable" : "Disable"),
      (ackCmd[12] & 0x02) ? "Modify default PRN" : "Default search PRN");
		add_msgtolist(strMsg);
		if(ackCmd[12] & 0x02)
    {
      CString txt;
      strMsg.Format("EGNOS search PRN (%d): ", ackCmd[17]);
      for(int i = 0; i < 3 && i < ackCmd[17]; ++i)
      {
        if(ackCmd[14 + i] != 0)
        {
          txt.Format("%d", ackCmd[18 + i]);
          if(i != 0)
          {
            strMsg += ", ";
          }
          strMsg += txt;
        }
      }
		  add_msgtolist(strMsg);
    }

		strMsg.Format("MSAS: %s (%s)", ((ackCmd[11] & 0x04) ? "Enable" : "Disable"),
      (ackCmd[12] & 0x04) ? "Modify default PRN" : "Default search PRN");
		add_msgtolist(strMsg);
		if(ackCmd[12] & 0x04)
    {
      CString txt;
      strMsg.Format("MSAS search PRN (%d): ", ackCmd[21]);
      for(int i = 0; i < 3 && i < ackCmd[21]; ++i)
      {
        if(ackCmd[14 + i] != 0)
        {
          txt.Format("%d", ackCmd[22 + i]);
          if(i != 0)
          {
            strMsg += ", ";
          }
          strMsg += txt;
        }
      }
		  add_msgtolist(strMsg);
    }
    
    strMsg.Format("GAGAN: %s (%s)", ((ackCmd[11] & 0x08) ? "Enable" : "Disable"),
      (ackCmd[12] & 0x08) ? "Modify default PRN" : "Default search PRN");
		add_msgtolist(strMsg);
		if(ackCmd[12] & 0x08)
    {
      CString txt;
      strMsg.Format("GAGAN search PRN (%d): ", ackCmd[25]);
      for(int i = 0; i < 3 && i < ackCmd[25]; ++i)
      {
        if(ackCmd[14 + i] != 0)
        {
          txt.Format("%d", ackCmd[26 + i]);
          if(i != 0)
          {
            strMsg += ", ";
          }
          strMsg += txt;
        }
      }
		  add_msgtolist(strMsg);
    }
	}
	return Ack;
}

CGPSDlg::CmdErrorCode CGPSDlg::QuerySbasDefault(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QuerySbasDefaultCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QuerySbasDefaultCmd].cmdId);
	cmd.SetU08(2, cmdTable[QuerySbasDefaultCmd].cmdSubId);

	BinaryData ackCmd;
  CmdErrorCode err = ExcuteBinaryCommand(QuerySbasDefaultCmd, &cmd, &ackCmd, (nMode == Display) ? 3000 : 1000);
	if(err != Ack)
	{
    return err;
  }

  if(Return == nMode)
	{	
    *((BinaryData*)outputData) = ackCmd;
		return err;
  }

	CString strMsg = "Query SBAS Default successfully";
	add_msgtolist(strMsg);

  CString txt;
  const int MaxTableSize = 3;
  strMsg.Format("WAAS default PRN (%d): ", ackCmd[6]);
  for(int i = 0; i < MaxTableSize; ++i)
  {
    txt.Format("%d", ackCmd[7 + i]);
    if(i != 0)
    {
      strMsg += ", ";
    }
    strMsg += txt;
  }
  add_msgtolist(strMsg);

  strMsg.Format("EGNOS default PRN (%d): ", ackCmd[10]);
  for(int i = 0; i < MaxTableSize; ++i)
  {
    txt.Format("%d", ackCmd[11 + i]);
    if(i != 0)
    {
      strMsg += ", ";
    }
    strMsg += txt;
  }
  add_msgtolist(strMsg);

  strMsg.Format("MSAS default PRN (%d): ", ackCmd[14]);
  for(int i = 0; i < MaxTableSize; ++i)
  {
    txt.Format("%d", ackCmd[15 + i]);
    if(i != 0)
    {
      strMsg += ", ";
    }
    strMsg += txt;
  }
  add_msgtolist(strMsg);

  strMsg.Format("GAGAN default PRN (%d): ", ackCmd[18]);
  for(int i = 0; i < MaxTableSize; ++i)
  {
    txt.Format("%d", ackCmd[19 + i]);
    if(i != 0)
    {
      strMsg += ", ";
    }
    strMsg += txt;
  }
  add_msgtolist(strMsg);
	return Ack;
}

CGPSDlg::CmdErrorCode CGPSDlg::QuerySagps(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QuerySagpsCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QuerySagpsCmd].cmdId);
	cmd.SetU08(2, cmdTable[QuerySagpsCmd].cmdSubId);

	BinaryData ackCmd;
	if(Ack == ExcuteBinaryCommand(QuerySagpsCmd, &cmd, &ackCmd))
	{
		if(nMode==Return)
		{
			*((U08*)outputData) = ackCmd[6];
			return Ack;
		}

		CString strMsg = "Query SAEE successfully";
		add_msgtolist(strMsg);
		strMsg = "SAEE System: ";
		if(0==(ackCmd[6]))
		{
			strMsg += "Default";
		}
		else if(1==(ackCmd[6]))
		{
			strMsg += "Enable";
		}
		else if(2==(ackCmd[6]))
		{
			strMsg += "Disable";
		}
		add_msgtolist(strMsg);
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryQzss(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QueryQzssCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryQzssCmd].cmdId);
	cmd.SetU08(2, cmdTable[QueryQzssCmd].cmdSubId);

	BinaryData ackCmd;
	if(Ack == ExcuteBinaryCommand(QueryQzssCmd, &cmd, &ackCmd))
	{
		CString strMsg = "Query QZSS successfully";
		add_msgtolist(strMsg);
		strMsg.Format("QZSS System: %s", ((ackCmd[6]) ? "Enable" : "Disable"));
		add_msgtolist(strMsg);
		strMsg.Format("Number of Tracking Channels: %d", ackCmd[7]);
		add_msgtolist(strMsg);
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryDgps(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QueryDgpsCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryDgpsCmd].cmdId);
	cmd.SetU08(2, cmdTable[QueryDgpsCmd].cmdSubId);

	BinaryData ackCmd;
	if(Ack == ExcuteBinaryCommand(QueryDgpsCmd, &cmd, &ackCmd))
	{
		CString strMsg = "Query DGPS successfully";
		add_msgtolist(strMsg);
		if(ackCmd[6] <= 1)
		{
			strMsg.Format("DGPS Status : %s", ((ackCmd[6]) ? "Enable" : "Disable"));
		}
		else
		{
			strMsg.Format("DGPS Status : %d", ackCmd[6]);
		}
		add_msgtolist(strMsg);
		strMsg.Format("Overdue seconds : %d", MAKEWORD(ackCmd[8], ackCmd[7]));
		add_msgtolist(strMsg);
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QuerySmoothMode(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QuerySmoothModeCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QuerySmoothModeCmd].cmdId);
	cmd.SetU08(2, cmdTable[QuerySmoothModeCmd].cmdSubId);

	BinaryData ackCmd;
	if(Ack == ExcuteBinaryCommand(QuerySmoothModeCmd, &cmd, &ackCmd))
	{
		CString strMsg = "Query Carrier Smooth Mode successfully";
		add_msgtolist(strMsg);
		if(ackCmd[6] <= 1)
		{
			strMsg.Format("Mode : %s", ((ackCmd[6]) ? "Enable" : "Disable"));
		}
		else
		{
			strMsg.Format("Mode : %d", ackCmd[6]);
		}
		add_msgtolist(strMsg);
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryTimeStamping(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QueryTimeStampingCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryTimeStampingCmd].cmdId);
	cmd.SetU08(2, cmdTable[QueryTimeStampingCmd].cmdSubId);

	BinaryData ackCmd;
	if(Ack == ExcuteBinaryCommand(QueryTimeStampingCmd, &cmd, &ackCmd))
	{
		CString strMsg = "Query Time Stamping successfully";
		add_msgtolist(strMsg);
		if(ackCmd[6] <= 1)
		{
			strMsg.Format("Time stamping %s", ((ackCmd[6]) ? "enable" : "disable"));
		}
		else
		{
			strMsg.Format("Mode : %d", ackCmd[6]);
		}
		add_msgtolist(strMsg);

		if(ackCmd[7] <= 1)
		{
			strMsg.Format("Tigger Mode : %s", ((ackCmd[7]) ? "Falling Edge" : "Rising Edge"));
		}
		else
		{
			strMsg.Format("Tigger Mode :  : %d", ackCmd[7]);
		}
		add_msgtolist(strMsg);

		if(ackCmd[8] <= 1)
		{
			strMsg.Format("Time Valid : %s", ((ackCmd[8]) ? "Valid" : "Invalid"));
		}
		else
		{
			strMsg.Format("Time Valid :  : %d", ackCmd[8]);
		}
		add_msgtolist(strMsg);
		
		strMsg.Format("Tigger Counter : %d", MAKEWORD(ackCmd[10], ackCmd[9]));
		add_msgtolist(strMsg);
		strMsg.Format("Week Number : %d", MAKEWORD(ackCmd[12], ackCmd[11]));
		add_msgtolist(strMsg);
		strMsg.Format("Time of Week : %d", MAKELONG(MAKEWORD(ackCmd[16], ackCmd[15]), MAKEWORD(ackCmd[14], ackCmd[13])));
		add_msgtolist(strMsg);
		strMsg.Format("Sub Time of Week : %d", MAKELONG(MAKEWORD(ackCmd[20], ackCmd[19]), MAKEWORD(ackCmd[18], ackCmd[17])));
		add_msgtolist(strMsg);
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryGpsTime(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QueryGpsTimeCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryGpsTimeCmd].cmdId);
	cmd.SetU08(2, cmdTable[QueryGpsTimeCmd].cmdSubId);

	BinaryData ackCmd;
	if(Ack == ExcuteBinaryCommand(QueryGpsTimeCmd, &cmd, &ackCmd))
	{
		CString strMsg = "Query GPS time successfully";
		add_msgtolist(strMsg);

		strMsg.Format("Time of week %d", MAKELONG(MAKEWORD(ackCmd[9], ackCmd[8]), MAKEWORD(ackCmd[7], ackCmd[6])));
		add_msgtolist(strMsg);

		strMsg.Format("Sub time of week %d", MAKELONG(MAKEWORD(ackCmd[13], ackCmd[12]), MAKEWORD(ackCmd[11], ackCmd[10])));
		add_msgtolist(strMsg);

		strMsg.Format("Week number %d", MAKEWORD(ackCmd[15], ackCmd[14]));
		add_msgtolist(strMsg);

		strMsg.Format("Default leap seconds %d", ackCmd[16]);
		add_msgtolist(strMsg);

		strMsg.Format("Current leap seconds %d", ackCmd[17]);
		add_msgtolist(strMsg);

		strMsg.Format("Valid %d", ackCmd[18]);
		add_msgtolist(strMsg);
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryNoisePowerControl(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QueryNoisePowerControlCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryNoisePowerControlCmd].cmdId);
	cmd.SetU08(2, cmdTable[QueryNoisePowerControlCmd].cmdSubId);

	BinaryData ackCmd;
	if(Ack == ExcuteBinaryCommand(QueryNoisePowerControlCmd, &cmd, &ackCmd))
	{
		CString strMsg;
		strMsg = "Query Noise Power Control successfully";
		add_msgtolist(strMsg);
		strMsg.Format("Noise Power Control: %s", ((ackCmd[6]) ? "Dynamic" : "Static"));
		add_msgtolist(strMsg);

		WORD ackSize = MAKEWORD(ackCmd[3], ackCmd[2]);
		if(ackSize == 8)
		{
			strMsg.Format("Default Noise Power Control: %d(%s)", ackCmd[7], (ackCmd[7]) ? "external" : "defalut");
			add_msgtolist(strMsg);
			U32 data = MAKELONG(MAKEWORD(ackCmd[11], ackCmd[10]), MAKEWORD(ackCmd[9], ackCmd[8]));
			strMsg.Format("External Noise Power Value: %u", data);
			add_msgtolist(strMsg);
		};
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryInterferenceDetectControl(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QueryInterferenceDetectControlCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryInterferenceDetectControlCmd].cmdId);
	cmd.SetU08(2, cmdTable[QueryInterferenceDetectControlCmd].cmdSubId);

	BinaryData ackCmd;
	if(Ack == ExcuteBinaryCommand(QueryInterferenceDetectControlCmd, &cmd, &ackCmd))
	{
		CString strMsg = "Query Interference Detect Control successfully";
		add_msgtolist(strMsg);
		strMsg.Format("Interference Control Detect: %s", ((ackCmd[6]) ? "Enable" : "Disable"));
		add_msgtolist(strMsg);
		strMsg.Format("Interference Status: ");
		if(0==ackCmd[7])
		{
			strMsg.Append("Unknown");
		}
		else if(1==ackCmd[7])
		{
			strMsg.Append("No Interference");
		}
		else if(2==ackCmd[7])
		{
			strMsg.Append("Lite");
		}
		else 
		{
			strMsg.Append("Critical");
		}
		add_msgtolist(strMsg);
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryNmeaBinaryOutputDestination(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QueryNmeaBinaryOutputDestinationCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryNmeaBinaryOutputDestinationCmd].cmdId);
	cmd.SetU08(2, cmdTable[QueryNmeaBinaryOutputDestinationCmd].cmdSubId);

	BinaryData ackCmd;
	if(Ack == ExcuteBinaryCommand(QueryNmeaBinaryOutputDestinationCmd, &cmd, &ackCmd))
	{
		CString strMsg = "Query NMEA Binary Output Dest. successfully";
		add_msgtolist(strMsg);
		if(ackCmd[7] & 0x0001)
		{
			add_msgtolist("UART Port");
		}
		if(ackCmd[7] & 0x0002)
		{
			add_msgtolist("SPI Slave Port");
		}
		if(ackCmd[7] & 0x0004)
		{
			add_msgtolist("I2C Slave Port");
		}
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryParameterSearchEngineNumber(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QueryParameterSearchEngineNumberCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryParameterSearchEngineNumberCmd].cmdId);
	cmd.SetU08(2, cmdTable[QueryParameterSearchEngineNumberCmd].cmdSubId);

	BinaryData ackCmd;
	if(Ack == ExcuteBinaryCommand(QueryParameterSearchEngineNumberCmd, &cmd, &ackCmd))
	{
		CString strMsg = "Query Param. Search Engine Num. successfully";
		add_msgtolist(strMsg);
		strMsg.Format("Search Engine Mode: ");
		if(0==ackCmd[6])
		{
			strMsg.Append("Default");
		}
		else if(1==ackCmd[6])
		{
			strMsg.Append("Low");
		}
		else if(2==ackCmd[6])
		{
			strMsg.Append("Middle");
		}
		else if(3==ackCmd[6])
		{
			strMsg.Append("High");
		}
		else if(4==ackCmd[6])
		{
			strMsg.Append("Full");
		}
		add_msgtolist(strMsg);
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryGnssKnumberSlotCnr2(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QueryGnssKnumberSlotCnr2Cmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryGnssKnumberSlotCnr2Cmd].cmdId);
	cmd.SetU08(2, cmdTable[QueryGnssKnumberSlotCnr2Cmd].cmdSubId);

	BinaryData ackCmd;
	if(Ack == ExcuteBinaryCommand(QueryGnssKnumberSlotCnr2Cmd, &cmd, &ackCmd))
	{
		CString strMsg = "Query GLONASS K-Num, Slot, CNR successfully";
		add_msgtolist(strMsg);
		for(int i=0; i<ackCmd[6]; ++i)
		{
			strMsg.Format("K-Num=%d, Slot=%d, CNR=%d", (int)(char)ackCmd[7+i*3], ackCmd[8+i*3], ackCmd[9+i*3]);
			add_msgtolist(strMsg);
		}
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryAgpsStatus(CmdExeMode nMode, void* outputData)
{
	SYSTEMTIME	now; 
	GetSystemTime(&now);

	BinaryCommand cmd(cmdTable[QueryAgpsStatusCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryAgpsStatusCmd].cmdId);
	cmd.SetU08(2, HIBYTE(now.wYear));
	cmd.SetU08(3, LOBYTE(now.wYear));
	cmd.SetU08(4, (U08)now.wMonth);
	cmd.SetU08(5, (U08)now.wDay);
	cmd.SetU08(6, (U08)now.wHour);
	cmd.SetU08(7, (U08)now.wMinute);
	cmd.SetU08(8, (U08)now.wSecond);

	BinaryData ackCmd;
	CmdErrorCode ret = ExcuteBinaryCommand(QueryAgpsStatusCmd, &cmd, &ackCmd);
	if(Return==nMode)
	{
		return ret;
	}

	if(!ret)
	{
		CString strMsg = "---------  AGPS Status  ---------";
		add_msgtolist(strMsg);
		strMsg.Format("Aiding days: %d days %d hours", MAKEWORD(ackCmd[5], ackCmd[6]) / 24, MAKEWORD(ackCmd[5], ackCmd[6]) % 24);
		add_msgtolist(strMsg);
		strMsg.Format("AGPS: %s", (ackCmd[7]) ? "Enable" : "Disable");
		add_msgtolist(strMsg);
	}
	return ret;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryGnssNmeaTalkId(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QueryGnssNmeaTalkIdCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryGnssNmeaTalkIdCmd].cmdId);

	BinaryData ackCmd;
	if(Ack == ExcuteBinaryCommand(QueryGnssNmeaTalkIdCmd, &cmd, &ackCmd))
	{
		CString strMsg = "Query Gnss NMEA Talk ID successfully";
		add_msgtolist(strMsg);
		strMsg = "NMEA Talk ID: ";

		if(ackCmd[5]==0)
		{
			strMsg.Append("GP Mode");
		}
		else if(ackCmd[5]==1)
		{
			strMsg.Append("GN Mode");
		}
		add_msgtolist(strMsg);
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryDatalogLogStatus(CmdExeMode nMode, void* outputData)
{
	SYSTEMTIME	now; 
	GetSystemTime(&now);

	BinaryCommand cmd(cmdTable[QueryDatalogLogStatusCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryDatalogLogStatusCmd].cmdId);

	BinaryData ackCmd;
	CmdErrorCode ret = ExcuteBinaryCommand(QueryDatalogLogStatusCmd, &cmd, &ackCmd);
	if(Return==nMode)
	{
		return ret;
	}

	if(!ret)
	{
		LogFlashInfo1 logInfo = {0};
//		memcpy(&logInfo.log_flash_current_prt, &ackCmd[5], sizeof(U32));
		logInfo.log_flash_current_prt = MAKELONG(MAKEWORD(ackCmd[5], ackCmd[6]), MAKEWORD(ackCmd[7], ackCmd[8]));
//		memcpy(&logInfo.sector_left, &ackCmd[9], sizeof(U16));
		logInfo.sector_left = MAKEWORD(ackCmd[9], ackCmd[10]);
//		memcpy(&logInfo.total_sector, &ackCmd[11], sizeof(U16));
		logInfo.total_sector = MAKEWORD(ackCmd[11], ackCmd[12]);
//		memcpy(&logInfo.max_time, &ackCmd[13], sizeof(U32));
		logInfo.max_time = MAKELONG(MAKEWORD(ackCmd[13], ackCmd[14]), MAKEWORD(ackCmd[15], ackCmd[16]));
//		memcpy(&logInfo.min_time, &ackCmd[17], sizeof(U32));
		logInfo.min_time = MAKELONG(MAKEWORD(ackCmd[17], ackCmd[18]), MAKEWORD(ackCmd[19], ackCmd[20]));
//		memcpy(&logInfo.max_distance, &ackCmd[21], sizeof(U32));
		logInfo.max_distance = MAKELONG(MAKEWORD(ackCmd[21], ackCmd[22]), MAKEWORD(ackCmd[23], ackCmd[24]));
//		memcpy(&logInfo.min_distance, &ackCmd[25], sizeof(U32));
		logInfo.min_distance = MAKELONG(MAKEWORD(ackCmd[25], ackCmd[26]), MAKEWORD(ackCmd[27], ackCmd[28]));
//		memcpy(&logInfo.max_speed, &ackCmd[29], sizeof(U32));
		logInfo.max_speed = MAKELONG(MAKEWORD(ackCmd[29], ackCmd[30]), MAKEWORD(ackCmd[31], ackCmd[32]));
//		memcpy(&logInfo.min_speed, &ackCmd[33], sizeof(U32));
		logInfo.min_speed = MAKELONG(MAKEWORD(ackCmd[33], ackCmd[34]), MAKEWORD(ackCmd[35], ackCmd[36]));
//		memcpy(&logInfo.datalog_enable, &ackCmd[37], sizeof(U08));
		logInfo.datalog_enable = ackCmd[37];
//		memcpy(&logInfo.fifo_mode, &ackCmd[38], sizeof(U08));
		logInfo.fifo_mode = ackCmd[38];
#if DATA_POI
//		memcpy(&logInfo.poi_entry, &ackCmd[39], sizeof(U32));
		logInfo.poi_entry = MAKELONG(MAKEWORD(ackCmd[39], ackCmd[40]), MAKEWORD(ackCmd[41], ackCmd[42]));
//		memcpy(&logInfo.autolog_full, &ackCmd[43], sizeof(U08));
		logInfo.autolog_full = ackCmd[43];
//		memcpy(&logInfo.poi_full, &ackCmd[44], sizeof(U08));
		logInfo.poi_full = ackCmd[44];
#endif

		add_msgtolist("Get Log Status successfully");
		add_msgtolist("---------  Log Status  ---------");
		CString strMsg;
		if(logInfo.total_sector)
		{
			if(logInfo.sector_left == 0x0)
			{
				strMsg = "Sector Full!";
			}
			else if((logInfo.sector_left &0x80000000) != 0)
			{
				logInfo.sector_left = logInfo.sector_left << 1 >> 1;
				strMsg.Format("Circular Sector left: %d / %d", logInfo.sector_left, logInfo.total_sector);
			}
			else if((logInfo.sector_left &0x80000000) == 0)
			{
				logInfo.sector_left = logInfo.sector_left << 1 >> 1;
				strMsg.Format("Sector left: %d / %d", logInfo.sector_left, logInfo.total_sector);
			}
		}
		else
		{
			if(logInfo.sector_left == 0x0)
			{
				strMsg = "Sector Full!";
			}
			else if((logInfo.sector_left &0x80000000) != 0)
			{
				logInfo.sector_left = logInfo.sector_left << 1 >> 1;
				strMsg.Format("Circular Sector left: %d", logInfo.sector_left);
			}
			else if((logInfo.sector_left &0x80000000) == 0)
			{
				logInfo.sector_left = logInfo.sector_left << 1 >> 1;
				strMsg.Format("Sector left: %d", logInfo.sector_left);
			}
		}
		add_msgtolist(strMsg);
		strMsg.Format("max T: %d, min T: %d", logInfo.max_time, logInfo.min_time);
		add_msgtolist(strMsg);
		strMsg.Format("max D: %d, min D: %d", logInfo.max_distance, logInfo.min_distance);
		add_msgtolist(strMsg);
		strMsg.Format("max V: %d, min V: %d", logInfo.max_speed, logInfo.min_speed);
		add_msgtolist(strMsg);
		strMsg.Format("Datalog: %s", (logInfo.datalog_enable) ? "Enable" : "Disable");
		add_msgtolist(strMsg);
		strMsg.Format("FIFO mode: %s", (logInfo.fifo_mode) ? "Circular" : "Oneway");
		
		if(DATA_POI && TWIN_DATALOG)
		{
			strMsg.Format("POI entry Datalogger1 : %d", logInfo.poi_entry & 0xffff);
			add_msgtolist(strMsg);
			strMsg.Format("POI entry Datalogger2 : %d", logInfo.poi_entry >> 16);
			add_msgtolist(strMsg);
			strMsg.Format("Autolog full : %d", logInfo.autolog_full);
			add_msgtolist(strMsg);
			strMsg.Format("POIlog full : %d", logInfo.poi_full);
			add_msgtolist(strMsg);
		}
		else if(DATA_POI && !TWIN_DATALOG)
		{
			strMsg.Format("POI entry Datalogger1 : %d", logInfo.poi_entry);
			add_msgtolist(strMsg);
			strMsg.Format("Autolog full : %d", logInfo.autolog_full);
			add_msgtolist(strMsg);
			strMsg.Format("POIlog full : %d", logInfo.poi_full);
			add_msgtolist(strMsg);
		}
	}
	return ret;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryRegister(CmdExeMode nMode, void* outputData)
{	    
	BinaryCommand cmd(cmdTable[QueryRegisterCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryRegisterCmd].cmdId);
	cmd.SetU08(2, HIBYTE(HIWORD(m_regAddress)));
	cmd.SetU08(3, LOBYTE(HIWORD(m_regAddress)));
	cmd.SetU08(4, HIBYTE(LOWORD(m_regAddress)));
	cmd.SetU08(5, LOBYTE(LOWORD(m_regAddress)));

	BinaryData ackCmd;
	if(Ack == ExcuteBinaryCommand(QueryRegisterCmd, &cmd, &ackCmd))
	{
		U32 data = MAKELONG(MAKEWORD(ackCmd[8], ackCmd[7]), MAKEWORD(ackCmd[6], ackCmd[5]));
		if(nMode==Return)
		{
			*((U32*)outputData) = data;
			return Ack;
		} 
		CString strMsg;
		strMsg.Format("Get Register in 0x%08X", m_regAddress);
		add_msgtolist(strMsg);
		strMsg.Format("0x%08X (%d)", data, data);
		add_msgtolist(strMsg);
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryRegister16(CmdExeMode nMode, void* outputData)
{	    
	BinaryCommand cmd(cmdTable[QueryRegisterCmd16].cmdSize);
	cmd.SetU08(1, cmdTable[QueryRegisterCmd16].cmdId);
	cmd.SetU08(2, HIBYTE(HIWORD(m_regAddress)));
	cmd.SetU08(3, LOBYTE(HIWORD(m_regAddress)));
	cmd.SetU08(4, HIBYTE(LOWORD(m_regAddress)));
	cmd.SetU08(5, LOBYTE(LOWORD(m_regAddress)));

	BinaryData ackCmd;
	if(Ack == ExcuteBinaryCommand(QueryRegisterCmd16, &cmd, &ackCmd))
	{
		U32 data = MAKELONG(MAKEWORD(ackCmd[8], ackCmd[7]), MAKEWORD(ackCmd[6], ackCmd[5]));
		if(nMode==Return)
		{
			*((U32*)outputData) = data;
			return Ack;
		} 
		CString strMsg;
		strMsg.Format("Get 16-IO Register in 0x%08X", m_regAddress);
		add_msgtolist(strMsg);
		strMsg.Format("0x%08X (%d)", data, data);
		add_msgtolist(strMsg);
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryGetGpsAlmanac(CmdExeMode nMode, void* outputData)
{	    
	BinaryCommand cmd(cmdTable[GetGpAlmanacCmd].cmdSize);
	cmd.SetU08(1, cmdTable[GetGpAlmanacCmd].cmdId);
	cmd.SetU08(2, *((U08*)(outputData)));

	BinaryData ackCmd;
	if(Ack == ExcuteBinaryCommand(GetGpAlmanacCmd, &cmd, &ackCmd))
	{
		CString strMsg;
    strMsg.Format("Query GPS almanac #%d successfully", *((U08*)(outputData)));
		add_msgtolist(strMsg);

    strMsg.Format("A:%f", ConvertLeonFloat(ackCmd.Ptr(8)));
		add_msgtolist(strMsg);
    strMsg.Format("Omega_0:%f", ConvertLeonFloat(ackCmd.Ptr(12)));
		add_msgtolist(strMsg);
    strMsg.Format("omega:%f", ConvertLeonFloat(ackCmd.Ptr(16)));
		add_msgtolist(strMsg);
    strMsg.Format("M0,:%f", ConvertLeonFloat(ackCmd.Ptr(20)));
		add_msgtolist(strMsg);
    strMsg.Format("e:%f", ConvertLeonFloat(ackCmd.Ptr(24)));
		add_msgtolist(strMsg);
    strMsg.Format("i0:%f", ConvertLeonFloat(ackCmd.Ptr(28)));
		add_msgtolist(strMsg);
    strMsg.Format("Omega_dot:%f", ConvertLeonFloat(ackCmd.Ptr(32)));
		add_msgtolist(strMsg);
    strMsg.Format("af0:%f", ConvertLeonFloat(ackCmd.Ptr(36)));
		add_msgtolist(strMsg);
    strMsg.Format("af1:%f", ConvertLeonFloat(ackCmd.Ptr(40)));
		add_msgtolist(strMsg);
    strMsg.Format("toa:%d", (S32)MAKELONG(MAKEWORD(ackCmd[47], ackCmd[46]), MAKEWORD(ackCmd[45], ackCmd[44])));
		add_msgtolist(strMsg);
    strMsg.Format("prn:%d", (S16)MAKEWORD(ackCmd[49], ackCmd[48]));
		add_msgtolist(strMsg);
    strMsg.Format("wna:%d", (S16)MAKEWORD(ackCmd[51], ackCmd[50]));
		add_msgtolist(strMsg);
    strMsg.Format("valid:%d", (S16)MAKEWORD(ackCmd[53], ackCmd[52]));
		add_msgtolist(strMsg);
    strMsg.Format("health:%d", ackCmd[54]);
		add_msgtolist(strMsg);
  }
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryGetBeidouAlmanac(CmdExeMode nMode, void* outputData)
{	    
	BinaryCommand cmd(cmdTable[GetBdAlmanacCmd].cmdSize);
	cmd.SetU08(1, cmdTable[GetBdAlmanacCmd].cmdId);
	cmd.SetU08(2, cmdTable[GetBdAlmanacCmd].cmdSubId);
	cmd.SetU08(3, *((U08*)(outputData)));

	BinaryData ackCmd;
	if(Ack == ExcuteBinaryCommand(GetBdAlmanacCmd, &cmd, &ackCmd))
	{
		CString strMsg;
    strMsg.Format("Query Beidou almanac #%d successfully", *((U08*)(outputData)));
		add_msgtolist(strMsg);

    strMsg.Format("A:%f", ConvertLeonFloat(ackCmd.Ptr(9)));
		add_msgtolist(strMsg);
    strMsg.Format("Omega_0:%f", ConvertLeonFloat(ackCmd.Ptr(13)));
		add_msgtolist(strMsg);
    strMsg.Format("omega:%f", ConvertLeonFloat(ackCmd.Ptr(17)));
		add_msgtolist(strMsg);
    strMsg.Format("M0,:%f", ConvertLeonFloat(ackCmd.Ptr(21)));
		add_msgtolist(strMsg);
    strMsg.Format("e:%f", ConvertLeonFloat(ackCmd.Ptr(25)));
		add_msgtolist(strMsg);
    strMsg.Format("i0:%f", ConvertLeonFloat(ackCmd.Ptr(29)));
		add_msgtolist(strMsg);
    strMsg.Format("Omega_dot:%f", ConvertLeonFloat(ackCmd.Ptr(33)));
		add_msgtolist(strMsg);
    strMsg.Format("af0:%f", ConvertLeonFloat(ackCmd.Ptr(37)));
		add_msgtolist(strMsg);
    strMsg.Format("af1:%f", ConvertLeonFloat(ackCmd.Ptr(41)));
		add_msgtolist(strMsg);
    strMsg.Format("toa:%d", (S32)MAKELONG(MAKEWORD(ackCmd[48], ackCmd[47]), MAKEWORD(ackCmd[46], ackCmd[45])));
		add_msgtolist(strMsg);
    strMsg.Format("prn:%d", (S16)MAKEWORD(ackCmd[50], ackCmd[49]));
		add_msgtolist(strMsg);
    strMsg.Format("wna:%d", (S16)MAKEWORD(ackCmd[52], ackCmd[51]));
		add_msgtolist(strMsg);
    strMsg.Format("valid:%d", (S16)MAKEWORD(ackCmd[54], ackCmd[53]));
		add_msgtolist(strMsg);
    strMsg.Format("health:%d", ackCmd[55]);
		add_msgtolist(strMsg);
  }
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryPositionFixNavigationMask(CmdExeMode nMode, void* outputData)
{	    
	BinaryCommand cmd(cmdTable[QueryPositionFixNavigationMaskCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryPositionFixNavigationMaskCmd].cmdId);
	cmd.SetU08(2, cmdTable[QueryPositionFixNavigationMaskCmd].cmdSubId);

	BinaryData ackCmd;
	if(Ack == ExcuteBinaryCommand(QueryPositionFixNavigationMaskCmd, &cmd, &ackCmd))
	{
		CString strMsg = "Query Position Fix Navigation Mask successfully";
		add_msgtolist(strMsg);
		strMsg.Format("First fix navigation mask : %s", (ackCmd[6]) ? "2D" : "3D");
		add_msgtolist(strMsg);
		strMsg.Format("Subsequent fix navigation mask : %s", (ackCmd[7]) ? "2D" : "3D");
		add_msgtolist(strMsg);
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryNavigationModeV8(CmdExeMode nMode, void* outputData)
{	    
	BinaryCommand cmd(cmdTable[QueryNavigationModeV8Cmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryNavigationModeV8Cmd].cmdId);
	cmd.SetU08(2, cmdTable[QueryNavigationModeV8Cmd].cmdSubId);

	BinaryData ackCmd;
	if(Ack == ExcuteBinaryCommand(QueryNavigationModeV8Cmd, &cmd, &ackCmd))
	{
		CString strMsg = "Query Navigation Mode successfully";
		add_msgtolist(strMsg);
		switch(ackCmd[6])
		{
		case 0:
			strMsg.SetString("Navigation Mode: Auto");
			break;
		case 1:
			strMsg.SetString("Navigation Mode: Pedestrian");
			break;
		case 2:
			strMsg.SetString("Navigation Mode: Car");
			break;
		case 3:
			strMsg.SetString("Navigation Mode: Marine");
			break;
		case 4:
			strMsg.SetString("Navigation Mode: Balloon");
			break;
		case 5:
			strMsg.SetString("Navigation Mode: Airborne");
			break;
		case 6:
			strMsg.SetString("Navigation Mode: Surveying and mapping");
			break;
		case 7:
			strMsg.SetString("Navigation Mode: Quadcopter");
			break;
		default:
			strMsg.Format("Navigation Mode: %d", ackCmd[6]);
			break;
		}
		add_msgtolist(strMsg);
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryGnssBootStatus(CmdExeMode nMode, void* outputData)
{	    
	BinaryCommand cmd(cmdTable[QueryGnssBootStatusCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryGnssBootStatusCmd].cmdId);
	cmd.SetU08(2, cmdTable[QueryGnssBootStatusCmd].cmdSubId);
	
	if(NoWait==nMode)
	{
		ExcuteBinaryCommandNoWait(QueryGnssBootStatusCmd, &cmd);
		return Timeout;
	}
	BinaryData ackCmd;
	if(Ack == ExcuteBinaryCommand(QueryGnssBootStatusCmd, &cmd, &ackCmd))
	{
		m_bootInfo = ackCmd;

		if(nMode == Return)
		{
			*((U32*)outputData) = MAKEWORD(ackCmd[6], ackCmd[7]);
			return Ack;
		}

		CString strMsg = "Get GNSS ROM Boot Status successfully";
		add_msgtolist(strMsg);
		switch(ackCmd[6])
		{
		case 0:
			strMsg.SetString("Status : Boot flash OK");
			break;
		case 1:
			strMsg.SetString("Status : Fail over to boot from ROM");
			break;
		default:
			strMsg.Format("Status : %d", ackCmd[6]);
			break;
		}
		add_msgtolist(strMsg);
		switch(ackCmd[7])
		{
		case 0:
			strMsg.SetString("Flash Type : ROM");
			break;
		case 1:
			strMsg.SetString("Flash Type : QSPI Flash Type 1");		//Winbond-type 
			break;
		case 2:
			strMsg.SetString("Flash Type : QSPI Flash Type 2");		// EON-type 
			break;
		case 4:
			strMsg.SetString("Flash Type : Parallel Flash");
			break;
		default:
			strMsg.Format("Flash Type : %d", ackCmd[7]);
			break;
		}
		add_msgtolist(strMsg);	
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryDrMultiHz(CmdExeMode nMode, void* outputData)
{	    
	BinaryCommand cmd(cmdTable[QueryDrMultiHzCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryDrMultiHzCmd].cmdId);
	cmd.SetU08(2, cmdTable[QueryDrMultiHzCmd].cmdSubId);

	BinaryData ackCmd;
	if(Ack == ExcuteBinaryCommand(QueryDrMultiHzCmd, &cmd, &ackCmd))
	{
		CString strMsg = "Get DR Multi-Hz successfully";
		strMsg.Format("DR Update Rate = %d Hz", ackCmd[6]);
		add_msgtolist(strMsg);
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryNmeaIntervalV8(CmdExeMode nMode, void* outputData)
{	    
	BinaryCommand cmd(cmdTable[QueryNmeaIntervalV8Cmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryNmeaIntervalV8Cmd].cmdId);
	cmd.SetU08(2, cmdTable[QueryNmeaIntervalV8Cmd].cmdSubId);

	BinaryData ackCmd;
	if(Ack == ExcuteBinaryCommand(QueryNmeaIntervalV8Cmd, &cmd, &ackCmd))
	{
		CString strMsg = "Query NMEA Message Interval successfully";
		add_msgtolist(strMsg);
		if(ackCmd[6])
		{
			strMsg.Format("GGA Interval : %d second(s)", ackCmd[6]);
			add_msgtolist(strMsg);
		}
		if(ackCmd[7])
		{
			strMsg.Format("GSA Interval : %d second(s)", ackCmd[7]);
			add_msgtolist(strMsg);
		}
		if(ackCmd[8])
		{
			strMsg.Format("GSV Interval : %d second(s)", ackCmd[8]);
			add_msgtolist(strMsg);
		}
		if(ackCmd[9])
		{
			strMsg.Format("GLL Interval : %d second(s)", ackCmd[9]);
			add_msgtolist(strMsg);
		}
		if(ackCmd[10])
		{
			strMsg.Format("RMC Interval : %d second(s)", ackCmd[10]);
			add_msgtolist(strMsg);
		}
		if(ackCmd[11])
		{
			strMsg.Format("VTG Interval : %d second(s)", ackCmd[11]);
			add_msgtolist(strMsg);
		}
		if(ackCmd[12])
		{
			strMsg.Format("ZDA Interval : %d second(s)", ackCmd[12]);
			add_msgtolist(strMsg);
		}
		if(ackCmd[13])
		{
			strMsg.Format("GNS Interval : %d second(s)", ackCmd[13]);
			add_msgtolist(strMsg);
		}
		if(ackCmd[14])
		{
			strMsg.Format("GBS Interval : %d second(s)", ackCmd[14]);
			add_msgtolist(strMsg);
		}
		if(ackCmd[15])
		{
			strMsg.Format("GRS Interval : %d second(s)", ackCmd[15]);
			add_msgtolist(strMsg);
		}
		if(ackCmd[16])
		{
			strMsg.Format("DTM Interval : %d second(s)", ackCmd[16]);
			add_msgtolist(strMsg);
		}
		if(ackCmd[17])
		{
			strMsg.Format("GST Interval : %d second(s)", ackCmd[17]);
			add_msgtolist(strMsg);
		}
		if(ackCmd[6]==0 && ackCmd[7]==0 && ackCmd[8]==0 && ackCmd[9]==0 && ackCmd[10]==0 && 
			ackCmd[11]==0 && ackCmd[12]==0 && ackCmd[13]==0 && ackCmd[14]==0 && ackCmd[15]==0 && 
			ackCmd[16]==0 && ackCmd[17]==0)
		{
			strMsg.Format("All message intervals are zero");
			add_msgtolist(strMsg);
		}
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryNmeaInterval2V8(CmdExeMode nMode, void* outputData)
{	    
	BinaryCommand cmd(cmdTable[QueryNmeaInterval2V8Cmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryNmeaInterval2V8Cmd].cmdId);
	cmd.SetU08(2, cmdTable[QueryNmeaInterval2V8Cmd].cmdSubId);
	cmd.SetU08(3, 0x02);

	BinaryData ackCmd;
	if(Ack == ExcuteBinaryCommand(QueryNmeaInterval2V8Cmd, &cmd, &ackCmd))
	{
		CString strMsg = "Query NMEA Message Interval successfully";
		add_msgtolist(strMsg);
		if(ackCmd[7])
		{
			strMsg.Format("GGA Interval : %d second(s)", ackCmd[7]);
			add_msgtolist(strMsg);
		}
		if(ackCmd[8])
		{
			strMsg.Format("GSA Interval : %d second(s)", ackCmd[8]);
			add_msgtolist(strMsg);
		}
		if(ackCmd[9])
		{
			strMsg.Format("GSV Interval : %d second(s)", ackCmd[9]);
			add_msgtolist(strMsg);
		}
		if(ackCmd[10])
		{
			strMsg.Format("GPtps Interval : %d second(s)", ackCmd[10]);
			add_msgtolist(strMsg);
		}
		if(ackCmd[11])
		{
			strMsg.Format("GPanc Interval : %d second(s)", ackCmd[11]);
			add_msgtolist(strMsg);
		}
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryEricssonInterval(CmdExeMode nMode, void* outputData)
{	    
	BinaryCommand cmd(cmdTable[QueryEricssonIntervalCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryEricssonIntervalCmd].cmdId);
	cmd.SetU08(2, cmdTable[QueryEricssonIntervalCmd].cmdSubId);
	cmd.SetU08(3, 0x02);

	BinaryData ackCmd;
	if(Ack == ExcuteBinaryCommand(QueryEricssonIntervalCmd, &cmd, &ackCmd))
	{
		CString strMsg = "Query Ericsson Sentence Interval successfully";
		add_msgtolist(strMsg);
		if(ackCmd[7])
		{
			strMsg.Format("GPppr Interval : %d second(s)", ackCmd[7]);
			add_msgtolist(strMsg);
		}
		if(ackCmd[8])
		{
			strMsg.Format("GPsts Interval : %d second(s)", ackCmd[8]);
			add_msgtolist(strMsg);
		}
		if(ackCmd[9])
		{
			strMsg.Format("GPver Interval : %d second(s)", ackCmd[9]);
			add_msgtolist(strMsg);
		}
		if(ackCmd[10])
		{
			strMsg.Format("GPavp Interval : %d second(s)", ackCmd[10]);
			add_msgtolist(strMsg);
		}
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QuerySerialNumber(CmdExeMode nMode, void* outputData)
{	    
	BinaryCommand cmd(cmdTable[QuerySerialNumberCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QuerySerialNumberCmd].cmdId);
	cmd.SetU08(2, cmdTable[QuerySerialNumberCmd].cmdSubId);
	cmd.SetU08(3, 0x02);

	BinaryData ackCmd;
	if(Ack == ExcuteBinaryCommand(QuerySerialNumberCmd, &cmd, &ackCmd))
	{
		CString strMsg = "Query Serial Number successfully";
		add_msgtolist(strMsg);
		if(ackCmd[7] > 0)	//Has Serial Number
		{
			strMsg.Format("Serial Number:");
			add_msgtolist(strMsg);

			char* p = strMsg.GetBuffer(ackCmd[7] + 1);
			memcpy(p, ackCmd.GetBuffer(8), ackCmd[7]);
			p[ackCmd[7]] = 0;
			strMsg.ReleaseBuffer();
			add_msgtolist(strMsg);
		}
		else	//No Serial Number
		{
			strMsg.Format("No valid serial number.");
			add_msgtolist(strMsg);
		}
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryGeofenceResult(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QueryGeofenceResultCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryGeofenceResultCmd].cmdId);
	cmd.SetU08(2, cmdTable[QueryGeofenceResultCmd].cmdSubId);

	BinaryData ackCmd;
	if(Ack == ExcuteBinaryCommand(QueryGeofenceResultCmd, &cmd, &ackCmd))
	{
		CString strMsg;
		strMsg = "Query geofencing result successfully";
		add_msgtolist(strMsg);

		const U08* ptr = ackCmd.Ptr(7);
		double d1 = ConvertLeonDouble(ptr);
		ptr = ackCmd.Ptr(15);
		double d2 = ConvertLeonDouble(ptr);
		if(INVERT_LON_LAT)
		{
			strMsg.Format("Geofencing in %12.9lf, %12.9lf", d1, d2);
		}
		else
		{
			strMsg.Format("Geofencing in %12.9lf, %12.9lf", d2, d1);
		}
		add_msgtolist(strMsg);

		strMsg.Format("Geofencing result : %d", ackCmd[6]);
		add_msgtolist(strMsg);
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryGeofenceResultEx(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QueryGeofenceResultCmdEx].cmdSize);
	cmd.SetU08(1, cmdTable[QueryGeofenceResultCmdEx].cmdId);
	cmd.SetU08(2, cmdTable[QueryGeofenceResultCmdEx].cmdSubId);

	BinaryData ackCmd;
	if(Ack == ExcuteBinaryCommand(QueryGeofenceResultCmdEx, &cmd, &ackCmd))
	{
		CString strMsg;
		strMsg = "Query geofencing result successfully";
		add_msgtolist(strMsg);

		const U08* ptr = ackCmd.Ptr(10);
		double d1 = ConvertLeonDouble(ptr);
		ptr = ackCmd.Ptr(18);
		double d2 = ConvertLeonDouble(ptr);
		if(INVERT_LON_LAT)
		{
			strMsg.Format("Geofencing in %12.9lf, %12.9lf", d1, d2);
		}
		else
		{
			strMsg.Format("Geofencing in %12.9lf, %12.9lf", d2, d1);
		}
		add_msgtolist(strMsg);

		strMsg.Format("Geofencing result NO.1 : %d", ackCmd[6]);
		add_msgtolist(strMsg);
		strMsg.Format("Geofencing result NO.2 : %d", ackCmd[7]);
		add_msgtolist(strMsg);
		strMsg.Format("Geofencing result NO.3 : %d", ackCmd[8]);
		add_msgtolist(strMsg);
		strMsg.Format("Geofencing result NO.4 : %d", ackCmd[9]);
		add_msgtolist(strMsg);
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryGeofenceEx(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QueryGeofenceCmdEx].cmdSize);
	cmd.SetU08(1, cmdTable[QueryGeofenceCmdEx].cmdId);
	cmd.SetU08(2, cmdTable[QueryGeofenceCmdEx].cmdSubId);
	cmd.SetU08(3, m_nGeofecingNo);

	BinaryData ackCmd;
  CmdErrorCode err = ExcuteBinaryCommand(QueryGeofenceCmdEx, &cmd, &ackCmd, 800);
	if(err != Ack)
	{
		return err;
	}

	U08 size = ackCmd[7];
  CStringArray* points = (CStringArray*)outputData;

	CString strMsg;
	strMsg.Format("Query geofencing data %d successfully", m_nGeofecingNo);
	add_msgtolist(strMsg);
	
	if(size == 0)
	{
		strMsg.Format("No geofencing data");
		add_msgtolist(strMsg);
	}

	const U08* ptr = ackCmd.Ptr(8);
	for(int i = 0; i < size; ++i)
	{
		U08 temp1[8] = {0};
		U08 temp2[8] = {0};
		for(int j = 0; j < 8; ++j)
		{
			temp1[7 - j] = *ptr++;
		}
		for(int j = 0; j < 8; ++j)
		{
			temp2[7 - j] = *ptr++;
		}
		if(INVERT_LON_LAT)
		{
			strMsg.Format("%12.9lf, %12.9lf", *((double*)temp1), *((double*)temp2));
		}
		else
		{
			strMsg.Format("%12.9lf, %12.9lf", *((double*)temp2), *((double*)temp1));
		}
    if(Return == nMode)
    {
      points->Add(strMsg);
    }
    else
    {
		  add_msgtolist(strMsg);
    }
	}
  return err;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryRtkMode(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QueryRtkModeCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryRtkModeCmd].cmdId);
	cmd.SetU08(2, cmdTable[QueryRtkModeCmd].cmdSubId);

	BinaryData ackCmd;
	if(Ack == ExcuteBinaryCommand(QueryRtkModeCmd, &cmd, &ackCmd))
	{
		CString strMsg;
		strMsg = "Query RTK mode successfully";
		add_msgtolist(strMsg);

		if(ackCmd[6] == 0)
		{
			strMsg.Format("RTK rover mode");
		}
		else if(ackCmd[6] == 1)
		{
			strMsg.Format("RTK base mode");
		}
    else if(ackCmd[6] == 2)
		{
			strMsg.Format("RTK precisely kinematic base mode");
		}
		add_msgtolist(strMsg);
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryPstmDeviceAddress(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QueryPstmDeviceAddressCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryPstmDeviceAddressCmd].cmdId);
	cmd.SetU08(2, cmdTable[QueryPstmDeviceAddressCmd].cmdSubId);
	cmd.SetU08(3, 0x02);

	BinaryData ackCmd;
	if(Ack == ExcuteBinaryCommand(QueryPstmDeviceAddressCmd, &cmd, &ackCmd))
	{
		CString strMsg;
		strMsg = "Query PSTM device address successfully";
		add_msgtolist(strMsg);
		strMsg.Format("PSCM device address : %d", ackCmd[7]);
		add_msgtolist(strMsg);
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryPstnLatLonDigits(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QueryPstnLatLonDigitsCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryPstnLatLonDigitsCmd].cmdId);
	cmd.SetU08(2, cmdTable[QueryPstnLatLonDigitsCmd].cmdSubId);
	cmd.SetU08(3, 0x04);

	BinaryData ackCmd;
	if(Ack == ExcuteBinaryCommand(QueryPstnLatLonDigitsCmd, &cmd, &ackCmd))
	{
		CString strMsg;
		strMsg = "Query PSTM LAT/LON fractional digits successfully";
		add_msgtolist(strMsg);
		strMsg.Format("LAT/LON Fractional Digits : %d", ackCmd[7]);
		add_msgtolist(strMsg);
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryPsti030(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QueryPstiCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryPstiCmd].cmdId);
	cmd.SetU08(2, cmdTable[QueryPstiCmd].cmdSubId);
	cmd.SetU08(3, 30);

	BinaryData ackCmd;
  CmdErrorCode err = ExcuteBinaryCommand(QueryPstiCmd, &cmd, &ackCmd, 800);
	if(err != Ack)
	{
		return err;
	}

	if(Return == nMode)
	{	//Return command length
		*((U08*)outputData) = ackCmd[6];
		return err;
	}

	CString strMsg;
	strMsg = "Query PSTI030 Interval successfully";
	add_msgtolist(strMsg);
	strMsg.Format("PSTI030 Interval : %d", ackCmd[6]);
	add_msgtolist(strMsg);

	return err;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryPsti032(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QueryPstiCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryPstiCmd].cmdId);
	cmd.SetU08(2, cmdTable[QueryPstiCmd].cmdSubId);
	cmd.SetU08(3, 32);

	BinaryData ackCmd;
  CmdErrorCode err = ExcuteBinaryCommand(QueryPstiCmd, &cmd, &ackCmd, 800);
	if(err != Ack)
	{
		return err;
	}

	if(Return == nMode)
	{	//Return command length
		*((U08*)outputData) = ackCmd[6];
		return err;
	}

	CString strMsg;
	strMsg = "Query PSTI032 Interval successfully";
	add_msgtolist(strMsg);
	strMsg.Format("PSTI032 Interval : %d", ackCmd[6]);
	add_msgtolist(strMsg);

	return err;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryPsti004(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QueryPstiCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryPstiCmd].cmdId);
	cmd.SetU08(2, cmdTable[QueryPstiCmd].cmdSubId);
	cmd.SetU08(3, 4);

	BinaryData ackCmd;
  CmdErrorCode err = ExcuteBinaryCommand(QueryPstiCmd, &cmd, &ackCmd, 800);
	if(err != Ack)
	{
		return err;
	}

	if(Return == nMode)
	{	//Return command length
		*((U08*)outputData) = ackCmd[6];
		return err;
	}

	CString strMsg;
	strMsg = "Query PSTI004 Interval successfully";
	add_msgtolist(strMsg);
	strMsg.Format("PSTI004 Interval : %d", ackCmd[6]);
	add_msgtolist(strMsg);

	return err;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryRtkMode2(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QueryRtkModeCmd2].cmdSize);
	cmd.SetU08(1, cmdTable[QueryRtkModeCmd2].cmdId);
	cmd.SetU08(2, cmdTable[QueryRtkModeCmd2].cmdSubId);

	BinaryData ackCmd;
	CmdErrorCode err = ExcuteBinaryCommand(QueryRtkModeCmd2, &cmd, &ackCmd, (nMode == Display) ? 3000 : 1000);
	if(err != Ack)
	{
		return err;
	}

	if(Return == nMode)
	{	//Return command data
    *((BinaryData*)outputData) = ackCmd;
		return err;
	}

	CString strMsg;
	strMsg = "Query RTK mode successfully";
	add_msgtolist(strMsg);

	U16 cmdLen = ConvertLeonU16(ackCmd.Ptr(2));

	UINT8 rtkMode = ackCmd[6];
	if(ackCmd[6] == 0)
	{
		strMsg.Format("RTK rover mode");
	}
	else if(ackCmd[6] == 1)
	{
		strMsg.Format("RTK base mode");
	}
  else if(ackCmd[6] == 2)
	{
		strMsg.Format("RTK precisely kinematic base mode");
	}
	add_msgtolist(strMsg);

	UINT8 rtkOpr = ackCmd[7];
	if(rtkMode == 1)	//base mode
	{
	  strMsg.Format("Saved Operational Function: ");
		switch(rtkOpr)
		{
		case 0:
			strMsg += "Kinematic Mode";
			break;
		case 1:
			strMsg += "Survey Mode";
			break;
		case 2:
			strMsg += "Static Mode";
			break;
    default:
      strMsg.Format("Operational Function: %d", rtkOpr);
      break;
		}
	}
	else if(rtkMode == 0)	//rover mode
	{
	  strMsg.Format("Operational Function: ");
		switch(rtkOpr)
		{
		case 0:
			strMsg += "Normal";
			break;
		case 1:
			strMsg += "Float";
			break;
		case 2:
			strMsg += "Moving base";
			if(cmdLen == 41)
			{	//The length in old style is 37, new is 41. 20160422 changed by Ryan
				add_msgtolist(strMsg);
				strMsg.Format("Baseline length:%f", ConvertLeonFloat(ackCmd.Ptr(36)));
			}
			break;
    default:
      strMsg.Format("Operational Function: %d", rtkOpr);
      break;
		}		
	}
  else if(rtkMode == 2)	//RTK precisely kinematic base mode
  {
    switch(rtkOpr)
    {
    case 0:
      strMsg.Format("Operational Function: Normal");
      break;
    case 1:
      strMsg.Format("Operational Function: Float");
      break;
    default:
      strMsg.Format("Operational Function: %d", rtkOpr);
      break;
    }
  }
	add_msgtolist(strMsg);
	
	if(rtkMode == 0 || rtkMode == 2 )	//rover mode or precisely kinematic base mode
	{
		return Ack;
	}

	strMsg.Format("Run-time Timing Mode: ");
	switch(rtkOpr)
	{
	case 0:
		break;
	case 1:
		strMsg.Format("Saved Survey Length:%u", MAKELONG(MAKEWORD(ackCmd[11], ackCmd[10]), MAKEWORD(ackCmd[9], ackCmd[8])));
		add_msgtolist(strMsg);
		strMsg.Format("Standard Deviation:%u", MAKELONG(MAKEWORD(ackCmd[15], ackCmd[14]), MAKEWORD(ackCmd[13], ackCmd[12])));
		add_msgtolist(strMsg);
		break;
	case 2:
		strMsg.Format("Saved Latitude:%f", ConvertLeonDouble(ackCmd.Ptr(16)));
		add_msgtolist(strMsg);
		strMsg.Format("Saved Longitude:%f", ConvertLeonDouble(ackCmd.Ptr(24)));
		add_msgtolist(strMsg);
		strMsg.Format("Saved Ellipsoidal Height:%f", ConvertLeonFloat(ackCmd.Ptr(32)));
		add_msgtolist(strMsg);
		break;
	}	

	UINT8 timingMode = ackCmd[36];
  strMsg.Format("Run-time Operational Function: ");
	switch(timingMode)
	{
	case 0:
		strMsg += "Kinematic Mode";
		add_msgtolist(strMsg);
		break;
	case 1:
		strMsg += "Survey Mode";
		add_msgtolist(strMsg);
		strMsg.Format("Run-time Survey Length:%u", MAKELONG(MAKEWORD(ackCmd[40], ackCmd[39]), MAKEWORD(ackCmd[38], ackCmd[37])));
		add_msgtolist(strMsg);
		break;
	case 2:
		strMsg += "Static Mode";
		add_msgtolist(strMsg);
		break;
	}	
	return Ack;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryBasePosition(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QueryBasePositionCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryBasePositionCmd].cmdId);

	BinaryData ackCmd;
	CmdErrorCode err = ExcuteBinaryCommand(QueryBasePositionCmd, &cmd, &ackCmd, (nMode == Display) ? 3000 : 1000);
	if(err != Ack)
	{
		return err;
	}

	if(Return == nMode)
	{	//Return command data
    *((BinaryData*)outputData) = ackCmd;
		return err;
	}

	CString strMsg;
	strMsg = "Query Base Position successfully";
	add_msgtolist(strMsg);

	U16 cmdLen = ConvertLeonU16(ackCmd.Ptr(2));
	UINT8 svBpMode = ackCmd[5];
	strMsg.Format("Saved Base Position Mode: ");
	switch(svBpMode)
	{
	case 0:
		strMsg += "Kinematic Mode";
		add_msgtolist(strMsg);
		break;
	case 1:
		strMsg += "Survey Mode";
		add_msgtolist(strMsg);
		strMsg.Format("Saved Survey Length:%u", MAKELONG(MAKEWORD(ackCmd[9], ackCmd[8]), MAKEWORD(ackCmd[7], ackCmd[6])));
		add_msgtolist(strMsg);
		strMsg.Format("Standard Deviation:%u", MAKELONG(MAKEWORD(ackCmd[13], ackCmd[12]), MAKEWORD(ackCmd[11], ackCmd[10])));
		add_msgtolist(strMsg);
		break;
	case 2:
		strMsg += "Static Mode";
		add_msgtolist(strMsg);
		strMsg.Format("Saved Latitude:%f", ConvertLeonDouble(ackCmd.Ptr(14)));
		add_msgtolist(strMsg);
		strMsg.Format("Saved Longitude:%f", ConvertLeonDouble(ackCmd.Ptr(22)));
		add_msgtolist(strMsg);
		strMsg.Format("Saved Ellipsoidal Height:%f", ConvertLeonFloat(ackCmd.Ptr(30)));
		add_msgtolist(strMsg);
		break;
	}	

	UINT8 rtBpMode = ackCmd[34];
  strMsg.Format("Run-time Base Position Mode: ");
	switch(rtBpMode)
	{
	case 0:
		strMsg += "Kinematic Mode";
		add_msgtolist(strMsg);
		break;
	case 1:
		strMsg += "Survey Mode";
		add_msgtolist(strMsg);
		strMsg.Format("Run-time Survey Length:%u", MAKELONG(MAKEWORD(ackCmd[38], ackCmd[37]), MAKEWORD(ackCmd[36], ackCmd[35])));
		add_msgtolist(strMsg);
		break;
	case 2:
		strMsg += "Static Mode";
		add_msgtolist(strMsg);
		break;
	}	
	return Ack;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryRtkParameters(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QueryRtkParametersCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryRtkParametersCmd].cmdId);
	cmd.SetU08(2, cmdTable[QueryRtkParametersCmd].cmdSubId);

	BinaryData ackCmd;
	if(Ack == ExcuteBinaryCommand(QueryRtkParametersCmd, &cmd, &ackCmd))
	{
		CString strMsg;
		strMsg = "Query RTK parameters successfully";
		add_msgtolist(strMsg);
		strMsg.Format("Fix Ratio Scale : %d", MAKEWORD(ackCmd[7], ackCmd[6]));
		add_msgtolist(strMsg);
		strMsg.Format("Elevation Mask : 0x%04X", MAKEWORD(ackCmd[9], ackCmd[8]));
		add_msgtolist(strMsg);
		strMsg.Format("CNR mask : 0x%04X", MAKEWORD(ackCmd[11], ackCmd[10]));
		add_msgtolist(strMsg);
		strMsg.Format("Geo CNR Mask : 0x%04X", MAKEWORD(ackCmd[13], ackCmd[12]));
		add_msgtolist(strMsg);
		strMsg.Format("GPS PRN Mask : 0x%08X", MAKELONG(MAKEWORD(ackCmd[17], ackCmd[16]), MAKEWORD(ackCmd[15], ackCmd[14])));
		add_msgtolist(strMsg);
		strMsg.Format("Galileo PRN Mask : 0x%08X", MAKELONG(MAKEWORD(ackCmd[21], ackCmd[20]), MAKEWORD(ackCmd[19], ackCmd[18])));
		add_msgtolist(strMsg);
		strMsg.Format("Glonass PRN Mask : 0x%08X", MAKELONG(MAKEWORD(ackCmd[25], ackCmd[24]), MAKEWORD(ackCmd[23], ackCmd[22])));
		add_msgtolist(strMsg);
		strMsg.Format("Beidou2 PRN Mask 1~32 : 0x%08X", MAKELONG(MAKEWORD(ackCmd[29], ackCmd[28]), MAKEWORD(ackCmd[27], ackCmd[26])));
		add_msgtolist(strMsg);
		strMsg.Format("Beidou2 PRN Mask 33~40 : 0x%02X", ackCmd[30]);
		add_msgtolist(strMsg);
		strMsg.Format("QZSS PRN mask : 0x%02X", ackCmd[31]);
		add_msgtolist(strMsg);
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryChannelDoppler(CmdExeMode nMode, void* outputData)
{	    
	const int ChannelCount = GSA_MAX_SATELLITE;
	CString strMsg("PRN, DOPPLER");
	if(nMode==Display)
	{
		add_msgtolist(strMsg);
	}
	for(int i=0; i<ChannelCount; ++i)
	{
		BinaryCommand cmd(cmdTable[QueryChannelDopplerCmd].cmdSize);
		cmd.SetU08(1, cmdTable[QueryChannelDopplerCmd].cmdId);
		cmd.SetU08(2, (U08)i);

		BinaryData ackCmd;
		if(Ack == ExcuteBinaryCommand(QueryChannelDopplerCmd, &cmd, &ackCmd))
		{
			//U32 data = MAKELONG(MAKEWORD(ackCmd[8], ackCmd[7]), MAKEWORD(ackCmd[6], ackCmd[5]));
			S16 prn = MAKEWORD(ackCmd[6], ackCmd[5]);
			S16 doppler = MAKEWORD(ackCmd[8], ackCmd[7]);
			if(nMode==Return)
			{
				int* ChannelTable = (int*)outputData;
				ChannelTable[i] = prn;
			}
			else if(nMode==Display)
			{
				strMsg.Format(" %02d, %d", prn, doppler);
				add_msgtolist(strMsg);
			}
		}
	}
	if(nMode==Return)
	{
		return Ack;
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryClockOffset(CmdExeMode nMode, void* outputData)
{	    
	const int ChannelCount = GSA_MAX_SATELLITE;
	CString strMsg("PRN, Clock Offset, ppm");
	if(nMode==Display)
	{
		add_msgtolist(strMsg);
	}
	int* ChannelTable = (int*)outputData;

	int totalCount = 0;
	int totalOffset = 0;
	for(int i=0; i<ChannelCount; ++i)
	{
		if(-1==ChannelTable[i])
		{
			continue;
		}
		BinaryCommand cmd(cmdTable[QueryChannelClockOffsetCmd].cmdSize);
		cmd.SetU08(1, cmdTable[QueryChannelClockOffsetCmd].cmdId);
		cmd.SetU08(6, HIBYTE(ChannelTable[i]));
		cmd.SetU08(7, LOBYTE(ChannelTable[i]));

		BinaryData ackCmd;
		if(Ack == ExcuteBinaryCommand(QueryChannelClockOffsetCmd, &cmd, &ackCmd))
		{
			S32 data = MAKELONG(MAKEWORD(ackCmd[8], ackCmd[7]), MAKEWORD(ackCmd[6], ackCmd[5]));
			strMsg.Format(" %3d, %5d, %2.6fppm", ChannelTable[i], data, data / (96.25 * 16.367667));
			add_msgtolist(strMsg);
			++totalCount;
			totalOffset += data;
		}
	}
	if(nMode==Display)
	{
		strMsg.Format("Avg : %5d, %2.6fppm", totalOffset / totalCount, (totalOffset / totalCount) / (96.25 * 16.367667));
		add_msgtolist(strMsg);
	}

	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryRefTimeSyncToGpsTime(CmdExeMode nMode, void* outputData)
{	    
	BinaryCommand cmd(cmdTable[QueryRefTimeSyncToGpsTimeCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryRefTimeSyncToGpsTimeCmd].cmdId);
	cmd.SetU08(2, cmdTable[QueryRefTimeSyncToGpsTimeCmd].cmdSubId);

	BinaryData ackCmd;
	if(Ack == ExcuteBinaryCommand(QueryRefTimeSyncToGpsTimeCmd, &cmd, &ackCmd))
	{
		CString strMsg = "Query Ref Time Sync To Gps Time successfully";
		add_msgtolist(strMsg);
		strMsg.Format("Enable : %d", ackCmd[6]);
		add_msgtolist(strMsg);
		strMsg.Format("Date : %d/%d/%d", MAKEWORD(ackCmd[8], ackCmd[7]), ackCmd[9], ackCmd[10]);
		add_msgtolist(strMsg);
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QuerySearchEngineSleepCriteria(CmdExeMode nMode, void* outputData)
{	    
	BinaryCommand cmd(cmdTable[QuerySearchEngineSleepCriteriaCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QuerySearchEngineSleepCriteriaCmd].cmdId);
	cmd.SetU08(2, cmdTable[QuerySearchEngineSleepCriteriaCmd].cmdSubId);

	BinaryData ackCmd;
	if(Ack == ExcuteBinaryCommand(QuerySearchEngineSleepCriteriaCmd, &cmd, &ackCmd))
	{
		CString strMsg = "QuerySearchEngineSleepCriteriaCmd successfully";
		add_msgtolist(strMsg);
		strMsg.Format("GPS Satellite Tracked Number : %d", ackCmd[6]);
		add_msgtolist(strMsg);
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryDatumIndex(CmdExeMode nMode, void* outputData)
{	    
	BinaryCommand cmd(cmdTable[QueryDatumIndexCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryDatumIndexCmd].cmdId);
	cmd.SetU08(2, cmdTable[QueryDatumIndexCmd].cmdSubId);

	BinaryData ackCmd;
	if(Ack == ExcuteBinaryCommand(QueryDatumIndexCmd, &cmd, &ackCmd))
	{
		CString strMsg = "QueryDatumIndex successfully";
		add_msgtolist(strMsg);
		int datumIndex = MAKEWORD(ackCmd[7], ackCmd[6]);
		strMsg.Format("Datum Index : %d", datumIndex);
		add_msgtolist(strMsg);
		if(datumIndex < DatumListSize)
		{
			strMsg = DatumList[datumIndex];
			add_msgtolist(strMsg);
		}
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QuerySignalDisturbanceStatus(CmdExeMode nMode, void* outputData)
{	    
	BinaryCommand cmd(cmdTable[QuerySignalDisturbanceStatusCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QuerySignalDisturbanceStatusCmd].cmdId);
	cmd.SetU08(2, cmdTable[QuerySignalDisturbanceStatusCmd].cmdSubId);

	BinaryData ackCmd;
	if(Ack == ExcuteBinaryCommand(QuerySignalDisturbanceStatusCmd, &cmd, &ackCmd))
	{
		CString strMsg = "QuerySignalDisturbanceStatus successfully";
		add_msgtolist(strMsg);
		strMsg.Format("Operation Type : %d (%s)", ackCmd[6], (ackCmd[6]==0) ? "Disable" : "Enable");
		add_msgtolist(strMsg);
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QuerySignalDisturbanceData(CmdExeMode nMode, void* outputData)
{	    
	BinaryCommand cmd(cmdTable[QuerySignalDisturbanceDataCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QuerySignalDisturbanceDataCmd].cmdId);
	cmd.SetU08(2, cmdTable[QuerySignalDisturbanceDataCmd].cmdSubId);

	BinaryData ackCmd;
	if(Ack == ExcuteBinaryCommand(QuerySignalDisturbanceDataCmd, &cmd, &ackCmd))
	{
		const int FieldLen = 7;
		int chCnt = (MAKEWORD(ackCmd[3], ackCmd[2]) - 2) / FieldLen;
		CString strMsg = "QuerySignalDisturbanceData successfully";
		add_msgtolist(strMsg);

		strMsg = "NO. GNSS_TYPE PRN CN0  Total Unreliable_Count";
		add_msgtolist(strMsg);
		for(int i = 0; i < chCnt; ++i)
		{
			int a = 6 + i * FieldLen;
			const char* type = NULL;
			switch(ackCmd[a])
			{
			case 0:
				type = "NONE";
				break;
			case 1:
				type = "GPS";
				break;
			case 2:
				type = "SBAS";
				break;
			case 3:
				type = "GLONASS";
				break;
			case 4:
				type = "GALILEO";
				break;
			case 5:
				type = "QZSS";
				break;
			case 6:
				type = "BD2";
				break;
			default:
				type = "Unknown";
				break;
			}
	
			strMsg.Format("%3d  %8s %3d %3d %6d %6d", i, type, ackCmd[a + 1], ackCmd[a + 2], 
				MAKEWORD(ackCmd[a + 4], ackCmd[a + 3]), MAKEWORD(ackCmd[a + 6], ackCmd[a + 5]));
			add_msgtolist(strMsg);
		}
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::ResetOdometer(CmdExeMode nMode, void* outputData)
{	    
	BinaryCommand cmd(1);
	cmd.SetU08(1, 0x7F);

	ClearQue();
	SendToTarget(cmd.GetBuffer(), cmd.Size(), "Reset Odometer successfully", true);	
	return Timeout;
}

//GPSDO command
//bit 0 : Enter pass through mode
//bit 1 : Disable GPS ISR(must reset master when leave)
//bit 2 : Reset slave, master only
//bit 3 : Drive slave CPU clock(must reset slave), master only
//bit 4 : Boost baud rate to 460800
#define GPSDO_PASS_THROUGH			(1 << 0)
#define GPSDO_DISABLE_GPS_ISR		(1 << 1)
#define GPSDO_RESET_SLAVE			(1 << 2)
#define GPSDO_DRIVE_SLAVE			(1 << 3)
#define GPSDO_BOOST_BAUDRATE		(1 << 4)
CGPSDlg::CmdErrorCode CGPSDlg::GpsdoResetSlave(CmdExeMode nMode, void* outputData)
{	    
	BinaryCommand cmd(4);
	cmd.SetU08(1, 0x7A);
	cmd.SetU08(2, 0x08);
	cmd.SetU08(3, 0x01);
	cmd.SetU08(4, GPSDO_RESET_SLAVE);

	ClearQue();
	SendToTarget(cmd.GetBuffer(), cmd.Size(), "Reset Slave MCU successfully", true);	
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::GpsdoEnterRom(CmdExeMode nMode, void* outputData)
{	    
	BinaryCommand cmd(4);
	cmd.SetU08(1, 0x7A);
	cmd.SetU08(2, 0x08);
	cmd.SetU08(3, 0x01);
	cmd.SetU08(4, (GPSDO_PASS_THROUGH | GPSDO_DISABLE_GPS_ISR | GPSDO_DRIVE_SLAVE));

	ClearQue();
	SendToTarget(cmd.GetBuffer(), cmd.Size(), "Enter Slave ROM Download successfully", true);	
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::GpsdoLeaveRom(CmdExeMode nMode, void* outputData)
{	    
	BinaryCommand cmd(4);
	cmd.SetU08(1, 0x7A);
	cmd.SetU08(2, 0x08);
	cmd.SetU08(3, 0x01);
	cmd.SetU08(4, GPSDO_DISABLE_GPS_ISR | GPSDO_DRIVE_SLAVE);

	ClearQue();
	SendToTarget(cmd.GetBuffer(), cmd.Size(), "Back To Normal Mode from ROM Download successfully", true);	
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::GpsdoEnterDownload(CmdExeMode nMode, void* outputData)
{	    
	BinaryCommand cmd(4);
	cmd.SetU08(1, 0x7A);
	cmd.SetU08(2, 0x08);
	cmd.SetU08(3, 0x01);
	cmd.SetU08(4, GPSDO_PASS_THROUGH | GPSDO_DISABLE_GPS_ISR);

	ClearQue();
	bool r = SendToTarget(cmd.GetBuffer(), cmd.Size(), "Enter Slave Download successfully", true);	
	return (r) ? Ack : Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::GpsdoLeaveDownload(CmdExeMode nMode, void* outputData)
{	    
	BinaryCommand cmd(4);
	cmd.SetU08(1, 0x7A);
	cmd.SetU08(2, 0x08);
	cmd.SetU08(3, 0x01);
	cmd.SetU08(4, GPSDO_DISABLE_GPS_ISR);

	ClearQue();
	SendToTarget(cmd.GetBuffer(), cmd.Size(), "Back To Normal Mode from Slave Download successfully", true);	
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::InsdrEnterDownload(CmdExeMode nMode, void* outputData)
{	    
	BinaryCommand cmd(4);
	cmd.SetU08(1, 0x7A);
	cmd.SetU08(2, 0x08);
	cmd.SetU08(3, 0x01);
	cmd.SetU08(4, GPSDO_DISABLE_GPS_ISR);

	ClearQue();
	SendToTarget(cmd.GetBuffer(), cmd.Size(), "Enter UART pass through for slave flash code", true);	
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::GpsdoEnterDownloadHigh(CmdExeMode nMode, void* outputData)
{	    
	BinaryCommand cmd(4);
	cmd.SetU08(1, 0x7A);
	cmd.SetU08(2, 0x08);
	cmd.SetU08(3, 0x01);
	cmd.SetU08(4, GPSDO_PASS_THROUGH | GPSDO_DISABLE_GPS_ISR | GPSDO_DRIVE_SLAVE | GPSDO_BOOST_BAUDRATE);

	ClearQue();
	bool r = SendToTarget(cmd.GetBuffer(), cmd.Size(), "Enter High-Speed Slave Download successfully", false);	
	CGPSDlg::gpsDlg->SetBaudrate(7);		//460800 bps
	PostMessage(UWM_GPSDO_HI_DOWNLOAD, 0, 0);
	
	return (r) ? Ack : Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::GpsdoEnterUart(CmdExeMode nMode, void* outputData)
{	    
	BinaryCommand cmd(4);
	cmd.SetU08(1, 0x7A);
	cmd.SetU08(2, 0x08);
	cmd.SetU08(3, 0x01);
	cmd.SetU08(4, GPSDO_PASS_THROUGH);

	ClearQue();
	SendToTarget(cmd.GetBuffer(), cmd.Size(), "Enter Slave UART Pass Through successfully", true);	
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::InsdrEnterUart(CmdExeMode nMode, void* outputData)
{	    
	BinaryCommand cmd(4);
	cmd.SetU08(1, 0x7A);
	cmd.SetU08(2, 0x08);
	cmd.SetU08(3, 0x01);
	cmd.SetU08(4, GPSDO_PASS_THROUGH);

	ClearQue();
	SendToTarget(cmd.GetBuffer(), cmd.Size(), "Enter UART pass through for slave ROM code", true);	
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::GpsdoLeaveUart(CmdExeMode nMode, void* outputData)
{	    
	BinaryCommand cmd(4);
	cmd.SetU08(1, 0x7A);
	cmd.SetU08(2, 0x08);
	cmd.SetU08(3, 0x01);
	cmd.SetU08(4, 0x00);

	ClearQue();
	SendToTarget(cmd.GetBuffer(), cmd.Size(), "Back To Normal Mode from UART Passthrough successfully", true);	
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::InsdrLeaveUart(CmdExeMode nMode, void* outputData)
{	    
	BinaryCommand cmd(4);
	cmd.SetU08(1, 0x7A);
	cmd.SetU08(2, 0x08);
	cmd.SetU08(3, 0x01);
	cmd.SetU08(4, 0x00);

	ClearQue();
	SendToTarget(cmd.GetBuffer(), cmd.Size(), "Back To Master successfully", true);	
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryUartPass(CmdExeMode nMode, void* outputData)
{	    
	BinaryCommand cmd(cmdTable[QueryUartPassCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryUartPassCmd].cmdId);
	cmd.SetU08(2, cmdTable[QueryUartPassCmd].cmdSubId);
	cmd.SetU08(3, 0x02);

	BinaryData ackCmd;
	if(Ack == ExcuteBinaryCommand(QueryUartPassCmd, &cmd, &ackCmd))
	{
		if(nMode==Return && outputData)
		{
			*((BYTE*)outputData) = ackCmd[7];
			return Ack;
		}
		CString strMsg = "Query UART pass-through successfully";
		add_msgtolist(strMsg);
		if(ackCmd[7] == 1)
		{
			strMsg.Format("UART pass-through : MASTER");
		}
		else if(ackCmd[7] == 0)
		{
			strMsg.Format("UART pass-through : SLAVE");
		}
		else
		{
			strMsg.Format("UART pass-through : %d", ackCmd[7]);
		}
		add_msgtolist(strMsg);
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryGnssNavSol(CmdExeMode nMode, void* outputData)
{	    
	BinaryCommand cmd(cmdTable[QueryGnssNavSolCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryGnssNavSolCmd].cmdId);
	cmd.SetU08(2, cmdTable[QueryGnssNavSolCmd].cmdSubId);

	BinaryData ackCmd;
	if(Ack == ExcuteBinaryCommand(QueryGnssNavSolCmd, &cmd, &ackCmd))
	{
		CString strMsg = "Query GNSS constellation type successfully";
		add_msgtolist(strMsg);
		U16 mode = MAKEWORD(ackCmd[7], ackCmd[6]);
		add_msgtolist("Navigation Solution : ");

		CString strOutput;
		if(mode & 0x0001)
		{
			strOutput += "GPS + ";
		}
		if(mode & 0x0002)
		{
			strOutput += "GLONASS + ";
		}
		if(mode & 0x0004)
		{
			strOutput += "Galileo + ";
		}
		if(mode & 0x0008)
		{
			strOutput += "Beidou + ";
		}

		if(mode==0)
		{
			add_msgtolist("None");
		}
		else
		{
			strOutput = strOutput.Left(strOutput.GetLength() - 2);
			add_msgtolist(strOutput);
		}
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryCustomerID(CmdExeMode nMode, void* outputData)
{	    
	BinaryCommand cmd(cmdTable[QueryCustomerIDCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryCustomerIDCmd].cmdId);

	BinaryData ackCmd;
	if(Return==nMode && outputData)
	{
		if(Ack == ExcuteBinaryCommand(QueryCustomerIDCmd, &cmd, &ackCmd, 300, true))
		{
			*((U32*)outputData) = MAKELONG(MAKEWORD(ackCmd[8], ackCmd[7]),MAKEWORD(ackCmd[6], ackCmd[5]));
		}
		return Timeout;
	}
	if(Ack == ExcuteBinaryCommand(QueryCustomerIDCmd, &cmd, &ackCmd))
	{
		CString strMsg = "Query Customer ID successfully";
		add_msgtolist(strMsg);
		strMsg.Format("Customer ID : 0X%08X",
			MAKELONG(MAKEWORD(ackCmd[8], ackCmd[7]),MAKEWORD(ackCmd[6], ackCmd[5])));
		add_msgtolist(strMsg);
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::Query1ppsFreqencyOutput(CmdExeMode nMode, void* outputData)
{	    
	BinaryCommand cmd(cmdTable[Query1ppsFreqencyOutputCmd].cmdSize);
	cmd.SetU08(1, cmdTable[Query1ppsFreqencyOutputCmd].cmdId);
	cmd.SetU08(2, cmdTable[Query1ppsFreqencyOutputCmd].cmdSubId);

	BinaryData ackCmd;
	if(Ack == ExcuteBinaryCommand(Query1ppsFreqencyOutputCmd, &cmd, &ackCmd))
	{
		CString strMsg = "Query 1PPS Freqency Output successfully";
		add_msgtolist(strMsg);
		strMsg.Format("Freqency : %d",
			MAKELONG(MAKEWORD(ackCmd[9], ackCmd[8]),MAKEWORD(ackCmd[7], ackCmd[6])));
		add_msgtolist(strMsg);
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryBinaryMeasurementDataOut(CmdExeMode nMode, void* outputData)
{	    
	BinaryCommand cmd(cmdTable[QueryBinaryMeasurementDataOutCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryBinaryMeasurementDataOutCmd].cmdId);

	BinaryData ackCmd;
  CmdErrorCode err = ExcuteBinaryCommand(QueryBinaryMeasurementDataOutCmd, &cmd, &ackCmd, (nMode == Display) ? 3000 : 1000);
	if(err != Ack)
	{
    return err;
  }

  if(Return == nMode)
	{	//Return command length
    *((BinaryData*)outputData) = ackCmd;
		return err;
  }

	U16 cmdLen = ConvertLeonU16(ackCmd.Ptr(2));
	CString strMsg = "Query Binary Measurement Data Out successfully";
	add_msgtolist(strMsg);
	if(ackCmd[5]==0)
	{
		strMsg.Format("Output Rate : 1Hz");	
	}
	else if(ackCmd[5]==1)
	{
		strMsg.Format("Output Rate : 2Hz");
	}
	else if(ackCmd[5]==2)
	{
		strMsg.Format("Output Rate : 4Hz");
	}
	else if(ackCmd[5]==3)
	{
		strMsg.Format("Output Rate : 5Hz");
	}
	else if(ackCmd[5]==4)
	{
		strMsg.Format("Output Rate : 10Hz");
	}
	else if(ackCmd[5]==5)
	{
		strMsg.Format("Output Rate : 20Hz");
	}
	else if(ackCmd[5]==6)	//Add in 20160512, request from Andrew
	{
		strMsg.Format("Output Rate : 8Hz");
	}
	add_msgtolist(strMsg);

	strMsg.Format("Meas Time : %s", (ackCmd[6]) ? "Enable" : "Disable");
	add_msgtolist(strMsg);

	strMsg.Format("Raw Meas : %s", (ackCmd[7]) ? "Enable" : "Disable");
	add_msgtolist(strMsg);

	strMsg.Format("SV CH Status : %s", (ackCmd[8]) ? "Enable" : "Disable");
	add_msgtolist(strMsg);

	strMsg.Format("RCV State : %s", (ackCmd[9]) ? "Enable" : "Disable");
	add_msgtolist(strMsg);

	if(cmdLen == 8)
	{	//The length in old style is 7, new is 8. 20160914 changed by Ryan
	  strMsg.Format("Extended Raw Meas : %s", (ackCmd[11]) ? "Enable" : "Disable");
	  add_msgtolist(strMsg);
	}

	//20160419 Modify BinaryMeasurementDataOut command format, remove constellation field, Report from Andrew, Oliver.
/*
	CString strOutput;
	if(ackCmd[10] & 0x01)
	{
		strOutput += "GPS + ";
	} 
	if(ackCmd[10] & 0x02)
	{
		strOutput += "GLONASS + ";
	}
	if(ackCmd[10] & 0x04)
	{
		strOutput += "Galileo + ";
	}
	if(ackCmd[10] & 0x08)
	{
		strOutput += "Beidou + ";
	}

	if(ackCmd[10]==0)
	{
		add_msgtolist("None");
	}
	else
	{
		add_msgtolist("Subframe for different constellation :");
		strOutput = strOutput.Left(strOutput.GetLength() - 2);
		add_msgtolist(strOutput);
	}
*/
	return Ack;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryRtcmMeasurementDataOut(CmdExeMode nMode, void* outputData)
{	    
	BinaryCommand cmd(cmdTable[QueryRtcmMeasurementDataOutCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryRtcmMeasurementDataOutCmd].cmdId);

	BinaryData ackCmd;
  CmdErrorCode err = ExcuteBinaryCommand(QueryRtcmMeasurementDataOutCmd, &cmd, &ackCmd, (nMode == Display) ? 3000 : 1000);
	if(err != Ack)
	{
    return err;
  }

  if(Return == nMode)
	{	
    *((BinaryData*)outputData) = ackCmd;
		return err;
  }

	U16 cmdLen = ConvertLeonU16(ackCmd.Ptr(2));
	CString strMsg = "Query RTCM Measurement Data Out successfully";
	add_msgtolist(strMsg);
  strMsg.Format("RTCM Output: %s", (ackCmd[5]) ? "Enable" : "Disable");	
	add_msgtolist(strMsg);
	if(ackCmd[6]==0)
	{
		strMsg.Format("Output Rate : 1Hz");	
	}
	else if(ackCmd[6]==1)
	{
		strMsg.Format("Output Rate for MSM: 2Hz");
	}
	else if(ackCmd[6]==2)
	{
		strMsg.Format("Output Rate for MSM: 4Hz");
	}
	else if(ackCmd[6]==3)
	{
		strMsg.Format("Output Rate for MSM: 5Hz");
	}
	else if(ackCmd[6]==4)
	{
		strMsg.Format("Output Rate for MSM : 10Hz");
	}
	else if(ackCmd[6]==5)
	{
		strMsg.Format("Output Rate for MSM: 20Hz");
	}
	else if(ackCmd[6]==6)	//Add in 20160512, request from Andrew
	{
		strMsg.Format("Output Rate for MSM: 8Hz");
	}
	add_msgtolist(strMsg);

	strMsg.Format("Message Type 1005: %s", (ackCmd[7]) ? "Enable" : "Disable");
	add_msgtolist(strMsg);

	strMsg.Format("Message Type 1077: %s", (ackCmd[8]) ? "Enable" : "Disable");
	add_msgtolist(strMsg);

	strMsg.Format("Message Type 1087: %s", (ackCmd[9]) ? "Enable" : "Disable");
	add_msgtolist(strMsg);

	strMsg.Format("Message Type 1107: %s", (ackCmd[11]) ? "Enable" : "Disable");
	add_msgtolist(strMsg);

	strMsg.Format("Message Type 1117: %s", (ackCmd[12]) ? "Enable" : "Disable");
	add_msgtolist(strMsg);

	strMsg.Format("Message Type 1127: %s", (ackCmd[13]) ? "Enable" : "Disable");
	add_msgtolist(strMsg);

	return Ack;
}


CGPSDlg::CmdErrorCode CGPSDlg::QueryCableDelay(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QueryCableDelayCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryCableDelayCmd].cmdId);

	BinaryData ackCmd;
	if(Ack == ExcuteBinaryCommand(QueryCableDelayCmd, &cmd, &ackCmd))
	{
		CString strMsg;
		strMsg = "Query 1PPS Cable Delay successfully";
		add_msgtolist(strMsg);

		U32 data = ackCmd[5]<<24 | ackCmd[6]<<16 | ackCmd[7]<<8 | ackCmd[8];
		strMsg.Format("Cable delay: %0.2fns", data / 100.0);
		add_msgtolist(strMsg);
	}
	return Timeout;
}

UINT SetFacMsgThread(LPVOID pParam)
{	
	int len = *((int*)(pParam));
	CGPSDlg::gpsDlg->ExecuteConfigureCommand(CGPSDlg::m_inputMsg, len, "Factroy Reset successfully");
#if defined(SAINTMAX_UI)
	Sleep(500);
	CGPSDlg::gpsDlg->m_nmea0183msg.EnableWindow(TRUE);
#endif
	return 0;
}

void CGPSDlg::SetFactoryDefault(bool isReboot)
{
	if(!CheckConnect())
	{
		return;
	}
	int cmdLen = 9;
	SetInputMode(NoOutputMode);
	memset(m_inputMsg, 0, cmdLen);   		    
	m_inputMsg[0]=(U08)0xa0;
	m_inputMsg[1]=(U08)0xa1;
	m_inputMsg[2]=0;
	m_inputMsg[3]=2;
	m_inputMsg[4]=4; //msgid
	if(isReboot)
	{
		m_inputMsg[5]=1;
		m_inputMsg[6]=5; //checksum right	 
	}
	else
	{
		m_inputMsg[5]=0;
		m_inputMsg[6]=4; //checksum right	 
	}
	m_inputMsg[7]=(U08)0x0d;
	m_inputMsg[8]=(U08)0x0a;
	//slgsv = 6;
	AfxBeginThread(SetFacMsgThread, &cmdLen);
}

void CGPSDlg::OnSetFactoryDefaultNoReboot()
{		
	SetFactoryDefault(false);
}

void CGPSDlg::OnSetFactoryDefaultReboot()
{	
	SetFactoryDefault(true);
}

void CGPSDlg::OnConfigureNmeaIntervalV8()
{
	if(!CheckConnect())
	{
		return;
	}
	SetInputMode(NoOutputMode);

	CConfigNmeaIntervalDlg dlg;
	if(dlg.DoModal() != IDOK)		
	{
		SetMode();  
		CreateGPSThread();
	}
}

void CGPSDlg::OnConfigureEricssonSentecneInterval()
{
	if(!CheckConnect())
	{
		return;
	}
	SetInputMode(NoOutputMode);
	ConfigEricssonIntervalDlg dlg;
	if(dlg.DoModal() != IDOK)		
	{
		SetMode();  
		CreateGPSThread();
	}
}

void CGPSDlg::OnConfigureSerialNumber()
{
	if(!CheckConnect())
	{
		return;
	}
	SetInputMode(NoOutputMode);
	ConfigSerialNumberDlg dlg;
	if(dlg.DoModal() != IDOK)		
	{
		SetMode();  
		CreateGPSThread();
	}
}

void CGPSDlg::OnBinaryConfiguredatum()
{
	if(!CheckConnect())
	{
		return;
	}

	SetInputMode(NoOutputMode);
	CConDauDlg dlg;
	if(dlg.DoModal() != IDOK)
	{
		SetMode();  
		CreateGPSThread();
	}
}

void CGPSDlg::OnConfigureSerialPort()
{
	if(!CheckConnect())
	{
		return;
	}

	SetInputMode(NoOutputMode);
	CConSrePorDlg dlg;
	if(dlg.DoModal() != IDOK) 	
	{
		SetMode();  
		CreateGPSThread();
	}
}

void CGPSDlg::GetGpsAlmanac(CString m_almanac_filename,U08 sv,U08 continues)
{
	int wait = 0;
	U08 msg[2];
	char BINMSG[1024] = {0};

	msg[0] = 0x50;
	msg[1] = sv;
	int res_len;
	int len = SetMessage(msg,sizeof(msg));
	FILE *f;

	//WaitEvent();
	ClearQue();
	if(SendToTarget(m_inputMsg, len, "Get Almance start...", true) == 1)
	{
		if(WRL == NULL)
		{
			WRL = new CWaitReadLog;
		}
		AfxBeginThread(WaitLogRead,0);
		WaitForSingleObject(waitlog,INFINITE);
		WRL->SetWindowText("Wait for get almanac");
		WRL->msg.SetWindowText("Please wait for get almanac!");

		U08 NumsOfEphemeris = 0;
		fopen_s(&f, m_almanac_filename, "wb+");
		fclose(f);

		while(1)
		{	
			wait++;
			if(wait == 50)
			{ 
				Sleep(500);
				WRL->msg.SetWindowText("Retrieve almanac data is Failed!");
				Sleep(500);
				WRL->IsFinish = true;
				add_msgtolist("Retrieve almanac Failed...");	
				//goto TheLast;
			}
			U08 buff[1024] = {0};
			res_len = m_serial->GetBinaryAck(buff, sizeof(buff));

			if(res_len==0x3b)
			{
				fopen_s(&f, m_almanac_filename, "ab");
				fwrite(&buff[5], 1, res_len - 8, f);
				fclose(f);
			}
			else
			{
				break;
			}

			NumsOfEphemeris = buff[7];
			//	}
			sprintf_s(BINMSG,"Retrieve Satellite ID # %d almanac",NumsOfEphemeris);
			WRL->msg.SetWindowText(BINMSG);

			if(NumsOfEphemeris==32)break;
			//}
		}	
		Sleep(500);
		WRL->msg.SetWindowText("Retrieve almanac data is completed!");
		Sleep(500);
		WRL->IsFinish = true;
		add_msgtolist("Retrieve almanac successfully");	
	}

	if(!continues)
	{
		SetMode();  
		CreateGPSThread();
	}
}

UINT GetGpsAlmanacThread(LPVOID param)
{
	CGPSDlg::gpsDlg->GetGpsAlmanac(m_almanac_filename,m_almanac_no,FALSE);
	return 0;
}

void CGPSDlg::OnGetGpsAlmanac()
{
	if(!CheckConnect())	
	{
    return;
  }
    
  CGetAlmanac frm;
	if(frm.DoModal()==IDOK)
	{
		m_almanac_filename = frm.fileName;
		m_almanac_no = frm.sv;
		::AfxBeginThread(GetGpsAlmanacThread, 0);
	}
  else
	{
		SetMode();  
		CreateGPSThread();
	}
}

void CGPSDlg::OnShowGpsAlmanac()
{
	if(!CheckConnect())
	{
		return;
	}

	CGetAlmanac frm;
  frm.isDecode = 1;
	if(frm.DoModal()==IDOK)
	{
	  U08 no = frm.sv;
		QueryGetGpsAlmanac(Display, &no);

		SetMode();
		CreateGPSThread();
	}
}

UINT SetGpsAlmanacThread(LPVOID pParam)
{	
	CGPSDlg::gpsDlg->SetGpsAlmanac(FALSE);	
	return 0;
}

void CGPSDlg::SetGpsAlmanac(U08 continues)	
{	
	U16 SVID;	

	U08 messages[100];  
	U08 msg[100];
	BYTE buffer[0x1000];
	ULONGLONG dwBytesRemaining = m_ephmsFile.GetLength();

	if(dwBytesRemaining == 0)
	{
		m_ephmsFile.Close();	
		if(!continues)
		{
			SetMode(); 
			CreateGPSThread();	
		}
		return;
	}

	m_ephmsFile.Read(buffer,1);
	int one_entry_size = buffer[0] + 3;	//Size 1 byte, satellite id 2 bytes.

	if(dwBytesRemaining == 32 * one_entry_size)
	{
		m_ephmsFile.SeekToBegin();

		while(1)
		{
			UINT nBytesRead = m_ephmsFile.Read(buffer,one_entry_size);	
			if(nBytesRead<=0)
				break;
			memset(msg, 0, sizeof(msg));
			memset(messages, 0, sizeof(messages));
			msg[0] = 0x51;

			memcpy(&msg[1],&buffer[1],one_entry_size-1);
			SVID = buffer[1]<<8 | buffer[2];	

			if(buffer[0] == 0) 
			{
				TRACE("SVID=%d,continue\r\n",SVID);
				continue;
			}

			if(continues)
			{
				msg[one_entry_size] = 0;
			}
			else
			{
				if(SVID == 32)
					msg[one_entry_size] = 1;
				else
					msg[one_entry_size] = 0;
			}

			int len = SetMessage2(messages, msg, one_entry_size + 1);
			sprintf_s(m_nmeaBuffer, "Set SV#%d almanac successfully",SVID);
			if(!SendToTargetNoWait(messages, len,m_nmeaBuffer))
			{
				sprintf_s(m_nmeaBuffer, "Set SV#%d almanac failed...",SVID);
				add_msgtolist(m_nmeaBuffer);
			}
		}
	}
	else
	{
		AfxMessageBox("The Almanac data Format of the file is wrong");
	}	
	m_ephmsFile.Close();	
	if(!continues)
	{
		SetMode(); 
		CreateGPSThread();	
	}
}

void CGPSDlg::OnSetGpsAlmanac()
{
	if(!CheckConnect())
	{
		return;
	}

	SetInputMode(NoOutputMode);
	CString fileName("GPS_Almanac.log");	
	CFileDialog dlgFile(true, _T("log"), fileName, OFN_HIDEREADONLY, _T("Almanac Files (*.log)|*.log||"), this);
	INT_PTR nResult = dlgFile.DoModal();
	fileName = dlgFile.GetPathName();	
	CFileException ef;
	try
	{
		if(nResult == IDOK)
		{  				
			if(!m_ephmsFile.Open(fileName, CFile::modeRead, &ef))
			{
				ef.ReportError();
				SetMode();
				CreateGPSThread();
				return;
			}				
			AfxBeginThread(SetGpsAlmanacThread, 0);			
		}		
		else
		{
			SetMode();
			CreateGPSThread();
		}
	}
	catch(CFileException *fe)
	{
		fe->ReportError();
		fe->Delete();
		return;
	}	
	fileName.ReleaseBuffer();		
}

void CGPSDlg::GetGlonassAlmanac(CString m_almanac_filename,U08 sv,U08 continues)
{
	int wait = 0;
	U08 msg[2];
	char BINMSG[1024] = {0};

	msg[0] = 0x5D;
	msg[1] = sv;
	int res_len;
	int len = SetMessage(msg,sizeof(msg));
	FILE *f = NULL;

	ClearQue();
	if(SendToTarget(m_inputMsg,len, "Get Glonass Almance start...", true) == 1)
	{
		if(WRL == NULL)
		{
			WRL = new CWaitReadLog;
		}
		AfxBeginThread(WaitLogRead,0);
		WaitForSingleObject(waitlog,INFINITE);
		WRL->SetWindowText("Wait for get Glonass almanac");
		WRL->msg.SetWindowText("Please wait for get Glonass almanac!");

		U08 NumsOfEphemeris = 0;

		fopen_s(&f, m_almanac_filename, "wb+");
		fclose(f);

		while(1)
		{	
			wait++;
			if(wait == 50){ 
				Sleep(500);
				WRL->msg.SetWindowText("Retrieve Glonass almanac data is Failed!");
				Sleep(500);
				WRL->IsFinish = true;
				add_msgtolist("Retrieve Glonass almanac Failed...");	
				//goto TheLast;
			}
			U08 buff[1024] = {0};
			res_len = m_serial->GetBinaryAck(buff, sizeof(buff));

			if(res_len==32)
			{
				fopen_s(&f, m_almanac_filename, "ab");
				//fwrite(&buff[5],1,res_len-7,f);
				fwrite(&buff[5],1,res_len-8,f);
				fclose(f);
			}
			else
			{
				break;
			}


			NumsOfEphemeris = buff[5];
			TRACE("NumsOfEphemeris=%d\r\n",NumsOfEphemeris);
			sprintf_s(BINMSG, sizeof(BINMSG), "Retrieve Glonass Satellite ID # %d almanac", NumsOfEphemeris);
			WRL->msg.SetWindowText(BINMSG);

			if(NumsOfEphemeris==24)break;
		}	
		Sleep(500);
		WRL->msg.SetWindowText("Retrieve Glonass almanac data is completed!");
		Sleep(500);
		WRL->IsFinish = true;
		add_msgtolist("Retrieve Glonass almanac successfully");	

	}

	if(!continues)
	{
		SetMode();  
		CreateGPSThread();
	}
}

UINT GetGlonassAlmanacThread(LPVOID param)
{
	CGPSDlg::gpsDlg->GetGlonassAlmanac(m_almanac_filename,m_almanac_no,FALSE);
	return 0;
}

void CGPSDlg::OnGetGlonassAlmanac()
{
	if(!CheckConnect())	
	{
    return;
  }
    
  CGetAlmanac frm;
	frm.isGlonass = 1;
	if(frm.DoModal()==IDOK)
	{
		m_almanac_filename = frm.fileName;
		m_almanac_no = frm.sv;
		::AfxBeginThread(GetGlonassAlmanacThread, 0);
	}
	else
	{
		SetMode();  
		CreateGPSThread();
	}
}

UINT SetGlonassAlmanacThread(LPVOID pParam)
{	
	CGPSDlg::gpsDlg->SetGlonassAlmanac(FALSE);	
	return 0;
}

void CGPSDlg::SetGlonassAlmanac(U08 continues)	
{	
	U16 SVID;	

	U08 messages[100];  
	U08 msg[100];
	BYTE  buffer[0x1000];
	ULONGLONG dwBytesRemaining = m_ephmsFile.GetLength();

	if(dwBytesRemaining == 0)
	{
		m_ephmsFile.Close();	
		if(!continues)
		{
		SetMode(); 
		CreateGPSThread();
		}	
		return;
	}

	m_ephmsFile.Read(buffer,1);
	int one_entry_size = 24;

	if(dwBytesRemaining == 24 * one_entry_size)
	{
		m_ephmsFile.SeekToBegin();

		while(1)
		{
			UINT nBytesRead = m_ephmsFile.Read(buffer,one_entry_size);	
			//TRACE("nBytesRead=%d\r\n",nBytesRead);
			if(nBytesRead<=0)
				break;
			memset(msg, 0, sizeof(msg));
			memset(messages, 0, sizeof(messages));
			msg[0] = 0x5E;

			memcpy(&msg[1],buffer,one_entry_size);
			SVID = buffer[0];	

			if(buffer[0] == 0) 
			{
				TRACE("SVID=%d,continue\r\n",SVID);
				continue;
			}

			if(continues)
			{
				msg[one_entry_size + 1] = 0;
			}
			else
			{
				msg[one_entry_size + 1] = (SVID == 24) ? 1 : 0;
            }

			int len = SetMessage2(messages, msg, one_entry_size + 2);
			sprintf_s(m_nmeaBuffer, sizeof(m_nmeaBuffer), "Set SV#%d Glonass almanac successfully",SVID);
			if(!SendToTargetNoWait(messages, len,m_nmeaBuffer))
			{
				sprintf_s(m_nmeaBuffer, sizeof(m_nmeaBuffer), "Set SV#%d Glonass almanac failed...",SVID);
				add_msgtolist(m_nmeaBuffer);
			}

			//Sleep(10);
		}
	}
	else
	{
		AfxMessageBox("The Glonass Almanac data Format of the file is wrong");
	}	
	m_ephmsFile.Close();	
	if(!continues)
	{
	SetMode(); 
	CreateGPSThread();
	}	
}

void CGPSDlg::OnSetGlonassAlmanac()
{
	if(!CheckConnect())
	{
		return;
	}

	SetInputMode(NoOutputMode);
	CString fileName("Glonass_Almanac.log");	
	CFileDialog dlgFile(true, _T("log"), fileName, OFN_HIDEREADONLY, _T("Almanac Files (*.log)|*.log||"), this);
	INT_PTR nResult = dlgFile.DoModal();
	fileName = dlgFile.GetPathName();	
	CFileException ef;
	try
	{
		if(nResult == IDOK)
		{  				
			if(!m_ephmsFile.Open(fileName,CFile::modeRead,&ef))
			{
				ef.ReportError();
				SetMode();
				CreateGPSThread();
				return;
			}				
			AfxBeginThread(SetGlonassAlmanacThread,0);			
		}		
		else
		{
			SetMode();
			CreateGPSThread();
		}
	}
	catch(CFileException *fe)
	{
		fe->ReportError();
		fe->Delete();
		return;
	}	
	fileName.ReleaseBuffer();		
}

void CGPSDlg::GetBeidouAlmanac(CString m_almanac_filename, U08 sv, U08 continues)
{
	int wait = 0;
	U08 msg[3];
	char BINMSG[1024] = {0};

	msg[0] = 0x67;
	msg[1] = 0x04;
	msg[2] = sv;
	int res_len;
	int len = SetMessage(msg, sizeof(msg));
	FILE *f = NULL;

	ClearQue();
	if(SendToTarget(m_inputMsg, len, "Get Beidou2 almance start...", true) == 1)
	{
		if(WRL == NULL)
		{
			WRL = new CWaitReadLog;
		}
		AfxBeginThread(WaitLogRead,0);
		WaitForSingleObject(waitlog,INFINITE);
		WRL->SetWindowText("Waiting for get Beidou2 almanac...");
		WRL->msg.SetWindowText("Please wait for the Beidou2 almanac.");

    U08 NumsOfEphemeris = 0;
		fopen_s(&f, m_almanac_filename, "wb+");
		fclose(f);

		while(1)
		{	
			wait++;
			if(wait == 50)
			{ 
				Sleep(500);
				WRL->msg.SetWindowText("Retrieving the Beidou2 almanac data fails!");
				Sleep(500);
				WRL->IsFinish = true;
				add_msgtolist("Retrieving the Beidou2 almanac fails!");	
			}
			U08 buff[1024] = {0};
			res_len = m_serial->GetBinaryAck(buff, sizeof(buff));

			if(res_len==60)
			{
				fopen_s(&f, m_almanac_filename, "ab");
				fwrite(&buff[6],1,res_len-9,f);
				fclose(f);
			}
			else
			{
				break;
			}

			NumsOfEphemeris = buff[8];
			sprintf_s(BINMSG, sizeof(BINMSG), "Retrieving the Beidou2 satellite ID #%d almanac.", NumsOfEphemeris);
			WRL->msg.SetWindowText(BINMSG);

			if(NumsOfEphemeris==37)
				break;
		}	
		Sleep(500);
		WRL->msg.SetWindowText("Retrieving the Beidou2 almanac data complete.");
		Sleep(500);
		WRL->IsFinish = true;
		add_msgtolist("Retrieving the Beidou2 almanac successfully.");	
	}

	if(!continues)
	{
		SetMode();  
		CreateGPSThread();
	}
}

UINT GetBeidouAlmanacThread(LPVOID param)
{
	CGPSDlg::gpsDlg->GetBeidouAlmanac(m_almanac_filename, m_almanac_no, FALSE);
	return 0;
}

void CGPSDlg::OnGetBeidouAlmanac()
{
	if(!CheckConnect())	
	{
    return;
  }

	CGetAlmanac frm;
	frm.isGlonass = 2;
	if(frm.DoModal()==IDOK)
	{
		m_almanac_filename = frm.fileName;
		m_almanac_no = frm.sv;
		::AfxBeginThread(GetBeidouAlmanacThread, 0);
	}
	else
	{
		SetMode();  
		CreateGPSThread();
	}
}

void CGPSDlg::OnShowBeidouAlmanac()
{
	if(!CheckConnect())
	{
		return;
	}

	CGetAlmanac frm;
	frm.isGlonass = 2;
  frm.isDecode = 1;
	if(frm.DoModal()==IDOK)
	{
	  U08 no = frm.sv;
		QueryGetBeidouAlmanac(Display, &no);

		SetMode();
		CreateGPSThread();
	}
}

UINT SetBeidouAlmanacThread(LPVOID pParam)
{	
	CGPSDlg::gpsDlg->SetBeidouAlmanac(FALSE);	
	return 0;
}

void CGPSDlg::SetBeidouAlmanac(U08 continues)	
{	
	BYTE buffer[1000];
	ULONGLONG dwBytesRemaining = m_ephmsFile.GetLength();

	if(dwBytesRemaining == 0)
	{
		m_ephmsFile.Close();	
		if(!continues)
		{
			SetMode(); 
			CreateGPSThread();
		}	
		return;
	}

	m_ephmsFile.Read(buffer, 1);
	int one_entry_size = 51;

	if(dwBytesRemaining == 37 * one_entry_size)
	{
		m_ephmsFile.SeekToBegin();
		while(1)
		{
			UINT nBytesRead = m_ephmsFile.Read(buffer, one_entry_size);	
			if(nBytesRead <= 0)
			{
				break;
			}
			U08 msg[53] = { 0 };	//id, sub-id, svid-h, svid-l, 48 bytes almanac, attributes
			U08 messages[100] = { 0 };
			msg[0] = 0x67;
			msg[1] = 0x03;

			memcpy(&msg[2], buffer + 1, one_entry_size - 1);
			U16 SVID = buffer[2];	
			if(SVID == 0) 
			{
				continue;
			}

			if(continues)
			{
				msg[one_entry_size] = 0;
			}
			else
			{
				msg[one_entry_size] = (SVID == 37) ? 1 : 0;
      }

			int len = SetMessage2(messages, msg, sizeof(msg));
			sprintf_s(m_nmeaBuffer, sizeof(m_nmeaBuffer), "Set SV#%d Beidou2 almanac successfuly.", SVID);
			if(!SendToTargetNoWait(messages, len, m_nmeaBuffer))
			{
				sprintf_s(m_nmeaBuffer, sizeof(m_nmeaBuffer), "Set SV#%d Beidou2 almanac fails!", SVID);
				add_msgtolist(m_nmeaBuffer);
			}
		}
	}
	else
	{
		AfxMessageBox("File of the Beidou2 almanac data format wrong!");
		//return;
	}	
	m_ephmsFile.Close();	
	if(!continues)
	{
		SetMode(); 
		CreateGPSThread();
	}	
}

void CGPSDlg::OnSetBeidouAlmanac()
{
	if(!CheckConnect())
	{
		return;
	}

	SetInputMode(NoOutputMode);
	CString fileName("Beidou_Almanac.log");	
	CFileDialog dlgFile(true, _T("log"), fileName, OFN_HIDEREADONLY, _T("Almanac Files (*.log)|*.log||"), this);
	INT_PTR nResult = dlgFile.DoModal();
	fileName = dlgFile.GetPathName();	
	CFileException ef;
	try
	{
		if(nResult == IDOK)
		{  				
			if(!m_ephmsFile.Open(fileName,CFile::modeRead,&ef))
			{
				ef.ReportError();
				SetMode();
				CreateGPSThread();
				return;
			}				
			AfxBeginThread(SetBeidouAlmanacThread,0);			
		}		
		else
		{
			SetMode();
			CreateGPSThread();
		}
	}
	catch(CFileException *fe)
	{
		fe->ReportError();
		fe->Delete();
		return;
	}	
	fileName.ReleaseBuffer();		
}

UINT AFX_CDECL ConfigGnssDozeModeThread(LPVOID param)
{
	CGPSDlg::gpsDlg->ExecuteConfigureCommand(CGPSDlg::gpsDlg->m_inputMsg, 9, "ConfigGnssDozeMode successfully");
	return 0;
}

void CGPSDlg::OnConfigGnssDozeMode()
{
	if(!CheckConnect())
	{
		return;
	}

	SetInputMode(NoOutputMode);
	{
		U08 msg[2] = {0};
		msg[0] = 0x64;
		msg[1] = 0x1C;

		int len = SetMessage(msg, sizeof(msg));
        AfxBeginThread(ConfigGnssDozeModeThread, 0);
	}
}

#include "GpsdoDownload.h"
UINT DownloadThread(LPVOID pParam);
void CGPSDlg::OnGpsdoFirmwareDownload()
{
	if(!CheckConnect())
	{
		return;
	}

	SetInputMode(NoOutputMode);
	CGpsdoDownload dlg;
	if(IDOK != dlg.DoModal())
	{
		SetMode();  
		CreateGPSThread();
		return;
	}

	m_nDownloadBaudIdx = 7;
	m_nDownloadBufferIdx = 0;
	m_DownloadMode = GpsdoMasterSlave;
	m_strDownloadImage = dlg.m_strMasterPath;
	m_strDownloadImage2 = dlg.m_strSlavePath;
	m_nSlaveSourceBaud = dlg.m_slaveSourceBaud;
	m_nSlaveTargetBaud = dlg.m_slaveTargetBaud;

	::AfxBeginThread(DownloadThread, 0);
}

void CGPSDlg::DoCommonConfig(CCommonConfigDlg* dlg)
{
	if(!CheckConnect())
	{
		return;
	}

	SetInputMode(NoOutputMode);
	INT_PTR nResult = dlg->DoModal();
	if(nResult == IDOK) 
	{
		dlg->DoCommand();
	}
	else
	{
		SetMode();  
		CreateGPSThread();
	}
}

void CGPSDlg::DoCommonConfigNoDisconnect(CCommonConfigDlg* dlg)
{
	INT_PTR nResult = dlg->DoModal();
	if(nResult == IDOK) 
	{
		dlg->DoCommand();
	}
}

void CGPSDlg::DoCommonConfigDirect(CCommonConfigDlg* dlg, int type)
{
	if(!CheckConnect())
	{
		return;
	}

	SetInputMode(NoOutputMode);
	INT_PTR nResult = dlg->DoDirect(type);
	if(nResult == IDOK) 
	{
		dlg->DoCommand();
	}
	else
	{
		SetMode();  
		CreateGPSThread();
	}
}

void CGPSDlg::OnBinaryConfigureQZSS()
{
	CConfigQZSS dlg;
	DoCommonConfig(&dlg);
}

void CGPSDlg::OnBinaryConfigureDGPS()
{
	CConfigDGPS dlg;
	DoCommonConfig(&dlg);
}

void CGPSDlg::OnBinaryConfigureSmoothMode()
{
	CConfigSmoothMode dlg;
	DoCommonConfig(&dlg);
}

void CGPSDlg::OnBinaryConfigTimeStamping()
{
	CConfigTimeStamping dlg;
	DoCommonConfig(&dlg);
}

void CGPSDlg::OnConfigLeapSeconds()
{
	CConfigLeapSeconds dlg;
	DoCommonConfig(&dlg);
}

void CGPSDlg::OnConfigParamSearchEngineSleepCriteria()
{
	CConfigParamSearchEngineSleepCriteria dlg;
	DoCommonConfig(&dlg);
}

void CGPSDlg::OnConfigDatumIndex()
{
	CConfigDatumIndex dlg;
	DoCommonConfig(&dlg);
}

void CGPSDlg::OnConfigVeryLowSpeed()
{
	CConfigVeryLowSpeed dlg;
	DoCommonConfig(&dlg);
}

void CGPSDlg::OnConfigDofunUniqueId()
{
	CConfigDofunUniqueId dlg;
	DoCommonConfig(&dlg);
}

void CGPSDlg::OnEraseDofunUniqueId()
{
	CConfigDofunUniqueId dlg;
	DoCommonConfigDirect(&dlg, 0);
}

void CGPSDlg::OnBinaryConfigureSBAS()
{
	CConfigSBAS dlg;
	DoCommonConfig(&dlg);
}

void CGPSDlg::OnBinaryConfigureSBAS2()
{
	CConfigSBAS2 dlg;
	DoCommonConfig(&dlg);
}

void CGPSDlg::OnBinaryConfigureSAGPS()
{
	CConfigSAEE dlg;
	DoCommonConfig(&dlg);
}

void CGPSDlg::OnConfigureInterferenceDetectControl()
{
	CConfigInterferenceDetectControl dlg;
	DoCommonConfig(&dlg);
}

void CGPSDlg::OnConfigNMEABinaryOutputDestination()
{
	CConfigNMEABinaryOutputDestination dlg;
	DoCommonConfig(&dlg);
}

void CGPSDlg::OnConfigParameterSearchEngineNumber()
{
	CConfigParameterSearchEngineNumber dlg;
	DoCommonConfig(&dlg);
}

void CGPSDlg::OnConfigPositionFixNavigationMask()
{
	CConfigPositionFixNavigationMask dlg;
	DoCommonConfig(&dlg);
}

void CGPSDlg::OnConfigRefTimeSyncToGpsTime()
{
	ConfigRefTimeToGpsTimeDlg dlg;
	DoCommonConfig(&dlg);
}

void CGPSDlg::OnConfigQueryGnssNavSol()
{
	ConfigGnssConstellationTypeDlg dlg;
	DoCommonConfig(&dlg);
}

void CGPSDlg::OnConfigBinaryMeasurementDataOut()
{
	ConfigBinaryMeasurementDataOutDlg dlg;
	DoCommonConfig(&dlg);
}

void CGPSDlg::OnConfigRtcmMeasurementDataOut()
{
	ConfigRtcmMeasurementDataOutDlg dlg;
	DoCommonConfig(&dlg);
}

void CGPSDlg::OnBinaryConfigurepowermode()
{
	CConfigPowerMode dlg;
	DoCommonConfig(&dlg);
}

void CGPSDlg::OnSup800EraseData()
{
	CSUP800EraseUserDataDlg dlg;
	DoCommonConfig(&dlg);
}

void CGPSDlg::OnSup800WriteData()
{
	CSUP800WriteUserDataDlg dlg;
	DoCommonConfig(&dlg);
}

void CGPSDlg::OnSup800ReadData()
{
	CSUP800ReadUserDataDlg dlg;
	DoCommonConfig(&dlg);
}

void CGPSDlg::OnConfigureSignalDisturbanceStatus()
{
	CConfigureSignalDisturbanceStatusDlg dlg;
	DoCommonConfig(&dlg);
}

void CGPSDlg::OnConfigureGpsUtcLeapSecondsInUtc()
{
	CConfigureGpsUtcLeapSecondsInUtcDlg dlg;
	DoCommonConfig(&dlg);
}

void CGPSDlg::OnConfigureNoisePowerControl()
{
	CConfigNoisePowerControlDlg dlg;
	DoCommonConfig(&dlg);
}
/*
void CGPSDlg::OnConfigPowerSavingParametersRom()
{
	ConfigPowerSavingParametersRomDlg dlg;
	dlg.SetRomMode(true);
	DoCommonConfig(&dlg);
}
*/
void CGPSDlg::OnConfigPowerSavingParameters()
{
	ConfigPowerSavingParametersRomDlg dlg;
	DoCommonConfig(&dlg);
}

void CGPSDlg::OnIqPlot()
{
	CIqPlot dlg;
	DoCommonConfig(&dlg);
}

void CGPSDlg::OnConfigGeofence()
{
	CConfigGeofencing dlg;
	DoCommonConfig(&dlg);
}

void CGPSDlg::OnConfigGeofence1()
{
	CConfigGeofencing dlg;
	dlg.SetDataNo(1);
	DoCommonConfig(&dlg);
}

void CGPSDlg::OnConfigGeofence2()
{
	CConfigGeofencing dlg;
	dlg.SetDataNo(2);
	DoCommonConfig(&dlg);
}

void CGPSDlg::OnConfigGeofence3()
{
	CConfigGeofencing dlg;
	dlg.SetDataNo(3);
	DoCommonConfig(&dlg);
}

void CGPSDlg::OnConfigGeofence4()
{
	CConfigGeofencing dlg;
	dlg.SetDataNo(4);
	DoCommonConfig(&dlg);
}

void CGPSDlg::OnConfigRtkMode()
{
	CConfigRtkMode dlg;
	DoCommonConfig(&dlg);
}

void CGPSDlg::OnConfigRtkParameters()
{
	CConfigRtkParameters dlg;
	DoCommonConfig(&dlg);
}

void CGPSDlg::OnRtkReset()
{
	CConfigRtkReset dlg;
	DoCommonConfig(&dlg);
}

void CGPSDlg::OnConfigRtkMode2()
{
	CConfigRtkMode2 dlg;
	DoCommonConfig(&dlg);
}

void CGPSDlg::OnConfigBasePosition()
{
	CConfigRtkMode2 dlg;
  dlg.SetCommandMode(CConfigRtkMode2::CfgBasePosition);
	DoCommonConfig(&dlg);
}

void CGPSDlg::OnConfigMessageOut()
{
	CConfigMessageOut dlg;
	DoCommonConfig(&dlg);
}

void CGPSDlg::OnConfigSubSecRegister()
{
	CConfigSubSecRegister dlg;
	DoCommonConfig(&dlg);
}

void CGPSDlg::OnConfigTiming()
{
	//CConfigTiming dlg;
	CConfigRtkMode2 dlg;
  dlg.SetCommandMode(CConfigRtkMode2::CfgTiming);
	DoCommonConfig(&dlg);
}

void CGPSDlg::OnConfigTimingCableDelay()
{
	CConfigTimingCableDelay dlg;
	DoCommonConfig(&dlg);
}

void CGPSDlg::OnConfigGpsMeasurementMode()
{
	CConfigGpsMeasurementMode dlg;
	DoCommonConfig(&dlg);
}

void CGPSDlg::OnConfigPstmDeviceAddress()
{
	CConfigPscmDeviceAddress dlg;
	DoCommonConfig(&dlg);
}

void CGPSDlg::OnConfigPstmLatLonDigits()
{
	CConfigPscmLatLonFractionalDigits dlg;
	DoCommonConfig(&dlg);
}

void CGPSDlg::OnConfigPsti030()
{
	CConfigPstiInterval dlg;
  dlg.SetPsti(30);
	DoCommonConfig(&dlg);
}

void CGPSDlg::OnConfigPsti032()
{
	CConfigPstiInterval dlg;
  dlg.SetPsti(32);
	DoCommonConfig(&dlg);
}

void CGPSDlg::OnConfigPsti004()
{
	CConfigPstiInterval dlg;
  dlg.SetPsti(04);
	DoCommonConfig(&dlg);
}

void CGPSDlg::OnRtkOnOffGpsSv()
{
	CRtkOnOffSv dlg;
  dlg.SetSvMode(CRtkOnOffSv::RtkSvGps);
	DoCommonConfigNoDisconnect(&dlg);
}

void CGPSDlg::OnRtkOnOffSbasQzssSv()
{
	CRtkOnOffSv dlg;
  dlg.SetSvMode(CRtkOnOffSv::RtkSvSbasQzss);
	DoCommonConfigNoDisconnect(&dlg);
}

void CGPSDlg::OnRtkOnOffGlonassSv()
{
	CRtkOnOffSv dlg;
  dlg.SetSvMode(CRtkOnOffSv::RtkSvGlonass);
	DoCommonConfigNoDisconnect(&dlg);
}

void CGPSDlg::OnRtkOnOffBeidouSv()
{
	CRtkOnOffSv dlg;
  dlg.SetSvMode(CRtkOnOffSv::RtkSvBeidou2);
	DoCommonConfigNoDisconnect(&dlg);
}

void CGPSDlg::OnQueryRtkReferencePosition()
{
	CConfigRtkReferencePosition dlg;
	DoCommonConfig(&dlg);
}

void CGPSDlg::OnInsdrTest()
{
	if(!CheckConnect())
	{
		return;
	}

	DrTestDlg dlg;
  dlg.DoModal();

	SetMode();
	CreateGPSThread();
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryVeryLowSpeed(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QueryVeryLowSpeedCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryVeryLowSpeedCmd].cmdId);
	cmd.SetU08(2, cmdTable[QueryVeryLowSpeedCmd].cmdSubId);

	BinaryData ackCmd;
	if(Ack == ExcuteBinaryCommand(QueryVeryLowSpeedCmd, &cmd, &ackCmd))
	{
		if(nMode==Return)
		{
			*((U08*)outputData) = ackCmd[6];
			return Ack;
		}

		CString strMsg = "Query kernel very low speed successfully";
		add_msgtolist(strMsg);
		strMsg = "Kernel Very low speed: ";
		add_msgtolist(strMsg);
		if(0==(ackCmd[6]))
		{
			strMsg = "Disable";
		}
		else if(1==(ackCmd[6]))
		{
			strMsg = "Enable";
		}
		add_msgtolist(strMsg);
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryDofunUniqueId(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QueryDofunUniqueIdCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryDofunUniqueIdCmd].cmdId);
	cmd.SetU08(2, cmdTable[QueryDofunUniqueIdCmd].cmdSubId);
	cmd.SetU08(3, 0x04);

	BinaryData ackCmd;
	if(Ack == ExcuteBinaryCommand(QueryDofunUniqueIdCmd, &cmd, &ackCmd))
	{
		if(nMode==Return)
		{
			*((U08*)outputData) = ackCmd[6];
			return Ack;
		}

		CString strMsg = "Query device uinque ID successfully";
		add_msgtolist(strMsg);
		strMsg = "ID: ";
		for(int i = 0; i < 16; ++i)
		{
			strMsg += ackCmd[i + 7];
		}
		add_msgtolist(strMsg);

		strMsg = "HEX: ";
		for(int i = 0; i < 8; ++i)
		{
			CString s;
			s.Format("%02X", ackCmd[i + 7]);
			strMsg += s;
			strMsg += (i != 7) ? " " : "-";
		}
		add_msgtolist(strMsg);
		strMsg = "     ";
		for(int i = 8; i < 16; ++i)
		{
			CString s;
			s.Format("%02X", ackCmd[i + 7]);
			strMsg += s;
			strMsg += (i != 15) ? " " : "";
		}
		add_msgtolist(strMsg);
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::ReCalcuteGlonassIfb(CmdExeMode nMode, void* outputData)
{	    
	BinaryCommand cmd(2);
	cmd.SetU08(1, 0x6A);
	cmd.SetU08(2, 0x0A);

	ClearQue();
	SendToTarget(cmd.GetBuffer(), cmd.Size(), "Re-calculate GLONASS IFB successfully", true);	
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::BinaryQueryClockOffset(CmdExeMode nMode, void* outputData)
{

  U32 temp = m_regAddress;
  U32 runTimeClkOft = 0, sramClkOft = 0, flashClkOst = 0;

  m_regAddress = 0x01;  //Runtime clock offset address
	CmdErrorCode ack = CGPSDlg::gpsDlg->QueryRegister(CGPSDlg::Return, &runTimeClkOft);
  if(Ack != ack)
  {
	  return Timeout;
  }
  m_regAddress = 0x02;  //SRAM clock offset address
	ack = CGPSDlg::gpsDlg->QueryRegister(CGPSDlg::Return, &sramClkOft);
  if(Ack != ack)
  {
	  return Timeout;
  }

  m_regAddress = 0xcdf;  //SRAM clock offset address
	ack = CGPSDlg::gpsDlg->QueryRegister(CGPSDlg::Return, &flashClkOst);
  if(Ack != ack)
  {
	  return Timeout;
  }

	CString strMsg = "Query clock offset succeeded";
	add_msgtolist(strMsg);
	strMsg.Format("Runtime Clock Offset: %d", (S32)runTimeClkOft);
	add_msgtolist(strMsg);
	strMsg.Format("SRAM Clock Offset: %d", (S32)sramClkOft);
	add_msgtolist(strMsg);
	strMsg.Format("FALSH Clock Offset: %d", (S32)flashClkOst);
	add_msgtolist(strMsg);
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::InsdrAccelerometerSelfTest(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[InsdrAccelerometerSelfTestCmd].cmdSize);
	cmd.SetU08(1, cmdTable[InsdrAccelerometerSelfTestCmd].cmdId);
	cmd.SetU08(2, cmdTable[InsdrAccelerometerSelfTestCmd].cmdSubId);

	BinaryData ackCmd;
	CmdErrorCode ack = ExcuteBinaryCommand(InsdrAccelerometerSelfTestCmd, &cmd, &ackCmd);
  if(Ack != ack)
  {
	  return Timeout;
  }

	if(nMode == Return)
	{
		*((U08*)outputData) = ackCmd[6];
		return ack;
	}

	CString strMsg = "INS DR accelerometer self-test successfully";
	add_msgtolist(strMsg);

  strMsg.Format("Test Status:%s",(ackCmd[6]) ? "PASS" : "FAILED");
	add_msgtolist(strMsg);

  strMsg.Format("POS X:%f", ConvertLeonFloat(ackCmd.Ptr(7)));
	add_msgtolist(strMsg);

  strMsg.Format("POS Y:%f", ConvertLeonFloat(ackCmd.Ptr(11)));
	add_msgtolist(strMsg);

  strMsg.Format("POS Z:%f", ConvertLeonFloat(ackCmd.Ptr(15)));
	add_msgtolist(strMsg);

  strMsg.Format("NEG X:%f", ConvertLeonFloat(ackCmd.Ptr(19)));
	add_msgtolist(strMsg);

  strMsg.Format("NEG Y:%f", ConvertLeonFloat(ackCmd.Ptr(23)));
	add_msgtolist(strMsg);

  strMsg.Format("NEG Z:%f", ConvertLeonFloat(ackCmd.Ptr(27)));
	add_msgtolist(strMsg);

	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::InsdrGyroscopeSelfTest(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[InsdrGyroscopeSelfTestCmd].cmdSize);
	cmd.SetU08(1, cmdTable[InsdrGyroscopeSelfTestCmd].cmdId);
	cmd.SetU08(2, cmdTable[InsdrGyroscopeSelfTestCmd].cmdSubId);

	BinaryData ackCmd;
	CmdErrorCode ack = ExcuteBinaryCommand(InsdrGyroscopeSelfTestCmd, &cmd, &ackCmd);
  if(Ack != ack)
  {
	  return Timeout;
  }

	if(nMode == Return)
	{
		*((U08*)outputData) = ackCmd[6];
		return ack;
	}

	CString strMsg = "INS DR gyroscope self-test successfully";
	add_msgtolist(strMsg);

  strMsg.Format("Test Status:%s",(ackCmd[6]) ? "PASS" : "FAILED");
	add_msgtolist(strMsg);

	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::InsdrAccumulateAngleStart(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[InsdrAccumulateAngleStartCmd].cmdSize);
	cmd.SetU08(1, cmdTable[InsdrAccumulateAngleStartCmd].cmdId);
	cmd.SetU08(2, cmdTable[InsdrAccumulateAngleStartCmd].cmdSubId);

	BinaryData ackCmd;
	CmdErrorCode ack = ExcuteBinaryCommand(InsdrAccumulateAngleStartCmd, &cmd, &ackCmd);
  if(Ack != ack)
  {
	  return Timeout;
  }

	if(nMode == Return)
	{
		*((U08*)outputData) = ackCmd[6];
		return ack;
	}

	CString strMsg = "INS DR accumulate angle start successfully";
	add_msgtolist(strMsg);

  strMsg.Format("Status:%s",(ackCmd[6]) ? "OK" : "FAILED");
	add_msgtolist(strMsg);

	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::InsdrAccumulateAngleStop(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[InsdrAccumulateAngleStopCmd].cmdSize);
	cmd.SetU08(1, cmdTable[InsdrAccumulateAngleStopCmd].cmdId);
	cmd.SetU08(2, cmdTable[InsdrAccumulateAngleStopCmd].cmdSubId);

	BinaryData ackCmd;
	CmdErrorCode ack = ExcuteBinaryCommand(InsdrAccumulateAngleStopCmd, &cmd, &ackCmd);
  if(Ack != ack)
  {
	  return Timeout;
  }

	if(nMode == Return)
	{
		*((U08*)outputData) = ackCmd[6];
		return ack;
	}

	CString strMsg = "INS DR accumulate angle stop successfully";
	add_msgtolist(strMsg);

  strMsg.Format("Status:%s",(ackCmd[6]) ? "OK" : "FAILED");
	add_msgtolist(strMsg);

  strMsg.Format("ANGLE X:%f", ConvertLeonFloat(ackCmd.Ptr(7)));
	add_msgtolist(strMsg);

  strMsg.Format("ANGLE Y:%f", ConvertLeonFloat(ackCmd.Ptr(11)));
	add_msgtolist(strMsg);

  strMsg.Format("ANGLE Z:%f", ConvertLeonFloat(ackCmd.Ptr(15)));
	add_msgtolist(strMsg);

	return Timeout;
}

#if defined(XN120_TESTER)
void CGPSDlg::DoXn120Tester()
{
  if(m_nXn120TestSatus != 0)
  {
    ::AfxMessageBox("The test is still in progress!");
		return;
  }

	if(!CheckConnect())
	{
		return;
	}

  m_strXn120Version = "";
  m_nXn120TestSatus = 1;
  QuerySoftwareVersionSystemCode(Return, &m_strXn120Version);
  //::AfxMessageBox(strVersion);

	SetInputMode(NoOutputMode);
	m_restartMode = 3;	
	AfxBeginThread(RestartThread, 0);
}
#endif

