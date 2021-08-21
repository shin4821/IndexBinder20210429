//PuttingForm.cpp
#include"PuttingForm.h"
#include"BusinessCardBinderForm.h"
#include"BusinessCardBinder.h"
#include"IndexBinder.h"
#include<afxcmn.h>

BEGIN_MESSAGE_MAP(PuttingForm, CDialog)
	ON_EN_KILLFOCUS(IDC_EDIT_COMPANYNAME, OnCompanyNameKillFocus)
	ON_BN_CLICKED(IDC_BUTTON_PUT, OnPutButtonClicked)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

PuttingForm::PuttingForm(CWnd* parent)
	:CDialog(PuttingForm::IDD, parent) {
}

BOOL PuttingForm::OnInitDialog() {
	CDialog::OnInitDialog();

	TCHAR rankNames[][8] = { "인턴","사원","주임","대리","과장","차장","부장","이사","전무","부사장","사장" };
	TCHAR portalSites[][32] = { "naver.com","daum.net","gmail.com" };
	Long i;

	//1. 꽂기 윈도우가 생성될 때
	  //1.1. 직급칸의 콤보박스에 직급들을 추가한다.
	  for (i = 0; i < sizeof(rankNames) / sizeof(rankNames[0]); i++) {
		((CComboBox*)GetDlgItem(IDC_EDIT_RANKNAME))->AddString((LPCTSTR)rankNames[i]);
	  }
	  //1.2. 이메일 주소칸의 콤보박스에 이메일 주소들을 추가한다.
	  for (i = 0; i < sizeof(portalSites) / sizeof(portalSites[0]); i++) {
		  ((CComboBox*)GetDlgItem(IDC_EDIT_PORTALSITE))->AddString((LPCTSTR)portalSites[i]);
	  }
	  return FALSE;
}

void PuttingForm::OnCompanyNameKillFocus() {
	CString companyName;
	CString address;
	CString companyTelephoneNumber;
	CString faxNumber;
	CString url;
	Index* index_;
	BusinessCard* index;
	BusinessCardBinderForm* businessCardBinderForm;

	//2. 회사 명칭에 대하여 포커스를 잃을 때,
	  //2.1. 상호를 읽는다.
	  GetDlgItem(IDC_EDIT_COMPANYNAME)->GetWindowText(companyName);

	  //2.2. 명함철윈도우를 찾는다.
	  businessCardBinderForm = (BusinessCardBinderForm*)CWnd::FindWindow("#32770", "명함철");

	  //2.3. 명함철윈도우의 색인철에서 해당상호를 찾는다.
	  index_ = businessCardBinderForm->indexBinder->Find((LPCTSTR)companyName);

	  //2.4. 같은 상호가 있을 경우, 주소, 전화번호, 팩스번호, URL을 명함에 출력한다.
	  if (index_ != NULL) {
		  index = index_->GetAt(0);
		  
		  address = CString(index->GetAddress().c_str());
		  GetDlgItem(IDC_EDIT_ADDRESS)->SetWindowText(address);
		  companyTelephoneNumber = CString(index->GetCompanyTelphoneNumber().c_str());
		  GetDlgItem(IDC_EDIT_COMPANYTELEPHONENUMBER)->SetWindowText(companyTelephoneNumber);
		  faxNumber = CString(index->GetFaxNumber().c_str());
		  GetDlgItem(IDC_EDIT_FAXNUMBER)->SetWindowText(faxNumber);
		  url = CString(index->GetUrl().c_str());
		  GetDlgItem(IDC_EDIT_URL)->SetWindowText(url);
	  }
}



