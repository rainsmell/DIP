
// DIPDoc.cpp : CDIPDoc 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
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


// CDIPDoc 构造/析构

CDIPDoc::CDIPDoc()
{
	// TODO: 在此添加一次性构造代码
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

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)
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
		AfxMessageBox(L"打开文件时出错！请确保正确的位图(*.bmp)文件类型。");
		return false;
	}

	EndWaitCursor();

	if (!m_Image.m_lpData)
	{
		CString strMsg;
		strMsg = L"读取图像时出错！可能是不支持该类型的图像文件！";

		MessageBox(NULL, strMsg, L"系统提示", MB_ICONINFORMATION | MB_OK);
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
		strMsg = L"无法保存BMP图像";

		MessageBox(NULL, strMsg, L"系统提示", MB_ICONINFORMATION | MB_OK);
		return false;
	}

	EndWaitCursor();
	SetModifiedFlag(FALSE);

	return TRUE;
}

//BOOL CDIPDoc::CanCloseFrame()


// CDIPDoc 序列化

void CDIPDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void CDIPDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
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

// 搜索处理程序的支持
void CDIPDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:     strSearchContent = _T("point;rectangle;circle;ole object;")；
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

// CDIPDoc 诊断

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


// CDIPDoc 命令
