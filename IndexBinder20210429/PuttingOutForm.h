//PuttingOutForm.h
#ifndef _PUTTINGOUTFORM_H
#define _PUTTINGOUTFORM_H
#include<afxwin.h>
#include"resource.h"

class PuttingOutForm :public CDialog {
public:
	enum{IDD=IDD_PUTTINGOUTFORM};
public:
	PuttingOutForm(CWnd* parent = NULL);
	virtual BOOL OnInitDialog();
protected:
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP();
};

#endif//_PUTTINGOUTFORM_H
