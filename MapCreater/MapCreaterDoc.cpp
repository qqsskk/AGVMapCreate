
// MapCreaterDoc.cpp: CMapCreaterDoc 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "MapCreater.h"
#endif

#include "MapCreaterDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMapCreaterDoc

IMPLEMENT_DYNCREATE(CMapCreaterDoc, CDocument)

BEGIN_MESSAGE_MAP(CMapCreaterDoc, CDocument)
	ON_COMMAND(ID_FILE_SEND_MAIL, &CMapCreaterDoc::OnFileSendMail)
	ON_UPDATE_COMMAND_UI(ID_FILE_SEND_MAIL, &CMapCreaterDoc::OnUpdateFileSendMail)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CMapCreaterDoc, CDocument)
END_DISPATCH_MAP()

// 注意: 我们添加了对 IID_IMapCreater 的支持来支持类型安全绑定
//  以支持来自 VBA 的类型安全绑定。  此 IID 必须同附加到 .IDL 文件中的
//  调度接口的 GUID 匹配。

// {7997ce74-f682-425d-ac6b-7ef5ff93b0b9}
static const IID IID_IMapCreater =
{0x7997ce74,0xf682,0x425d,{0xac,0x6b,0x7e,0xf5,0xff,0x93,0xb0,0xb9}};

BEGIN_INTERFACE_MAP(CMapCreaterDoc, CDocument)
	INTERFACE_PART(CMapCreaterDoc, IID_IMapCreater, Dispatch)
END_INTERFACE_MAP()


// CMapCreaterDoc 构造/析构

CMapCreaterDoc::CMapCreaterDoc() noexcept
{
	// TODO: 在此添加一次性构造代码

	EnableAutomation();

	AfxOleLockApp();
}

CMapCreaterDoc::~CMapCreaterDoc()
{
	AfxOleUnlockApp();
}

BOOL CMapCreaterDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CMapCreaterDoc 序列化

void CMapCreaterDoc::Serialize(CArchive& ar)
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
void CMapCreaterDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
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
void CMapCreaterDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:     strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}

void CMapCreaterDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CMapCreaterDoc 诊断

#ifdef _DEBUG
void CMapCreaterDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMapCreaterDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMapCreaterDoc 命令
