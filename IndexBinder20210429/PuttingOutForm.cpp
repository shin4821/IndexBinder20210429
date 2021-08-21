//PuttingOutForm.cpp
#include"PuttingOutForm.h"
#include"BusinessCardBinderForm.h"
#include"BusinessCardBinder.h"
#include"IndexBinder.h"
#include<afxcmn.h>

BEGIN_MESSAGE_MAP(PuttingOutForm, CDialog)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

PuttingOutForm::PuttingOutForm(CWnd* parent)
	:CDialog(PuttingOutForm::IDD, parent) {
}

BOOL PuttingOutForm::OnInitDialog() {
	CDialog::OnInitDialog();

	BusinessCardBinderForm* businessCardBinderForm;
	BusinessCard* index;
	BusinessCard* temp;
	BusinessCard businessCard;
	HTREEITEM parent;
	HTREEITEM company;
	HTREEITEM personal;
	CString companyText;
	Index* index_;
	Index* indexPutOut=NULL;
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

	//1. 뽑기윈도우 생성될 때
	  //1.1. 명함철 윈도우를 찾는다.
	  businessCardBinderForm = (BusinessCardBinderForm*)CWnd::FindWindow("#32770", "명함철");

	  //1.2. 명함철 윈도우의 명함철에서 현재 명함의 위치를 찾는다.
	  index = businessCardBinderForm->businessCardBinder->GetCurrent();
	  temp = businessCardBinderForm->businessCardBinder->GetCurrent();

	  //1.3. 명함철 윈도우의 명함철에서 현재 명함을 뽑는다.
	  businessCard = businessCardBinderForm->businessCardBinder->PutOut(index);

	  businessCardBinderForm->Delete(businessCard);

	  //1.4. 트리뷰 상 상호핸들 찾는다.
	  //회사들 핸들 찾는다.
	  parent = ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(NULL, TVGN_ROOT);
	  //상호명 찾는다.
	  companyName = CString(businessCard.GetCompanyName().c_str());
	  //상호핸들 찾는다.
	  company = ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(parent, TVGN_CHILD);
	  companyText = ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))->GetItemText(company);
	  while (companyText != companyName) {
		  company = ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))
			  ->GetNextItem(company, TVGN_NEXT);
		  companyText = ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))->GetItemText(company);
	  }

	  //1.5. 트리뷰 상 개인핸들 찾는다.
	  index_ = businessCardBinderForm->indexBinder->Find((LPCTSTR)companyName);
	  arrayIndex = index_->Find(temp);
	  personal = ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(company, TVGN_CHILD);
	  while (arrayIndex > i) {
		  personal = ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))
			  ->GetNextItem(personal, TVGN_NEXT);
		  i++;
	  }

	  //1.6. 개인핸들 없앤다.
	  ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))->DeleteItem(personal);

	  //1.7. 색인철에서 뽑는다.
	  businessCardBinderForm->indexBinder->PutOut((LPCTSTR)companyName, temp);

	  indexPutOut = businessCardBinderForm->indexBinder->Find((LPCTSTR)companyName);

	  //1.8. 색인철에 색인이 없는 경우, 상호핸들 없앤다.
	  if (indexPutOut == NULL) {
		  ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))->DeleteItem(company);
	  }

	  //1.9. 뽑은 명함을 뽑기윈도우에 출력한다.
	  personalName = CString(businessCard.GetPersonalName().c_str());
	  GetDlgItem(IDC_EDIT_PERSONALNAME)->SetWindowText(personalName);
	  rankName = CString(businessCard.GetRankName().c_str());
	  GetDlgItem(IDC_EDIT_RANKNAME)->SetWindowText(rankName);
	  personalTelephoneNumber = CString(businessCard.GetPersonalTelephoneNumber().c_str());
	  GetDlgItem(IDC_EDIT_PERSONALTELEPHONENUMBER)->SetWindowText(personalTelephoneNumber);
	  emailAddress = CString(businessCard.GetEmailAddress().c_str());
	  GetDlgItem(IDC_EDIT_EMAILADDRESS)->SetWindowText(emailAddress);
	  companyName = CString(businessCard.GetCompanyName().c_str());
	  GetDlgItem(IDC_EDIT_COMPANYNAME)->SetWindowText(companyName);
	  address = CString(businessCard.GetAddress().c_str());
	  GetDlgItem(IDC_EDIT_ADDRESS)->SetWindowText(address);
	  companyTelephoneNumber = CString(businessCard.GetCompanyTelphoneNumber().c_str());
	  GetDlgItem(IDC_EDIT_COMPANYTELEPHONENUMBER)->SetWindowText(companyTelephoneNumber);
	  faxNumber = CString(businessCard.GetFaxNumber().c_str());
	  GetDlgItem(IDC_EDIT_FAXNUMBER)->SetWindowText(faxNumber);
	  url = CString(businessCard.GetUrl().c_str());
	  GetDlgItem(IDC_EDIT_URL)->SetWindowText(url);

	  //2.0. 명함철 윈도우의 명함철에서 현재 명함의 위치를 읽는다.
	  index = businessCardBinderForm->businessCardBinder->GetCurrent();

	  //2.1. 현재 명함이 있는 경우,
	  if (index != NULL) {
		  //2.1.1. 명함철 윈도우의 명함에 현재 명함을 출력한다.
		  personalName = CString(index->GetPersonalName().c_str());
		  businessCardBinderForm->GetDlgItem(IDC_EDIT_PERSONALNAME)->SetWindowText(personalName);
		  rankName = CString(index->GetRankName().c_str());
		  businessCardBinderForm->GetDlgItem(IDC_EDIT_RANKNAME)->SetWindowText(rankName);
		  personalTelephoneNumber = CString(index->GetPersonalTelephoneNumber().c_str());
		  businessCardBinderForm->GetDlgItem(IDC_EDIT_PERSONALTELEPHONENUMBER)
			  ->SetWindowText(personalTelephoneNumber);
		  emailAddress = CString(index->GetEmailAddress().c_str());
		  businessCardBinderForm->GetDlgItem(IDC_EDIT_EMAILADDRESS)->SetWindowText(emailAddress);
		  companyName = CString(index->GetCompanyName().c_str());
		  businessCardBinderForm->GetDlgItem(IDC_EDIT_COMPANYNAME)->SetWindowText(companyName);
		  address = CString(index->GetAddress().c_str());
		  businessCardBinderForm->GetDlgItem(IDC_EDIT_COMPANYNAME)->SetWindowText(address);
		  companyTelephoneNumber = CString(index->GetCompanyTelphoneNumber().c_str());
		  businessCardBinderForm->GetDlgItem(IDC_EDIT_COMPANYTELEPHONENUMBER)->SetWindowText(companyTelephoneNumber);
		  faxNumber = CString(index->GetFaxNumber().c_str());
		  businessCardBinderForm->GetDlgItem(IDC_EDIT_FAXNUMBER)->SetWindowText(faxNumber);
		  url = CString(index->GetUrl().c_str());
		  businessCardBinderForm->GetDlgItem(IDC_EDIT_URL)->SetWindowText(url);

		  //2.1.2. 트리뷰 상 상호핸들 찾는다.
		  company=((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))
			  ->GetNextItem(parent, TVGN_CHILD);
		  companyText = ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))->GetItemText(company);
		  while (companyText != companyName) {
			  company = ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))
				  ->GetNextItem(company, TVGN_NEXT);
			  companyText = ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))
				  ->GetItemText(company);
		  }

		  //2.1.3. 트리뷰 상 개인핸들 찾는다.
		  index_ = businessCardBinderForm->indexBinder->Find((LPCTSTR)companyName);
		  arrayIndex = index_->Find(index);
		  
		  personal = ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))
			  ->GetNextItem(company, TVGN_CHILD);
		  i = 0;
		  while (arrayIndex > i) {
			  personal = ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))
				  ->GetNextItem(personal, TVGN_NEXT);
			  i++;
		  }
		  
		  //2.1.4. 회사들 핸들 펼친다.
		  ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))->Expand(parent, TVE_EXPAND);

		  //2.1.5. 상호핸들 펼친다.
		  ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))->Expand(company, TVE_EXPAND);

		  //2.1.6. 개인핸들 선택한다.
		  ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))->SelectItem(personal);
	  }
	  //2.2. 현재 명함이 없는 경우, 명함철 윈도우의 명함을 공백 처리한다.
	  else {
		  businessCardBinderForm->GetDlgItem(IDC_EDIT_PERSONALNAME)->SetWindowText("");
		  businessCardBinderForm->GetDlgItem(IDC_EDIT_RANKNAME)->SetWindowText("");
		  businessCardBinderForm->GetDlgItem(IDC_EDIT_PERSONALTELEPHONENUMBER)->SetWindowText("");
		  businessCardBinderForm->GetDlgItem(IDC_EDIT_EMAILADDRESS)->SetWindowText("");
		  businessCardBinderForm->GetDlgItem(IDC_EDIT_COMPANYNAME)->SetWindowText("");
		  businessCardBinderForm->GetDlgItem(IDC_EDIT_ADDRESS)->SetWindowText("");
		  businessCardBinderForm->GetDlgItem(IDC_EDIT_COMPANYTELEPHONENUMBER)->SetWindowText("");
		  businessCardBinderForm->GetDlgItem(IDC_EDIT_FAXNUMBER)->SetWindowText("");
		  businessCardBinderForm->GetDlgItem(IDC_EDIT_URL)->SetWindowText("");
	  }

	  return FALSE;
}

