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

	//1. ???????? ?????? ?????? ?????? ?????? ????????.
	sql.Format(
		"SELECT Company.companyCode FROM Company WHERE name='%s' AND address='%s' AND telephoneNumber='%s' "
		"AND faxNumber='%s' AND url='%s';", 
		index->GetCompanyName().c_str(), index->GetAddress().c_str(), index->GetCompanyTelphoneNumber().c_str(), 
		index->GetFaxNumber().c_str(), index->GetUrl().c_str());
	
	db.OpenEx("DSN=BusinessCardBinder;UID=Sydney;PWD=4821");
	rs.Open(AFX_DB_USE_DEFAULT_TYPE, sql);


	//2. ???? ?????????? ???? ????, ???? ?????? ??????.
	if (!rs.IsEOF()) {
		rs.GetFieldValue((short)0, companyCode);
	}
    
	// ???? ?????? ??????.
	time(&timer);
	today = localtime(&timer);
	year.Format("%4d", today->tm_year + 1900); //2021 ????      
	year = year.Right(2); //21 ????
	todayDate.Format("%s%02d%02d", year, today->tm_mon + 1, today->tm_mday); //210414 ????


	sql = "SELECT Company.companyCode FROM Company ORDER BY companyCode DESC;";
	rs1.Open(AFX_DB_USE_DEFAULT_TYPE, sql);


	if (rs1.IsEOF()) {
		companyCode.Format("C%s001", todayDate);
	}
	//4. ???? ?????????? ??????, ???? ?????? ???? ????,
	else if (!rs1.IsEOF() && rs.IsEOF()) {
		//4.1. ???? ???? ?? ???? ?????? ?????? ??????.
		rs1.GetFieldValue((short)0, companyCode); //P210414003
		companyCode_ = companyCode;

		//4.2. ?????? ?????? ???? ?????? ????????.
		companyCode = companyCode.Mid(1, 6);//210414

		//4.3. ?????? ?????? ???? ?????? ???? ????, ???? ?????? ????????.
		if (companyCode != todayDate) {
			companyCode_.Format("C%s000", todayDate); //P210416000
		}

		//4.4. ?? ?? ?????? ?????????? ????????.
		companyCode_ = companyCode_.Right(3);

		//4.5. ?????? ?????????? ?????? ????????.
		number = _ttoi(companyCode_);

		//4.6. ?????? 1 ????????.
		number++;

		//4.7. ?????? ???? ?????? ????????.
		companyCode.Format("C%s%03d", todayDate, number);
	}


#if 0
	//3. ???????? ???? ????,
	else {
		//3.1. ???? ?????? ??????.
		time(&timer);
		today = localtime(&timer);
		year.Format("%4d", today->tm_year + 1900); //2021 ????      
		year = year.Right(2); //21 ????
		todayDate.Format("%s%02d%02d", year, today->tm_mon + 1, today->tm_mday); //210414 ????


		//3.2. ?????? ?????? ???? ??????, ???? ???? ?????? ?????????? ????????.
		//3.3. ???? ?????? ?????? ???? ????,
		if (!rs1.IsEOF()) { //???? ???? ?????? ?????? ??.
			//3.3.1. ???? ???? ?? ???? ?????? ?????? ??????.
			rs1.GetFieldValue((short)0, companyCode); //P210414003
			companyCode_ = companyCode; 

			//3.3.2. ?????? ?????? ???? ?????? ????????.
			companyCode = companyCode.Mid(1, 6);//210414

			//3.3.3. ?????? ?????? ???? ?????? ???? ????, ???? ?????? ????????.
			if (companyCode !=todayDate) {
				companyCode_.Format("C%s000", todayDate); //P210416000
			}

			//3.3.4. ?? ?? ?????? ?????????? ????????.
			companyCode_ = companyCode_.Right(3);
			
			//3.3.5. ?????? ?????????? ?????? ????????.
			number = _ttoi(companyCode_);

			//3.3.6. ?????? 1 ????????.
			number++;

			//3.3.7. ?????? ???? ?????? ????????.
			companyCode.Format("C%s%03d", todayDate, number);
			
		}
		//3.4. ?????? ?????? ???? ????, ?????? ??????????.
		else {
			companyCode.Format("C%s001", todayDate);
		}

	}
