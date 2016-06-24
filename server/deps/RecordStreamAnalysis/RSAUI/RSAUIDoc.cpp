// RSAUIDoc.cpp : CRSAUIDoc 类的实现
//

#include "stdafx.h"
#include "RSAUI.h"

#include "RSAUIDoc.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRSAUIDoc

IMPLEMENT_DYNCREATE(CRSAUIDoc, CDocument)

BEGIN_MESSAGE_MAP(CRSAUIDoc, CDocument)
	ON_COMMAND(ID_FILE_OPEN, &CRSAUIDoc::OnFileOpen)
END_MESSAGE_MAP()


// CRSAUIDoc 构造/析构

CRSAUIDoc::CRSAUIDoc()
:m_notify(1)
{
	// TODO: 在此添加一次性构造代码

}

CRSAUIDoc::~CRSAUIDoc()
{
}

BOOL CRSAUIDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	SetTitle(_T("RecordStreamAnalysis"));

	return TRUE;
}




// CRSAUIDoc 序列化

void CRSAUIDoc::Serialize(CArchive& ar)
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

void CRSAUIDoc::Load()
{
	FILES_MAP::iterator it = m_files.begin();
	for(; it != m_files.end(); ++it)
	{
		m_manager.LoadDatabase(UnicodeToMbcs(it->second).c_str());
	}
}

void CRSAUIDoc::Notify()
{	
	m_manager.Wait();
	DispatchMsg(UM_LOADFINISH, 0, 0);
}

void CRSAUIDoc::DispatchMsg(UINT message, WPARAM wparam, LPARAM lparam)
{
	POSITION pos = GetFirstViewPosition();
	if(NULL == pos)
		return;

	for(CView * view = GetNextView(pos); NULL != view; view = GetNextView(pos))
		view->PostMessage(message, wparam, lparam);

	CWnd* mainFrame = AfxGetApp()->m_pMainWnd;
	mainFrame->PostMessage(message, wparam, lparam);
}


// CRSAUIDoc 诊断

#ifdef _DEBUG
void CRSAUIDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CRSAUIDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CRSAUIDoc 命令

BOOL CRSAUIDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  在此添加您专用的创建代码

	return TRUE;
}

void CRSAUIDoc::OnFileOpen()
{
	// TODO: 在此添加命令处理程序代码
	CString path = ChoosePath();
	if( 0 == path.GetLength())
		return ;
	
	IterateFiles(path);

	DispatchMsg(UM_LOADSTART, 0, 0);

	Load();

	m_notify.schedule( boost::bind(&CRSAUIDoc::Notify, this));	
}

CString CRSAUIDoc::ChoosePath()
{	
	CString file_path;

	BROWSEINFO  bi;
	bi.hwndOwner = NULL;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = NULL;
	bi.lpszTitle = NULL;
	bi.ulFlags = 0;
	bi.lpfn = NULL;
	bi.iImage = 0;
	LPCITEMIDLIST pidl = SHBrowseForFolder(&bi);
	if(!pidl)
		return file_path;

	TCHAR  szDisplayName[MAX_PATH];
	SHGetPathFromIDList(pidl,szDisplayName);
	file_path = szDisplayName;
	
	return file_path;
}

void CRSAUIDoc::IterateFiles(CString& file_path)
{
	WIN32_FIND_DATA findFileData;
	CString find_file = file_path;
	find_file += _T("\\*.sqlite");
	HANDLE hFind = FindFirstFile(find_file, &findFileData);
	if(INVALID_HANDLE_VALUE == hFind)
		return;
	
	CString buff;
	do
	{	
		buff = file_path;
		buff += _T("\\");
		buff += findFileData.cFileName;
		m_files.insert( make_pair(wstring(findFileData.cFileName), wstring(buff.GetBuffer(0))));
		buff.ReleaseBuffer();
	}while(0 != FindNextFile(hFind, &findFileData));
	
	FindClose(hFind);
}
