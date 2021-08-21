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

	//1. 입력받은 회사와 동일한 회사가 있는지 확인한다.
	sql.Format(
		"SELECT Company.companyCode FROM Company WHERE name='%s' AND address='%s' AND telephoneNumber='%s' "
		"AND faxNumber='%s' AND url='%s';", 
		index->GetCompanyName().c_str(), index->GetAddress().c_str(), index->GetCompanyTelphoneNumber().c_str(), 
		index->GetFaxNumber().c_str(), index->GetUrl().c_str());
	
	db.OpenEx("DSN=BusinessCardBinder;UID=Sydney;PWD=4821");
	rs.Open(AFX_DB_USE_DEFAULT_TYPE, sql);


	//2. 해당 회사코드가 있는 경우, 회사 코드를 구한다.
	if (!rs.IsEOF()) {
		rs.GetFieldValue((short)0, companyCode);
	}
    
	// 오늘 날짜를 구한다.
	time(&timer);
	today = localtime(&timer);
	year.Format("%4d", today->tm_year + 1900); //2021 출력      
	year = year.Right(2); //21 출력
	todayDate.Format("%s%02d%02d", year, today->tm_mon + 1, today->tm_mday); //210414 출력


	sql = "SELECT Company.companyCode FROM Company ORDER BY companyCode DESC;";
	rs1.Open(AFX_DB_USE_DEFAULT_TYPE, sql);


	if (rs1.IsEOF()) {
		companyCode.Format("C%s001", todayDate);
	}
	//4. 해당 회사코드는 없지만, 다른 코드는 있는 경우,
	else if (!rs1.IsEOF() && rs.IsEOF()) {
		//4.1. 해당 코드 내 제일 마지막 코드를 구한다.
		rs1.GetFieldValue((short)0, companyCode); //P210414003
		companyCode_ = companyCode;

		//4.2. 마지막 코드의 날짜 부분을 분리한다.
		companyCode = companyCode.Mid(1, 6);//210414

		//4.3. 마지막 코드와 오늘 날짜가 다를 경우, 회사 코드를 설정한다.
		if (companyCode != todayDate) {
			companyCode_.Format("C%s000", todayDate); //P210416000
		}

		//4.4. 맨 뒤 세자리 숫자부분을 분리한다.
		companyCode_ = companyCode_.Right(3);

		//4.5. 분리된 숫자부분을 정수로 바꿔준다.
		number = _ttoi(companyCode_);

		//4.6. 숫자를 1 더해준다.
		number++;

		//4.7. 숫자를 오늘 날짜와 합쳐준다.
		companyCode.Format("C%s%03d", todayDate, number);
	}


#if 0
	//3. 데이터가 없는 경우,
	else {
		//3.1. 오늘 날짜를 구한다.
		time(&timer);
		today = localtime(&timer);
		year.Format("%4d", today->tm_year + 1900); //2021 출력      
		year = year.Right(2); //21 출력
		todayDate.Format("%s%02d%02d", year, today->tm_mon + 1, today->tm_mday); //210414 출력


		//3.2. 데이터 자체가 없는 것인지, 해당 회사 코드만 없는것인지 확인한다.
		//3.3. 해당 회사의 코드만 없는 경우,
		if (!rs1.IsEOF()) { //다른 회사 코드는 있다는 것.
			//3.3.1. 해당 코드 내 제일 마지막 코드를 구한다.
			rs1.GetFieldValue((short)0, companyCode); //P210414003
			companyCode_ = companyCode; 

			//3.3.2. 마지막 코드의 날짜 부분을 분리한다.
			companyCode = companyCode.Mid(1, 6);//210414

			//3.3.3. 마지막 코드와 오늘 날짜가 다를 경우, 회사 코드를 설정한다.
			if (companyCode !=todayDate) {
				companyCode_.Format("C%s000", todayDate); //P210416000
			}

			//3.3.4. 맨 뒤 세자리 숫자부분을 분리한다.
			companyCode_ = companyCode_.Right(3);
			
			//3.3.5. 분리된 숫자부분을 정수로 바꿔준다.
			number = _ttoi(companyCode_);

			//3.3.6. 숫자를 1 더해준다.
			number++;

			//3.3.7. 숫자를 오늘 날짜와 합쳐준다.
			companyCode.Format("C%s%03d", todayDate, number);
			
		}
		//3.4. 데이터 자체가 없을 경우, 코드를 설정해준다.
		else {
			companyCode.Format("C%s001", todayDate);
		}

	}
