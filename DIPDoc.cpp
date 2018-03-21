
// DIPDoc.cpp : CDIPDoc ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "DIP.h"
#endif

#include "DIPDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CDIPDoc

IMPLEMENT_DYNCREATE(CDIPDoc, CDocument)

BEGIN_MESSAGE_MAP(CDIPDoc, CDocument)
	
END_MESSAGE_MAP()


// CDIPDoc ����/����

CDIPDoc::CDIPDoc()
{
	// TODO: �ڴ����һ���Թ������
	m_sizeDoc = CSize(1, 1);
	m_pPalette = NULL;

}

CDIPDoc::~CDIPDoc()
{
	if (m_pPalette != NULL)
		delete m_pPalette;
}

BOOL CDIPDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)
	//SetModifiedFlag(FALSE);

	return TRUE;
}

void CDIPDoc::Init()
{
	if (!m_Image.IsValidate())
	{
		return;
	}

	m_sizeDoc = CSize(m_Image.GetWidthPixel(), m_Image.GetHeight());

	if (m_pPalette != NULL)
	{
		delete m_pPalette;
		m_pPalette = NULL;
	}

	CClientDC dc(NULL);
	if ((dc.GetDeviceCaps(RASTERCAPS) & RC_PALETTE) == 0)
		return;

	m_pPalette = new CPalette;

	int nEntries = m_Image.GetColorTableEntriesNum();
	if (nEntries == 0)
		return;

	LOGPALETTE* lpPal = (LOGPALETTE*) new BYTE[sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY)*(nEntries - 1))];

	lpPal->palVersion = 0x300;
	lpPal->palNumEntries = (WORD)nEntries;

	RGBQUAD *pPal = (RGBQUAD*)(m_Image.GetColorTable());

	int i;
	for (i = 0; i < nEntries; i++)
	{
		lpPal->palPalEntry[i].peRed = pPal[i].rgbRed;
		lpPal->palPalEntry[i].peBlue = pPal[i].rgbBlue;
		lpPal->palPalEntry[i].peGreen = pPal[i].rgbGreen;
		lpPal->palPalEntry[i].peFlags = 0;
	}

	m_pPalette->CreatePalette(lpPal);
	
	delete lpPal;
}

void CDIPDoc::DeleteContents()
{
	if (m_pPalette != NULL)
		m_pPalette->DeleteObject();

	CDocument::DeleteContents();
}

BOOL CDIPDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	DeleteContents();

	BeginWaitCursor();

	if (!m_Image.AttachFromFile(lpszPathName))
	{
		EndWaitCursor();
		AfxMessageBox(L"���ļ�ʱ������ȷ����ȷ��λͼ(*.bmp)�ļ����͡�");
		return false;
	}

	EndWaitCursor();

	if (!m_Image.m_lpData)
	{
		CString strMsg;
		strMsg = L"��ȡͼ��ʱ���������ǲ�֧�ָ����͵�ͼ���ļ���";

		MessageBox(NULL, strMsg, L"ϵͳ��ʾ", MB_ICONINFORMATION | MB_OK);
		return false;
	}

	Init();

	SetPathName(lpszPathName);

	m_OImage = m_Image;

	SetModifiedFlag(FALSE);

	return true;
}

BOOL CDIPDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	if (!m_Image.SaveToFile(lpszPathName))
	{
		CString strMsg;
		strMsg = L"�޷�����BMPͼ��";

		MessageBox(NULL, strMsg, L"ϵͳ��ʾ", MB_ICONINFORMATION | MB_OK);
		return false;
	}

	EndWaitCursor();
	SetModifiedFlag(FALSE);

	return TRUE;
}

//BOOL CDIPDoc::CanCloseFrame()


// CDIPDoc ���л�

void CDIPDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}

#ifdef SHARED_HANDLERS

// ����ͼ��֧��
void CDIPDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// �޸Ĵ˴����Ի����ĵ�����
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// ������������֧��
void CDIPDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ���ĵ����������������ݡ�
	// ���ݲ���Ӧ�ɡ�;���ָ�

	// ����:     strSearchContent = _T("point;rectangle;circle;ole object;")��
	SetSearchContent(strSearchContent);
}

void CDIPDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CDIPDoc ���

#ifdef _DEBUG
void CDIPDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDIPDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CDIPDoc ����
