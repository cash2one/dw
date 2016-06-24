// RSAUIView.cpp : CRSAUIView 类的实现
//

#include "stdafx.h"
#include "RSAUI.h"

#include "RSAUIDoc.h"
#include "RSAUIView.h"
#include "commonTools.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern UINT WM_XEDIT_KILL_FOCUS;



/****list列表定义****/
#define LIST_INDEX			_T("index")
#define	LIST_GUID			_T("guid")
#define LIST_OWNER_GUID		_T("owner guid")
#define LIST_OPCODE			_T("opcode")
#define LIST_STREAM_SIZE	_T("DataSize")
#define LIST_OPTYPE			_T("optype")
#define LIST_TIME			_T("last modify time")
#define LIST_TRANS_INDEX	_T("tIndex")
#define LIST_TRANS_NUM		_T("tNum")
#define LIST_TRANS_ID		_T("tId")
#define LIST_LINE_ID		_T("lId")


// CRSAUIView

IMPLEMENT_DYNCREATE(CRSAUIView, CListView)

BEGIN_MESSAGE_MAP(CRSAUIView, CListView)
	ON_WM_STYLECHANGED()
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, &CRSAUIView::OnLvnItemchanged)
	ON_WM_LBUTTONDOWN()

	ON_REGISTERED_MESSAGE(WM_XEDIT_KILL_FOCUS, OnXEditKillFocus)
	ON_REGISTERED_MESSAGE(WM_XEDIT_VK_ESCAPE, OnXEditEscape)

    ON_WM_VSCROLL()
    ON_WM_HSCROLL()
END_MESSAGE_MAP()

// CRSAUIView 构造/析构

CRSAUIView::CRSAUIView()
:m_sub_item(-1)
,m_iSubItem(-1)
,m_iItem(-1)
,m_pEdit(NULL)
{
	// TODO: 在此处添加构造代码

}

CRSAUIView::~CRSAUIView()
{
	if(NULL == m_pEdit)
		return;

	delete m_pEdit;
}

BOOL CRSAUIView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CListView::PreCreateWindow(cs);
}

void CRSAUIView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();


	// TODO: 调用 GetListCtrl() 直接访问 ListView 的列表控件，
	//  从而可以用项填充 ListView。

	CListCtrl& showCtrl = GetListCtrl();
	DWORD dwStyle = showCtrl.GetExtendedStyle();
	//dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_GRIDLINES;
	showCtrl.SetExtendedStyle(dwStyle);

	showCtrl.ModifyStyle(LVS_TYPEMASK, LVS_REPORT | LVS_EDITLABELS );

	showCtrl.InsertColumn(0, LIST_INDEX, LVCFMT_LEFT);
	showCtrl.InsertColumn(1, LIST_GUID, LVCFMT_RIGHT);
	showCtrl.InsertColumn(2, LIST_OWNER_GUID, LVCFMT_RIGHT);
	showCtrl.InsertColumn(3, LIST_OPCODE, LVCFMT_RIGHT);
	showCtrl.InsertColumn(4, LIST_STREAM_SIZE, LVCFMT_RIGHT);
	showCtrl.InsertColumn(5, LIST_OPTYPE, LVCFMT_RIGHT);
	showCtrl.InsertColumn(6, LIST_TIME, LVCFMT_RIGHT);
	showCtrl.InsertColumn(7, LIST_TRANS_INDEX, LVCFMT_RIGHT);
	showCtrl.InsertColumn(8, LIST_TRANS_NUM, LVCFMT_RIGHT);
	showCtrl.InsertColumn(9, LIST_TRANS_ID, LVCFMT_RIGHT);
	showCtrl.InsertColumn(10, LIST_LINE_ID, LVCFMT_RIGHT);



	showCtrl.SetColumnWidth(0, 50);
	showCtrl.SetColumnWidth(1, 150);
	showCtrl.SetColumnWidth(2, 150);
	showCtrl.SetColumnWidth(3, 60);
	showCtrl.SetColumnWidth(4, 80);
	showCtrl.SetColumnWidth(5, 60);
	showCtrl.SetColumnWidth(6, 150);
	showCtrl.SetColumnWidth(7, 60);
	showCtrl.SetColumnWidth(8, 60);
	showCtrl.SetColumnWidth(9, 60);
	showCtrl.SetColumnWidth(10, 60);

}


