//BusinessCardBinderForm.cpp
#include"BusinessCardBinderForm.h"
#include"BusinessCardBinder.h"
#include"IndexBinder.h"
#include"PuttingForm.h"
#include"FindingForm.h"
#include"PuttingOutForm.h"
#include<afxdb.h>
#include<afxcmn.h>
#include<ctime>
#include<cstdio>
#pragma warning(disable:4996)

BEGIN_MESSAGE_MAP(BusinessCardBinderForm, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_PUT, OnPutButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON_FIND, OnFindButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON_PUTOUT, OnPutOutButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON_ARRANGE, OnArrangeButtonClicked)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE_INDEXES, OnTreeViewItemDoubleClicked)
	ON_BN_CLICKED(IDC_BUTTON_FIRST, OnFirstButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON_PREVIOUS, OnPreviousButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, OnNextButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON_LAST, OnLastButtonClicked)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

BusinessCardBinderForm::BusinessCardBinderForm(CWnd* parent)
	:CDialog(BusinessCardBinderForm::IDD, parent) {
	this->businessCardBinder = NULL;
}

Long BusinessCardBinderForm::Load() {
	CDatabase db;
	CRecordset rs(&db);
	CString sql = "SELECT Personal.name, Personal.rankName, Personal.telephoneNumber, Personal.emailAddress,"
		"Company.name, Company.address, Company.telephoneNumber, Company.faxNumber, "
		"Company.url FROM Company INNER JOIN Personal ON Company.companyCode=Personal.companyCode"
		" ORDER BY Personal.code ASC;";

	db.OpenEx("DSN=BusinessCardBinder;UID=Sydney;PWD=4821");
	rs.Open(AFX_DB_USE_DEFAULT_TYPE, sql);
	CString personalName;
	CString rankName;
	CString personalTelephoneNumber;
	CString emailAddress;
	CString companyName;
	CString address;
	CString companyTelephoneNumber;
	CString faxNumber;
	CString url;

	while (!rs.IsEOF()) {
		rs.GetFieldValue((short)0, personalName);
		rs.GetFieldValue((short)1, rankName);
		rs.GetFieldValue((short)2, personalTelephoneNumber);
		rs.GetFieldValue((short)3, emailAddress);
		rs.GetFieldValue((short)4, companyName);
		rs.GetFieldValue((short)5, address);
		rs.GetFieldValue((short)6, companyTelephoneNumber);
		rs.GetFieldValue((short)7, faxNumber);
		rs.GetFieldValue((short)8, url);
		
		BusinessCard businessCard((LPCTSTR)personalName, (LPCTSTR)rankName, (LPCTSTR)personalTelephoneNumber,
			(LPCTSTR)emailAddress, (LPCTSTR)companyName, (LPCTSTR)address, (LPCTSTR)companyTelephoneNumber,
			(LPCTSTR)faxNumber, (LPCTSTR)url);

		this->businessCardBinder->PutIn(businessCard);
		rs.MoveNext();
	}
	rs.Close();
	db.Close();

	return this->businessCardBinder->GetLength();
}
CString BusinessCardBinderForm::GetCompanyCode(BusinessCard* index) {
	CDatabase db;
	CRecordset rs(&db);
	CRecordset rs1(&db);
	CString sql;
	int number;
	time_t timer;
	struct tm* today;
	CString todayDate;
	CString year;
	CString companyCode;
	CString companyCode_;

	//1. �Է¹��� ȸ��� ������ ȸ�簡 �ִ��� Ȯ���Ѵ�.
	sql.Format(
		"SELECT Company.companyCode FROM Company WHERE name='%s' AND address='%s' AND telephoneNumber='%s' "
		"AND faxNumber='%s' AND url='%s';", 
		index->GetCompanyName().c_str(), index->GetAddress().c_str(), index->GetCompanyTelphoneNumber().c_str(), 
		index->GetFaxNumber().c_str(), index->GetUrl().c_str());
	
	db.OpenEx("DSN=BusinessCardBinder;UID=Sydney;PWD=4821");
	rs.Open(AFX_DB_USE_DEFAULT_TYPE, sql);


	//2. �ش� ȸ���ڵ尡 �ִ� ���, ȸ�� �ڵ带 ���Ѵ�.
	if (!rs.IsEOF()) {
		rs.GetFieldValue((short)0, companyCode);
	}
    
	// ���� ��¥�� ���Ѵ�.
	time(&timer);
	today = localtime(&timer);
	year.Format("%4d", today->tm_year + 1900); //2021 ���      
	year = year.Right(2); //21 ���
	todayDate.Format("%s%02d%02d", year, today->tm_mon + 1, today->tm_mday); //210414 ���


	sql = "SELECT Company.companyCode FROM Company ORDER BY companyCode DESC;";
	rs1.Open(AFX_DB_USE_DEFAULT_TYPE, sql);


	if (rs1.IsEOF()) {
		companyCode.Format("C%s001", todayDate);
	}
	//4. �ش� ȸ���ڵ�� ������, �ٸ� �ڵ�� �ִ� ���,
	else if (!rs1.IsEOF() && rs.IsEOF()) {
		//4.1. �ش� �ڵ� �� ���� ������ �ڵ带 ���Ѵ�.
		rs1.GetFieldValue((short)0, companyCode); //P210414003
		companyCode_ = companyCode;

		//4.2. ������ �ڵ��� ��¥ �κ��� �и��Ѵ�.
		companyCode = companyCode.Mid(1, 6);//210414

		//4.3. ������ �ڵ�� ���� ��¥�� �ٸ� ���, ȸ�� �ڵ带 �����Ѵ�.
		if (companyCode != todayDate) {
			companyCode_.Format("C%s000", todayDate); //P210416000
		}

		//4.4. �� �� ���ڸ� ���ںκ��� �и��Ѵ�.
		companyCode_ = companyCode_.Right(3);

		//4.5. �и��� ���ںκ��� ������ �ٲ��ش�.
		number = _ttoi(companyCode_);

		//4.6. ���ڸ� 1 �����ش�.
		number++;

		//4.7. ���ڸ� ���� ��¥�� �����ش�.
		companyCode.Format("C%s%03d", todayDate, number);
	}


#if 0
	//3. �����Ͱ� ���� ���,
	else {
		//3.1. ���� ��¥�� ���Ѵ�.
		time(&timer);
		today = localtime(&timer);
		year.Format("%4d", today->tm_year + 1900); //2021 ���      
		year = year.Right(2); //21 ���
		todayDate.Format("%s%02d%02d", year, today->tm_mon + 1, today->tm_mday); //210414 ���


		//3.2. ������ ��ü�� ���� ������, �ش� ȸ�� �ڵ常 ���°����� Ȯ���Ѵ�.
		//3.3. �ش� ȸ���� �ڵ常 ���� ���,
		if (!rs1.IsEOF()) { //�ٸ� ȸ�� �ڵ�� �ִٴ� ��.
			//3.3.1. �ش� �ڵ� �� ���� ������ �ڵ带 ���Ѵ�.
			rs1.GetFieldValue((short)0, companyCode); //P210414003
			companyCode_ = companyCode; 

			//3.3.2. ������ �ڵ��� ��¥ �κ��� �и��Ѵ�.
			companyCode = companyCode.Mid(1, 6);//210414

			//3.3.3. ������ �ڵ�� ���� ��¥�� �ٸ� ���, ȸ�� �ڵ带 �����Ѵ�.
			if (companyCode !=todayDate) {
				companyCode_.Format("C%s000", todayDate); //P210416000
			}

			//3.3.4. �� �� ���ڸ� ���ںκ��� �и��Ѵ�.
			companyCode_ = companyCode_.Right(3);
			
			//3.3.5. �и��� ���ںκ��� ������ �ٲ��ش�.
			number = _ttoi(companyCode_);

			//3.3.6. ���ڸ� 1 �����ش�.
			number++;

			//3.3.7. ���ڸ� ���� ��¥�� �����ش�.
			companyCode.Format("C%s%03d", todayDate, number);
			
		}
		//3.4. ������ ��ü�� ���� ���, �ڵ带 �������ش�.
		else {
			companyCode.Format("C%s001", todayDate);
		}

	}
#endif
	//4. �ڵ带 ����Ѵ�.
	rs1.Close();
	rs.Close();
	db.Close();

	return companyCode;
}

