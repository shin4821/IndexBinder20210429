//PuttingForm.h
#ifndef _PUTTINGFORM_H
#define _PUTTINGFORM_H
#include<afxwin.h>
#include"resource.h"

class PuttingForm:public CDialog {
public:
	enum{IDD=IDD_PUTTINGFORM};
public:
	PuttingForm(CWnd* parent = NULL);
	virtual BOOL OnInitDialog();
protected:
	afx_msg void OnCompanyNameKillFocus();
	afx_msg void OnPutButtonClicked();
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()
};

#endif//_PUTTINGFORM_H