// CRSAUIView 诊断

#ifdef _DEBUG
void CRSAUIView::AssertValid() const
{
	CListView::AssertValid();
}

void CRSAUIView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CRSAUIDoc* CRSAUIView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRSAUIDoc)));
	return (CRSAUIDoc*)m_pDocument;
}
#endif //_DEBUG


// CRSAUIView 消息处理程序
void CRSAUIView::OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct)
{
	//TODO: 添加代码以响应用户对窗口视图样式的更改	
	CListView::OnStyleChanged(nStyleType,lpStyleStruct);	
}

BOOL CRSAUIView::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: 在此添加专用代码和/或调用基类
	
	switch(message)
	{
	case UM_SELECTEDFILE:
		loadFile();
		break;

	case UM_QUERY:
		query();
		CRSAUIDoc *pDoc = (CRSAUIDoc *)GetDocument();
		pDoc->DispatchMsg(UM_QUERYFINISH, 0, 0);
		break;
	}

	return CListView::OnWndMsg(message, wParam, lParam, pResult);
}

void CRSAUIView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
}

void CRSAUIView::loadFile()
{
	CRSAUIDoc *pDoc = (CRSAUIDoc *)GetDocument();
	if(NULL == pDoc)
		return;
	
	m_show_mode = eShowFileRecord;

	Clear();
	bool success = pDoc->m_manager.QueryDatabase(pDoc->m_selected_file.c_str(),
									"select * from record", pDoc->m_record_array);
	if( !success)
		return;
	
	show();
}

void CRSAUIView::query()
{
	CRSAUIDoc *pDoc = (CRSAUIDoc *)GetDocument();
	if(NULL == pDoc)
		return;

	Clear();
	
	bool success = false;
	if(pDoc->m_is_Guid)
		success = pDoc->m_manager.GetObject(pDoc->m_guid, pDoc->m_start_time, pDoc->m_end_time, pDoc->m_history_array);
	else
		success = pDoc->m_manager.GetOwner(pDoc->m_guid, pDoc->m_start_time, pDoc->m_end_time, pDoc->m_history_array);

	if( !success)
		return ;
	
	m_show_mode = eShowHistoryRecrod;
	
	show();
}

void CRSAUIView::show()
{	
	CRSAUIDoc *pDoc = (CRSAUIDoc *)GetDocument();

	if( eShowFileRecord == m_show_mode)
	{
		RecordArray::const_iterator it = pDoc->m_record_array.begin();
		for(; it != pDoc->m_record_array.end(); ++it)
		{
			insertItem(*it);
		}

		return;
	}


	HistoryRecord::const_iterator pos = pDoc->m_history_array.begin();
	for( ; pos != pDoc->m_history_array.end(); ++pos)
	{
		insertItem(pos->second);
	}

}