CString BusinessCardBinderForm::GetCode() {
	CDatabase db;
	CRecordset rs(&db);
	CString sql = "SELECT Personal.code FROM Personal ORDER BY code DESC;";
	CString code;
	int Number;
	time_t timer;
	struct tm* today;
	CString todayDate;
	CString year;
	CString code_;
	int number;

	//1. ���� ��¥�� ���Ѵ�. (210416 ��������)
	time(&timer);
	today = localtime(&timer);
	year.Format("%4d", today->tm_year + 1900); //2021 ���      
	year = year.Right(2); //21 ���
	todayDate.Format("%s%02d%02d", year, today->tm_mon + 1, today->tm_mday); //210414 ���

	//2. �ڵ尡 �ִ� ���,
	db.OpenEx("DSN=BusinessCardBinder;UID=Sydney;PWD=4821");
	rs.Open(AFX_DB_USE_DEFAULT_TYPE, sql);

	if (!rs.IsEOF()) {
		//2.1. ���� ū �ڵ带 ���Ѵ�.
		rs.GetFieldValue((short)0, code); //P210414003
		code_ = code;

		//2.2. ��¥�κи� �и��Ѵ�.
		code = code.Mid(1, 6); //210414

		//2.3. ���� ��¥�� ���� ���� ���, �⺻ �ڵ带 �������ش�.

		if (code!=todayDate) {
			code_.Format("P%s000", todayDate);
		}

		//2.4. �ڵ��� �� ���ڸ��� �и��Ѵ�.
		code_ = code_.Right(3);

		//2.5. �и��� ���ںκ��� ������ ��ȯ�Ѵ�.
		number = _ttoi(code_);

		//2.6. ������ 1�� ���Ѵ�.
		number++;

		//2.7. ���ڸ� P210414001 �������� ��ȯ�Ѵ�.
		code.Format("P%s%03d", todayDate, number);
	}
	//3. �ڵ尡 �ƿ� ���� ���, �⺻ �ڵ�� �������ش�.
	else {
		code.Format("P%s001", todayDate);
	}
	//4. �ڵ带 ����Ѵ�.
	rs.Close();
	db.Close();

	return code;
}

