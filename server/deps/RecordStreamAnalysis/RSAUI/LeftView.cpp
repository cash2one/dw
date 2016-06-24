// LeftView.cpp : CLeftView 类的实现
//

#include "stdafx.h"
#include "RSAUI.h"

#include "RSAUIDoc.h"
#include "LeftView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern UINT WM_XEDIT_KILL_FOCUS;

// CLeftView

IMPLEMENT_DYNCREATE(CLeftView, CTreeView)

BEGIN_MESSAGE_MAP(CLeftView, CTreeView)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CLeftView::OnTvnSelchanged)
    ON_NOTIFY_REFLECT(NM_KILLFOCUS, &CLeftView::OnNMKillfocus)
END_MESSAGE_MAP()


// CLeftView 构造/析构

CLeftView::CLeftView()
:m_root(NULL)
{
	// TODO: 在此处添加构造代码
}

CLeftView::~CLeftView()
{
}

BOOL CLeftView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改 CREATESTRUCT cs 来修改窗口类或样式

	return CTreeView::PreCreateWindow(cs);
}

void CLeftView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();

	// TODO: 调用 GetTreeCtrl() 直接访问 TreeView 的树控件，
	//  从而可以用项填充 TreeView。
	GetTreeCtrl().ModifyStyle(0, TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS);
}


// CLeftView 诊断

#ifdef _DEBUG
void CLeftView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CLeftView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}

CRSAUIDoc* CLeftView::GetDocument() // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRSAUIDoc)));
	return (CRSAUIDoc*)m_pDocument;
}
#endif //_DEBUG


// CLeftView 消息处理程序


void CLeftView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{

}

BOOL CLeftView::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: 在此添加专用代码和/或调用基类
	
	switch(message)
	{
	case UM_LOADSTART:
		{	
			CTreeCtrl& tree = GetTreeCtrl();
			tree.DeleteAllItems();

			m_root = GetTreeCtrl().InsertItem(_T("文件列表"));
			CRSAUIDoc* doc = GetDocument();
			FILES_MAP::const_iterator it = doc->m_files.begin();
			for(; it != doc->m_files.end(); ++it)
			{
				tree.InsertItem(it->first.c_str(), m_root);
			}

			tree.Expand(m_root, TVE_EXPAND);

			break;
		}
	}

	return CTreeView::OnWndMsg(message, wParam, lParam, pResult);
}

void CLeftView::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

    CRSAUIDoc* pDoc = (CRSAUIDoc *)GetDocument();
    pDoc->DispatchMsg(WM_XEDIT_KILL_FOCUS, 0, 0);

	*pResult = 0;
	CString str = GetTreeCtrl().GetItemText(pNMTreeView->itemNew.hItem);
	FILES_MAP::iterator it = pDoc->m_files.find(wstring(str.GetBuffer(0)));
	if(it == pDoc->m_files.end())
		return;

	pDoc->m_selected_file = UnicodeToMbcs(it->second);
	pDoc->DispatchMsg(UM_SELECTEDFILE, 0, 0);
}

void CLeftView::OnNMKillfocus(NMHDR *pNMHDR, LRESULT *pResult)
{
    // TODO: 在此添加控件通知处理程序代码
    *pResult = 0;
}