void CRSAUIView::insertItem(boost::shared_ptr<Recordst> record)
{
	CListCtrl& list = GetListCtrl();
	int index = list.GetItemCount();
	TCHAR index_buf[20];
	_stprintf_s(index_buf, 20, _T("%d"), index);
	
	TCHAR guid_buf[30];
	_stprintf_s(guid_buf, 30, _T("%llu"), record->object_guid);

	TCHAR owner_guid_buf[30];
	_stprintf_s(owner_guid_buf, 30, _T("%llu"), record->owner_guid);

	TCHAR opcode_buf[20];
	_stprintf_s(opcode_buf, 20, _T("%d"), record->opcode);

	TCHAR stream_size_buf[20];
	_stprintf_s(stream_size_buf, 20, _T("%d"), record->stream_size);

	TCHAR optype_buf[20];
	_stprintf_s(optype_buf, 20, _T("%d"), record->optype);

	TCHAR time_buf[60];
    TCHAR format_buf[30];
    time_t modify_time = (time_t)(record->last_modify_time / (1000 * 1000));
    FormatTime(modify_time, format_buf);
	_stprintf_s(time_buf, 60, _T("%s__%llu"), format_buf, record->last_modify_time);

	TCHAR trans_index_buf[20];
	_stprintf_s(trans_index_buf, 20, _T("%d"), record->trans_index);

	TCHAR trans_num_buf[20];
	_stprintf_s(trans_num_buf, 20, _T("%d"), record->trans_num);

	TCHAR trans_id_buf[20];
	_stprintf_s(trans_id_buf, 20, _T("%d"), record->trans_id);

	TCHAR line_id_buf[20];
	_stprintf_s(line_id_buf, 20, _T("%d"), record->line_id);



	int item_index = list.InsertItem(index, index_buf);
	list.SetItemText(item_index, 1, guid_buf);
	list.SetItemText(item_index, 2, owner_guid_buf);
	list.SetItemText(item_index, 3, opcode_buf);
	list.SetItemText(item_index, 4, stream_size_buf);
	list.SetItemText(item_index, 5, optype_buf);
	list.SetItemText(item_index, 6, time_buf);
	list.SetItemText(item_index, 7, trans_index_buf);
	list.SetItemText(item_index, 8, trans_num_buf);
	list.SetItemText(item_index, 9, trans_id_buf);
	list.SetItemText(item_index, 10, line_id_buf);

}

void CRSAUIView::Clear()
{	
	CRSAUIDoc *pDoc = (CRSAUIDoc *)GetDocument();

	GetListCtrl().DeleteAllItems();
	pDoc->m_record_array.clear();
	pDoc->m_history_array.clear();
}

void CRSAUIView::OnItemChanged(int item)
{
	CRSAUIDoc *pDoc = (CRSAUIDoc *)GetDocument();
	if( !GetItem(item, pDoc->m_current_record))
		return;

	pDoc->DispatchMsg(UM_ITEMCHANGED, 0, 0);
}

void CRSAUIView::OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;

	if( -1 != m_sub_item)
	{
		if( m_sub_item == pNMLV->iItem)
			return;
	}

	m_sub_item = pNMLV->iItem;
	CRSAUIDoc *pDoc = (CRSAUIDoc *)GetDocument();
	if( !GetItem(m_sub_item, pDoc->m_current_record))
		return;
	
	pDoc->DispatchMsg(UM_ITEMCHANGED, 0, 0);
}

bool CRSAUIView::GetItem(DWORD item, boost::shared_ptr<Recordst>& record)
{
// 	CString  strguid = GetListCtrl().GetItemText(item, 1);
// 	if( 0 == strguid.GetLength())
// 		return false;
// 
// 	Integer guid = _tstoi64(strguid.GetBuffer(0));

	CRSAUIDoc *pDoc = (CRSAUIDoc *)GetDocument();
	if(eShowFileRecord == m_show_mode)
	{	
		RecordArray::const_iterator it = pDoc->m_record_array.begin();
		for( int i = 0; it != pDoc->m_record_array.end(); ++it, i++)
		{
// 			if( guid == (*it)->object_guid)
// 			{
// 				record = *it;
// 				return true;
// 			}
            if( i == item)
            {
                record = *it;
                return true;
            }
		}

		return false;
	}

	HistoryRecord::const_iterator pos = pDoc->m_history_array.begin();
	for(int i = 0; pos != pDoc->m_history_array.end(); ++pos, i++)
	{
// 		if( guid == pos->second->object_guid)
// 		{
// 			record = pos->second;
// 			return true;
// 		}
        if( i == item)
        {
            record = pos->second;
            return true;
        }
	}

	return false;
}

