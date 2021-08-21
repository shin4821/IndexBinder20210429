//BusinessCard.h
#ifndef _BUSINESSCARD_H
#define _BUSINESSCARD_H
#include"Personal.h"
#include"Company.h"
#include<string>
using namespace std;

class BusinessCard {
public:
	BusinessCard();
	BusinessCard(string personalName, string rankName, string personalTelephoneNumber, string emailAddress,
		string companyName, string address, string companyTelephoneNumber, string faxNumber, string url);
	~BusinessCard();
	BusinessCard(const BusinessCard& source);
	bool IsEqual(const BusinessCard& other);
	bool IsNotEqaul(const BusinessCard& other);
	bool operator==(const BusinessCard& other);
	bool operator!=(const BusinessCard& other);
	BusinessCard& operator=(const BusinessCard& source);
	string& GetPersonalName() const;
	string& GetRankName() const;
	string& GetPersonalTelephoneNumber() const;
	string& GetEmailAddress() const;
	string& GetCompanyName() const;
	string& GetAddress() const;
	string& GetCompanyTelphoneNumber() const;
	string& GetFaxNumber() const;
	string& GetUrl() const;

private:
	Personal personal;
	Company company;

};

inline string& BusinessCard::GetPersonalName() const {
	return const_cast<string&>(this->personal.GetName());
}
inline string& BusinessCard::GetRankName() const {
	return const_cast<string&>(this->personal.GetRankName());
}
inline string& BusinessCard::GetPersonalTelephoneNumber() const {
	return const_cast<string&>(this->personal.GetTelephoneNumber());
}
inline string& BusinessCard::GetEmailAddress() const {
	return const_cast<string&>(this->personal.GetEmailAddress());
}
inline string& BusinessCard::GetCompanyName() const {
	return const_cast<string&>(this->company.GetName());
}
inline string& BusinessCard::GetAddress() const {
	return const_cast<string&>(this->company.GetAddress());
}
inline string& BusinessCard::GetCompanyTelphoneNumber() const {
	return const_cast<string&>(this->company.GetTelephoneNumber());
}
inline string& BusinessCard::GetFaxNumber() const {
	return const_cast<string&>(this->company.GetFaxNumber());
}
inline string& BusinessCard::GetUrl() const {
	return const_cast<string&>(this->company.GetUrl());
}

#endif//_BUSINESSCARD_H









