//Company.cpp
#include"Company.h"

Company::Company()
	:name(""), address(""), telephoneNumber(""), faxNumber(""), url("") {
}

Company::~Company() {
}

Company::Company(string name, string address, string telephoneNumber, string faxNumber, string url)
	: name(name), address(address), telephoneNumber(telephoneNumber), faxNumber(faxNumber), url(url) {
}

Company::Company(const Company& source)
	: name(source.name), address(source.address), telephoneNumber(source.telephoneNumber),
	faxNumber(source.faxNumber), url(source.url) {
}

bool Company::IsEqual(const Company& other) {
	bool ret = false;

	if (this->name.compare(other.name) == 0 && this->address.compare(other.address) == 0 &&
		this->telephoneNumber.compare(other.telephoneNumber) == 0 &&
		this->faxNumber.compare(other.faxNumber) == 0 && this->url.compare(other.url)==0) {
		ret = true;
	}
	return ret;
}

bool Company::IsNotEqual(const Company& other) {
	bool ret = false;

	if (this->name.compare(other.name) != 0 || this->address.compare(other.address) != 0 ||
		this->telephoneNumber.compare(other.telephoneNumber) != 0 ||
		this->faxNumber.compare(other.faxNumber) != 0 || this->url.compare(other.url) != 0) {
		ret = true;
	}
	return ret;
}

bool Company::operator==(const Company& other) {
	bool ret = false;

	if (this->name.compare(other.name) == 0 && this->address.compare(other.address) == 0 &&
		this->telephoneNumber.compare(other.telephoneNumber) == 0 &&
		this->faxNumber.compare(other.faxNumber) == 0 && this->url.compare(other.url) == 0) {
		ret = true;
	}
	return ret;
}

bool Company::operator!=(const Company& other) {
	bool ret = false;

	if (this->name.compare(other.name) != 0 || this->address.compare(other.address) != 0 ||
		this->telephoneNumber.compare(other.telephoneNumber) != 0 ||
		this->faxNumber.compare(other.faxNumber) != 0 || this->url.compare(other.url) != 0) {
		ret = true;
	}
	return ret;
}

Company& Company::operator=(const Company& source) {
	this->name = source.name;
	this->address = source.address;
	this->telephoneNumber = source.telephoneNumber;
	this->faxNumber = source.faxNumber;
	this->url = source.url;

	return *this;
}

#if 0

#include<iostream>
using namespace std;
int main(int argc, char* argv[]) {
	Company company;
	bool ret;

	Company one("삼성", "서울시", "02333", "02333", "www.samsung@");
	cout << one.GetName() << "-" << one.GetAddress() << "-" << one.GetTelephoneNumber() << "-" << 
		one.GetFaxNumber() << "-" <<one.GetUrl() << endl;

	Company two("현대", "서울시", "02344", "02344", "www.hyundai@");
	cout << two.GetName() << "-" << two.GetAddress() << "-" << two.GetTelephoneNumber() << "-" << two.GetFaxNumber() << "-" <<
		two.GetUrl() << endl;

	Company three(one);
	cout << one.GetName() << "-" << one.GetAddress() << "-" << one.GetTelephoneNumber() << "-" <<
		one.GetFaxNumber() << "-" << one.GetUrl() << endl;

	ret = one.IsEqual(three);
	if (ret == true) {
		cout << "같습니다." << endl;
	}
	else {
		cout << "같지 않습니다." << endl;
	}

	ret = one.IsNotEqual(two);
	if (ret == true) {
		cout << "같지 않습니다." << endl;
	}
	else {
		cout << "같습니다." << endl;
	}

	if (one == three) {
		cout << "같습니다." << endl;
	}
	else {
		cout << "같지 않습니다." << endl;
	}

	if (one != two) {
		cout << "같지 않습니다." << endl;
	}
	else {
		cout << "같습니다." << endl;
	}

	company = two;
	cout << company.GetName() << "-" << company.GetAddress() << "-" << company.GetTelephoneNumber() << "-" <<
		company.GetFaxNumber() << "-" << company.GetUrl() << endl;

	company = three;
	cout << company.GetName() << "-" << company.GetAddress() << "-" << company.GetTelephoneNumber() << "-" <<
		company.GetFaxNumber() << "-" << company.GetUrl() << endl;

	return 0;
}

#endif












