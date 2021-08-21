//BusinessCard.cpp
#include"BusinessCard.h"

BusinessCard::BusinessCard() {
}

BusinessCard::~BusinessCard() {
}

BusinessCard::BusinessCard(string personalName, string rankName, string personalTelephoneNumber, string emailAddress,
	string companyName, string address, string companyTelephoneNumber, string faxNumber, string url)
	:personal(personalName, rankName, personalTelephoneNumber, emailAddress),
	company(companyName, address, companyTelephoneNumber, faxNumber, url) {
}

BusinessCard::BusinessCard(const BusinessCard& source)
	: personal(source.personal), company(source.company) {
}


bool BusinessCard::IsEqual(const BusinessCard& other) {
	bool ret = false;

	if (this->personal.IsEqual(other.personal) == true &&
		this->company.IsEqual(other.company) == true) {
		ret = true;
	}
	return ret;
}

bool BusinessCard::IsNotEqaul(const BusinessCard& other) {
	bool ret = false;

	if (this->personal.IsNotEqual(other.personal) == true &&
		this->company.IsNotEqual(other.company) == true) {
		ret = true;
	}
	return ret;
}

bool BusinessCard::operator==(const BusinessCard& other) {
	bool ret = false;

	if (this->personal == other.personal && this->company == other.company) {
		ret = true;
	}
	return ret;
}

bool BusinessCard::operator!=(const BusinessCard& other) {
	bool ret = false;

	if (this->personal != other.personal || this->company != other.company) {
		ret = true;
	}

	return ret;
}


BusinessCard& BusinessCard::operator=(const BusinessCard& source) {
	this->personal = source.personal;
	this->company = source.company;

	return *this;
}


#if 0
#include<iostream>
using namespace std;
int main(int argc, char* argv[]) {
	BusinessCard businessCard;
	bool ret;

	BusinessCard one("나길동", "부장", "0101111", "na@", "삼성", "서울시", "02333", "02333", "www.samsung@");
	cout << one.GetPersonalName() << "-" << one.GetRankName() << "-" << one.GetPersonalTelephoneNumber() << "-" <<
		one.GetEmailAddress() << "-" << one.GetCompanyName() << "-" << one.GetAddress() << "-" <<
		one.GetCompanyTelphoneNumber() << "-" << one.GetFaxNumber() << "-" << one.GetUrl() << endl;

	BusinessCard two("김길동", "인턴", "0102222", "kim@", "현대", "서울시", "02344", "02344", "www.hyundai@");
	cout << two.GetPersonalName() << "-" << two.GetRankName() << "-" << two.GetPersonalTelephoneNumber() << "-" <<
		two.GetEmailAddress() << "-" << two.GetCompanyName() << "-" << two.GetAddress() << "-" <<
		two.GetCompanyTelphoneNumber() << "-" << two.GetFaxNumber() << "-" << two.GetUrl() << endl;

	BusinessCard three(one);
	cout << three.GetPersonalName() << "-" << three.GetRankName() << "-" << three.GetPersonalTelephoneNumber() 
		<< "-" <<three.GetEmailAddress() << "-" << three.GetCompanyName() << "-" << three.GetAddress() << "-" <<
		three.GetCompanyTelphoneNumber() << "-" << three.GetFaxNumber() << "-" << three.GetUrl() << endl;



	ret = one.IsEqual(three);
	if (ret == true) {
		cout << "같습니다." << endl;
	}
	else {
		cout << "같지 않습니다." << endl;
	}

	ret = one.IsNotEqaul(two);
	if (ret == true) {
		cout << "같지 않습니다." << endl;
	}
	else {
		cout << "같습니다." << endl;
	}


	businessCard = two;
	cout << businessCard.GetPersonalName() << "-" << businessCard.GetRankName() << "-" <<
		businessCard.GetPersonalTelephoneNumber() << "-" << businessCard.GetEmailAddress() << "-" <<
		businessCard.GetCompanyName() << "-" << businessCard.GetAddress() << "-" <<
		businessCard.GetCompanyTelphoneNumber() << "-" << businessCard.GetFaxNumber() << "-" <<
		businessCard.GetUrl() << endl;

	if (businessCard == two) {
		cout << "같습니다" << endl;
	}
	else {
		cout << "같지 않습니다." << endl;
	}

	if (businessCard != one) {
		cout << "같지 않습니다." << endl;
	}
	else {
		cout << "같습니다." << endl;
	}

	return 0;
}

#endif














