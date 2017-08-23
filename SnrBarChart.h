#pragma once

struct GPGSA;
struct GPGSV;
struct GPGGA;
struct Satellite;
class Satellites;
class CMaskedBitmap;
class UISetting;

class CSnrBarChart : public CStatic
{
public:
	enum SnrBarChartMode
	{
		Gps,
		Gnss,
		Beidou,
		Galileo
	};

	CSnrBarChart(void);
	~CSnrBarChart(void);

	void SetGsvData(GPGSV* gsv) { gsvData = gsv; };
	void SetGsaData(GPGSA* gsa) { gsaData = gsa; };
	void SetGgaData(GPGGA* gga) { ggaData = gga; };
	void SetSateStatus(Satellites* sata) { sateStatus = sata; };
//	void SetSnrBarChartMode(SnrBarChartMode mode) { barChartMode = mode; };
	void SetUISetting(UISetting* p) { uiSetting = p; }
	static void DrawBarChartId(CDC *dc, UISetting* s, bool isInUse, int x, int y, int id, bool inCenter = false);
	void SetCustomerID(U32 id) { customerID = id; };
protected:
	GPGSV* gsvData;
	GPGSA* gsaData;
	GPGGA* ggaData;
	Satellites* sateStatus;
	UISetting* uiSetting;
	U32	customerID;

	int m_minId;
	int m_maxId;	CString m_titleText;

	CPoint m_startBarChart;

	virtual void ShowBoxChart(CDC *dc);
	virtual void DrawSnr(CDC *dc, int& startId, UISetting* s, GPGSV* gsv, GPGSA* gsa, 
				GPGGA* gga, Satellites* sate);
	//void DrawSnrInRange(CDC *dc, int& startId, UISetting* s, GPGSV* gsv, 
	//			GPGSA* gsa, GPGGA* gga, Satellite* sate, int start, int end);

	void RefreshBarChart();
	void CreateBarChart(CDC *dc);


	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()
};

class CSnrBarChartGps : public CSnrBarChart
{
public:
	CSnrBarChartGps(void);
	~CSnrBarChartGps(void);

protected:
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()
};

class CSnrBarChartGlonass : public CSnrBarChart
{
public:
	CSnrBarChartGlonass(void);
	~CSnrBarChartGlonass(void);

protected:
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()
};


class CSnrBarChartDual : public CSnrBarChart
{
public:
	CSnrBarChartDual();
	~CSnrBarChartDual(void);

	void SetGsvData2(GPGSV* gsv) { gsvData2 = gsv; };
	void SetGsaData2(GPGSA* gsa) { gsaData2 = gsa; };
	void SetGgaData2(GPGGA* gga) { ggaData2 = gga; };
	void SetSateStatus2(Satellites* sata) { sateStatus2 = sata; };
	void SetUISetting2(UISetting* p) { uiSetting2 = p; }

protected:
	GPGSV* gsvData2;
	GPGSA* gsaData2;
	GPGGA* ggaData2;
	Satellites* sateStatus2;
	UISetting* uiSetting2;

	virtual void ShowBoxChart(CDC *dc);
	//void DrawGpsSnr(CDC *dc, int& end);
	//void DrawGlonassSnr(CDC *dc, int& start);
	//bool CheckInUse2(int id);

	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()
};

class CSnrBarChartL2 : public CSnrBarChart
{
public:
	CSnrBarChartL2();
	CSnrBarChartL2(int type);
	~CSnrBarChartL2(void);

	//void SetGsvDataSub(GPGSV* gsv) { gsvDataSub = gsv; };
	//void SetGsaDataSub(GPGSA* gsa) { gsaDataSub = gsa; };
	//void SetGgaDataSub(GPGGA* gga) { ggaDataSub = gga; };
	//void SetSateStatusSub(Satellite* sata) { sateStatusSub = sata; };
	void SetUISettingSub(UISetting* p) { uiSettingSub = p; }

protected:
  void Init(int t);
	virtual void ShowBoxChart(CDC *dc);
  virtual void DrawSnr(CDC *dc, int& startId, UISetting* s, UISetting* sSub, 
        GPGSV* gsv, GPGSA* gsa, GPGGA* gga, Satellites* sate);

//#if (FLOAT_SNR)
//  F32 GetGsvSubSnr(Satellite* s, int id);
//#else
//  int GetGsvSubSnr(Satellite* s,int id);
//#endif

	//GPGSV* gsvDataSub;
	//GPGSA* gsaDataSub;
	//GPGGA* ggaDataSub;
	//Satellite* sateStatusSub;
	UISetting* uiSettingSub;

	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()
};

class CSnrBarChartBeidou : public CSnrBarChart
{
public:
	CSnrBarChartBeidou(void);
	~CSnrBarChartBeidou(void);

protected:
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()
};

class CSnrBarChartGalileo : public CSnrBarChart
{
public:
	CSnrBarChartGalileo(void);
	~CSnrBarChartGalileo(void);



protected:
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()
};

class CSnrBarChartDualL2 : public CSnrBarChartL2
{
public:
	CSnrBarChartDualL2();
	~CSnrBarChartDualL2(void);

	void SetGsvData2(GPGSV* gsv) { gsvData2 = gsv; };
	void SetGsaData2(GPGSA* gsa) { gsaData2 = gsa; };
	void SetGgaData2(GPGGA* gga) { ggaData2 = gga; };
	void SetSateStatus2(Satellites* sata) { sateStatus2 = sata; };
	void SetUISetting2(UISetting* p) { uiSetting2 = p; }
  void SetUISetting2Sub(UISetting* p) { uiSetting2Sub = p; }
protected:
	GPGSV* gsvData2;
	GPGSA* gsaData2;
	GPGGA* ggaData2;
	Satellites* sateStatus2;
	UISetting* uiSetting2;
	UISetting* uiSetting2Sub;

protected:
	virtual void ShowBoxChart(CDC *dc);\
  void Init();
	//virtual void DrawSnr(CDC *dc, int& startId, UISetting* s, GPGSV* gsv, GPGSA* gsa, 
	//			GPGGA* gga, Satellite* sate);
#if (FLOAT_SNR)
  F32 GetGsv2SubSnr(int id);
#else
  int GetGsv2SubSnr(int id);
#endif

	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()
};