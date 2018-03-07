#pragma once


// CDlgLinerPara dialog

class CDlgLinerPara : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgLinerPara)

public:
	CDlgLinerPara(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgLinerPara();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_LinerPara };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
//	CString m_fA;
	float m_fA;
	float m_fB;

	CRect m_MouseRect;

	BOOL m_bIsDraging;

	BOOL m_bDrawed;

	CPoint m_p1;
	CPoint m_p2;

	afx_msg void OnEnKillfocusEditA();
	afx_msg void OnEnKillfocusEditB();
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};
