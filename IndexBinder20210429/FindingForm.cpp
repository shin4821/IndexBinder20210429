//FindingForm.cpp
#include"FindingForm.h"
#include"BusinessCardBinderForm.h"
#include"IndexBinder.h"
#include"BusinessCardBinder.h"
#include<afxcmn.h> //CTreeCtrl

BEGIN_MESSAGE_MAP(FindingForm, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_FIND, OnFindButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON_SELECT, OnSelectButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON_FIRST, OnFirstButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, OnNextButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON_PREVIOUS, OnPreviousButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON_LAST, OnLastButtonClicked)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

FindingForm::FindingForm(CWnd* parent)
	:CDialog(FindingForm::IDD, parent) {
	this->indexes = NULL;
	this->count = 0;
	this->current = 0;
}

void FindingForm::OnFindButtonClicked() {
	BusinessCardBinderForm* businessCardBinderForm;
	CString personalName;
	CString rankName;
	CString personalTelephoneNumber;
	CString emailAddress;
	CString companyName;
	CString address;
	CString companyTelephoneNumber;
	CString faxNumber;
	CString url;

	//1. 찾기버튼 클릭했을 때
	  //1.1. 성명을 읽는다.
	  GetDlgItem(IDC_EDIT_NAME)->GetWindowText(personalName);

	  //1.2. 명함철 윈도우 찾는다.
	  businessCardBinderForm = (BusinessCardBinderForm*)CWnd::FindWindow("#32770", "명함철");

	  //1.3. 명함철 윈도우의 명함철에서 찾는다.
	  if (this->indexes != NULL) {
		  delete[] this->indexes;
	  }
	  businessCardBinderForm->businessCardBinder->Find((LPCTSTR)personalName, &this->indexes, &this->count);

	  //1.4. 명함이 있는 경우, 찾은 명함들 중 첫번째 명함을 출력한다.
	  if (this->count > 0) {
		  this->current = 0;

		  personalName = CString(this->indexes[this->current]->GetPersonalName().c_str());
		  GetDlgItem(IDC_EDIT_PERSONALNAME)->SetWindowText(personalName);
		  rankName = CString(this->indexes[this->current]->GetRankName().c_str());
		  GetDlgItem(IDC_EDIT_RANKNAME)->SetWindowText(rankName);
		  personalTelephoneNumber = CString(this->indexes[this->current]->GetPersonalTelephoneNumber().c_str());
		  GetDlgItem(IDC_EDIT_PERSONALTELEPHONENUMBER)->SetWindowText(personalTelephoneNumber);
		  emailAddress = CString(this->indexes[this->current]->GetEmailAddress().c_str());
		  GetDlgItem(IDC_EDIT_EMAILADDRESS)->SetWindowText(emailAddress);
		  companyName = CString(this->indexes[this->current]->GetCompanyName().c_str());
		  GetDlgItem(IDC_EDIT_COMPANYNAME)->SetWindowText(companyName);
		  address = CString(this->indexes[this->current]->GetAddress().c_str());
		  GetDlgItem(IDC_EDIT_ADDRESS)->SetWindowText(address);
		  companyTelephoneNumber = CString(this->indexes[this->current]->GetCompanyTelphoneNumber().c_str());
		  GetDlgItem(IDC_EDIT_COMPANYTELEPHONENUMBER)->SetWindowText(companyTelephoneNumber);
		  faxNumber = CString(this->indexes[this->current]->GetFaxNumber().c_str());
		  GetDlgItem(IDC_EDIT_FAXNUMBER)->SetWindowText(faxNumber);
		  url = CString(this->indexes[this->current]->GetUrl().c_str());
		  GetDlgItem(IDC_EDIT_URL)->SetWindowText(url);
	  }

	  //1.5. 명함이 없는 경우, 공백 처리한다.
	  else {
		  GetDlgItem(IDC_EDIT_PERSONALNAME)->SetWindowText("");
		  GetDlgItem(IDC_EDIT_RANKNAME)->SetWindowText("");
		  GetDlgItem(IDC_EDIT_PERSONALTELEPHONENUMBER)->SetWindowText("");
		  GetDlgItem(IDC_EDIT_EMAILADDRESS)->SetWindowText("");
		  GetDlgItem(IDC_EDIT_COMPANYNAME)->SetWindowText("");
		  GetDlgItem(IDC_EDIT_ADDRESS)->SetWindowText("");
		  GetDlgItem(IDC_EDIT_COMPANYTELEPHONENUMBER)->SetWindowText("");
		  GetDlgItem(IDC_EDIT_FAXNUMBER)->SetWindowText("");
		  GetDlgItem(IDC_EDIT_URL)->SetWindowText("");
	  }
}

