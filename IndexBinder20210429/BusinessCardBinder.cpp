//BusinessCardBinder.cpp
#include"BusinessCardBinder.h"

BusinessCardBinder::BusinessCardBinder() {
	this->length = 0;
	this->current = 0;
}

BusinessCardBinder::~BusinessCardBinder() {
}

BusinessCardBinder::BusinessCardBinder(const BusinessCardBinder& source)
	:businessCards(source.businessCards) {
	this->length = source.length;
	this->current = &this->businessCards.GetCurrent()->GetObject();
}

BusinessCardBinder& BusinessCardBinder::operator=(const BusinessCardBinder& source) {
	this->businessCards = source.businessCards;
	this->length = source.length;
	this->current = &this->businessCards.GetCurrent()->GetObject();

	return *this;
}

BusinessCard* BusinessCardBinder::PutIn(BusinessCard businessCard) {
	LinkedList<BusinessCard>::Node* node;

	node = this->businessCards.AppendFromTail(businessCard);
	this->length++;
	this->current = &node->GetObject();

	return this->current;
}

void BusinessCardBinder::Find(string name, BusinessCard** (*indexes), Long* count) {
	LinkedList<BusinessCard>::Node* (*nodes);
	Long i = 0;

	this->businessCards.LinearSearchDuplicate(&name, &nodes, count, CompareNames);
	*indexes = new BusinessCard * [this->length];

	while (i < *count) {
		(*indexes)[i] = &nodes[i]->GetObject();
		i++;
	}
	if (nodes != 0) {
		delete[] nodes;
	}
}

BusinessCard* BusinessCardBinder::FindCompanyName(string companyName) {
	LinkedList<BusinessCard>::Node* node;
	BusinessCard* index = 0;

	node = this->businessCards.LinearSearchUnique(&companyName, CompareCompanyNames);

	if (node != 0) {
		index = &node->GetObject();
	}
	return index;
}

BusinessCard BusinessCardBinder::PutOut(BusinessCard* index) {
	LinkedList<BusinessCard>::Node* node;
	BusinessCard businessCard;

	node = this->businessCards.LinearSearchUnique(index, CompareBusinessCards);
	businessCard = node->GetObject();
	this->businessCards.Delete(node);
	this->length--;

	if (this->businessCards.GetCurrent() != 0) {
		this->current = &this->businessCards.GetCurrent()->GetObject();
	}
	else {//���� �̰��� ������ current�� 0ó�� �������.
		this->current = 0;
	}
	
	return businessCard;
}

BusinessCard* BusinessCardBinder::Move(BusinessCard* index) {
	LinkedList<BusinessCard>::Node* node;

	node = this->businessCards.LinearSearchUnique(index, CompareBusinessCards);
	this->current = &node->GetObject();

	return this->current;
}

BusinessCard* BusinessCardBinder::First() {
	LinkedList<BusinessCard>::Node* node;

	node = this->businessCards.First();
	this->current = &node->GetObject();

	return this->current;
}

BusinessCard* BusinessCardBinder::Previous() {
	LinkedList<BusinessCard>::Node* node;

	node = this->businessCards.Previous();
	this->current = &node->GetObject();

	return this->current;
}

BusinessCard* BusinessCardBinder::Next() {
	LinkedList<BusinessCard>::Node* node;
	
	node = this->businessCards.Next();
	this->current = &node->GetObject();

	return this->current;
}

BusinessCard* BusinessCardBinder::Last() {
	LinkedList<BusinessCard>::Node* node;

	node = this->businessCards.Last();
	this->current = &node->GetObject();

	return this->current;
}

int CompareNames(void* one, void* other) {
	BusinessCard* one_ = (BusinessCard*)one;
	string* other_ = (string*)other;
	int ret;

	ret = one_->GetPersonalName().compare(*other_);

	return ret;
}

