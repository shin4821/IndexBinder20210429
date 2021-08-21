//Index.h
#ifndef _INDEX_H
#define _INDEX_H
typedef signed long int Long;
#include"Array.h"
#include<string>
using namespace std;

class BusinessCard;
class Index {
public:
	Index(Long capacity = 256);
	Index(string companyName, Long capacity = 256);
	~Index();
	Long PutIn(BusinessCard* key);
	Long Find(BusinessCard* key);
	Long PutOut(Long index);
	void Arrange();
	BusinessCard* GetAt(Long index);
	string& GetCompanyName() const;
	Long GetCapacity() const;
	Long GetLength() const;

private:
	string companyName;
	Array<BusinessCard*> businessCards;
	Long capacity;
	Long length;
};

inline string& Index::GetCompanyName() const {
	return const_cast<string&>(this->companyName);
}
inline Long Index::GetCapacity() const {
	return this->capacity;
}
inline Long Index::GetLength() const {
	return this->length;
}

int CompareBusinessCard(void* one, void* other);
int CompareName(void* one, void* other);


#endif//_INDEX_H