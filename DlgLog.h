#pragma once


// DlgLog dialog

class DlgLog : public CDialogEx
{
	DECLARE_DYNAMIC(DlgLog)

public:
	DlgLog(CWnd* pParent = NULL);   // standard constructor
	virtual ~DlgLog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_LOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double m_dC;
};
