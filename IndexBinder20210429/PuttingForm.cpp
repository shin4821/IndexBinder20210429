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

	TCHAR rankNames[][8] = { "����","���","����","�븮","����","����","����","�̻�","����","�λ���","����" };
	TCHAR portalSites[][32] = { "naver.com","daum.net","gmail.com" };
	Long i;

	//1. �ȱ� �����찡 ������ ��
	  //1.1. ����ĭ�� �޺��ڽ��� ���޵��� �߰��Ѵ�.
	  for (i = 0; i < sizeof(rankNames) / sizeof(rankNames[0]); i++) {
		((CComboBox*)GetDlgItem(IDC_EDIT_RANKNAME))->AddString((LPCTSTR)rankNames[i]);
	  }
	  //1.2. �̸��� �ּ�ĭ�� �޺��ڽ��� �̸��� �ּҵ��� �߰��Ѵ�.
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

	//2. ȸ�� ��Ī�� ���Ͽ� ��Ŀ���� ���� ��,
	  //2.1. ��ȣ�� �д´�.
	  GetDlgItem(IDC_EDIT_COMPANYNAME)->GetWindowText(companyName);

	  //2.2. ����ö�����츦 ã�´�.
	  businessCardBinderForm = (BusinessCardBinderForm*)CWnd::FindWindow("#32770", "����ö");

	  //2.3. ����ö�������� ����ö���� �ش��ȣ�� ã�´�.
	  index_ = businessCardBinderForm->indexBinder->Find((LPCTSTR)companyName);

	  //2.4. ���� ��ȣ�� ���� ���, �ּ�, ��ȭ��ȣ, �ѽ���ȣ, URL�� ���Կ� ����Ѵ�.
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

	//3. �ȱ��ư Ŭ������ ��
	  //3.1. ������ �д´�.
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
	  
	  // �̸��� �ּ� �����.
	  totalEmailAddress.Format("%s@%s", emailAddress, portalSite);

	  //3.2. ����ö�����츦 ã�´�.
	  businessCardBinderForm = (BusinessCardBinderForm*)CWnd::FindWindow("#32770", "����ö");

	  //3.3. ����ö�������� ����ö�� �ȴ´�.
	  BusinessCard businessCard((LPCTSTR)personalName, (LPCTSTR)rankName, (LPCTSTR)personalTelephoneNumber, 
		  (LPCTSTR)totalEmailAddress, (LPCTSTR)companyName, (LPCTSTR)address, (LPCTSTR)companyTelephoneNumber, 
		  (LPCTSTR)faxNumber, (LPCTSTR)url);

	  index = businessCardBinderForm->businessCardBinder->PutIn(businessCard);

	  businessCardBinderForm->Insert(index);

	  //3.4. ����ö�������� ���Կ� ���� ������ ����Ѵ�.
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

	  //3.5. ����ö�������� ����ö�� �ȴ´�.
	  businessCardBinderForm->indexBinder->PutIn(index);

	  //3.6. ȸ��� �ڵ��� ã�´�.
	  parent = ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(NULL, TVGN_ROOT);

	  //3.7. ��ȣ�ڵ� ã�´�.
	  company = ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(parent, TVGN_CHILD);
	  companyText = ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))->GetItemText(company);

	  while (company != NULL && companyText != companyName) {
		  company = ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))
			  ->GetNextItem(company, TVGN_NEXT);
		  companyText = ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))->GetItemText(company);
	  }

	  //3.8. �ش� ��ȣ�ڵ��� ������ ȸ����ڵ� ������ �߰��Ѵ�.
	  if (company == NULL) {
		  company = ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))->InsertItem(companyName,
			  parent, TVI_SORT);
	  }
	  //3.9. ��ȣ�ڵ� ������ �����ڵ� �߰��Ѵ�.
	  personal = ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))->InsertItem(personalName,
		  company, TVI_LAST);

	  //4.0. ȸ��� �ڵ� ��ģ��.
	  ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))->Expand(parent, TVE_EXPAND);
	  //4.1. ��ȣ�ڵ� ��ģ��.
	  ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))->Expand(company, TVE_EXPAND);
	  //4.2. �����ڵ� �����Ѵ�.
	  ((CTreeCtrl*)businessCardBinderForm->GetDlgItem(IDC_TREE_INDEXES))->SelectItem(personal);

	  //4.3. �ȱ������� �����Ѵ�.
	  EndDialog(0);
}




void PuttingForm::OnClose() {
	//4. �ݱ��ư Ŭ������ ��
	  //4.1. �ȱ������� �����Ѵ�.
	  EndDialog(0);
}

