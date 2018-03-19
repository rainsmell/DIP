#pragma once


// CDlgGemoScale dialog

class CDlgGemoScale : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgGemoScale)

public:
	CDlgGemoScale(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgGemoScale();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_Scale };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double m_dTimes;
};
