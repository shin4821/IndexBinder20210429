//IndexBinder.cpp
#include"IndexBinder.h"
#include"BusinessCardBinder.h"

IndexBinder::IndexBinder() {
	this->length = 0;
}

IndexBinder::~IndexBinder() {
}

Index* IndexBinder::PutIn(BusinessCard* current) {
	BinaryTree<Index>::Node* node;

	//1. indexCard�� �����.
	Index indexCard(current->GetCompanyName(), 256);

	//2. ����(index)�� �ִ��� Ȯ���Ѵ�.
	node = this->indexes.Search(indexCard, CompareCompanyName);

	//3. �ش� ������ ���� ���, ������ �����.
	if (node == 0) {
		node = this->indexes.Insert(indexCard, CompareCompanyName);
		this->length++;
	}

	//4. �Է¹��� ������ ���ο� �����.
	node->GetKey().PutIn(current);

	//5. ������ ����Ѵ�.
	return &node->GetKey();
}

Index* IndexBinder::Find(string companyName) {
	BinaryTree<Index>::Node* node;
	Index indexCard(companyName, 256);
	Index* index = 0;

	//1. ������ ã�´�.
	node = this->indexes.Search(indexCard, CompareCompanyName);

	//2. ������ ������ ������ ����Ѵ�.
	if (node != 0) {
		index = &node->GetKey();
	}

	return index;
}


Index* IndexBinder::PutOut(string companyName, BusinessCard* index) {
	BinaryTree<Index>::Node* node;
	Index* index1 = 0;
	Long index_;


	//1. �ش� ������ ���� ������ ã�´�.
	Index indexCard(companyName, 256);
	node = this->indexes.Search(indexCard, CompareCompanyName);

	//2. �ش� ���� ��, �ش� ������ ��ġ�� ã�´�.
	index_ = node->GetKey().Find(index);

	//3. �ش� ��ġ�� ������ ���ο��� �̴´�.
	node->GetKey().PutOut(index_);

	//4. ������ length�� 0�����̸� ������ �̴´�.
	if (node->GetKey().GetLength() <= 0) {
		node = this->indexes.Delete(indexCard, CompareCompanyName);
	}

	//5. �ش� ������ ����Ѵ�.
	if (node != 0) {
		index1 = &node->GetKey();
	}

	return index1;
}

void IndexBinder::Node_Arrange(BinaryTree<Index>::Node* node){
	if (node->GetLeft() != 0) {
		Node_Arrange(node->GetLeft());
	}

	node->GetKey().Arrange();

	if (node->GetRight() != 0) {
		Node_Arrange(node->GetRight());
	}
}

void IndexBinder::Arrange() {
	this->indexes.MakeBalance();
	this->Node_Arrange(this->indexes.GetRoot());
}

void IndexBinder::MakeList(Index* (*indexes), Long* count) {
	this->indexes.MakeKeys(indexes, count);
}


int CompareCompanyName(void* one, void* other) {
	Index* one_ = (Index*)one;
	Index* other_ = (Index*)other;
	int ret;

	if (one_->GetCompanyName() == other_->GetCompanyName()) {
		ret = 0;
	}
	else if (one_->GetCompanyName() > other_->GetCompanyName()) {
		ret = 1;
	}
	else if (one_->GetCompanyName() < other_->GetCompanyName()) {
		ret = -1;
	}
	return ret;
}


BinaryTree<Index>::Node* IndexBinder::GetRoot() {
	
	return this->indexes.GetRoot();
}

Long IndexBinder::GetBalance() {
	return this->indexes.GetBalance();
}


#include<iostream>
using namespace std;

void Print(BinaryTree<Index>::Node* node);

