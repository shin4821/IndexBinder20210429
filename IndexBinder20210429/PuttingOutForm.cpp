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

	//1. �̱������� ������ ��
	  //1.1. ����ö �����츦 ã�´�.
	  businessCardBinderForm = (BusinessCardBinderForm*)CWnd::FindWindow("#32770", "����ö");

	  //1.2. ����ö �������� ����ö���� ���� ������ ��ġ�� ã�´�.
	  index = businessCardBinderForm->businessCardBinder->GetCurrent();
	  temp = businessCardBinderForm->businessCardBinder->GetCurrent();

	  //1.3. ����ö �������� ����ö���� ���� ������ �̴´�.
	  businessCard = businessCardBinderForm->businessCardBinder->PutOut(index);

	  businessCardBinderForm->Delete(businessCard);

	  //1.4. Ʈ���� �� ��ȣ�ڵ� ã�´�.
	  //ȸ��� �ڵ� ã�´�.
	  parent = ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(NULL, TVGN_ROOT);
	  //��ȣ�� ã�´�.
	  companyName = CString(businessCard.GetCompanyName().c_str());
	  //��ȣ�ڵ� ã�´�.
	  company = ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(parent, TVGN_CHILD);
	  companyText = ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))->GetItemText(company);
	  while (companyText != companyName) {
		  company = ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))
			  ->GetNextItem(company, TVGN_NEXT);
		  companyText = ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))->GetItemText(company);
	  }

	  //1.5. Ʈ���� �� �����ڵ� ã�´�.
	  index_ = businessCardBinderForm->indexBinder->Find((LPCTSTR)companyName);
	  arrayIndex = index_->Find(temp);
	  personal = ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(company, TVGN_CHILD);
	  while (arrayIndex > i) {
		  personal = ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))
			  ->GetNextItem(personal, TVGN_NEXT);
		  i++;
	  }

	  //1.6. �����ڵ� ���ش�.
	  ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))->DeleteItem(personal);

	  //1.7. ����ö���� �̴´�.
	  businessCardBinderForm->indexBinder->PutOut((LPCTSTR)companyName, temp);

	  indexPutOut = businessCardBinderForm->indexBinder->Find((LPCTSTR)companyName);

	  //1.8. ����ö�� ������ ���� ���, ��ȣ�ڵ� ���ش�.
	  if (indexPutOut == NULL) {
		  ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))->DeleteItem(company);
	  }

	  //1.9. ���� ������ �̱������쿡 ����Ѵ�.
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

	  //2.0. ����ö �������� ����ö���� ���� ������ ��ġ�� �д´�.
	  index = businessCardBinderForm->businessCardBinder->GetCurrent();

	  //2.1. ���� ������ �ִ� ���,
	  if (index != NULL) {
		  //2.1.1. ����ö �������� ���Կ� ���� ������ ����Ѵ�.
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

		  //2.1.2. Ʈ���� �� ��ȣ�ڵ� ã�´�.
		  company=((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))
			  ->GetNextItem(parent, TVGN_CHILD);
		  companyText = ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))->GetItemText(company);
		  while (companyText != companyName) {
			  company = ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))
				  ->GetNextItem(company, TVGN_NEXT);
			  companyText = ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))
				  ->GetItemText(company);
		  }

		  //2.1.3. Ʈ���� �� �����ڵ� ã�´�.
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
		  
		  //2.1.4. ȸ��� �ڵ� ��ģ��.
		  ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))->Expand(parent, TVE_EXPAND);

		  //2.1.5. ��ȣ�ڵ� ��ģ��.
		  ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))->Expand(company, TVE_EXPAND);

		  //2.1.6. �����ڵ� �����Ѵ�.
		  ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))->SelectItem(personal);
	  }
	  //2.2. ���� ������ ���� ���, ����ö �������� ������ ���� ó���Ѵ�.
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

	//2. �ݱ��ư Ŭ������ ��
	  //2.1. �޼����ڽ� ����.
	  ret = MessageBox("�����ðڽ��ϱ�?", "�˸�â", MB_YESNOCANCEL);

	  //2.2. '��'��ư�� Ŭ������ ��
	  if (ret == IDYES) {
		  //2.2.1. ������ �д´�.
		  GetDlgItem(IDC_EDIT_PERSONALNAME)->GetWindowText(personalName);
		  GetDlgItem(IDC_EDIT_RANKNAME)->GetWindowText(rankName);
		  GetDlgItem(IDC_EDIT_PERSONALTELEPHONENUMBER)->GetWindowText(personalTelephoneNumber);
		  GetDlgItem(IDC_EDIT_EMAILADDRESS)->GetWindowText(emailAddress);
		  GetDlgItem(IDC_EDIT_COMPANYNAME)->GetWindowText(companyName);
		  GetDlgItem(IDC_EDIT_ADDRESS)->GetWindowText(address);
		  GetDlgItem(IDC_EDIT_COMPANYTELEPHONENUMBER)->GetWindowText(companyTelephoneNumber);
		  GetDlgItem(IDC_EDIT_FAXNUMBER)->GetWindowText(faxNumber);
		  GetDlgItem(IDC_EDIT_URL)->GetWindowText(url);

		  //2.2.2. ����ö �����츦 ã�´�.
		  businessCardBinderForm = (BusinessCardBinderForm*)CWnd::FindWindow("#32770", "����ö");

		  //2.2.3. ����ö �������� ����ö�� �ȴ´�.
		  BusinessCard businessCard((LPCTSTR)personalName, (LPCTSTR)rankName, (LPCTSTR)personalTelephoneNumber,
			  (LPCTSTR)emailAddress, (LPCTSTR)companyName, (LPCTSTR)address, (LPCTSTR)companyTelephoneNumber,
			  (LPCTSTR)faxNumber, (LPCTSTR)url);

		  index = businessCardBinderForm->businessCardBinder->PutIn(businessCard);

		  //2.2.4. ����ö �������� ����ö�� �ȴ´�.
		  index_ = businessCardBinderForm->indexBinder->PutIn(index);

		  businessCardBinderForm->Insert(index);

		  //2.2.5. ȸ��� �ڵ��� ã�´�.
		  parent = ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(NULL, TVGN_ROOT);

		  //2.2.6. ��ȣ�ڵ� ã�´�.
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
		  //2.2.7. �ش� ��ȣ�ڵ� ������, ��ȣ�ڵ� �߰��Ѵ�.
		  if (company == NULL) {
			  company = ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))
				  ->InsertItem(companyName, parent, TVI_SORT);
		  }

		  //2.2.8. ��ȣ�ڵ� ������ �����ڵ� �߰��Ѵ�.
		  personal = ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))
			  ->InsertItem(personalName, company, TVI_LAST);

		  //2.2.9. ȸ��� �ڵ� ��ģ��.
		  ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))->Expand(parent, TVE_EXPAND);

		  //2.3.0. ��ȣ�ڵ� ��ģ��.
		  ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))->Expand(company, TVE_EXPAND);

		  //2.3.1. �����ڵ� �����Ѵ�.
		  ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))->SelectItem(personal);

		  //2.3.2. ����ö �������� ���Կ� ���� ������ ����Ѵ�.
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

		  //2.3.3. �̱������� �����Ѵ�.
		  EndDialog(0);
	  }
	  //2.3. '�ƴϿ�'��ư Ŭ������ ��
	  else if (ret == IDNO) {
		  //2.3.1. �̱������� �����Ѵ�.
		  EndDialog(0);
	  }
}






