int CompareCompanyNames(void* one, void* other) {
	BusinessCard* one_ = (BusinessCard*)one;
	string* other_ = (string*)other;
	int ret;

	ret = one_->GetCompanyName().compare(*other_);

	return ret;
}

int CompareBusinessCards(void* one, void* other){
	BusinessCard* one_ = (BusinessCard*)one;
	BusinessCard* other_ = (BusinessCard*)other;
	int ret;

	if (one_ == other_) {
		ret = 0;
	}
	else {
		ret = -1;
	}
	return ret;
}

#if 0

#include<iostream>
using namespace std;
int main(int argc, char* argv[]) {
	BusinessCardBinder businessCardBinder;
	BusinessCard* index;
	BusinessCard* (*indexes);
	BusinessCard* previous = 0;
	BusinessCard businessCard;
	Long count;
	Long i = 0;

	BusinessCard one("�赵��", "����", "0101111", "kim@", "����", "�����", "02333", "02333", "www.hyundai@");
	index = businessCardBinder.PutIn(one);
	cout << index->GetPersonalName() << "-" << index->GetRankName() << "-" << index->GetPersonalTelephoneNumber() 
		<< "-" <<index->GetEmailAddress() << "-" << index->GetCompanyName() << "-" << index->GetAddress() << "-" 
		<<index->GetCompanyTelphoneNumber() << "-" << index->GetFaxNumber() << "-" << index->GetUrl() << endl;

	BusinessCard two("��ȿ��", "�븮", "0102222", "hs@", "�Ｚ", "�����", "02344", "02344", "www.samsung@");
	index = businessCardBinder.PutIn(two);
	cout << index->GetPersonalName() << "-" << index->GetRankName() << "-" << index->GetPersonalTelephoneNumber()
		<< "-" << index->GetEmailAddress() << "-" << index->GetCompanyName() << "-" << index->GetAddress() << "-"
		<< index->GetCompanyTelphoneNumber() << "-" << index->GetFaxNumber() << "-" << index->GetUrl() << endl;

	BusinessCard three("�赵��", "�̻�", "0103333", "do@", "����", "�����", "02355", "02355", "www.lg@");
	index = businessCardBinder.PutIn(three);
	cout << index->GetPersonalName() << "-" << index->GetRankName() << "-" << index->GetPersonalTelephoneNumber()
		<< "-" << index->GetEmailAddress() << "-" << index->GetCompanyName() << "-" << index->GetAddress() << "-"
		<< index->GetCompanyTelphoneNumber() << "-" << index->GetFaxNumber() << "-" << index->GetUrl() << endl;

	BusinessCard four("����ȫ", "����", "0104444", "hong@", "�Ｚ", "�����", "02344", "02344", "www.samsung@");
	index = businessCardBinder.PutIn(four);
	cout << index->GetPersonalName() << "-" << index->GetRankName() << "-" << index->GetPersonalTelephoneNumber()
		<< "-" << index->GetEmailAddress() << "-" << index->GetCompanyName() << "-" << index->GetAddress() << "-"
		<< index->GetCompanyTelphoneNumber() << "-" << index->GetFaxNumber() << "-" << index->GetUrl() << endl;


	businessCardBinder.Find("�赵��", &indexes, &count);
	while (i < count) {
		cout << indexes[i]->GetPersonalName() << "-" << indexes[i]->GetRankName() << "-" << 
			indexes[i]->GetPersonalTelephoneNumber()<< "-" << indexes[i]->GetEmailAddress() << "-" << 
			indexes[i]->GetCompanyName() << "-" << indexes[i]->GetAddress() << "-"<< 
			indexes[i]->GetCompanyTelphoneNumber() << "-" << indexes[i]->GetFaxNumber() << "-" << 
			indexes[i]->GetUrl() << endl;
		i++;
	}

	if (indexes != 0) {
		delete[] indexes;
	}

	cout << "�Ｚ" << endl;

	index = businessCardBinder.FindCompanyName("�Ｚ");
	cout << index->GetPersonalName() << "-" << index->GetRankName() << "-" << index->GetPersonalTelephoneNumber()
		<< "-" << index->GetEmailAddress() << "-" << index->GetCompanyName() << "-" << index->GetAddress() << "-"
		<< index->GetCompanyTelphoneNumber() << "-" << index->GetFaxNumber() << "-" << index->GetUrl() << endl;


	cout << "���� ����" << endl;

	businessCardBinder.First();
	index = businessCardBinder.Next();
	businessCard = businessCardBinder.PutOut(index);

	cout << businessCard.GetPersonalName() << "-" << businessCard.GetRankName() << "-" << 
		businessCard.GetPersonalTelephoneNumber()<< "-" << businessCard.GetEmailAddress() << "-" << 
		businessCard.GetCompanyName() << "-" << businessCard.GetAddress() << "-"
		<< businessCard.GetCompanyTelphoneNumber() << "-" << businessCard.GetFaxNumber() << "-" << 
		businessCard.GetUrl() << endl;

	cout << "��ü ����" << endl;
	index = businessCardBinder.First();
	while (index != previous) {
		cout << index->GetPersonalName() << "-" << index->GetRankName() << "-" << 
			index->GetPersonalTelephoneNumber()<< "-" << index->GetEmailAddress() << "-" << 
			index->GetCompanyName() << "-" << index->GetAddress() << "-"<< index->GetCompanyTelphoneNumber() 
			<< "-" << index->GetFaxNumber() << "-" << index->GetUrl() << endl;

		previous = index;
		index = businessCardBinder.Next();
	}

	BusinessCardBinder other_;
	BusinessCard one_("��浿", "����", "0101111", "kim@", "�Ｚ", "�����", "02333", "02333", "www.samsung@");
	index = other_.PutIn(one_);
	cout << index->GetPersonalName() << "-" << index->GetRankName() << "-" << index->GetPersonalTelephoneNumber()
		<< "-" << index->GetEmailAddress() << "-" << index->GetCompanyName() << "-" << index->GetAddress() << "-"
		<< index->GetCompanyTelphoneNumber() << "-" << index->GetFaxNumber() << "-" << index->GetUrl() << endl;

	BusinessCard two_("�ֱ浿", "�븮", "0102222", "choi@", "����", "�����", "02444", "02444", "www.lg@");
	index = other_.PutIn(two_);
	cout << index->GetPersonalName() << "-" << index->GetRankName() << "-" << index->GetPersonalTelephoneNumber()
		<< "-" << index->GetEmailAddress() << "-" << index->GetCompanyName() << "-" << index->GetAddress() << "-"
		<< index->GetCompanyTelphoneNumber() << "-" << index->GetFaxNumber() << "-" << index->GetUrl() << endl;

	cout << "other ��ü" << endl;

	businessCardBinder = other_;
	index = businessCardBinder.First();
	while (index != previous) {
		cout << index->GetPersonalName() << "-" << index->GetRankName() << "-" <<
			index->GetPersonalTelephoneNumber() << "-" << index->GetEmailAddress() << "-" <<
			index->GetCompanyName() << "-" << index->GetAddress() << "-" << index->GetCompanyTelphoneNumber()
			<< "-" << index->GetFaxNumber() << "-" << index->GetUrl() << endl;

		previous = index;
		index = businessCardBinder.Next();
	}

	cout << "current��ġ" << endl;
	index= other_.GetCurrent();
	cout << index->GetPersonalName() << "-" << index->GetRankName() << "-" << index->GetPersonalTelephoneNumber()
		<< "-" << index->GetEmailAddress() << "-" << index->GetCompanyName() << "-" << index->GetAddress() << "-"
		<< index->GetCompanyTelphoneNumber() << "-" << index->GetFaxNumber() << "-" << index->GetUrl() << endl;


	return 0;
}

#endif




















