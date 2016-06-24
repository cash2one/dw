// LeftView.cpp : CLeftView ���ʵ��
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


// CLeftView ����/����

CLeftView::CLeftView()
:m_root(NULL)
{
	// TODO: �ڴ˴���ӹ������
}

CLeftView::~CLeftView()
{
}

BOOL CLeftView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸� CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CTreeView::PreCreateWindow(cs);
}

void CLeftView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();

	// TODO: ���� GetTreeCtrl() ֱ�ӷ��� TreeView �����ؼ���
	//  �Ӷ������������ TreeView��
	GetTreeCtrl().ModifyStyle(0, TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS);
}


// CLeftView ���

#ifdef _DEBUG
void CLeftView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CLeftView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}

CRSAUIDoc* CLeftView::GetDocument() // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRSAUIDoc)));
	return (CRSAUIDoc*)m_pDocument;
}
#endif //_DEBUG


// CLeftView ��Ϣ�������


void CLeftView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{

}

BOOL CLeftView::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: �ڴ����ר�ô����/����û���
	
	switch(message)
	{
	case UM_LOADSTART:
		{	
			CTreeCtrl& tree = GetTreeCtrl();
			tree.DeleteAllItems();

			m_root = GetTreeCtrl().InsertItem(_T("�ļ��б�"));
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������

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
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    *pResult = 0;
}
