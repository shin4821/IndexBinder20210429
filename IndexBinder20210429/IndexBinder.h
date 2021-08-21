//IndexBinder.h
#ifndef _INDEXBINDER_H
#define _INDEXBINDER_H

typedef signed long int Long;
#include"BinaryTree.h"
#include"Index.h"
#include<string>
using namespace std;

class IndexBinder {
public:
	IndexBinder();
	~IndexBinder();
	Index* PutIn(BusinessCard* current);
	Index* Find(string companyName);
	Index* PutOut(string companyName, BusinessCard* index);
	void Node_Arrange(BinaryTree<Index>::Node* node);
	void Arrange();
	void MakeList(Index* (*indexes), Long* count);
	Long GetBalance();
	BinaryTree<Index>::Node* GetRoot(); // 전체 출력할 때
	Long GetLength() const;

private:
	BinaryTree<Index> indexes;
	Long length;
};

inline Long IndexBinder::GetLength() const {
	return this->length;
}

int CompareCompanyName(void* one, void* other);


#endif//_INDEXBINDER_H