#endif
	//4. 코드를 출력한다.
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

	//1. 오늘 날짜를 구한다. (210416 형식으로)
	time(&timer);
	today = localtime(&timer);
	year.Format("%4d", today->tm_year + 1900); //2021 출력      
	year = year.Right(2); //21 출력
	todayDate.Format("%s%02d%02d", year, today->tm_mon + 1, today->tm_mday); //210414 출력

	//2. 코드가 있는 경우,
	db.OpenEx("DSN=BusinessCardBinder;UID=Sydney;PWD=4821");
	rs.Open(AFX_DB_USE_DEFAULT_TYPE, sql);

	if (!rs.IsEOF()) {
		//2.1. 제일 큰 코드를 구한다.
		rs.GetFieldValue((short)0, code); //P210414003
		code_ = code;

		//2.2. 날짜부분만 분리한다.
		code = code.Mid(1, 6); //210414

		//2.3. 오늘 날짜와 같지 않은 경우, 기본 코드를 설정해준다.

		if (code!=todayDate) {
			code_.Format("P%s000", todayDate);
		}

		//2.4. 코드의 뒤 세자리를 분리한다.
		code_ = code_.Right(3);

		//2.5. 분리한 숫자부분을 정수로 변환한다.
		number = _ttoi(code_);

		//2.6. 정수를 1씩 더한다.
		number++;

		//2.7. 숫자를 P210414001 형식으로 변환한다.
		code.Format("P%s%03d", todayDate, number);
	}
	//3. 코드가 아예 없는 경우, 기본 코드로 설정해준다.
	else {
		code.Format("P%s001", todayDate);
	}
	//4. 코드를 출력한다.
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

	//1. 회사 DB에 중복되는 회사가 없으면 Insert한다.
	if (rs.IsEOF()) {
		sql.Format("INSERT INTO Company(companyCode, name, address,telephoneNumber,faxNumber,url)" 
			" VALUES('%s','%s', '%s', '%s', '%s', '%s');", (LPCTSTR)companyCode, index->GetCompanyName().c_str(),
			index->GetAddress().c_str(), index->GetCompanyTelphoneNumber().c_str(),
			index->GetFaxNumber().c_str(), index->GetUrl().c_str());
		db.ExecuteSQL(sql);
	}

	//2. 개인 Insert한다.
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

	//1. 삭제할 code와 companyCode를 찾는다.
	db.OpenEx("DSN=BusinessCardBinder;UID=Sydney;PWD=4821");
	rs.Open(AFX_DB_USE_DEFAULT_TYPE, sql);
	CString code;
	CString companyCode;

	rs.GetFieldValue((short)0, code);
	rs.GetFieldValue((short)1, companyCode);

	//2. 개인에서 해당 code를 삭제한다.
	sql.Format("DELETE FROM Personal WHERE code='%s';", (LPCTSTR)code);
	db.ExecuteSQL(sql);

	//3. 개인에 해당 companyCode를 가진 개인이 없으면, 해당 코드 회사를 삭제한다.
	sql.Format("SELECT Personal.companyCode FROM Personal WHERE companyCode='%s';",(LPCTSTR)companyCode); 

	rs1.Open(AFX_DB_USE_DEFAULT_TYPE, sql);
	if (rs.IsEOF()) {
		sql.Format("DELETE FROM Company WHERE companyCode='%s';", (LPCTSTR)companyCode);
		db.ExecuteSQL(sql);
	}

	//4. 파일을 닫는다
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

	//1. index번째의 code와 companyCode를 구한다.
	db.OpenEx("DSN=BusinessCardBinder;UID=Sydney;PWD=4821");
	rs.Open(AFX_DB_USE_DEFAULT_TYPE, sql);

	it = this->businessCardBinder->First();
	while (it != index) {
		rs.GetFieldValue((short)0, code);
		rs.GetFieldValue((short)1, companyCode);

		rs.MoveNext();
		it = this->businessCardBinder->Next();
	}

	//2. 해당 코드를 업데이트한다.
	sql.Format("Update Personal SET name='%s', rankName='%s', telephoneNumber='%s', emailAddress='%s'"
		" WHERE code='%s';", index->GetPersonalName().c_str(), index->GetRankName().c_str(),
		index->GetPersonalTelephoneNumber().c_str(), index->GetEmailAddress().c_str(), (LPCTSTR)code);

	db.ExecuteSQL(sql);

	//3. 해당 회사코드를 업데이트한다.
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


	//1. DB의 모든 데이터를 지운다.
	db.OpenEx("DSN=BusinessCardBinder;UID=Sydney;PWD=4821");
	rs.Open(AFX_DB_USE_DEFAULT_TYPE, sql);

	db.ExecuteSQL("DELETE FROM Personal");
	db.ExecuteSQL("DELETE FROM Company");

	//2. 명함의 끝이 아닌동안 반복한다.
	it = this->businessCardBinder->First();

	while (!rs.IsEOF() && it != previous) {
		rs.GetFieldValue((short)0, code);
		rs.GetFieldValue((short)1, companyCode);

		//2.1. DB에 동일한 회사가 있는지 확인한다.
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


		//2.2. DB에 없는 회사이면, 회사DB에 추가한다.
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

		//2.3. 개인을 추가한다.
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

	//1. 윈도우가 생성될 때
	  //1.1. 명함철을 만든다.
	  this->businessCardBinder = new BusinessCardBinder;

	  //1.2. 색인철을 만든다.
	  this->indexBinder = new IndexBinder;

	  //1.3. 트리뷰에 회사들 핸들을 만든다.
	  parent = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->InsertItem("회사들", TVI_ROOT, TVI_FIRST);

	  //1.4. Load한다.
	  count = Load();

	  //1.5. count가 0보다 큰 경우,
	  if (count > 0) {
		  //1.5.1. 명함의 처음부터 끝까지 반복한다.
		  index = this->businessCardBinder->First();
		  while (index != previous) {
			  //1.5.1.1. 색인철에 꽂는다
			  this->indexBinder->PutIn(index);
			  previous = index;
			  index = this->businessCardBinder->Next();
		  }

		  //1.5.2. 색인리스트를 만든다.
		  this->indexBinder->MakeList(&indexes, &countList);

		  //1.5.3. 색인의 개수만큼 반복한다.
		  while (countList > i) {
			  //1.5.3.1. 회사들 하위에 상호핸들 추가한다.
			  companyName = CString(indexes[i].GetCompanyName().c_str());
			  company = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->InsertItem(companyName, parent, TVI_SORT);

			  //1.5.3.2. 상호명 하위에 개인핸들 추가한다.
			  j = 0;
			  while (indexes[i].GetLength() > j) {
				  index = indexes[i].GetAt(j);
				  personalName = CString(index->GetPersonalName().c_str());
				  ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->InsertItem(personalName, company, TVI_LAST);

				  j++;
			  }
			  i++;
		  }
		  //1.5.4.  첫번째 명함을 최근 명함으로 설정한다.
		  index = this->businessCardBinder->First();

		  //1.5.5. 상호핸들 구한다.
		  companyName = CString(index->GetCompanyName().c_str());
		  company = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(parent, TVGN_CHILD);
		  companyText = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetItemText(company);
		  while (companyText != companyName) {
			  company = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(company, TVGN_NEXT);
			  companyText = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetItemText(company);
		  }

		  //1.5.6. 개인성명 찾는다.
		  //색인철 찾는다.
		  index_ = this->indexBinder->Find((LPCTSTR)companyName);
		  //해당 색인철에서 해당 개인이 몇번째 있는지
		  arrayIndex = index_->Find(index);
		  //개인핸들 찾는다.
		  personal = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(company, TVGN_CHILD);
		  i = 0;
		  while (arrayIndex > i) {
			  personal = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(personal, TVGN_NEXT);
			  i++;
		  }

		  //1.5.7. 회사들 핸들을 펼친다.
		  ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->Expand(parent, TVE_EXPAND);
		  //1.5.8. 상호핸들 펼친다.
		  ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->Expand(company, TVE_EXPAND);
		  //1.5.9. 해당 개인핸들을 선택된 상태로 처리한다.
		  ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->SelectItem(personal);

		  //1.6.0. 첫번째 명함을 명함에 출력한다.
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
	//2. 꽂기버튼 클릭했을 때
	  //2.1. 꽂기 윈도우를 출력한다.
	  PuttingForm puttingForm;
	  puttingForm.DoModal();
}


void BusinessCardBinderForm::OnFindButtonClicked() {
	//3. 찾기버튼 클릭했을 때
	  //3.1. 찾기 윈도우를 출력한다.
	  FindingForm findingForm;
	  findingForm.DoModal();
}

void BusinessCardBinderForm::OnPutOutButtonClicked() {
	//4. 뽑기버튼 클릭했을 때
	  //4.1. 뽑기 윈도우를 출력한다.
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

	//5. 정리하기 버튼 클릭했을 때
	  //5.1. 색인철에서 정리한다.
	this->indexBinder->Arrange();

	//5.2. 트리뷰 초기화한다.
	((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->DeleteAllItems();

	//5.3. 색인리스트를 구한다.
	this->indexBinder->MakeList(&indexes, &count);

	//5.4. 트리뷰에 회사들 항목 추가한다.
	parent = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->InsertItem("회사들", TVI_ROOT, TVI_FIRST);

	//5.5. 색인의 개수만큼 반복한다.
	while (count > i) {
		//5.5.1. 회사들 핸들 하위에 상호명을 추가한다.
		companyName = CString(indexes[i].GetCompanyName().c_str());
		company = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->InsertItem(companyName, parent, TVI_LAST);

		//5.5.2. 명함의 개수만큼 반복한다.
		j = 0;
		while (indexes[i].GetLength() > j) {
			//5.5.2.1. 상호명 하위에 개인성명 추가한다.
			index = indexes[i].GetAt(j);
			personalName = CString(index->GetPersonalName().c_str());
			personal = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->InsertItem(personalName, company, TVI_LAST);
			j++;
		}
		i++;
	}

	//5.6. 명함의 현재위치를 첫번째 명함위치로 옮긴다.
	index = this->businessCardBinder->First();

	//5.7. 상호핸들 찾는다.
	companyName = CString(index->GetCompanyName().c_str());
	company = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(parent, TVGN_CHILD);
	companyText = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetItemText(company);
	while (companyText != companyName) {
		company = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(company, TVGN_NEXT);
		companyText = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetItemText(company);
	}

	//5.8. 개인성명 찾는다.
	//색인철 찾는다.
	index_ = this->indexBinder->Find((LPCTSTR)companyName);
	//해당 색인철에서 해당 개인이 몇번째 있는지
	arrayIndex = index_->Find(index);
	//개인핸들 찾는다.
	personal = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(company, TVGN_CHILD);
	i = 0;
	while (arrayIndex > i) {
		personal = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(personal, TVGN_NEXT);
		i++;
	}

	//5.9. 회사들 핸들을 펼친다.
	((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->Expand(parent, TVE_EXPAND);
	//6.0. 상호핸들 펼친다.
	((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->Expand(company, TVE_EXPAND);
	//6.1. 해당 개인핸들을 선택된 상태로 처리한다.
	((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->SelectItem(personal);

	//6.2. 명함철에 현재 위치의 명함을 출력한다.
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

	//6. 트리뷰 상의 성명핸들 또는 상호핸들 더블클릭 했을 때
	  //6.1. 클릭된 핸들이 성명핸들인지 상호핸들인지 확인한다.
	handle = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(NULL, TVGN_CARET);
	handleParent = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(handle, TVGN_PARENT);
	parent = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(NULL, TVGN_ROOT);

	//6.2. 클릭된 핸들이 개인핸들일 경우,
	if (handleParent != parent) {
		//6.2.1. 클릭된 개인핸들 구한다.
		personal = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(NULL, TVGN_CARET);

		//6.2.2. 상호핸들 구한다.
		company = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(personal, TVGN_PARENT);

		//6.2.3. 상호핸들 TEXT구한다.
		companyText = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetItemText(company);

		//6.2.4. 해당 상호핸들에서 해당 개인핸들이 몇변째인지 확인한다.(첨자)
		companyChild = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(company, TVGN_CHILD);
		while (companyChild != personal) {
			companyChild = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(companyChild, TVGN_NEXT);
			arrayIndex++;
		}

		//6.2.5. 색인철에서 상호 위치를 구한다.
		index_ = this->indexBinder->Find((LPCTSTR)companyText);

		//6.2.6. 해당 상호 내 명함의 위치를 찾는다.
		index = index_->GetAt(arrayIndex);

		//6.2.7. 해당 위치의 명함을 현재 명함으로 설정한다.
		index = this->businessCardBinder->Move(index);

		//6.2.8. 현재 명함을 명함에 출력한다.
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

	//5. 처음버튼 클릭했을 때
	  //5.1. 처음 명함을 현재 위치로 갖다둔다.
	index = this->businessCardBinder->First();

	//5.2. 현재 명함의 상호로 상호핸들 찾는다.
	companyName = CString(index->GetCompanyName().c_str());

	parent = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(NULL, TVGN_ROOT);

	company = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(parent, TVGN_CHILD);
	companyText = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetItemText(company);
	while (companyText != companyName) {
		company = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(company, TVGN_NEXT);
		companyText = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetItemText(company);
	}

	//5.3. 개인핸들을 찾는다.
	index_ = this->indexBinder->Find((LPCTSTR)companyName);
	arrayIndex = index_->Find(index);

	personal = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(company, TVGN_CHILD);
	while (arrayIndex > i) {
		personal = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(personal, TVGN_NEXT);
		i++;
	}

	//5.4. 회사들 핸들을 펼친다.
	((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->Expand(parent, TVE_EXPAND);
	//5.5. 상호핸들 펼친다.
	((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->Expand(company, TVE_EXPAND);
	//5.6. 해당 개인핸들을 선택된 상태로 처리한다.
	((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->SelectItem(personal);

	//5.7. 명함철에 현재 위치의 명함을 출력한다.
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

	//6. 이전버튼 클릭했을 때
	  //6.1. 이전 명함을 현재 위치로 갖다둔다.
	index = this->businessCardBinder->Previous();

	//6.2. 현재 명함의 상호로 상호핸들 찾는다.
	companyName = CString(index->GetCompanyName().c_str());
	parent = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(NULL, TVGN_ROOT);
	company = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(parent, TVGN_CHILD);
	companyText = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetItemText(company);
	while (companyText != companyName) {
		company = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(company, TVGN_NEXT);
		companyText = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetItemText(company);
	}

	//6.3. 개인핸들을 찾는다.
	index_ = this->indexBinder->Find((LPCTSTR)companyName);
	arrayIndex = index_->Find(index);
	personal = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(company, TVGN_CHILD);
	while (arrayIndex > i) {
		personal = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(personal, TVGN_NEXT);
		i++;
	}

	//6.4. 회사들 핸들을 펼친다.
	((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->Expand(parent, TVE_EXPAND);
	//6.5. 상호핸들 펼친다.
	((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->Expand(company, TVE_EXPAND);
	//6.6. 해당 개인핸들을 선택된 상태로 처리한다.
	((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->SelectItem(personal);

	//6.7. 명함철에 현재 위치의 명함을 출력한다.
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

	//7. 다음버튼 클릭했을 때
	  //7.1. 다음 명함을 현재 위치로 갖다둔다.
	index = this->businessCardBinder->Next();

	//7.2. 현재 명함의 상호로 상호핸들 찾는다.
	companyName = CString(index->GetCompanyName().c_str());
	parent = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(NULL, TVGN_ROOT);
	company = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(parent, TVGN_CHILD);
	companyText = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetItemText(company);
	while (companyText != companyName) {
		company = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(company, TVGN_NEXT);
		companyText = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetItemText(company);
	}

	//7.3. 개인핸들을 찾는다.
	index_ = this->indexBinder->Find((LPCTSTR)companyName);
	arrayIndex = index_->Find(index);
	personal = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(company, TVGN_CHILD);
	while (arrayIndex > i) {
		personal = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(personal, TVGN_NEXT);
		i++;
	}

	//7.4. 회사들 핸들을 펼친다.
	((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->Expand(parent, TVE_EXPAND);
	//7.5. 상호핸들 펼친다.
	((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->Expand(company, TVE_EXPAND);
	//7.6. 해당 개인핸들을 선택된 상태로 처리한다.
	((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->SelectItem(personal);

	//7.7. 명함철에 현재 위치의 명함을 출력한다.
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

	//8. 마지막버튼 클릭했을 때
	  //8.1. 마지막 명함을 현재 위치로 갖다둔다.
	index = this->businessCardBinder->Last();

	//8.2. 현재 명함의 상호로 상호핸들 찾는다.
	companyName = CString(index->GetCompanyName().c_str());
	parent = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(NULL, TVGN_ROOT);
	company = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(parent, TVGN_CHILD);
	companyText = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetItemText(company);
	while (companyText != companyName) {
		company = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(company, TVGN_NEXT);
		companyText = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetItemText(company);
	}

	//8.3. 개인핸들을 찾는다.
	index_ = this->indexBinder->Find((LPCTSTR)companyName);
	arrayIndex = index_->Find(index);
	personal = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(company, TVGN_CHILD);
	while (arrayIndex > i) {
		personal = ((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->GetNextItem(personal, TVGN_NEXT);
		i++;
	}

	//8.4. 회사들 핸들을 펼친다.
	((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->Expand(parent, TVE_EXPAND);
	//8.5. 상호핸들 펼친다.
	((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->Expand(company, TVE_EXPAND);
	//8.6. 해당 개인핸들을 선택된 상태로 처리한다.
	((CTreeCtrl*)GetDlgItem(IDC_TREE_INDEXES))->SelectItem(personal);

	//8.7. 명함철에 현재 위치의 명함을 출력한다.
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
	//9. 닫기 버튼 클릭했을 때

	  //9.0.  저장한다.
	   Save();

	  //9.1. 명함철을 없앤다.
	  if (this->businessCardBinder != NULL) {
		  delete this->businessCardBinder;
		  this->businessCardBinder = NULL;
	  }

	  //10.3. 색인철을 없앤다.
	  if (this->indexBinder != NULL) {
		  delete this->indexBinder;
		  this->indexBinder = NULL;
	  }
	  //9.2. 윈도우를 종료한다.
	  EndDialog(0);
}






