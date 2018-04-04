#pragma once


// CDlgFreqLPF dialog

class CDlgFreqLPF : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgFreqLPF)

public:
	CDlgFreqLPF(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgFreqLPF();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_LPF };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double m_dFreq;
};
