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

	//1. ã���ư Ŭ������ ��
	  //1.1. ������ �д´�.
	  GetDlgItem(IDC_EDIT_NAME)->GetWindowText(personalName);

	  //1.2. ����ö ������ ã�´�.
	  businessCardBinderForm = (BusinessCardBinderForm*)CWnd::FindWindow("#32770", "����ö");

	  //1.3. ����ö �������� ����ö���� ã�´�.
	  if (this->indexes != NULL) {
		  delete[] this->indexes;
	  }
	  businessCardBinderForm->businessCardBinder->Find((LPCTSTR)personalName, &this->indexes, &this->count);

	  //1.4. ������ �ִ� ���, ã�� ���Ե� �� ù��° ������ ����Ѵ�.
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

	  //1.5. ������ ���� ���, ���� ó���Ѵ�.
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

	//6. ���ù�ư Ŭ������ ��
	  //6.1. ����ö �����츦 ã�´�. 
	  businessCardBinderForm = (BusinessCardBinderForm*)CWnd::FindWindow("#32770", "����ö");

	  //6.2. ����ö�������� ����ö���� ���� ������ ���� �������� �̵���Ų��.
	  index = businessCardBinderForm->businessCardBinder->Move(this->indexes[this->current]);

	  //6.3. Ʈ���信�� ��ȣ�ڵ� ã�´�.
	  //��ȣ�̸� ã�´�.
	  companyName = CString(index->GetCompanyName().c_str());
	  //ȸ��� �ڵ� ã�´�.
	  parent = ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(NULL, TVGN_ROOT);
	  //��ȣ�ڵ� ã�´�.
	  company = ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(parent, TVGN_CHILD);
	  companyText = ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))->GetItemText(company);
	  while (companyText != companyName) {
		  company = ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))
			  ->GetNextItem(company, TVGN_NEXT);
		  companyText = ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))->GetItemText(company);
	  }
	  
	  //6.4. Ʈ���信�� �����ڵ� ã�´�.
	  //����ö ã�´�.
	  index_ = businessCardBinderForm->indexBinder->Find((LPCTSTR)companyName);
	  //������ġ ã�´�.
	  arrayIndex = index_->Find(index);
	  //�����ڵ� ã�´�.
	  personal = ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))
		  ->GetNextItem(company, TVGN_CHILD);
	  while (arrayIndex > i) {
		  personal = ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))
			  ->GetNextItem(personal, TVGN_NEXT);
		  i++;
	  }

	  //6.5. ȸ��� �ڵ� ��ģ��.
	  ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))->Expand(parent, TVE_EXPAND);
	  //6.6. ��ȣ�ڵ� ��ģ��.
	  ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))->Expand(company, TVE_EXPAND);
	  //6.7. �����ڵ� �����Ѵ�.
	  ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))->SelectItem(personal);


	  //6.8. ����ö �������� ���Կ� ���� ������ ����Ѵ�.
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

	  //6.9. ã�� �����츦 �����Ѵ�.
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

	//2. ó����ư Ŭ������ ��
	  //2.1. ã�� ���Ե� ��, ù��° ������ ���Կ� ����Ѵ�.
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

	//3. ������ư Ŭ������ ��
	  //3.1. ���� ������ ���� ������ ���Կ� ����Ѵ�.
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

	//4. ������ư Ŭ������ ��
	  //4.1. ���� ������ ���� ������ ���Կ� ����Ѵ�.
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

	//5. ��������ư Ŭ������ ��
	  //5.1. ã�� ���Ե� �� ������ ������ ���Կ� ����Ѵ�.
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
	//7. �ݱ��ư Ŭ������ ��
	  //7.1. ã�������� �����Ѵ�.
	  if (this->indexes != NULL) {
		  delete[] this->indexes;
		  this->indexes = NULL;
	  }
	  EndDialog(0);
}