#endif
	//4. ?????? ????????.
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

	//1. ???? ?????? ??????. (210416 ????????)
	time(&timer);
	today = localtime(&timer);
	year.Format("%4d", today->tm_year + 1900); //2021 ????      
	year = year.Right(2); //21 ????
	todayDate.Format("%s%02d%02d", year, today->tm_mon + 1, today->tm_mday); //210414 ????

	//2. ?????? ???? ????,
	db.OpenEx("DSN=BusinessCardBinder;UID=Sydney;PWD=4821");
	rs.Open(AFX_DB_USE_DEFAULT_TYPE, sql);

	if (!rs.IsEOF()) {
		//2.1. ???? ?? ?????? ??????.
		rs.GetFieldValue((short)0, code); //P210414003
		code_ = code;

		//2.2. ?????????? ????????.
		code = code.Mid(1, 6); //210414

		//2.3. ???? ?????? ???? ???? ????, ???? ?????? ??????????.

		if (code!=todayDate) {
			code_.Format("P%s000", todayDate);
		}

		//2.4. ?????? ?? ???????? ????????.
		code_ = code_.Right(3);

		//2.5. ?????? ?????????? ?????? ????????.
		number = _ttoi(code_);

		//2.6. ?????? 1?? ??????.
		number++;

		//2.7. ?????? P210414001 ???????? ????????.
		code.Format("P%s%03d", todayDate, number);
	}
	//3. ?????? ???? ???? ????, ???? ?????? ??????????.
	else {
		code.Format("P%s001", todayDate);
	}
	//4. ?????? ????????.
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

	//1. ???? DB?? ???????? ?????? ?????? Insert????.
	if (rs.IsEOF()) {
		sql.Format("INSERT INTO Company(companyCode, name, address,telephoneNumber,faxNumber,url)" 
			" VALUES('%s','%s', '%s', '%s', '%s', '%s');", (LPCTSTR)companyCode, index->GetCompanyName().c_str(),
			index->GetAddress().c_str(), index->GetCompanyTelphoneNumber().c_str(),
			index->GetFaxNumber().c_str(), index->GetUrl().c_str());
		db.ExecuteSQL(sql);
	}

	//2. ???? Insert????.
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

	//1. ?????? code?? companyCode?? ??????.
	db.OpenEx("DSN=BusinessCardBinder;UID=Sydney;PWD=4821");
	rs.Open(AFX_DB_USE_DEFAULT_TYPE, sql);
	CString code;
	CString companyCode;

	rs.GetFieldValue((short)0, code);
	rs.GetFieldValue((short)1, companyCode);

	//2. ???????? ???? code?? ????????.
	sql.Format("DELETE FROM Personal WHERE code='%s';", (LPCTSTR)code);
	db.ExecuteSQL(sql);

	//3. ?????? ???? companyCode?? ???? ?????? ??????, ???? ???? ?????? ????????.
	sql.Format("SELECT Personal.companyCode FROM Personal WHERE companyCode='%s';",(LPCTSTR)companyCode); 

	rs1.Open(AFX_DB_USE_DEFAULT_TYPE, sql);
	if (rs.IsEOF()) {
		sql.Format("DELETE FROM Company WHERE companyCode='%s';", (LPCTSTR)companyCode);
		db.ExecuteSQL(sql);
	}

	//4. ?????? ??????
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

	//1. index?????? code?? companyCode?? ??????.
	db.OpenEx("DSN=BusinessCardBinder;UID=Sydney;PWD=4821");
	rs.Open(AFX_DB_USE_DEFAULT_TYPE, sql);

	it = this->businessCardBinder->First();
	while (it != index) {
		rs.GetFieldValue((short)0, code);
		rs.GetFieldValue((short)1, companyCode);

		rs.MoveNext();
		it = this->businessCardBinder->Next();
	}

	//2. ???? ?????? ????????????.
	sql.Format("Update Personal SET name='%s', rankName='%s', telephoneNumber='%s', emailAddress='%s'"
		" WHERE code='%s';", index->GetPersonalName().c_str(), index->GetRankName().c_str(),
		index->GetPersonalTelephoneNumber().c_str(), index->GetEmailAddress().c_str(), (LPCTSTR)code);

	db.ExecuteSQL(sql);

	//3. ???? ?????????? ????????????.
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


	//1. DB?? ???? ???????? ??????.
	db.OpenEx("DSN=BusinessCardBinder;UID=Sydney;PWD=4821");
	rs.Open(AFX_DB_USE_DEFAULT_TYPE, sql);

	db.ExecuteSQL("DELETE FROM Personal");
	db.ExecuteSQL("DELETE FROM Company");

	//2. ?????? ???? ???????? ????????.
	it = this->businessCardBinder->First();

	while (!rs.IsEOF() && it != previous) {
		rs.GetFieldValue((short)0, code);
		rs.GetFieldValue((short)1, companyCode);

		//2.1. DB?? ?????? ?????? ?????? ????????.
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


		//2.2. DB?? ???? ????????, ????DB?? ????????.
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

		//2.3. ?????? ????????.
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

	//1. ???????? ?????? ??
	  //1.1. ???????? ??????.
	  this->businessCardBinder = new BusinessCardBinder;

	  //1.2. ???????? ??????.
	  this->indexBinder = new IndexBinder;

	  //1.3. ???????? ?????? ?????? ??????.
	  parent = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->InsertItem("??????", TVI_ROOT, TVI_FIRST);

	  //1.4. Load????.
	  count = Load();

	  //1.5. count?? 0???? ?? ????,
	  if (count > 0) {
		  //1.5.1. ?????? ???????? ?????? ????????.
		  index = this->businessCardBinder->First();
		  while (index != previous) {
			  //1.5.1.1. ???????? ??????
			  this->indexBinder->PutIn(index);
			  previous = index;
			  index = this->businessCardBinder->Next();
		  }

		  //1.5.2. ???????????? ??????.
		  this->indexBinder->MakeList(&indexes, &countList);

		  //1.5.3. ?????? ???????? ????????.
		  while (countList > i) {
			  //1.5.3.1. ?????? ?????? ???????? ????????.
			  companyName = CString(indexes[i].GetCompanyName().c_str());
			  company = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->InsertItem(companyName, parent, TVI_SORT);

			  //1.5.3.2. ?????? ?????? ???????? ????????.
			  j = 0;
			  while (indexes[i].GetLength() > j) {
				  index = indexes[i].GetAt(j);
				  personalName = CString(index->GetPersonalName().c_str());
				  ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->InsertItem(personalName, company, TVI_LAST);

				  j++;
			  }
			  i++;
		  }
		  //1.5.4.  ?????? ?????? ???? ???????? ????????.
		  index = this->businessCardBinder->First();

		  //1.5.5. ???????? ??????.
		  companyName = CString(index->GetCompanyName().c_str());
		  company = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(parent, TVGN_CHILD);
		  companyText = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetItemText(company);
		  while (companyText != companyName) {
			  company = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(company, TVGN_NEXT);
			  companyText = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetItemText(company);
		  }

		  //1.5.6. ???????? ??????.
		  //?????? ??????.
		  index_ = this->indexBinder->Find((LPCTSTR)companyName);
		  //???? ?????????? ???? ?????? ?????? ??????
		  arrayIndex = index_->Find(index);
		  //???????? ??????.
		  personal = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(company, TVGN_CHILD);
		  i = 0;
		  while (arrayIndex > i) {
			  personal = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(personal, TVGN_NEXT);
			  i++;
		  }

		  //1.5.7. ?????? ?????? ??????.
		  ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->Expand(parent, TVE_EXPAND);
		  //1.5.8. ???????? ??????.
		  ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->Expand(company, TVE_EXPAND);
		  //1.5.9. ???? ?????????? ?????? ?????? ????????.
		  ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->SelectItem(personal);

		  //1.6.0. ?????? ?????? ?????? ????????.
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
	//2. ???????? ???????? ??
	  //2.1. ???? ???????? ????????.
	  PuttingForm puttingForm;
	  puttingForm.DoModal();
}


void BusinessCardBinderForm::OnFindButtonClicked() {
	//3. ???????? ???????? ??
	  //3.1. ???? ???????? ????????.
	  FindingForm findingForm;
	  findingForm.DoModal();
}

void BusinessCardBinderForm::OnPutOutButtonClicked() {
	//4. ???????? ???????? ??
	  //4.1. ???? ???????? ????????.
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

	//5. ???????? ???? ???????? ??
	  //5.1. ?????????? ????????.
	this->indexBinder->Arrange();

	//5.2. ?????? ??????????.
	((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->DeleteAllItems();

	//5.3. ???????????? ??????.
	this->indexBinder->MakeList(&indexes, &count);

	//5.4. ???????? ?????? ???? ????????.
	parent = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->InsertItem("??????", TVI_ROOT, TVI_FIRST);

	//5.5. ?????? ???????? ????????.
	while (count > i) {
		//5.5.1. ?????? ???? ?????? ???????? ????????.
		companyName = CString(indexes[i].GetCompanyName().c_str());
		company = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->InsertItem(companyName, parent, TVI_LAST);

		//5.5.2. ?????? ???????? ????????.
		j = 0;
		while (indexes[i].GetLength() > j) {
			//5.5.2.1. ?????? ?????? ???????? ????????.
			index = indexes[i].GetAt(j);
			personalName = CString(index->GetPersonalName().c_str());
			personal = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->InsertItem(personalName, company, TVI_LAST);
			j++;
		}
		i++;
	}

	//5.6. ?????? ?????????? ?????? ?????????? ??????.
	index = this->businessCardBinder->First();

	//5.7. ???????? ??????.
	companyName = CString(index->GetCompanyName().c_str());
	company = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(parent, TVGN_CHILD);
	companyText = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetItemText(company);
	while (companyText != companyName) {
		company = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(company, TVGN_NEXT);
		companyText = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetItemText(company);
	}

	//5.8. ???????? ??????.
	//?????? ??????.
	index_ = this->indexBinder->Find((LPCTSTR)companyName);
	//???? ?????????? ???? ?????? ?????? ??????
	arrayIndex = index_->Find(index);
	//???????? ??????.
	personal = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(company, TVGN_CHILD);
	i = 0;
	while (arrayIndex > i) {
		personal = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(personal, TVGN_NEXT);
		i++;
	}

	//5.9. ?????? ?????? ??????.
	((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->Expand(parent, TVE_EXPAND);
	//6.0. ???????? ??????.
	((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->Expand(company, TVE_EXPAND);
	//6.1. ???? ?????????? ?????? ?????? ????????.
	((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->SelectItem(personal);

	//6.2. ???????? ???? ?????? ?????? ????????.
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

	//6. ?????? ???? ???????? ???? ???????? ???????? ???? ??
	  //6.1. ?????? ?????? ???????????? ???????????? ????????.
	handle = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(NULL, TVGN_CARET);
	handleParent = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(handle, TVGN_PARENT);
	parent = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(NULL, TVGN_ROOT);

	//6.2. ?????? ?????? ?????????? ????,
	if (handleParent != parent) {
		//6.2.1. ?????? ???????? ??????.
		personal = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(NULL, TVGN_CARET);

		//6.2.2. ???????? ??????.
		company = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(personal, TVGN_PARENT);

		//6.2.3. ???????? TEXT??????.
		companyText = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetItemText(company);

		//6.2.4. ???? ???????????? ???? ?????????? ?????????? ????????.(????)
		companyChild = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(company, TVGN_CHILD);
		while (companyChild != personal) {
			companyChild = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(companyChild, TVGN_NEXT);
			arrayIndex++;
		}

		//6.2.5. ?????????? ???? ?????? ??????.
		index_ = this->indexBinder->Find((LPCTSTR)companyText);

		//6.2.6. ???? ???? ?? ?????? ?????? ??????.
		index = index_->GetAt(arrayIndex);

		//6.2.7. ???? ?????? ?????? ???? ???????? ????????.
		index = this->businessCardBinder->Move(index);

		//6.2.8. ???? ?????? ?????? ????????.
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

	//5. ???????? ???????? ??
	  //5.1. ???? ?????? ???? ?????? ????????.
	index = this->businessCardBinder->First();

	//5.2. ???? ?????? ?????? ???????? ??????.
	companyName = CString(index->GetCompanyName().c_str());

	parent = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(NULL, TVGN_ROOT);

	company = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(parent, TVGN_CHILD);
	companyText = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetItemText(company);
	while (companyText != companyName) {
		company = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(company, TVGN_NEXT);
		companyText = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetItemText(company);
	}

	//5.3. ?????????? ??????.
	index_ = this->indexBinder->Find((LPCTSTR)companyName);
	arrayIndex = index_->Find(index);

	personal = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(company, TVGN_CHILD);
	while (arrayIndex > i) {
		personal = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(personal, TVGN_NEXT);
		i++;
	}

	//5.4. ?????? ?????? ??????.
	((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->Expand(parent, TVE_EXPAND);
	//5.5. ???????? ??????.
	((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->Expand(company, TVE_EXPAND);
	//5.6. ???? ?????????? ?????? ?????? ????????.
	((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->SelectItem(personal);

	//5.7. ???????? ???? ?????? ?????? ????????.
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

	//6. ???????? ???????? ??
	  //6.1. ???? ?????? ???? ?????? ????????.
	index = this->businessCardBinder->Previous();

	//6.2. ???? ?????? ?????? ???????? ??????.
	companyName = CString(index->GetCompanyName().c_str());
	parent = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(NULL, TVGN_ROOT);
	company = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(parent, TVGN_CHILD);
	companyText = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetItemText(company);
	while (companyText != companyName) {
		company = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(company, TVGN_NEXT);
		companyText = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetItemText(company);
	}

	//6.3. ?????????? ??????.
	index_ = this->indexBinder->Find((LPCTSTR)companyName);
	arrayIndex = index_->Find(index);
	personal = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(company, TVGN_CHILD);
	while (arrayIndex > i) {
		personal = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(personal, TVGN_NEXT);
		i++;
	}

	//6.4. ?????? ?????? ??????.
	((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->Expand(parent, TVE_EXPAND);
	//6.5. ???????? ??????.
	((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->Expand(company, TVE_EXPAND);
	//6.6. ???? ?????????? ?????? ?????? ????????.
	((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->SelectItem(personal);

	//6.7. ???????? ???? ?????? ?????? ????????.
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

	//7. ???????? ???????? ??
	  //7.1. ???? ?????? ???? ?????? ????????.
	index = this->businessCardBinder->Next();

	//7.2. ???? ?????? ?????? ???????? ??????.
	companyName = CString(index->GetCompanyName().c_str());
	parent = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(NULL, TVGN_ROOT);
	company = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(parent, TVGN_CHILD);
	companyText = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetItemText(company);
	while (companyText != companyName) {
		company = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(company, TVGN_NEXT);
		companyText = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetItemText(company);
	}

	//7.3. ?????????? ??????.
	index_ = this->indexBinder->Find((LPCTSTR)companyName);
	arrayIndex = index_->Find(index);
	personal = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(company, TVGN_CHILD);
	while (arrayIndex > i) {
		personal = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(personal, TVGN_NEXT);
		i++;
	}

	//7.4. ?????? ?????? ??????.
	((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->Expand(parent, TVE_EXPAND);
	//7.5. ???????? ??????.
	((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->Expand(company, TVE_EXPAND);
	//7.6. ???? ?????????? ?????? ?????? ????????.
	((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->SelectItem(personal);

	//7.7. ???????? ???? ?????? ?????? ????????.
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

	//8. ?????????? ???????? ??
	  //8.1. ?????? ?????? ???? ?????? ????????.
	index = this->businessCardBinder->Last();

	//8.2. ???? ?????? ?????? ???????? ??????.
	companyName = CString(index->GetCompanyName().c_str());
	parent = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(NULL, TVGN_ROOT);
	company = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(parent, TVGN_CHILD);
	companyText = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetItemText(company);
	while (companyText != companyName) {
		company = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(company, TVGN_NEXT);
		companyText = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetItemText(company);
	}

	//8.3. ?????????? ??????.
	index_ = this->indexBinder->Find((LPCTSTR)companyName);
	arrayIndex = index_->Find(index);
	personal = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(company, TVGN_CHILD);
	while (arrayIndex > i) {
		personal = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(personal, TVGN_NEXT);
		i++;
	}

	//8.4. ?????? ?????? ??????.
	((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->Expand(parent, TVE_EXPAND);
	//8.5. ???????? ??????.
	((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->Expand(company, TVE_EXPAND);
	//8.6. ???? ?????????? ?????? ?????? ????????.
	((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->SelectItem(personal);

	//8.7. ???????? ???? ?????? ?????? ????????.
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
	//9. ???? ???? ???????? ??

	  //9.0.  ????????.
	   Save();

	  //9.1. ???????? ??????.
	  if (this->businessCardBinder != NULL) {
		  delete this->businessCardBinder;
		  this->businessCardBinder = NULL;
	  }

	  //10.3. ???????? ??????.
	  if (this->indexBinder != NULL) {
		  delete this->indexBinder;
		  this->indexBinder = NULL;
	  }
	  //9.2. ???????? ????????.
	  EndDialog(0);
}






