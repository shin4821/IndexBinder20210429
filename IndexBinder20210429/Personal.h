//Personal.h
#ifndef _PERSONAL_H
#define _PERSONAL_H
#include<string>
using namespace std;

class Personal {
public:
	Personal();
	Personal(string name, string rankName, string telephoneNumber, string emailAddress);
	~Personal();
	Personal(const Personal& source);
	bool IsEqual(const Personal& other);
	bool IsNotEqual(const Personal& other);
	bool operator==(const Personal& other);
	bool operator!=(const Personal& other);
	Personal& operator=(const Personal& source);
	string& GetName() const;
	string& GetRankName() const;
	string& GetTelephoneNumber() const;
	string& GetEmailAddress() const;

private:
	string name;
	string rankName;
	string telephoneNumber;
	string emailAddress;
};

inline string& Personal::GetName() const {
	return const_cast<string&>(this->name);
}
inline string& Personal::GetRankName() const {
	return const_cast<string&>(this->rankName);
}
inline string& Personal::GetTelephoneNumber() const {
	return const_cast<string&>(this->telephoneNumber);
}
inline string& Personal::GetEmailAddress() const {
	return const_cast<string&>(this->emailAddress);
}

#endif;//_PERSONAL_H
