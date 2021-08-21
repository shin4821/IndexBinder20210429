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

	//1. indexCard를 만든다.
	Index indexCard(current->GetCompanyName(), 256);

	//2. 색인(index)이 있는지 확인한다.
	node = this->indexes.Search(indexCard, CompareCompanyName);

	//3. 해당 색인이 없는 경우, 색인을 끼운다.
	if (node == 0) {
		node = this->indexes.Insert(indexCard, CompareCompanyName);
		this->length++;
	}

	//4. 입력받은 명함을 색인에 끼운다.
	node->GetKey().PutIn(current);

	//5. 색인을 출력한다.
	return &node->GetKey();
}

Index* IndexBinder::Find(string companyName) {
	BinaryTree<Index>::Node* node;
	Index indexCard(companyName, 256);
	Index* index = 0;

	//1. 색인을 찾는다.
	node = this->indexes.Search(indexCard, CompareCompanyName);

	//2. 색인이 있으면 색인을 출력한다.
	if (node != 0) {
		index = &node->GetKey();
	}

	return index;
}


Index* IndexBinder::PutOut(string companyName, BusinessCard* index) {
	BinaryTree<Index>::Node* node;
	Index* index1 = 0;
	Long index_;


	//1. 해당 명함을 뽑을 색인을 찾는다.
	Index indexCard(companyName, 256);
	node = this->indexes.Search(indexCard, CompareCompanyName);

	//2. 해당 색인 상, 해당 명함의 위치를 찾는다.
	index_ = node->GetKey().Find(index);

	//3. 해당 위치의 명함을 색인에서 뽑는다.
	node->GetKey().PutOut(index_);

	//4. 색인의 length가 0이하이면 색인을 뽑는다.
	if (node->GetKey().GetLength() <= 0) {
		node = this->indexes.Delete(indexCard, CompareCompanyName);
	}

	//5. 해당 색인을 출력한다.
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

	//홍길동 명함꽂기
	BusinessCard businessCard1("홍길동", "대리", "0101111", "hong@", "삼성", "서울시", "02333", "02333",
		"www.samsung@");

	index_ = businessCardBinder.PutIn(businessCard1);
#if 0
	cout << index_->GetPersonalName() << "-" << index_->GetRankName() << "-" << index_->GetPersonalTelephoneNumber() << "-"
		<< index_->GetEmailAddress() << "-" << index_->GetCompanyName() << "-" << index_->GetAddress() << "-" <<
		index_->GetCompanyTelphoneNumber() << "-" << index_->GetFaxNumber() << "-" <<
		index_->GetEmailAddress() << endl;
#endif
	//홍길동 색인꽂기
	index = indexBinder.PutIn(index_);
#if 0
	cout << index->GetCompanyName() << endl;
	index_ = index->GetAt(0);
	cout << index_->GetPersonalName() << "-" << index_->GetRankName() << "-" << index_->GetPersonalTelephoneNumber() << "-"
		<< index_->GetEmailAddress() << "-" << index_->GetCompanyName() << "-" << index_->GetAddress() << "-" <<
		index_->GetCompanyTelphoneNumber() << "-" << index_->GetFaxNumber() << "-" <<
		index_->GetEmailAddress() << endl;
#endif

	//서길동 명함꽂기
	BusinessCard businessCard2("서길동", "주임", "0102222", "seo@", "삼성", "서울시", "02333", "02333",
		"www.samsung@");

	index_ = businessCardBinder.PutIn(businessCard2);

	//서길동 색인꽂기
	index = indexBinder.PutIn(index_);


	//김길동 명함꽂기
	BusinessCard businessCard3("김길동", "과장", "0103333", "kim@", "삼성", "서울시", "02333", "02333",
		"www.samsung@");

	index_ = businessCardBinder.PutIn(businessCard3);

	//김길동 색인꽂기
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


	//홍길동 명함꽂기
	BusinessCard businessCard4("홍길동", "대리", "0104444", "hong1@", "엘지", "서울시", "02344", "02344",
		"www.lg@");

	index_ = businessCardBinder.PutIn(businessCard4);

	//홍길동 색인꽂기
	index = indexBinder.PutIn(index_);

	//서길동 명함꽂기
	BusinessCard businessCard5("서길동", "주임", "0105555", "seo1@", "엘지", "서울시", "02344", "02344",
		"www.lg@");

	index_ = businessCardBinder.PutIn(businessCard5);

	//서길동 색인꽂기
	index = indexBinder.PutIn(index_);

	//김길동 명함꽂기
	BusinessCard businessCard6("김길동", "과장", "0106666", "kim1@", "엘지", "서울시", "02344", "02344",
		"www.lg@");

	index_ = businessCardBinder.PutIn(businessCard6);

	//김길동 색인꽂기
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

	//홍길동 명함꽂기
	BusinessCard businessCard7("홍길동", "대리", "0107777", "hong3@", "현대", "서울시", "02355", "02355",
		"www.amore@");

	index_ = businessCardBinder.PutIn(businessCard7);

	//홍길동 색인꽂기
	index = indexBinder.PutIn(index_);

	//서길동 명함꽂기
	BusinessCard businessCard8("서길동", "과장", "0108888", "seo3@", "현대", "서울시", "02355", "02355",
		"www.amore@");

	index_ = businessCardBinder.PutIn(businessCard8);

	//서길동 색인꽂기
	index = indexBinder.PutIn(index_);

	//김길동 명함꽂기
	BusinessCard businessCard9("김길동", "과장", "0109999", "kim3@", "현대", "서울시", "02355", "02355",
		"www.amore@");

	index_ = businessCardBinder.PutIn(businessCard9);

	//김길동 색인꽂기
	index = indexBinder.PutIn(index_);


	//노길동 명함꽂기
	BusinessCard businessCard10("노길동", "과장", "0101234", "no3@", "삼성", "서울시", "02333", "02333",
		"www.samsung@");

	index_ = businessCardBinder.PutIn(businessCard10);

	//노길동 색인꽂기
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


	cout << "찾기 - 삼성" << endl << endl;

	index = indexBinder.Find("삼성");
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

	cout << "찾기 - 엘지" << endl << endl;

	index = indexBinder.Find("엘지");
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

	cout << "찾기 - 현대" << endl << endl;

	index = indexBinder.Find("현대");
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
	cout << "뽑기" << endl << endl;
	businessCardBinder.First();
	index_ = businessCardBinder.Next();
	companyName = index_->GetCompanyName();

	//명함철에서 먼저 뽑기
	businessCardBinder.PutOut(index_);

	//색인에서 뽑기
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


	cout << "뽑기" << endl << endl;
	index_ = businessCardBinder.Last();

	//명함철에서 먼저 뽑기
	businessCardBinder.PutOut(index_);

	//색인에서 뽑기
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


	cout << "뽑기" << endl << endl;
	index_ = businessCardBinder.First();

	//명함철에서 먼저 뽑기
	businessCardBinder.PutOut(index_);

	//색인에서 뽑기
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

	cout << "뽑기" << endl << endl;
	index_ = businessCardBinder.First();

	//명함철에서 먼저 뽑기
	businessCardBinder.PutOut(index_);

	//색인에서 뽑기
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
	cout << "정리하기 전 balance" << endl << endl;
	cout << indexBinder.GetBalance() << endl;

	indexBinder.Arrange();

	cout << "정리 후 balance" << endl << endl;
	cout << indexBinder.GetBalance() << endl;

	cout << "전체출력" << endl << endl;
	Print(indexBinder.GetRoot());
	

	cout << "MakeList 전체출력" << endl;
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
















