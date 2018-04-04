#pragma once


// CDlgInvRad dialog

class CDlgInvRad : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgInvRad)

public:
	CDlgInvRad(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgInvRad();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_InvRad };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_nRad;
};
