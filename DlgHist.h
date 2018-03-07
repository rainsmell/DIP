#pragma once


// CDlgHist dialog

class CDlgHist : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgHist)

public:
	CDlgHist(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgHist();


// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_Hist };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	double* m_pdHist;
};