void FindingForm::OnSelectButtonClicked() {
	BusinessCardBinderForm* businessCardBinderForm;
	BusinessCard* index;
	Index* index_;
	HTREEITEM parent;
	HTREEITEM company;
	HTREEITEM personal;
	CString companyText;
	Long arrayIndex;
	Long i = 0;
	CString personalName;
	CString rankName;
	CString personalTelephoneNumber;
	CString emailAddress;
	CString companyName;
	CString address;
	CString companyTelephoneNumber;
	CString faxNumber;
	CString url;

	//6. 선택버튼 클릭했을 때
	  //6.1. 명함철 윈도우를 찾는다. 
	  businessCardBinderForm = (BusinessCardBinderForm*)CWnd::FindWindow("#32770", "명함철");

	  //6.2. 명함철윈도우의 명함철에서 읽은 명함을 현재 명함으로 이동시킨다.
	  index = businessCardBinderForm->businessCardBinder->Move(this->indexes[this->current]);

	  //6.3. 트리뷰에서 상호핸들 찾는다.
	  //상호이름 찾는다.
	  companyName = CString(index->GetCompanyName().c_str());
	  //회사들 핸들 찾는다.
	  parent = ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(NULL, TVGN_ROOT);
	  //상호핸들 찾는다.
	  company = ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(parent, TVGN_CHILD);
	  companyText = ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))->GetItemText(company);
	  while (companyText != companyName) {
		  company = ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))
			  ->GetNextItem(company, TVGN_NEXT);
		  companyText = ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))->GetItemText(company);
	  }
	  
	  //6.4. 트리뷰에서 개인핸들 찾는다.
	  //색인철 찾는다.
	  index_ = businessCardBinderForm->indexBinder->Find((LPCTSTR)companyName);
	  //개인위치 찾는다.
	  arrayIndex = index_->Find(index);
	  //개인핸들 찾는다.
	  personal = ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))
		  ->GetNextItem(company, TVGN_CHILD);
	  while (arrayIndex > i) {
		  personal = ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))
			  ->GetNextItem(personal, TVGN_NEXT);
		  i++;
	  }

	  //6.5. 회사들 핸들 펼친다.
	  ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))->Expand(parent, TVE_EXPAND);
	  //6.6. 상호핸들 펼친다.
	  ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))->Expand(company, TVE_EXPAND);
	  //6.7. 개인핸들 선택한다.
	  ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))->SelectItem(personal);


	  //6.8. 명함철 윈도우의 명함에 현재 명함을 출력한다.
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

	  //6.9. 찾기 윈도우를 종료한다.
	  if (this->indexes != NULL) {
		  delete[] this->indexes;
		  this->indexes = NULL;
	  }

	  EndDialog(0);
}

