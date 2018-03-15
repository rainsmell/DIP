#pragma once


// CDlgGamma dialog

class CDlgGamma : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgGamma)

public:
	CDlgGamma(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgGamma();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_Gamma };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double m_dEsp;
	double m_dGamma;
};