void CRSAUIView::OnLButtonDown(UINT nFlags, CPoint point)
{
	//CListView::OnLButtonDown(nFlags, point);
	CListCtrl& list = GetListCtrl();
	LVHITTESTINFO  lvhit;
	lvhit.pt = point;
	int item = list.SubItemHitTest(&lvhit);
	
	CListView::OnLButtonDown(nFlags, point);

	//if (over a item/subitem)
	if (item != -1 && (lvhit.flags & LVHT_ONITEM))
	{	
		if(m_iItem != lvhit.iItem)
			OnItemChanged(lvhit.iItem);

		if(m_iItem == lvhit.iItem && m_iSubItem == lvhit.iSubItem)
			return;

		m_iItem = lvhit.iItem;
		m_iSubItem = lvhit.iSubItem;
	

		// get the column offset
		int offset = 0;
		for (int i = 0; i < m_iSubItem; i++)
			offset += list.GetColumnWidth(i);

		CRect rect;
		list.GetItemRect(m_iItem, &rect, LVIR_BOUNDS);

		// now scroll if we need to expose the column
		CRect rectClient;
		GetClientRect(&rectClient);
		if (offset + rect.left < 0 || offset + rect.left > rectClient.right)
		{
			CSize size;
			size.cx = offset + rect.left;
			size.cy = 0;
			list.Scroll(size);
			rect.left -= size.cx;
		}

		// Get Column alignment
		LV_COLUMN lvcol;
		lvcol.mask = LVCF_FMT;
		list.GetColumn(m_iSubItem, &lvcol);
		DWORD dwStyle = 0;
		if ((lvcol.fmt & LVCFMT_JUSTIFYMASK) == LVCFMT_LEFT)
			dwStyle = ES_LEFT;
		else if ((lvcol.fmt & LVCFMT_JUSTIFYMASK) == LVCFMT_RIGHT)
			dwStyle = ES_RIGHT;
		else dwStyle = ES_CENTER;

		dwStyle |= WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_LEFT /*| WS_BORDER*/ ;

		//rect.top -= 2;
		rect.bottom += 1;
		rect.left += offset + 2;
		rect.right = rect.left + list.GetColumnWidth(m_iSubItem) - 4;

		if (rect.right > rectClient.right) 
			rect.right = rectClient.right;
		
		if(NULL != m_pEdit)
			OnXEditKillFocus(0, 0);
		
		CString item_show = list.GetItemText(m_iItem, m_iSubItem);
		m_pEdit = new CXEdit(this, item_show);
		BOOL bSuccess = m_pEdit->Create(dwStyle, rect, this, 99);
		if (bSuccess)
		{
			m_pEdit->SetFocus();
		}
	
	}
}



LRESULT CRSAUIView::OnXEditKillFocus(WPARAM, LPARAM)
{	

	if (m_pEdit && ::IsWindow(m_pEdit->m_hWnd))
	{
		m_pEdit->DestroyWindow();
		delete m_pEdit;
	}

	m_pEdit = NULL;
	return 0;
}

LRESULT CRSAUIView::OnXEditEscape(WPARAM, LPARAM)
{

	if (m_pEdit && ::IsWindow(m_pEdit->m_hWnd))
	{
		m_pEdit->DestroyWindow();
		delete m_pEdit;
	}
	m_pEdit = NULL;

	return 0;
}






void CRSAUIView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    CRSAUIDoc* pDoc = (CRSAUIDoc *)GetDocument();
    pDoc->DispatchMsg(WM_XEDIT_KILL_FOCUS, 0, 0);

    CListView::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CRSAUIView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    
    CRSAUIDoc* pDoc = (CRSAUIDoc *)GetDocument();
    pDoc->DispatchMsg(WM_XEDIT_KILL_FOCUS, 0, 0);

    CListView::OnHScroll(nSBCode, nPos, pScrollBar);
}