void FindingForm::OnFirstButtonClicked() {
	CString personalName;
	CString rankName;
	CString personalTelephoneNumber;
	CString emailAddress;
	CString companyName;
	CString address;
	CString companyTelephoneNumber;
	CString faxNumber;
	CString url;

	//2. 처음버튼 클릭했을 때
	  //2.1. 찾은 명함들 중, 첫번째 명함을 명함에 출력한다.
	  this->current = 0;

	  personalName = CString(this->indexes[this->current]->GetPersonalName().c_str());
	  GetDlgItem(IDC_EDIT_PERSONALNAME)->SetWindowText(personalName);
	  rankName = CString(this->indexes[this->current]->GetRankName().c_str());
	  GetDlgItem(IDC_EDIT_RANKNAME)->SetWindowText(rankName);
	  personalTelephoneNumber = CString(this->indexes[this->current]->GetPersonalTelephoneNumber().c_str());
	  GetDlgItem(IDC_EDIT_PERSONALTELEPHONENUMBER)->SetWindowText(personalTelephoneNumber);
	  emailAddress = CString(this->indexes[this->current]->GetEmailAddress().c_str());
	  GetDlgItem(IDC_EDIT_EMAILADDRESS)->SetWindowText(emailAddress);
	  companyName = CString(this->indexes[this->current]->GetCompanyName().c_str());
	  GetDlgItem(IDC_EDIT_COMPANYNAME)->SetWindowText(companyName);
	  address = CString(this->indexes[this->current]->GetAddress().c_str());
	  GetDlgItem(IDC_EDIT_ADDRESS)->SetWindowText(address);
	  companyTelephoneNumber = CString(this->indexes[this->current]->GetCompanyTelphoneNumber().c_str());
	  GetDlgItem(IDC_EDIT_COMPANYTELEPHONENUMBER)->SetWindowText(companyTelephoneNumber);
	  faxNumber = CString(this->indexes[this->current]->GetFaxNumber().c_str());
	  GetDlgItem(IDC_EDIT_FAXNUMBER)->SetWindowText(faxNumber);
	  url = CString(this->indexes[this->current]->GetUrl().c_str());
	  GetDlgItem(IDC_EDIT_URL)->SetWindowText(url);
}

void FindingForm::OnPreviousButtonClicked() {
	CString personalName;
	CString rankName;
	CString personalTelephoneNumber;
	CString emailAddress;
	CString companyName;
	CString address;
	CString companyTelephoneNumber;
	CString faxNumber;
	CString url;

	//3. 이전버튼 클릭했을 때
	  //3.1. 현재 명함의 이전 명함을 명함에 출력한다.
	this->current--;
	if (this->current < 0) {
		this->current = 0;
	}

	personalName = CString(this->indexes[this->current]->GetPersonalName().c_str());
	GetDlgItem(IDC_EDIT_PERSONALNAME)->SetWindowText(personalName);
	rankName = CString(this->indexes[this->current]->GetRankName().c_str());
	GetDlgItem(IDC_EDIT_RANKNAME)->SetWindowText(rankName);
	personalTelephoneNumber = CString(this->indexes[this->current]->GetPersonalTelephoneNumber().c_str());
	GetDlgItem(IDC_EDIT_PERSONALTELEPHONENUMBER)->SetWindowText(personalTelephoneNumber);
	emailAddress = CString(this->indexes[this->current]->GetEmailAddress().c_str());
	GetDlgItem(IDC_EDIT_EMAILADDRESS)->SetWindowText(emailAddress);
	companyName = CString(this->indexes[this->current]->GetCompanyName().c_str());
	GetDlgItem(IDC_EDIT_COMPANYNAME)->SetWindowText(companyName);
	address = CString(this->indexes[this->current]->GetAddress().c_str());
	GetDlgItem(IDC_EDIT_ADDRESS)->SetWindowText(address);
	companyTelephoneNumber = CString(this->indexes[this->current]->GetCompanyTelphoneNumber().c_str());
	GetDlgItem(IDC_EDIT_COMPANYTELEPHONENUMBER)->SetWindowText(companyTelephoneNumber);
	faxNumber = CString(this->indexes[this->current]->GetFaxNumber().c_str());
	GetDlgItem(IDC_EDIT_FAXNUMBER)->SetWindowText(faxNumber);
	url = CString(this->indexes[this->current]->GetUrl().c_str());
	GetDlgItem(IDC_EDIT_URL)->SetWindowText(url);
}