void BusinessCardBinderForm::Insert(BusinessCard* index) {
	CDatabase db;
	CRecordset rs(&db);
	CString sql;

	sql.Format(
		"SELECT Company.companyCode FROM Company WHERE name='%s' AND address='%s' AND telephoneNumber='%s' "
		"AND faxNumber='%s' AND url='%s';",
		index->GetCompanyName().c_str(), index->GetAddress().c_str(), index->GetCompanyTelphoneNumber().c_str(),
		index->GetFaxNumber().c_str(), index->GetUrl().c_str());
	
	CString companyCode;
	CString code;


	db.OpenEx("DSN=BusinessCardBinder;UID=Sydney;PWD=4821");
	rs.Open(AFX_DB_USE_DEFAULT_TYPE, sql);

	companyCode = GetCompanyCode(index);

	//1. ȸ�� DB�� �ߺ��Ǵ� ȸ�簡 ������ Insert�Ѵ�.
	if (rs.IsEOF()) {
		sql.Format("INSERT INTO Company(companyCode, name, address,telephoneNumber,faxNumber,url)" 
			" VALUES('%s','%s', '%s', '%s', '%s', '%s');", (LPCTSTR)companyCode, index->GetCompanyName().c_str(),
			index->GetAddress().c_str(), index->GetCompanyTelphoneNumber().c_str(),
			index->GetFaxNumber().c_str(), index->GetUrl().c_str());
		db.ExecuteSQL(sql);
	}

	//2. ���� Insert�Ѵ�.
	code = GetCode();

	sql.Format("INSERT INTO Personal(code, companyCode, name, rankName, telephoneNumber, emailAddress)"
		" VALUES('%s','%s','%s','%s','%s','%s');", (LPCTSTR)code, (LPCTSTR)companyCode,
		index->GetPersonalName().c_str(),
		index->GetRankName().c_str(), index->GetPersonalTelephoneNumber().c_str(),
		index->GetEmailAddress().c_str());
	db.ExecuteSQL(sql);


	rs.Close();
	db.Close();
}
void BusinessCardBinderForm::Delete(BusinessCard businessCard) {
	CDatabase db;
	CRecordset rs(&db);
	CRecordset rs1(&db);
	CString sql;
	sql.Format(
		"SELECT Personal.code, Personal.companyCode FROM Personal WHERE Personal.name='%s' AND"
		" Personal.rankName='%s' AND Personal.telephoneNumber='%s' AND Personal.emailAddress='%s';",
		businessCard.GetPersonalName().c_str(), businessCard.GetRankName().c_str(), 
		businessCard.GetPersonalTelephoneNumber().c_str(),businessCard.GetEmailAddress().c_str()
	);

	//1. ������ code�� companyCode�� ã�´�.
	db.OpenEx("DSN=BusinessCardBinder;UID=Sydney;PWD=4821");
	rs.Open(AFX_DB_USE_DEFAULT_TYPE, sql);
	CString code;
	CString companyCode;

	rs.GetFieldValue((short)0, code);
	rs.GetFieldValue((short)1, companyCode);

	//2. ���ο��� �ش� code�� �����Ѵ�.
	sql.Format("DELETE FROM Personal WHERE code='%s';", (LPCTSTR)code);
	db.ExecuteSQL(sql);

	//3. ���ο� �ش� companyCode�� ���� ������ ������, �ش� �ڵ� ȸ�縦 �����Ѵ�.
	sql.Format("SELECT Personal.companyCode FROM Personal WHERE companyCode='%s';",(LPCTSTR)companyCode); 

	rs1.Open(AFX_DB_USE_DEFAULT_TYPE, sql);
	if (rs.IsEOF()) {
		sql.Format("DELETE FROM Company WHERE companyCode='%s';", (LPCTSTR)companyCode);
		db.ExecuteSQL(sql);
	}

	//4. ������ �ݴ´�
	rs1.Close();
	rs.Close();
	db.Close();
}
void BusinessCardBinderForm::Modify(BusinessCard* index) {
	CDatabase db;
	CRecordset rs(&db);
	CString sql = "SELECT Personal.code, Personal.companyCode FROM Personal;";
	BusinessCard* it;
	CString code;
	CString companyCode;

	//1. index��°�� code�� companyCode�� ���Ѵ�.
	db.OpenEx("DSN=BusinessCardBinder;UID=Sydney;PWD=4821");
	rs.Open(AFX_DB_USE_DEFAULT_TYPE, sql);

	it = this->businessCardBinder->First();
	while (it != index) {
		rs.GetFieldValue((short)0, code);
		rs.GetFieldValue((short)1, companyCode);

		rs.MoveNext();
		it = this->businessCardBinder->Next();
	}

	//2. �ش� �ڵ带 ������Ʈ�Ѵ�.
	sql.Format("Update Personal SET name='%s', rankName='%s', telephoneNumber='%s', emailAddress='%s'"
		" WHERE code='%s';", index->GetPersonalName().c_str(), index->GetRankName().c_str(),
		index->GetPersonalTelephoneNumber().c_str(), index->GetEmailAddress().c_str(), (LPCTSTR)code);

	db.ExecuteSQL(sql);

	//3. �ش� ȸ���ڵ带 ������Ʈ�Ѵ�.
	sql.Format("Update Company SET name='%s', address='%s', telephoneNumber='%s', faxNumber='%s',url='%s'"
		" WHERE companyCode='%s';", index->GetCompanyName().c_str(), index->GetAddress().c_str(),
		index->GetCompanyTelphoneNumber().c_str(), index->GetFaxNumber().c_str(), index->GetUrl().c_str(),
		(LPCTSTR)companyCode);

	db.ExecuteSQL(sql);

	rs.Close();
	db.Close();
}

Long BusinessCardBinderForm::Save() {
	CDatabase db;
	CRecordset rs(&db);
	CRecordset rs1(&db);
	CRecordset rs2(&db);
	CString sql = "SELECT Personal.code, Company.companyCode"
		" FROM Personal INNER JOIN Company ON Company.companyCode = Personal.companyCode"
		" ORDER BY Personal.code ASC;";
	CString sql_;
	CString sqll;
	CString sqlPersonal;
	CString sqlCompany;
	CString code;
	CString companyCode;
	BusinessCard* it;
	BusinessCard* previous = NULL;


	//1. DB�� ��� �����͸� �����.
	db.OpenEx("DSN=BusinessCardBinder;UID=Sydney;PWD=4821");
	rs.Open(AFX_DB_USE_DEFAULT_TYPE, sql);

	db.ExecuteSQL("DELETE FROM Personal");
	db.ExecuteSQL("DELETE FROM Company");

	//2. ������ ���� �ƴѵ��� �ݺ��Ѵ�.
	it = this->businessCardBinder->First();

	while (!rs.IsEOF() && it != previous) {
		rs.GetFieldValue((short)0, code);
		rs.GetFieldValue((short)1, companyCode);

		//2.1. DB�� ������ ȸ�簡 �ִ��� Ȯ���Ѵ�.
#if 0
		sqll.Format("SELECT Company.companyCode FROM Company WHERE companyCode='%s';", (LPCTSTR)companyCode);
#endif
		sql_.Format(
			"SELECT Company.companyCode FROM Company WHERE name='%s' AND address='%s'AND telephoneNumber='%s' AND"
			" faxNumber='%s' AND url='%s';",
			it->GetCompanyName().c_str(), it->GetAddress().c_str(), it->GetCompanyTelphoneNumber().c_str(),
			it->GetFaxNumber().c_str(), it->GetUrl().c_str());


#if 0
		rs2.Open(AFX_DB_USE_DEFAULT_TYPE, sqll);
#endif
		rs1.Open(AFX_DB_USE_DEFAULT_TYPE, sql_);


		//2.2. DB�� ���� ȸ���̸�, ȸ��DB�� �߰��Ѵ�.
		if (rs1.IsEOF()) {
			sqlCompany.Format("INSERT INTO Company(companyCode, name, address, telephoneNumber, faxNumber, url)" 
				" VALUES('%s','%s','%s','%s','%s','%s');",(LPCTSTR)companyCode, it->GetCompanyName().c_str(),
				it->GetAddress().c_str(), it->GetCompanyTelphoneNumber().c_str(), it->GetFaxNumber().c_str(),
				it->GetUrl().c_str());

			db.ExecuteSQL(sqlCompany);
		}
#if 0
		rs2.Close();
#endif
		rs1.Close();

		//2.3. ������ �߰��Ѵ�.
		sqlPersonal.Format(
			"INSERT INTO Personal(code, companyCode, name, rankName, telephoneNumber, emailAddress) VALUES('%s',"
			"'%s','%s','%s','%s','%s');", (LPCTSTR)code, (LPCTSTR)companyCode, it->GetPersonalName().c_str(),
			it->GetRankName().c_str(), it->GetPersonalTelephoneNumber().c_str(), it->GetEmailAddress().c_str());
		
		db.ExecuteSQL(sqlPersonal);

		rs.MoveNext();
		previous = it;
		it = this->businessCardBinder->Next();

	}

	rs.Close();
	db.Close();

	return this->businessCardBinder->GetLength();
}

