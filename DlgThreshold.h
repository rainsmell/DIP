#pragma once


// CDlgThreshold dialog

class CDlgThreshold : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgThreshold)

public:
	CDlgThreshold(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgThreshold();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double m_dThre;
};
