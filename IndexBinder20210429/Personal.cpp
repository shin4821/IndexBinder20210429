//Personal.cpp
#include"Personal.h"

Personal::Personal()
	:name(""), rankName(""), telephoneNumber(""), emailAddress("") {
}

Personal::~Personal() {
}

Personal::Personal(string name, string rankName, string telephoneNumber, string emailAddress)
	:name(name), rankName(rankName), telephoneNumber(telephoneNumber), emailAddress(emailAddress) {
}

Personal::Personal(const Personal& source)
	: name(source.name), rankName(source.rankName), telephoneNumber(source.telephoneNumber),
	emailAddress(source.emailAddress) {
}

bool Personal::IsEqual(const Personal& other) {
	bool ret = false;

	if (this->name.compare(other.name) == 0 && this->rankName.compare(other.rankName) == 0
		&& this->telephoneNumber.compare(other.telephoneNumber) == 0 &&
		this->emailAddress.compare(other.emailAddress) == 0) {
		ret = true;
	}
	return ret;
}

bool Personal::IsNotEqual(const Personal& other) {
	bool ret = false;

	if (this->name.compare(other.name) != 0 || this->rankName.compare(other.rankName) != 0 ||
		this->telephoneNumber.compare(other.telephoneNumber) != 0 ||
		this->emailAddress.compare(other.emailAddress) != 0) {
		ret = true;
	}
	return ret;
}

bool Personal::operator==(const Personal& other) {
	bool ret = false;

	if (this->name.compare(other.name) == 0 && this->rankName.compare(other.rankName) == 0 &&
		this->telephoneNumber.compare(other.telephoneNumber) == 0 &&
		this->emailAddress.compare(other.emailAddress) == 0) {
		ret = true;
	}
	return ret;
}

bool Personal::operator!=(const Personal& other) {
	bool ret = false;

	if (this->name.compare(other.name) != 0 || this->rankName.compare(other.rankName) != 0 ||
		this->telephoneNumber.compare(other.telephoneNumber) != 0 ||
		this->emailAddress.compare(other.emailAddress) != 0) {
		ret = true;
	}
	return ret;
}

Personal& Personal::operator=(const Personal& source) {
	this->name = source.name;
	this->rankName = source.rankName;
	this->telephoneNumber = source.telephoneNumber;
	this->emailAddress = source.emailAddress;

	return *this;
}

#if 0

#include<iostream>
using namespace std;
int main(int argc, char* argv[]) {
	Personal personal;
	bool ret;

	Personal one("��浿", "����", "0101111", "kim@");
	cout << one.GetName() << "-" << one.GetRankName() << "-" << one.GetTelephoneNumber() << "-" 
		<< one.GetEmailAddress() << endl;

	Personal other("ȫ�浿", "�븮", "0102222", "hong@");
	cout << other.GetName() << "-" << other.GetRankName() << "-" << other.GetTelephoneNumber() << "-"
		<< other.GetEmailAddress() << endl;

	Personal another(one);
	cout << another.GetName() << "-" << another.GetRankName() << "-" << another.GetTelephoneNumber() << "-"
		<< another.GetEmailAddress() << endl;

	ret = another.IsEqual(one);
	if (ret == true) {
		cout << "�����ϴ�." << endl;
	}
	else {
		cout << "���� �ʽ��ϴ�." << endl;
	}

	ret = one.IsNotEqual(another);
	if (ret == true) {
		cout << "���� �ʽ��ϴ�." << endl;
	}
	else {
		cout << "�����ϴ�." << endl;
	}

	if (one == another) {
		cout << "�����ϴ�." << endl;
	}

	if (one != other) {
		cout << "���� �ʽ��ϴ�." << endl;
	}

	personal = one;
	cout << personal.GetName() << "-" << personal.GetRankName() << "-" << personal.GetTelephoneNumber() << "-" <<
		personal.GetEmailAddress() << endl;

	return 0;
}

#endif