BOOL BusinessCardBinderForm::OnInitDialog() {
	CDialog::OnInitDialog();
	HTREEITEM parent;
	HTREEITEM company;
	HTREEITEM personal;
	Long count;
	Long countList;
	BusinessCard* index;
	Index* index_;
	BusinessCard* previous = NULL;
	Index(*indexes);
	Long i = 0;
	Long j = 0;
	CString companyText;
	Long arrayIndex;
	CString personalName;
	CString rankName;
	CString personalTelephoneNumber;
	CString emailAddress;
	CString companyName;
	CString address;
	CString companyTelephoneNumber;
	CString faxNumber;
	CString url;

	//1. �����찡 ������ ��
	  //1.1. ����ö�� �����.
	  this->businessCardBinder = new BusinessCardBinder;

	  //1.2. ����ö�� �����.
	  this->indexBinder = new IndexBinder;

	  //1.3. Ʈ���信 ȸ��� �ڵ��� �����.
	  parent = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->InsertItem("ȸ���", TVI_ROOT, TVI_FIRST);

	  //1.4. Load�Ѵ�.
	  count = Load();

	  //1.5. count�� 0���� ū ���,
	  if (count > 0) {
		  //1.5.1. ������ ó������ ������ �ݺ��Ѵ�.
		  index = this->businessCardBinder->First();
		  while (index != previous) {
			  //1.5.1.1. ����ö�� �ȴ´�
			  this->indexBinder->PutIn(index);
			  previous = index;
			  index = this->businessCardBinder->Next();
		  }

		  //1.5.2. ���θ���Ʈ�� �����.
		  this->indexBinder->MakeList(&indexes, &countList);

		  //1.5.3. ������ ������ŭ �ݺ��Ѵ�.
		  while (countList > i) {
			  //1.5.3.1. ȸ��� ������ ��ȣ�ڵ� �߰��Ѵ�.
			  companyName = CString(indexes[i].GetCompanyName().c_str());
			  company = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->InsertItem(companyName, parent, TVI_SORT);

			  //1.5.3.2. ��ȣ�� ������ �����ڵ� �߰��Ѵ�.
			  j = 0;
			  while (indexes[i].GetLength() > j) {
				  index = indexes[i].GetAt(j);
				  personalName = CString(index->GetPersonalName().c_str());
				  ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->InsertItem(personalName, company, TVI_LAST);

				  j++;
			  }
			  i++;
		  }
		  //1.5.4.  ù��° ������ �ֱ� �������� �����Ѵ�.
		  index = this->businessCardBinder->First();

		  //1.5.5. ��ȣ�ڵ� ���Ѵ�.
		  companyName = CString(index->GetCompanyName().c_str());
		  company = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(parent, TVGN_CHILD);
		  companyText = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetItemText(company);
		  while (companyText != companyName) {
			  company = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(company, TVGN_NEXT);
			  companyText = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetItemText(company);
		  }

		  //1.5.6. ���μ��� ã�´�.
		  //����ö ã�´�.
		  index_ = this->indexBinder->Find((LPCTSTR)companyName);
		  //�ش� ����ö���� �ش� ������ ���° �ִ���
		  arrayIndex = index_->Find(index);
		  //�����ڵ� ã�´�.
		  personal = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(company, TVGN_CHILD);
		  i = 0;
		  while (arrayIndex > i) {
			  personal = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(personal, TVGN_NEXT);
			  i++;
		  }

		  //1.5.7. ȸ��� �ڵ��� ��ģ��.
		  ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->Expand(parent, TVE_EXPAND);
		  //1.5.8. ��ȣ�ڵ� ��ģ��.
		  ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->Expand(company, TVE_EXPAND);
		  //1.5.9. �ش� �����ڵ��� ���õ� ���·� ó���Ѵ�.
		  ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->SelectItem(personal);

		  //1.6.0. ù��° ������ ���Կ� ����Ѵ�.
		  personalName = CString(index->GetPersonalName().c_str());
		  GetDlgItem(IDC_EDIT_PERSONALNAME)->SetWindowText(personalName);
		  rankName = CString(index->GetRankName().c_str());
		  GetDlgItem(IDC_EDIT_RANKNAME)->SetWindowText(rankName);
		  personalTelephoneNumber = CString(index->GetPersonalTelephoneNumber().c_str());
		  GetDlgItem(IDC_EDIT_PERSONALTELEPHONENUMBER)->SetWindowText(personalTelephoneNumber);
		  emailAddress = CString(index->GetEmailAddress().c_str());
		  GetDlgItem(IDC_EDIT_EMAILADDRESS)->SetWindowText(emailAddress);
		  companyName = CString(index->GetCompanyName().c_str());
		  GetDlgItem(IDC_EDIT_COMPANYNAME)->SetWindowText(companyName);
		  address = CString(index->GetAddress().c_str());
		  GetDlgItem(IDC_EDIT_ADDRESS)->SetWindowText(address);
		  companyTelephoneNumber = CString(index->GetCompanyTelphoneNumber().c_str());
		  GetDlgItem(IDC_EDIT_COMPANYTELEPHONENUMBER)->SetWindowText(companyTelephoneNumber);
		  faxNumber = CString(index->GetFaxNumber().c_str());
		  GetDlgItem(IDC_EDIT_FAXNUMBER)->SetWindowText(faxNumber);
		  url = CString(index->GetUrl().c_str());
		  GetDlgItem(IDC_EDIT_URL)->SetWindowText(url);
	  }
	return FALSE;
}


