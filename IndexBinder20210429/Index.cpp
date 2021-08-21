//Index.cpp
#include"Index.h"
#include"BusinessCardBinder.h"

Index::Index(Long capacity)
	:companyName("") {
	this->capacity = capacity;
	this->length = 0;
}

Index::Index(string companyName, Long capacity)
	: companyName(companyName) {
	this->capacity = capacity;
	this->length = 0;
}

Index::~Index() {
}

Long Index::PutIn(BusinessCard* key) {
	Long index_;

	//1. index(첨자), key를 입력받는다.
	//2. capacity가 length보다 크면 Store를 호출한다.
	if (this->capacity > this->length) {

		index_ = this->businessCards.Store(this->length, key);
	}
	//3. capacity가 length보다 작거나 같으면 AppendForRear를 호출한다.
	else if (this->capacity <= this->length) {
		index_ = this->businessCards.AppendForRear(key);
		this->capacity++;
	}
	//4. length를 1씩 증가시킨다.
	this->length++;
	//5. 첨자를 출력한다.
	return index_;
}

Long Index::Find(BusinessCard* key) {
	Long index;

	index = this->businessCards.LinearSearchUnique(key, CompareBusinessCard);

	return index;
}

Long Index::PutOut(Long index) {
	Long index_;

	index_ = this->businessCards.Delete(index);

	this->capacity--;
	this->length--;

	return index_;
}

void Index::Arrange() {
	this->businessCards.SelectionSort(CompareName);
}

BusinessCard* Index::GetAt(Long index) {
	return this->businessCards.GetAt(index);
}

int CompareBusinessCard(void* one, void* other) {
	BusinessCard** one_ = (BusinessCard**)one;
	BusinessCard* other_ = (BusinessCard*)other;
	int ret;

	if (*one_ == other_) {
		ret = 0;
	}
	else if (*one_ > other_) {
		ret = 1;
	}
	else if (*one_ < other_) {
		ret = -1;
	}
	return ret;
}

int CompareName(void* one, void* other) {
	BusinessCard** one_ = (BusinessCard**)one;
	BusinessCard** other_ = (BusinessCard**)other;
	int ret;

	if ((*one_)->GetPersonalName() == (*other_)->GetPersonalName()) {
		ret = 0;
	}
	else if ((*one_)->GetPersonalName() > (*other_)->GetPersonalName()) {
		ret = 1;
	}
	else if ((*one_)->GetPersonalName() < (*other_)->GetPersonalName()) {
		ret = -1;
	}

	return ret;
}


#if 0

#include<iostream>
using namespace std;
int main(int argc, char* argv[]) {
	Index index("삼성", 256);
	BusinessCardBinder businessCardBinder;
	BusinessCard* index_;
	BusinessCard* businessCard;
	Long i;

	//홍길동 명함꽂기
	BusinessCard businessCard1("홍길동", "대리", "0101111", "hong@", "삼성", "서울시", "02333", "02333", 
		"www.samsung@");

	index_=businessCardBinder.PutIn(businessCard1);
#if 0
	cout << index_->GetPersonalName() << "-" << index_->GetRankName() << "-" << index_->GetPersonalTelephoneNumber() << "-"
		<< index_->GetEmailAddress() << "-" << index_->GetCompanyName() << "-" << index_->GetAddress() << "-" <<
		index_->GetCompanyTelphoneNumber() << "-" << index_->GetFaxNumber() << "-" << 
		index_->GetEmailAddress() << endl;
#endif
	//홍길동 색인꽂기
	i = index.PutIn(index_);



	//서길동 명함꽂기
	BusinessCard businessCard2("서길동", "주임", "0102222", "seo@", "삼성", "서울시", "02333", "02333",
		"www.samsung@");

	index_ = businessCardBinder.PutIn(businessCard2);

	//서길동 색인꽂기
	i = index.PutIn(index_);



	//김길동 명함꽂기
	BusinessCard businessCard3("김길동", "과장", "0103333", "kim@", "삼성", "서울시", "02333", "02333",
		"www.samsung@");

	index_ = businessCardBinder.PutIn(businessCard3);

	//김길동 색인꽂기
	i = index.PutIn(index_);

	i = 0;
	while (i < index.GetLength()) {
		businessCard = index.GetAt(i);

		cout << businessCard->GetPersonalName() << "-" << businessCard->GetAddress() << "-" <<
			businessCard->GetPersonalTelephoneNumber() << "-" << businessCard->GetEmailAddress() << "-" <<
			businessCard->GetCompanyName() << "-" << businessCard->GetAddress() << "-" <<
			businessCard->GetCompanyTelphoneNumber() << "-" << businessCard->GetFaxNumber() << "-" <<
			businessCard->GetUrl() << endl;

		i++;
	}

#if 0
	index_ =businessCardBinder.First();
	//명함철에서 뽑는다.
	businessCardBinder.PutOut(index_);

	//색인에서 몇번째에 뽑힌 명함이 있는지 찾는다.
	i = index.Find(index_);
	cout << i << endl;
	
	//색인에서 뽑는다. (몇번째인지 알아야 뽑을 수 있음)
	index.PutOut(i);

	i = 0;
	while (i < index.GetLength()) {
		businessCard = index.GetAt(i);

		cout << businessCard->GetPersonalName() << "-" << businessCard->GetAddress() << "-" <<
			businessCard->GetPersonalTelephoneNumber() << "-" << businessCard->GetEmailAddress() << "-" <<
			businessCard->GetCompanyName() << "-" << businessCard->GetAddress() << "-" <<
			businessCard->GetCompanyTelphoneNumber() << "-" << businessCard->GetFaxNumber() << "-" <<
			businessCard->GetUrl() << endl;

		i++;
	}
#endif

	index.Arrange();
	cout << "정렬하기" << endl << endl;
	i = 0;
	while (i < index.GetLength()) {
		businessCard = index.GetAt(i);

		cout << businessCard->GetPersonalName() << "-" << businessCard->GetAddress() << "-" <<
			businessCard->GetPersonalTelephoneNumber() << "-" << businessCard->GetEmailAddress() << "-" <<
			businessCard->GetCompanyName() << "-" << businessCard->GetAddress() << "-" <<
			businessCard->GetCompanyTelphoneNumber() << "-" << businessCard->GetFaxNumber() << "-" <<
			businessCard->GetUrl() << endl;

		i++;
	}



	return 0;
}


#endif
