void PuttingOutForm::OnClose() {
	int ret;
	CString personalName;
	CString rankName;
	CString personalTelephoneNumber;
	CString emailAddress;
	CString companyName;
	CString address;
	CString companyTelephoneNumber;
	CString faxNumber;
	CString url;
	BusinessCardBinderForm* businessCardBinderForm;
	BusinessCard* index;
	Index* index_;
	HTREEITEM parent;
	HTREEITEM company;
	HTREEITEM personal;
	CString companyText;

	//2. 닫기버튼 클릭했을 때
	  //2.1. 메세지박스 띄운다.
	  ret = MessageBox("꽂으시겠습니까?", "알림창", MB_YESNOCANCEL);

	  //2.2. '예'버튼을 클릭했을 때
	  if (ret == IDYES) {
		  //2.2.1. 명함을 읽는다.
		  GetDlgItem(IDC_EDIT_PERSONALNAME)->GetWindowText(personalName);
		  GetDlgItem(IDC_EDIT_RANKNAME)->GetWindowText(rankName);
		  GetDlgItem(IDC_EDIT_PERSONALTELEPHONENUMBER)->GetWindowText(personalTelephoneNumber);
		  GetDlgItem(IDC_EDIT_EMAILADDRESS)->GetWindowText(emailAddress);
		  GetDlgItem(IDC_EDIT_COMPANYNAME)->GetWindowText(companyName);
		  GetDlgItem(IDC_EDIT_ADDRESS)->GetWindowText(address);
		  GetDlgItem(IDC_EDIT_COMPANYTELEPHONENUMBER)->GetWindowText(companyTelephoneNumber);
		  GetDlgItem(IDC_EDIT_FAXNUMBER)->GetWindowText(faxNumber);
		  GetDlgItem(IDC_EDIT_URL)->GetWindowText(url);

		  //2.2.2. 명함철 윈도우를 찾는다.
		  businessCardBinderForm = (BusinessCardBinderForm*)CWnd::FindWindow("#32770", "명함철");

		  //2.2.3. 명함철 윈도우의 명함철에 꽂는다.
		  BusinessCard businessCard((LPCTSTR)personalName, (LPCTSTR)rankName, (LPCTSTR)personalTelephoneNumber,
			  (LPCTSTR)emailAddress, (LPCTSTR)companyName, (LPCTSTR)address, (LPCTSTR)companyTelephoneNumber,
			  (LPCTSTR)faxNumber, (LPCTSTR)url);

		  index = businessCardBinderForm->businessCardBinder->PutIn(businessCard);

		  //2.2.4. 명함철 윈도우의 색인철에 꽂는다.
		  index_ = businessCardBinderForm->indexBinder->PutIn(index);

		  businessCardBinderForm->Insert(index);

		  //2.2.5. 회사들 핸들을 찾는다.
		  parent = ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(NULL, TVGN_ROOT);

		  //2.2.6. 상호핸들 찾는다.
		  company = ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))
			  ->GetNextItem(parent, TVGN_CHILD);
		  companyText = ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))
			  ->GetItemText(company);
		  while (company != NULL && companyText != companyName) {
			  company = ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))
				  ->GetNextItem(company, TVGN_NEXT);
			  companyText = ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))
				  ->GetItemText(company);
		  }
		  //2.2.7. 해당 상호핸들 없으면, 상호핸들 추가한다.
		  if (company == NULL) {
			  company = ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))
				  ->InsertItem(companyName, parent, TVI_SORT);
		  }

		  //2.2.8. 상호핸들 하위에 개인핸들 추가한다.
		  personal = ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))
			  ->InsertItem(personalName, company, TVI_LAST);

		  //2.2.9. 회사들 핸들 펼친다.
		  ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))->Expand(parent, TVE_EXPAND);

		  //2.3.0. 상호핸들 펼친다.
		  ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))->Expand(company, TVE_EXPAND);

		  //2.3.1. 개인핸들 선택한다.
		  ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))->SelectItem(personal);

		  //2.3.2. 명함철 윈도우의 명함에 현재 명함을 출력한다.
		  personalName = CString(index->GetPersonalName().c_str());
		  businessCardBinderForm->GetDlgItem(IDC_EDIT_PERSONALNAME)->SetWindowText(personalName);
		  rankName = CString(index->GetRankName().c_str());
		  businessCardBinderForm->GetDlgItem(IDC_EDIT_RANKNAME)->SetWindowText(rankName);
		  personalTelephoneNumber = CString(index->GetPersonalTelephoneNumber().c_str());
		  businessCardBinderForm->GetDlgItem(IDC_EDIT_PERSONALTELEPHONENUMBER)
			  ->SetWindowText(personalTelephoneNumber);
		  emailAddress = CString(index->GetEmailAddress().c_str());
		  businessCardBinderForm->GetDlgItem(IDC_EDIT_EMAILADDRESS)->SetWindowText(emailAddress);
		  companyName = CString(index->GetCompanyName().c_str());
		  businessCardBinderForm->GetDlgItem(IDC_EDIT_COMPANYNAME)->SetWindowText(companyName);
		  address = CString(index->GetAddress().c_str());
		  businessCardBinderForm->GetDlgItem(IDC_EDIT_ADDRESS)->SetWindowText(address);
		  companyTelephoneNumber = CString(index->GetCompanyTelphoneNumber().c_str());
		  businessCardBinderForm->GetDlgItem(IDC_EDIT_COMPANYTELEPHONENUMBER)
			  ->SetWindowText(companyTelephoneNumber);
		  faxNumber = CString(index->GetFaxNumber().c_str());
		  businessCardBinderForm->GetDlgItem(IDC_EDIT_FAXNUMBER)->SetWindowText(faxNumber);
		  url = CString(index->GetUrl().c_str());
		  businessCardBinderForm->GetDlgItem(IDC_EDIT_URL)->SetWindowText(url);

		  //2.3.3. 뽑기윈도우 종료한다.
		  EndDialog(0);
	  }
	  //2.3. '아니오'버튼 클릭했을 때
	  else if (ret == IDNO) {
		  //2.3.1. 뽑기윈도우 종료한다.
		  EndDialog(0);
	  }
}






































