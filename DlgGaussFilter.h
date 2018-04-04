#pragma once


// CDlgGaussFilter dialog

class CDlgGaussFilter : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgGaussFilter)

public:
	CDlgGaussFilter(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgGaussFilter();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_GaussFilter };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double m_dSigma;
};
