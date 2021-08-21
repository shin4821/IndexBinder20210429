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

	//1. index(÷��), key�� �Է¹޴´�.
	//2. capacity�� length���� ũ�� Store�� ȣ���Ѵ�.
	if (this->capacity > this->length) {

		index_ = this->businessCards.Store(this->length, key);
	}
	//3. capacity�� length���� �۰ų� ������ AppendForRear�� ȣ���Ѵ�.
	else if (this->capacity <= this->length) {
		index_ = this->businessCards.AppendForRear(key);
		this->capacity++;
	}
	//4. length�� 1�� ������Ų��.
	this->length++;
	//5. ÷�ڸ� ����Ѵ�.
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
	Index index("�Ｚ", 256);
	BusinessCardBinder businessCardBinder;
	BusinessCard* index_;
	BusinessCard* businessCard;
	Long i;

	//ȫ�浿 ���Բȱ�
	BusinessCard businessCard1("ȫ�浿", "�븮", "0101111", "hong@", "�Ｚ", "�����", "02333", "02333", 
		"www.samsung@");

	index_=businessCardBinder.PutIn(businessCard1);
#if 0
	cout << index_->GetPersonalName() << "-" << index_->GetRankName() << "-" << index_->GetPersonalTelephoneNumber() << "-"
		<< index_->GetEmailAddress() << "-" << index_->GetCompanyName() << "-" << index_->GetAddress() << "-" <<
		index_->GetCompanyTelphoneNumber() << "-" << index_->GetFaxNumber() << "-" << 
		index_->GetEmailAddress() << endl;
#endif
	//ȫ�浿 ���βȱ�
	i = index.PutIn(index_);



	//���浿 ���Բȱ�
	BusinessCard businessCard2("���浿", "����", "0102222", "seo@", "�Ｚ", "�����", "02333", "02333",
		"www.samsung@");

	index_ = businessCardBinder.PutIn(businessCard2);

	//���浿 ���βȱ�
	i = index.PutIn(index_);



	//��浿 ���Բȱ�
	BusinessCard businessCard3("��浿", "����", "0103333", "kim@", "�Ｚ", "�����", "02333", "02333",
		"www.samsung@");

	index_ = businessCardBinder.PutIn(businessCard3);

	//��浿 ���βȱ�
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
	//����ö���� �̴´�.
	businessCardBinder.PutOut(index_);

	//���ο��� ���°�� ���� ������ �ִ��� ã�´�.
	i = index.Find(index_);
	cout << i << endl;
	
	//���ο��� �̴´�. (���°���� �˾ƾ� ���� �� ����)
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
	cout << "�����ϱ�" << endl << endl;
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
