int main(int argc, char* argv[]) {
	IndexBinder indexBinder;
	BusinessCardBinder businessCardBinder;
	Index* index;
	Long i = 0;
	Long j;
	Index(*indexes);
	Long count;
	BusinessCard* index_;
	string companyName;
	BusinessCard* temp;

	//ȫ�浿 ���Բȱ�
	BusinessCard businessCard1("ȫ�浿", "�븮", "0101111", "hong@", "�Ｚ", "�����", "02333", "02333",
		"www.samsung@");

	index_ = businessCardBinder.PutIn(businessCard1);
#if 0
	cout << index_->GetPersonalName() << "-" << index_->GetRankName() << "-" << index_->GetPersonalTelephoneNumber() << "-"
		<< index_->GetEmailAddress() << "-" << index_->GetCompanyName() << "-" << index_->GetAddress() << "-" <<
		index_->GetCompanyTelphoneNumber() << "-" << index_->GetFaxNumber() << "-" <<
		index_->GetEmailAddress() << endl;
#endif
	//ȫ�浿 ���βȱ�
	index = indexBinder.PutIn(index_);
#if 0
	cout << index->GetCompanyName() << endl;
	index_ = index->GetAt(0);
	cout << index_->GetPersonalName() << "-" << index_->GetRankName() << "-" << index_->GetPersonalTelephoneNumber() << "-"
		<< index_->GetEmailAddress() << "-" << index_->GetCompanyName() << "-" << index_->GetAddress() << "-" <<
		index_->GetCompanyTelphoneNumber() << "-" << index_->GetFaxNumber() << "-" <<
		index_->GetEmailAddress() << endl;
#endif

	//���浿 ���Բȱ�
	BusinessCard businessCard2("���浿", "����", "0102222", "seo@", "�Ｚ", "�����", "02333", "02333",
		"www.samsung@");

	index_ = businessCardBinder.PutIn(businessCard2);

	//���浿 ���βȱ�
	index = indexBinder.PutIn(index_);


	//��浿 ���Բȱ�
	BusinessCard businessCard3("��浿", "����", "0103333", "kim@", "�Ｚ", "�����", "02333", "02333",
		"www.samsung@");

	index_ = businessCardBinder.PutIn(businessCard3);

	//��浿 ���βȱ�
	index = indexBinder.PutIn(index_);


	while (i < index->GetLength()) {

		index_ = index->GetAt(i);

		cout << index_->GetPersonalName() << "-" << index_->GetRankName() << "-" << 
			index_->GetPersonalTelephoneNumber() << "-"
			<< index_->GetEmailAddress() << "-" << index_->GetCompanyName() << "-" << index_->GetAddress()
			<< "-" <<index_->GetCompanyTelphoneNumber() << "-" << index_->GetFaxNumber() << "-" <<
			index_->GetEmailAddress() << endl;

		i++;

	}


	//ȫ�浿 ���Բȱ�
	BusinessCard businessCard4("ȫ�浿", "�븮", "0104444", "hong1@", "����", "�����", "02344", "02344",
		"www.lg@");

	index_ = businessCardBinder.PutIn(businessCard4);

	//ȫ�浿 ���βȱ�
	index = indexBinder.PutIn(index_);

	//���浿 ���Բȱ�
	BusinessCard businessCard5("���浿", "����", "0105555", "seo1@", "����", "�����", "02344", "02344",
		"www.lg@");

	index_ = businessCardBinder.PutIn(businessCard5);

	//���浿 ���βȱ�
	index = indexBinder.PutIn(index_);

	//��浿 ���Բȱ�
	BusinessCard businessCard6("��浿", "����", "0106666", "kim1@", "����", "�����", "02344", "02344",
		"www.lg@");

	index_ = businessCardBinder.PutIn(businessCard6);

	//��浿 ���βȱ�
	index = indexBinder.PutIn(index_);

	i = 0;
	while (i < index->GetLength()) {

		index_ = index->GetAt(i);

		cout << index_->GetPersonalName() << "-" << index_->GetRankName() << "-" <<
			index_->GetPersonalTelephoneNumber() << "-"
			<< index_->GetEmailAddress() << "-" << index_->GetCompanyName() << "-" << index_->GetAddress()
			<< "-" << index_->GetCompanyTelphoneNumber() << "-" << index_->GetFaxNumber() << "-" <<
			index_->GetEmailAddress() << endl;

		i++;

	}

	//ȫ�浿 ���Բȱ�
	BusinessCard businessCard7("ȫ�浿", "�븮", "0107777", "hong3@", "����", "�����", "02355", "02355",
		"www.amore@");

	index_ = businessCardBinder.PutIn(businessCard7);

	//ȫ�浿 ���βȱ�
	index = indexBinder.PutIn(index_);

	//���浿 ���Բȱ�
	BusinessCard businessCard8("���浿", "����", "0108888", "seo3@", "����", "�����", "02355", "02355",
		"www.amore@");

	index_ = businessCardBinder.PutIn(businessCard8);

	//���浿 ���βȱ�
	index = indexBinder.PutIn(index_);

	//��浿 ���Բȱ�
	BusinessCard businessCard9("��浿", "����", "0109999", "kim3@", "����", "�����", "02355", "02355",
		"www.amore@");

	index_ = businessCardBinder.PutIn(businessCard9);

	//��浿 ���βȱ�
	index = indexBinder.PutIn(index_);


	//��浿 ���Բȱ�
	BusinessCard businessCard10("��浿", "����", "0101234", "no3@", "�Ｚ", "�����", "02333", "02333",
		"www.samsung@");

	index_ = businessCardBinder.PutIn(businessCard10);

	//��浿 ���βȱ�
	index = indexBinder.PutIn(index_);

	i = 0;
	while (i < index->GetLength()) {

		index_ = index->GetAt(i);

		cout << index_->GetPersonalName() << "-" << index_->GetRankName() << "-" <<
			index_->GetPersonalTelephoneNumber() << "-"
			<< index_->GetEmailAddress() << "-" << index_->GetCompanyName() << "-" << index_->GetAddress()
			<< "-" << index_->GetCompanyTelphoneNumber() << "-" << index_->GetFaxNumber() << "-" <<
			index_->GetEmailAddress() << endl;

		i++;

	}


	cout << "ã�� - �Ｚ" << endl << endl;

	index = indexBinder.Find("�Ｚ");
	cout << index->GetCompanyName() << endl;
	i = 0;
	while (i < index->GetLength()) {

		index_ = index->GetAt(i);

		cout << index_->GetPersonalName() << "-" << index_->GetRankName() << "-" <<
			index_->GetPersonalTelephoneNumber() << "-"
			<< index_->GetEmailAddress() << "-" << index_->GetCompanyName() << "-" << index_->GetAddress()
			<< "-" << index_->GetCompanyTelphoneNumber() << "-" << index_->GetFaxNumber() << "-" <<
			index_->GetEmailAddress() << endl;

		i++;

	}

	cout << "ã�� - ����" << endl << endl;

	index = indexBinder.Find("����");
	cout << index->GetCompanyName() << endl;
	i = 0;
	while (i < index->GetLength()) {

		index_ = index->GetAt(i);

		cout << index_->GetPersonalName() << "-" << index_->GetRankName() << "-" <<
			index_->GetPersonalTelephoneNumber() << "-"
			<< index_->GetEmailAddress() << "-" << index_->GetCompanyName() << "-" << index_->GetAddress()
			<< "-" << index_->GetCompanyTelphoneNumber() << "-" << index_->GetFaxNumber() << "-" <<
			index_->GetEmailAddress() << endl;

		i++;

	}

	cout << "ã�� - ����" << endl << endl;

	index = indexBinder.Find("����");
	cout << index->GetCompanyName() << endl;
	i = 0;
	while (i < index->GetLength()) {

		index_ = index->GetAt(i);

		cout << index_->GetPersonalName() << "-" << index_->GetRankName() << "-" <<
			index_->GetPersonalTelephoneNumber() << "-"
			<< index_->GetEmailAddress() << "-" << index_->GetCompanyName() << "-" << index_->GetAddress()
			<< "-" << index_->GetCompanyTelphoneNumber() << "-" << index_->GetFaxNumber() << "-" <<
			index_->GetEmailAddress() << endl;

		i++;

	}

#if 0
	cout << "�̱�" << endl << endl;
	businessCardBinder.First();
	index_ = businessCardBinder.Next();
	companyName = index_->GetCompanyName();

	//����ö���� ���� �̱�
	businessCardBinder.PutOut(index_);

	//���ο��� �̱�
	index = indexBinder.PutOut(companyName, index_);

	i = 0;
	while (i < index->GetLength()) {

		index_ = index->GetAt(i);

		cout << index_->GetPersonalName() << "-" << index_->GetRankName() << "-" <<
			index_->GetPersonalTelephoneNumber() << "-"
			<< index_->GetEmailAddress() << "-" << index_->GetCompanyName() << "-" << index_->GetAddress()
			<< "-" << index_->GetCompanyTelphoneNumber() << "-" << index_->GetFaxNumber() << "-" <<
			index_->GetEmailAddress() << endl;

		i++;

	}


	cout << "�̱�" << endl << endl;
	index_ = businessCardBinder.Last();

	//����ö���� ���� �̱�
	businessCardBinder.PutOut(index_);

	//���ο��� �̱�
	index = indexBinder.PutOut(companyName, index_);

	i = 0;
	while (i < index->GetLength()) {

		index_ = index->GetAt(i);

		cout << index_->GetPersonalName() << "-" << index_->GetRankName() << "-" <<
			index_->GetPersonalTelephoneNumber() << "-"
			<< index_->GetEmailAddress() << "-" << index_->GetCompanyName() << "-" << index_->GetAddress()
			<< "-" << index_->GetCompanyTelphoneNumber() << "-" << index_->GetFaxNumber() << "-" <<
			index_->GetEmailAddress() << endl;

		i++;

	}


	cout << "�̱�" << endl << endl;
	index_ = businessCardBinder.First();

	//����ö���� ���� �̱�
	businessCardBinder.PutOut(index_);

	//���ο��� �̱�
	index = indexBinder.PutOut(companyName, index_);

	i = 0;
	while (i < index->GetLength()) {

		index_ = index->GetAt(i);

		cout << index_->GetPersonalName() << "-" << index_->GetRankName() << "-" <<
			index_->GetPersonalTelephoneNumber() << "-"
			<< index_->GetEmailAddress() << "-" << index_->GetCompanyName() << "-" << index_->GetAddress()
			<< "-" << index_->GetCompanyTelphoneNumber() << "-" << index_->GetFaxNumber() << "-" <<
			index_->GetEmailAddress() << endl;

		i++;

	}

	cout << "�̱�" << endl << endl;
	index_ = businessCardBinder.First();

	//����ö���� ���� �̱�
	businessCardBinder.PutOut(index_);

	//���ο��� �̱�
	index = indexBinder.PutOut(companyName, index_);

	i = 0;
	while (i < index->GetLength()) {

		index_ = index->GetAt(i);

		cout << index_->GetPersonalName() << "-" << index_->GetRankName() << "-" <<
			index_->GetPersonalTelephoneNumber() << "-"
			<< index_->GetEmailAddress() << "-" << index_->GetCompanyName() << "-" << index_->GetAddress()
			<< "-" << index_->GetCompanyTelphoneNumber() << "-" << index_->GetFaxNumber() << "-" <<
			index_->GetEmailAddress() << endl;

		i++;

	}
#endif
	cout << "�����ϱ� �� balance" << endl << endl;
	cout << indexBinder.GetBalance() << endl;

	indexBinder.Arrange();

	cout << "���� �� balance" << endl << endl;
	cout << indexBinder.GetBalance() << endl;

	cout << "��ü���" << endl << endl;
	Print(indexBinder.GetRoot());
	

	cout << "MakeList ��ü���" << endl;
	indexBinder.MakeList(&indexes, &count);
	i = 0;
	while (i < count) {
		cout << indexes[i].GetCompanyName() << endl;

		j = 0;
		while (j < indexes[i].GetLength()) {
			index_ = indexes[i].GetAt(j);

			cout << index_->GetPersonalName() << "-" << index_->GetRankName() << "-" <<
				index_->GetPersonalTelephoneNumber() << "-"
				<< index_->GetEmailAddress() << "-" << index_->GetCompanyName() << "-" << index_->GetAddress()
				<< "-" << index_->GetCompanyTelphoneNumber() << "-" << index_->GetFaxNumber() << "-" <<
				index_->GetEmailAddress() << endl;

			j++;
		}
		i++;
	}


	return 0;
}

void Print(BinaryTree<Index>::Node* node) {
	Index* index;
	Long i;
	BusinessCard* index_;

	if (node->GetLeft() != 0) {
		Print(node->GetLeft());
	}

	index = &node->GetKey();
	i = 0;
	cout << index->GetCompanyName() << endl;
	while (i < index->GetLength()) {
		index_ = index->GetAt(i);

		cout << index_->GetPersonalName() << "-" << index_->GetRankName() << "-" <<
			index_->GetPersonalTelephoneNumber() << "-"
			<< index_->GetEmailAddress() << "-" << index_->GetCompanyName() << "-" << index_->GetAddress()
			<< "-" << index_->GetCompanyTelphoneNumber() << "-" << index_->GetFaxNumber() << "-" <<
			index_->GetEmailAddress() << endl;

		i++;
	}

	if (node->GetRight() != 0) {
		Print(node->GetRight());
	}
}
















