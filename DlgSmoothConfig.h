#pragma once


// CDlgSmoothConfig dialog

class CDlgSmoothConfig : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSmoothConfig)

public:
	CDlgSmoothConfig(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSmoothConfig();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_Smooth };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	void UpdateEdit();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRadioAvg();
	int m_intType;
	
	int m_iTempH;
	int m_iTempW;
	int m_iTempMX;
	int m_iTempMY;

	float* m_fpArray;
	float m_fTempC;
	virtual void OnOK();
	afx_msg void OnBnClickedRadioGauss();
};