void FindingForm::OnNextButtonClicked() {
	CString personalName;
	CString rankName;
	CString personalTelephoneNumber;
	CString emailAddress;
	CString companyName;
	CString address;
	CString companyTelephoneNumber;
	CString faxNumber;
	CString url;

	//4. 다음버튼 클릭했을 때
	  //4.1. 현재 명함의 다음 명함을 명함에 출력한다.
	this->current++;
	if (this->current >= this->count) {
		this->current = this->count - 1;
	}

	personalName = CString(this->indexes[this->current]->GetPersonalName().c_str());
	GetDlgItem(IDC_EDIT_PERSONALNAME)->SetWindowText(personalName);
	rankName = CString(this->indexes[this->current]->GetRankName().c_str());
	GetDlgItem(IDC_EDIT_RANKNAME)->SetWindowText(rankName);
	personalTelephoneNumber = CString(this->indexes[this->current]->GetPersonalTelephoneNumber().c_str());
	GetDlgItem(IDC_EDIT_PERSONALTELEPHONENUMBER)->SetWindowText(personalTelephoneNumber);
	emailAddress = CString(this->indexes[this->current]->GetEmailAddress().c_str());
	GetDlgItem(IDC_EDIT_EMAILADDRESS)->SetWindowText(emailAddress);
	companyName = CString(this->indexes[this->current]->GetCompanyName().c_str());
	GetDlgItem(IDC_EDIT_COMPANYNAME)->SetWindowText(companyName);
	address = CString(this->indexes[this->current]->GetAddress().c_str());
	GetDlgItem(IDC_EDIT_ADDRESS)->SetWindowText(address);
	companyTelephoneNumber = CString(this->indexes[this->current]->GetCompanyTelphoneNumber().c_str());
	GetDlgItem(IDC_EDIT_COMPANYTELEPHONENUMBER)->SetWindowText(companyTelephoneNumber);
	faxNumber = CString(this->indexes[this->current]->GetFaxNumber().c_str());
	GetDlgItem(IDC_EDIT_FAXNUMBER)->SetWindowText(faxNumber);
	url = CString(this->indexes[this->current]->GetUrl().c_str());
	GetDlgItem(IDC_EDIT_URL)->SetWindowText(url);
}

void FindingForm::OnLastButtonClicked() {
	CString personalName;
	CString rankName;
	CString personalTelephoneNumber;
	CString emailAddress;
	CString companyName;
	CString address;
	CString companyTelephoneNumber;
	CString faxNumber;
	CString url;

	//5. 마지막버튼 클릭했을 때
	  //5.1. 찾은 명함들 중 마지막 명함을 명함에 출력한다.
	this->current = this->count - 1;

	personalName = CString(this->indexes[this->current]->GetPersonalName().c_str());
	GetDlgItem(IDC_EDIT_PERSONALNAME)->SetWindowText(personalName);
	rankName = CString(this->indexes[this->current]->GetRankName().c_str());
	GetDlgItem(IDC_EDIT_RANKNAME)->SetWindowText(rankName);
	personalTelephoneNumber = CString(this->indexes[this->current]->GetPersonalTelephoneNumber().c_str());
	GetDlgItem(IDC_EDIT_PERSONALTELEPHONENUMBER)->SetWindowText(personalTelephoneNumber);
	emailAddress = CString(this->indexes[this->current]->GetEmailAddress().c_str());
	GetDlgItem(IDC_EDIT_EMAILADDRESS)->SetWindowText(emailAddress);
	companyName = CString(this->indexes[this->current]->GetCompanyName().c_str());
	GetDlgItem(IDC_EDIT_COMPANYNAME)->SetWindowText(companyName);
	address = CString(this->indexes[this->current]->GetAddress().c_str());
	GetDlgItem(IDC_EDIT_ADDRESS)->SetWindowText(address);
	companyTelephoneNumber = CString(this->indexes[this->current]->GetCompanyTelphoneNumber().c_str());
	GetDlgItem(IDC_EDIT_COMPANYTELEPHONENUMBER)->SetWindowText(companyTelephoneNumber);
	faxNumber = CString(this->indexes[this->current]->GetFaxNumber().c_str());
	GetDlgItem(IDC_EDIT_FAXNUMBER)->SetWindowText(faxNumber);
	url = CString(this->indexes[this->current]->GetUrl().c_str());
	GetDlgItem(IDC_EDIT_URL)->SetWindowText(url);


}



void FindingForm::OnClose() {
	//7. 닫기버튼 클릭했을 때
	  //7.1. 찾기윈도우 종료한다.
	  if (this->indexes != NULL) {
		  delete[] this->indexes;
		  this->indexes = NULL;
	  }
	  EndDialog(0);
}