void BusinessCardBinderForm::OnPutButtonClicked() {
	//2. �ȱ��ư Ŭ������ ��
	  //2.1. �ȱ� �����츦 ����Ѵ�.
	  PuttingForm puttingForm;
	  puttingForm.DoModal();
}


void BusinessCardBinderForm::OnFindButtonClicked() {
	//3. ã���ư Ŭ������ ��
	  //3.1. ã�� �����츦 ����Ѵ�.
	  FindingForm findingForm;
	  findingForm.DoModal();
}

void BusinessCardBinderForm::OnPutOutButtonClicked() {
	//4. �̱��ư Ŭ������ ��
	  //4.1. �̱� �����츦 ����Ѵ�.
	PuttingOutForm puttingOutForm;
	puttingOutForm.DoModal();
}


void BusinessCardBinderForm::OnArrangeButtonClicked() {
	HTREEITEM parent;
	HTREEITEM company;
	HTREEITEM personal;
	Index(*indexes);
	Long count;
	Long i = 0;
	Long j = 0;
	Index* index_;
	BusinessCard* index;
	CString companyText;
	Long arrayIndex;
	CString personalName;
	CString rankName;
	CString personalTelephoneNumber;
	CString emailAddress;
	CString companyName;
	CString address;
	CString companyTelephoneNumber;
	CString faxNumber;
	CString url;

	//5. �����ϱ� ��ư Ŭ������ ��
	  //5.1. ����ö���� �����Ѵ�.
	this->indexBinder->Arrange();

	//5.2. Ʈ���� �ʱ�ȭ�Ѵ�.
	((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->DeleteAllItems();

	//5.3. ���θ���Ʈ�� ���Ѵ�.
	this->indexBinder->MakeList(&indexes, &count);

	//5.4. Ʈ���信 ȸ��� �׸� �߰��Ѵ�.
	parent = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->InsertItem("ȸ���", TVI_ROOT, TVI_FIRST);

	//5.5. ������ ������ŭ �ݺ��Ѵ�.
	while (count > i) {
		//5.5.1. ȸ��� �ڵ� ������ ��ȣ���� �߰��Ѵ�.
		companyName = CString(indexes[i].GetCompanyName().c_str());
		company = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->InsertItem(companyName, parent, TVI_LAST);

		//5.5.2. ������ ������ŭ �ݺ��Ѵ�.
		j = 0;
		while (indexes[i].GetLength() > j) {
			//5.5.2.1. ��ȣ�� ������ ���μ��� �߰��Ѵ�.
			index = indexes[i].GetAt(j);
			personalName = CString(index->GetPersonalName().c_str());
			personal = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->InsertItem(personalName, company, TVI_LAST);
			j++;
		}
		i++;
	}

	//5.6. ������ ������ġ�� ù��° ������ġ�� �ű��.
	index = this->businessCardBinder->First();

	//5.7. ��ȣ�ڵ� ã�´�.
	companyName = CString(index->GetCompanyName().c_str());
	company = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(parent, TVGN_CHILD);
	companyText = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetItemText(company);
	while (companyText != companyName) {
		company = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(company, TVGN_NEXT);
		companyText = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetItemText(company);
	}

	//5.8. ���μ��� ã�´�.
	//����ö ã�´�.
	index_ = this->indexBinder->Find((LPCTSTR)companyName);
	//�ش� ����ö���� �ش� ������ ���° �ִ���
	arrayIndex = index_->Find(index);
	//�����ڵ� ã�´�.
	personal = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(company, TVGN_CHILD);
	i = 0;
	while (arrayIndex > i) {
		personal = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(personal, TVGN_NEXT);
		i++;
	}

	//5.9. ȸ��� �ڵ��� ��ģ��.
	((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->Expand(parent, TVE_EXPAND);
	//6.0. ��ȣ�ڵ� ��ģ��.
	((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->Expand(company, TVE_EXPAND);
	//6.1. �ش� �����ڵ��� ���õ� ���·� ó���Ѵ�.
	((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->SelectItem(personal);

	//6.2. ����ö�� ���� ��ġ�� ������ ����Ѵ�.
	personalName = CString(index->GetPersonalName().c_str());
	GetDlgItem(IDC_EDIT_PERSONALNAME)->SetWindowText(personalName);
	rankName = CString(index->GetRankName().c_str());
	GetDlgItem(IDC_EDIT_RANKNAME)->SetWindowText(rankName);
	personalTelephoneNumber = CString(index->GetPersonalTelephoneNumber().c_str());
	GetDlgItem(IDC_EDIT_PERSONALTELEPHONENUMBER)
		->SetWindowText(personalTelephoneNumber);
	emailAddress = CString(index->GetEmailAddress().c_str());
	GetDlgItem(IDC_EDIT_EMAILADDRESS)->SetWindowText(emailAddress);
	companyName = CString(index->GetCompanyName().c_str());
	GetDlgItem(IDC_EDIT_COMPANYNAME)->SetWindowText(companyName);
	address = CString(index->GetAddress().c_str());
	GetDlgItem(IDC_EDIT_ADDRESS)->SetWindowText(address);
	companyTelephoneNumber = CString(index->GetCompanyTelphoneNumber().c_str());
	GetDlgItem(IDC_EDIT_COMPANYTELEPHONENUMBER)
		->SetWindowText(companyTelephoneNumber);
	faxNumber = CString(index->GetFaxNumber().c_str());
	GetDlgItem(IDC_EDIT_FAXNUMBER)->SetWindowText(faxNumber);
	url = CString(index->GetUrl().c_str());
	GetDlgItem(IDC_EDIT_URL)->SetWindowText(url);

	if (indexes != NULL) {
		delete[] indexes;
		indexes = NULL;
	}
}

void BusinessCardBinderForm::OnTreeViewItemDoubleClicked(NMHDR* pNotifyStruct, LRESULT* result) {
	HTREEITEM handle;
	HTREEITEM handleParent;
	HTREEITEM parent;
	HTREEITEM company;
	HTREEITEM personal;
	HTREEITEM companyChild;
	CString companyText;
	CString personalName;
	CString rankName;
	CString personalTelephoneNumber;
	CString emailAddress;
	CString companyName;
	CString address;
	CString companyTelephoneNumber;
	CString faxNumber;
	CString url;
	Index* index_;
	BusinessCard* index;
	Long arrayIndex = 0;
	Long i = 0;

	//6. Ʈ���� ���� �����ڵ� �Ǵ� ��ȣ�ڵ� ����Ŭ�� ���� ��
	  //6.1. Ŭ���� �ڵ��� �����ڵ����� ��ȣ�ڵ����� Ȯ���Ѵ�.
	handle = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(NULL, TVGN_CARET);
	handleParent = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(handle, TVGN_PARENT);
	parent = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(NULL, TVGN_ROOT);

	//6.2. Ŭ���� �ڵ��� �����ڵ��� ���,
	if (handleParent != parent) {
		//6.2.1. Ŭ���� �����ڵ� ���Ѵ�.
		personal = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(NULL, TVGN_CARET);

		//6.2.2. ��ȣ�ڵ� ���Ѵ�.
		company = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(personal, TVGN_PARENT);

		//6.2.3. ��ȣ�ڵ� TEXT���Ѵ�.
		companyText = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetItemText(company);

		//6.2.4. �ش� ��ȣ�ڵ鿡�� �ش� �����ڵ��� �°���� Ȯ���Ѵ�.(÷��)
		companyChild = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(company, TVGN_CHILD);
		while (companyChild != personal) {
			companyChild = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(companyChild, TVGN_NEXT);
			arrayIndex++;
		}

		//6.2.5. ����ö���� ��ȣ ��ġ�� ���Ѵ�.
		index_ = this->indexBinder->Find((LPCTSTR)companyText);

		//6.2.6. �ش� ��ȣ �� ������ ��ġ�� ã�´�.
		index = index_->GetAt(arrayIndex);

		//6.2.7. �ش� ��ġ�� ������ ���� �������� �����Ѵ�.
		index = this->businessCardBinder->Move(index);

		//6.2.8. ���� ������ ���Կ� ����Ѵ�.
		personalName = CString(index->GetPersonalName().c_str());
		GetDlgItem(IDC_EDIT_PERSONALNAME)->SetWindowText(personalName);
		rankName = CString(index->GetRankName().c_str());
		GetDlgItem(IDC_EDIT_RANKNAME)->SetWindowText(rankName);
		personalTelephoneNumber = CString(index->GetPersonalTelephoneNumber().c_str());
		GetDlgItem(IDC_EDIT_PERSONALTELEPHONENUMBER)
			->SetWindowText(personalTelephoneNumber);
		emailAddress = CString(index->GetEmailAddress().c_str());
		GetDlgItem(IDC_EDIT_EMAILADDRESS)->SetWindowText(emailAddress);
		companyName = CString(index->GetCompanyName().c_str());
		GetDlgItem(IDC_EDIT_COMPANYNAME)->SetWindowText(companyName);
		address = CString(index->GetAddress().c_str());
		GetDlgItem(IDC_EDIT_ADDRESS)->SetWindowText(address);
		companyTelephoneNumber = CString(index->GetCompanyTelphoneNumber().c_str());
		GetDlgItem(IDC_EDIT_COMPANYTELEPHONENUMBER)
			->SetWindowText(companyTelephoneNumber);
		faxNumber = CString(index->GetFaxNumber().c_str());
		GetDlgItem(IDC_EDIT_FAXNUMBER)->SetWindowText(faxNumber);
		url = CString(index->GetUrl().c_str());
		GetDlgItem(IDC_EDIT_URL)->SetWindowText(url);
	}
}

void BusinessCardBinderForm::OnFirstButtonClicked() {
	BusinessCard* index;
	HTREEITEM parent;
	HTREEITEM company;
	HTREEITEM personal;
	CString companyText;
	Index* index_;
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

	//5. ó����ư Ŭ������ ��
	  //5.1. ó�� ������ ���� ��ġ�� ���ٵд�.
	index = this->businessCardBinder->First();

	//5.2. ���� ������ ��ȣ�� ��ȣ�ڵ� ã�´�.
	companyName = CString(index->GetCompanyName().c_str());

	parent = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(NULL, TVGN_ROOT);

	company = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(parent, TVGN_CHILD);
	companyText = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetItemText(company);
	while (companyText != companyName) {
		company = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(company, TVGN_NEXT);
		companyText = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetItemText(company);
	}

	//5.3. �����ڵ��� ã�´�.
	index_ = this->indexBinder->Find((LPCTSTR)companyName);
	arrayIndex = index_->Find(index);

	personal = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(company, TVGN_CHILD);
	while (arrayIndex > i) {
		personal = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(personal, TVGN_NEXT);
		i++;
	}

	//5.4. ȸ��� �ڵ��� ��ģ��.
	((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->Expand(parent, TVE_EXPAND);
	//5.5. ��ȣ�ڵ� ��ģ��.
	((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->Expand(company, TVE_EXPAND);
	//5.6. �ش� �����ڵ��� ���õ� ���·� ó���Ѵ�.
	((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->SelectItem(personal);

	//5.7. ����ö�� ���� ��ġ�� ������ ����Ѵ�.
	personalName = CString(index->GetPersonalName().c_str());
	GetDlgItem(IDC_EDIT_PERSONALNAME)->SetWindowText(personalName);
	rankName = CString(index->GetRankName().c_str());
	GetDlgItem(IDC_EDIT_RANKNAME)->SetWindowText(rankName);
	personalTelephoneNumber = CString(index->GetPersonalTelephoneNumber().c_str());
	GetDlgItem(IDC_EDIT_PERSONALTELEPHONENUMBER)
		->SetWindowText(personalTelephoneNumber);
	emailAddress = CString(index->GetEmailAddress().c_str());
	GetDlgItem(IDC_EDIT_EMAILADDRESS)->SetWindowText(emailAddress);
	companyName = CString(index->GetCompanyName().c_str());
	GetDlgItem(IDC_EDIT_COMPANYNAME)->SetWindowText(companyName);
	address = CString(index->GetAddress().c_str());
	GetDlgItem(IDC_EDIT_ADDRESS)->SetWindowText(address);
	companyTelephoneNumber = CString(index->GetCompanyTelphoneNumber().c_str());
	GetDlgItem(IDC_EDIT_COMPANYTELEPHONENUMBER)
		->SetWindowText(companyTelephoneNumber);
	faxNumber = CString(index->GetFaxNumber().c_str());
	GetDlgItem(IDC_EDIT_FAXNUMBER)->SetWindowText(faxNumber);
	url = CString(index->GetUrl().c_str());
	GetDlgItem(IDC_EDIT_URL)->SetWindowText(url);
}


void BusinessCardBinderForm::OnPreviousButtonClicked() {
	BusinessCard* index;
	HTREEITEM parent;
	HTREEITEM company;
	HTREEITEM personal;
	CString companyText;
	Index* index_;
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

	//6. ������ư Ŭ������ ��
	  //6.1. ���� ������ ���� ��ġ�� ���ٵд�.
	index = this->businessCardBinder->Previous();

	//6.2. ���� ������ ��ȣ�� ��ȣ�ڵ� ã�´�.
	companyName = CString(index->GetCompanyName().c_str());
	parent = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(NULL, TVGN_ROOT);
	company = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(parent, TVGN_CHILD);
	companyText = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetItemText(company);
	while (companyText != companyName) {
		company = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(company, TVGN_NEXT);
		companyText = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetItemText(company);
	}

	//6.3. �����ڵ��� ã�´�.
	index_ = this->indexBinder->Find((LPCTSTR)companyName);
	arrayIndex = index_->Find(index);
	personal = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(company, TVGN_CHILD);
	while (arrayIndex > i) {
		personal = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(personal, TVGN_NEXT);
		i++;
	}

	//6.4. ȸ��� �ڵ��� ��ģ��.
	((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->Expand(parent, TVE_EXPAND);
	//6.5. ��ȣ�ڵ� ��ģ��.
	((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->Expand(company, TVE_EXPAND);
	//6.6. �ش� �����ڵ��� ���õ� ���·� ó���Ѵ�.
	((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->SelectItem(personal);

	//6.7. ����ö�� ���� ��ġ�� ������ ����Ѵ�.
	personalName = CString(index->GetPersonalName().c_str());
	GetDlgItem(IDC_EDIT_PERSONALNAME)->SetWindowText(personalName);
	rankName = CString(index->GetRankName().c_str());
	GetDlgItem(IDC_EDIT_RANKNAME)->SetWindowText(rankName);
	personalTelephoneNumber = CString(index->GetPersonalTelephoneNumber().c_str());
	GetDlgItem(IDC_EDIT_PERSONALTELEPHONENUMBER)
		->SetWindowText(personalTelephoneNumber);
	emailAddress = CString(index->GetEmailAddress().c_str());
	GetDlgItem(IDC_EDIT_EMAILADDRESS)->SetWindowText(emailAddress);
	companyName = CString(index->GetCompanyName().c_str());
	GetDlgItem(IDC_EDIT_COMPANYNAME)->SetWindowText(companyName);
	address = CString(index->GetAddress().c_str());
	GetDlgItem(IDC_EDIT_ADDRESS)->SetWindowText(address);
	companyTelephoneNumber = CString(index->GetCompanyTelphoneNumber().c_str());
	GetDlgItem(IDC_EDIT_COMPANYTELEPHONENUMBER)
		->SetWindowText(companyTelephoneNumber);
	faxNumber = CString(index->GetFaxNumber().c_str());
	GetDlgItem(IDC_EDIT_FAXNUMBER)->SetWindowText(faxNumber);
	url = CString(index->GetUrl().c_str());
	GetDlgItem(IDC_EDIT_URL)->SetWindowText(url);
}

void BusinessCardBinderForm::OnNextButtonClicked() {
	BusinessCard* index;
	HTREEITEM parent;
	HTREEITEM company;
	HTREEITEM personal;
	CString companyText;
	Index* index_;
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

	//7. ������ư Ŭ������ ��
	  //7.1. ���� ������ ���� ��ġ�� ���ٵд�.
	index = this->businessCardBinder->Next();

	//7.2. ���� ������ ��ȣ�� ��ȣ�ڵ� ã�´�.
	companyName = CString(index->GetCompanyName().c_str());
	parent = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(NULL, TVGN_ROOT);
	company = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(parent, TVGN_CHILD);
	companyText = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetItemText(company);
	while (companyText != companyName) {
		company = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(company, TVGN_NEXT);
		companyText = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetItemText(company);
	}

	//7.3. �����ڵ��� ã�´�.
	index_ = this->indexBinder->Find((LPCTSTR)companyName);
	arrayIndex = index_->Find(index);
	personal = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(company, TVGN_CHILD);
	while (arrayIndex > i) {
		personal = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(personal, TVGN_NEXT);
		i++;
	}

	//7.4. ȸ��� �ڵ��� ��ģ��.
	((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->Expand(parent, TVE_EXPAND);
	//7.5. ��ȣ�ڵ� ��ģ��.
	((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->Expand(company, TVE_EXPAND);
	//7.6. �ش� �����ڵ��� ���õ� ���·� ó���Ѵ�.
	((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->SelectItem(personal);

	//7.7. ����ö�� ���� ��ġ�� ������ ����Ѵ�.
	personalName = CString(index->GetPersonalName().c_str());
	GetDlgItem(IDC_EDIT_PERSONALNAME)->SetWindowText(personalName);
	rankName = CString(index->GetRankName().c_str());
	GetDlgItem(IDC_EDIT_RANKNAME)->SetWindowText(rankName);
	personalTelephoneNumber = CString(index->GetPersonalTelephoneNumber().c_str());
	GetDlgItem(IDC_EDIT_PERSONALTELEPHONENUMBER)
		->SetWindowText(personalTelephoneNumber);
	emailAddress = CString(index->GetEmailAddress().c_str());
	GetDlgItem(IDC_EDIT_EMAILADDRESS)->SetWindowText(emailAddress);
	companyName = CString(index->GetCompanyName().c_str());
	GetDlgItem(IDC_EDIT_COMPANYNAME)->SetWindowText(companyName);
	address = CString(index->GetAddress().c_str());
	GetDlgItem(IDC_EDIT_ADDRESS)->SetWindowText(address);
	companyTelephoneNumber = CString(index->GetCompanyTelphoneNumber().c_str());
	GetDlgItem(IDC_EDIT_COMPANYTELEPHONENUMBER)
		->SetWindowText(companyTelephoneNumber);
	faxNumber = CString(index->GetFaxNumber().c_str());
	GetDlgItem(IDC_EDIT_FAXNUMBER)->SetWindowText(faxNumber);
	url = CString(index->GetUrl().c_str());
	GetDlgItem(IDC_EDIT_URL)->SetWindowText(url);
}

void BusinessCardBinderForm::OnLastButtonClicked() {
	BusinessCard* index;
	HTREEITEM parent;
	HTREEITEM company;
	HTREEITEM personal;
	CString companyText;
	Index* index_;
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

	//8. ��������ư Ŭ������ ��
	  //8.1. ������ ������ ���� ��ġ�� ���ٵд�.
	index = this->businessCardBinder->Last();

	//8.2. ���� ������ ��ȣ�� ��ȣ�ڵ� ã�´�.
	companyName = CString(index->GetCompanyName().c_str());
	parent = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(NULL, TVGN_ROOT);
	company = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(parent, TVGN_CHILD);
	companyText = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetItemText(company);
	while (companyText != companyName) {
		company = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(company, TVGN_NEXT);
		companyText = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetItemText(company);
	}

	//8.3. �����ڵ��� ã�´�.
	index_ = this->indexBinder->Find((LPCTSTR)companyName);
	arrayIndex = index_->Find(index);
	personal = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(company, TVGN_CHILD);
	while (arrayIndex > i) {
		personal = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(personal, TVGN_NEXT);
		i++;
	}

	//8.4. ȸ��� �ڵ��� ��ģ��.
	((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->Expand(parent, TVE_EXPAND);
	//8.5. ��ȣ�ڵ� ��ģ��.
	((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->Expand(company, TVE_EXPAND);
	//8.6. �ش� �����ڵ��� ���õ� ���·� ó���Ѵ�.
	((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->SelectItem(personal);

	//8.7. ����ö�� ���� ��ġ�� ������ ����Ѵ�.
	personalName = CString(index->GetPersonalName().c_str());
	GetDlgItem(IDC_EDIT_PERSONALNAME)->SetWindowText(personalName);
	rankName = CString(index->GetRankName().c_str());
	GetDlgItem(IDC_EDIT_RANKNAME)->SetWindowText(rankName);
	personalTelephoneNumber = CString(index->GetPersonalTelephoneNumber().c_str());
	GetDlgItem(IDC_EDIT_PERSONALTELEPHONENUMBER)
		->SetWindowText(personalTelephoneNumber);
	emailAddress = CString(index->GetEmailAddress().c_str());
	GetDlgItem(IDC_EDIT_EMAILADDRESS)->SetWindowText(emailAddress);
	companyName = CString(index->GetCompanyName().c_str());
	GetDlgItem(IDC_EDIT_COMPANYNAME)->SetWindowText(companyName);
	address = CString(index->GetAddress().c_str());
	GetDlgItem(IDC_EDIT_ADDRESS)->SetWindowText(address);
	companyTelephoneNumber = CString(index->GetCompanyTelphoneNumber().c_str());
	GetDlgItem(IDC_EDIT_COMPANYTELEPHONENUMBER)
		->SetWindowText(companyTelephoneNumber);
	faxNumber = CString(index->GetFaxNumber().c_str());
	GetDlgItem(IDC_EDIT_FAXNUMBER)->SetWindowText(faxNumber);
	url = CString(index->GetUrl().c_str());
	GetDlgItem(IDC_EDIT_URL)->SetWindowText(url);
}

void BusinessCardBinderForm::OnClose() {
	//9. �ݱ� ��ư Ŭ������ ��

	  //9.0.  �����Ѵ�.
	   Save();

	  //9.1. ����ö�� ���ش�.
	  if (this->businessCardBinder != NULL) {
		  delete this->businessCardBinder;
		  this->businessCardBinder = NULL;
	  }

	  //10.3. ����ö�� ���ش�.
	  if (this->indexBinder != NULL) {
		  delete this->indexBinder;
		  this->indexBinder = NULL;
	  }
	  //9.2. �����츦 �����Ѵ�.
	  EndDialog(0);
}






