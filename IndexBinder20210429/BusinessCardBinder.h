//BusinessCardBinder.h
#ifndef _BUSINESSCARDBINDER_H
#define _BUSINESSCARDBINDER_H

#include"BusinessCard.h"
#include"LinkedList.h"
#include<string>
using namespace std;
typedef signed long int Long;

class BusinessCardBinder {
public: 
	BusinessCardBinder();
	~BusinessCardBinder();
	BusinessCardBinder(const BusinessCardBinder& source);
	BusinessCardBinder& operator=(const BusinessCardBinder& source);
	BusinessCard* PutIn(BusinessCard businessCard);
	void Find(string name, BusinessCard** (*indexes), Long* count);
	BusinessCard* FindCompanyName(string companyName);
	BusinessCard PutOut(BusinessCard* index);
	BusinessCard* Move(BusinessCard* index);
	BusinessCard* First();
	BusinessCard* Previous();
	BusinessCard* Next();
	BusinessCard* Last();
	Long GetLength() const;
	BusinessCard* GetCurrent() const;

private:
	LinkedList<BusinessCard> businessCards;
	Long length;
	BusinessCard* current;
};

inline Long BusinessCardBinder::GetLength() const {
	return this->length;
}
inline BusinessCard* BusinessCardBinder::GetCurrent() const {
	return const_cast<BusinessCard*>(this->current);
}

int CompareNames(void* one, void* other);
int CompareCompanyNames(void* one, void* other);
int CompareBusinessCards(void* one, void* other);


#endif//_BUSINESSCARDBINDER_H


