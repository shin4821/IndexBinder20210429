//BusinessCardBinderForm.h
#ifndef _BUSINESSCARDBINDERFORM_H
#define _BUSINESSCARDBINDERFORM_H
#include<afxwin.h>
#include"resource.h"
typedef signed long int Long;

class BusinessCard;
class BusinessCardBinder;
class IndexBinder;
class BusinessCardBinderForm :public CDialog {
public:
	enum{IDD=IDD_BUSINESSCARDBINDERFORM};
public:
	BusinessCardBinderForm(CWnd* parent = NULL);
	virtual BOOL OnInitDialog();
public:
	BusinessCardBinder* businessCardBinder;
	IndexBinder* indexBinder;
public:
	Long Load();
	Long Save();
	CString GetCompanyCode(BusinessCard* index);
	CString GetCode();
	void Insert(BusinessCard* index);
	void Delete(BusinessCard businessCard);
	void Modify(BusinessCard* index);

protected:
	afx_msg void OnPutButtonClicked();
	afx_msg void OnFindButtonClicked();
	afx_msg void OnPutOutButtonClicked();
	afx_msg void OnArrangeButtonClicked();
	afx_msg void OnTreeViewItemDoubleClicked(NMHDR* pNotifyStruct, LRESULT* result);
	afx_msg void OnFirstButtonClicked();
	afx_msg void OnPreviousButtonClicked();
	afx_msg void OnNextButtonClicked();
	afx_msg void OnLastButtonClicked();
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()
};

#endif//_BUSINESSCARDBINDERFORM_H