void PuttingForm::OnPutButtonClicked() {
	BusinessCardBinderForm* businessCardBinderForm;
	CString personalName;
	CString rankName;
	CString personalTelephoneNumber;
	CString emailAddress;
	CString portalSite;
	CString totalEmailAddress;
	CString companyName;
	CString address;
	CString companyTelephoneNumber;
	CString faxNumber;
	CString url;
	BusinessCard* index;
	HTREEITEM parent;
	HTREEITEM company;
	HTREEITEM personal;
	CString companyText;

	//3. 꽂기버튼 클릭했을 때
	  //3.1. 명함을 읽는다.
	  GetDlgItem(IDC_EDIT_PERSONALNAME)->GetWindowText(personalName);
	  GetDlgItem(IDC_EDIT_RANKNAME)->GetWindowText(rankName);
	  GetDlgItem(IDC_EDIT_PERSONALTELEPHONENUMBER)->GetWindowText(personalTelephoneNumber);
	  GetDlgItem(IDC_EDIT_EMAILADDRESS)->GetWindowText(emailAddress);
	  GetDlgItem(IDC_EDIT_PORTALSITE)->GetWindowText(portalSite);
	  GetDlgItem(IDC_EDIT_COMPANYNAME)->GetWindowText(companyName);
	  GetDlgItem(IDC_EDIT_ADDRESS)->GetWindowText(address);
	  GetDlgItem(IDC_EDIT_COMPANYTELEPHONENUMBER)->GetWindowText(companyTelephoneNumber);
	  GetDlgItem(IDC_EDIT_FAXNUMBER)->GetWindowText(faxNumber);
	  GetDlgItem(IDC_EDIT_URL)->GetWindowText(url);
	  
	  // 이메일 주소 만든다.
	  totalEmailAddress.Format("%s@%s", emailAddress, portalSite);

	  //3.2. 명함철윈도우를 찾는다.
	  businessCardBinderForm = (BusinessCardBinderForm*)CWnd::FindWindow("#32770", "명함철");

	  //3.3. 명함철윈도우의 명함철에 꽂는다.
	  BusinessCard businessCard((LPCTSTR)personalName, (LPCTSTR)rankName, (LPCTSTR)personalTelephoneNumber, 
		  (LPCTSTR)totalEmailAddress, (LPCTSTR)companyName, (LPCTSTR)address, (LPCTSTR)companyTelephoneNumber, 
		  (LPCTSTR)faxNumber, (LPCTSTR)url);

	  index = businessCardBinderForm->businessCardBinder->PutIn(businessCard);

	  businessCardBinderForm->Insert(index);

	  //3.4. 명함철윈도우의 명함에 현재 명함을 출력한다.
	  personalName = CString(index->GetPersonalName().c_str());
	  businessCardBinderForm->GetDlgItem(IDC_EDIT_PERSONALNAME)->SetWindowText(personalName);
	  rankName = CString(index->GetRankName().c_str());
	  businessCardBinderForm->GetDlgItem(IDC_EDIT_RANKNAME)->SetWindowText(rankName);
	  personalTelephoneNumber = CString(index->GetPersonalTelephoneNumber().c_str());
	  businessCardBinderForm->GetDlgItem(IDC_EDIT_PERSONALTELEPHONENUMBER)->SetWindowText(personalTelephoneNumber);
	  emailAddress = CString(index->GetEmailAddress().c_str());
	  businessCardBinderForm->GetDlgItem(IDC_EDIT_EMAILADDRESS)->SetWindowText(emailAddress);
	  companyName = CString(index->GetCompanyName().c_str());
	  businessCardBinderForm->GetDlgItem(IDC_EDIT_COMPANYNAME)->SetWindowText(companyName);
	  address = CString(index->GetAddress().c_str());
	  businessCardBinderForm->GetDlgItem(IDC_EDIT_ADDRESS)->SetWindowText(address);
	  companyTelephoneNumber = CString(index->GetCompanyTelphoneNumber().c_str());
	  businessCardBinderForm->GetDlgItem(IDC_EDIT_COMPANYTELEPHONENUMBER)->SetWindowText(companyTelephoneNumber);
	  faxNumber = CString(index->GetFaxNumber().c_str());
	  businessCardBinderForm->GetDlgItem(IDC_EDIT_FAXNUMBER)->SetWindowText(faxNumber);
	  url = CString(index->GetUrl().c_str());
	  businessCardBinderForm->GetDlgItem(IDC_EDIT_URL)->SetWindowText(url);

	  //3.5. 명함철윈도우의 색인철에 꽂는다.
	  businessCardBinderForm->indexBinder->PutIn(index);

	  //3.6. 회사들 핸들을 찾는다.
	  parent = ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(NULL, TVGN_ROOT);

	  //3.7. 상호핸들 찾는다.
	  company = ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(parent, TVGN_CHILD);
	  companyText = ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))->GetItemText(company);

	  while (company != NULL && companyText != companyName) {
		  company = ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))
			  ->GetNextItem(company, TVGN_NEXT);
		  companyText = ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))->GetItemText(company);
	  }

	  //3.8. 해당 상호핸들이 없으면 회사들핸들 하위에 추가한다.
	  if (company == NULL) {
		  company = ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))->InsertItem(companyName,
			  parent, TVI_SORT);
	  }
	  //3.9. 상호핸들 하위에 개인핸들 추가한다.
	  personal = ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))->InsertItem(personalName,
		  company, TVI_LAST);

	  //4.0. 회사들 핸들 펼친다.
	  ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))->Expand(parent, TVE_EXPAND);
	  //4.1. 상호핸들 펼친다.
	  ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))->Expand(company, TVE_EXPAND);
	  //4.2. 개인핸들 선택한다.
	  ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))->SelectItem(personal);

	  //4.3. 꽂기윈도우 종료한다.
	  EndDialog(0);
}




void PuttingForm::OnClose() {
	//4. 닫기버튼 클릭했을 때
	  //4.1. 꽂기윈도우 종료한다.
	  EndDialog(0);
}

