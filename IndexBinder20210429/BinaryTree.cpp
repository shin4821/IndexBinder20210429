//BinaryTree.cpp
#include"BinaryTree.h"
#include<iostream>
using namespace std;

#if 0

int Compare(void* one, void* other);
void Print(BinaryTree<int>::Node* node);

int main(int argc, char* argv[]) {
	BinaryTree<int> binaryTree;
	BinaryTree<int>::Node* node;
	int(*keys);
	Long count;
	Long i = 0;
	int key;


	node = binaryTree.Insert(100, Compare);
	cout << node->GetKey() << endl;

	node = binaryTree.Insert(90, Compare);
	cout << node->GetKey() << endl;

	node = binaryTree.Insert(80, Compare);
	cout << node->GetKey() << endl;

	node = binaryTree.Insert(150, Compare);
	cout << node->GetKey() << endl;

	node = binaryTree.Insert(95, Compare);
	cout << node->GetKey() << endl;

	node = binaryTree.Insert(200, Compare);
	cout << node->GetKey() << endl;

	cout << "전체출력" << endl;
	Print(binaryTree.GetRoot());

	cout << "Search:80" << endl;

	node = binaryTree.Search(80, Compare);
	cout << node->GetKey() << endl;

#if 0
	cout << "Delete 시작" << endl;
	cout << "Delete 150" << endl;
	binaryTree.Delete(150, Compare);
	Print(binaryTree.GetRoot());

	cout << "Delete 90" << endl;
	binaryTree.Delete(90, Compare);
	Print(binaryTree.GetRoot());

	cout << "Delete 100" << endl;
	binaryTree.Delete(100, Compare);
	Print(binaryTree.GetRoot());

	cout << "Delete 95" << endl;
	binaryTree.Delete(95, Compare);
	Print(binaryTree.GetRoot());

	cout << "Delete 200" << endl;
	binaryTree.Delete(200, Compare);
	Print(binaryTree.GetRoot());

	cout << "Delete 80" << endl;
	binaryTree.Delete(80, Compare);
	Print(binaryTree.GetRoot());
#endif


	cout << "MakeBalance" << endl;
	binaryTree.MakeBalance();
	Print(binaryTree.GetRoot());
	cout << "length:" <<binaryTree.GetLength()<< endl;
	cout << "balance:" << binaryTree.GetBalance() << endl;


	cout << "binaryTree2 전체" << endl;
	BinaryTree<int> binaryTree2;

	node = binaryTree2.Insert(10, Compare);

	node = binaryTree2.Insert(9, Compare);

	node = binaryTree2.Insert(8, Compare);

	node = binaryTree2.Insert(7, Compare);

	node = binaryTree2.Insert(6, Compare);

	node = binaryTree2.Insert(5, Compare);

	node = binaryTree2.Insert(4, Compare);

	node = binaryTree2.Insert(3, Compare);
	Print(binaryTree2.GetRoot());
	cout << "length:" << binaryTree2.GetLength() << endl;
	cout << "balance:" << binaryTree2.GetBalance() << endl;

	cout << "binaryTree2 : MakeBalance" << endl;
	binaryTree2.MakeBalance();
	Print(binaryTree2.GetRoot());
	cout << "length:" << binaryTree2.GetLength() << endl;
	cout << "balance:" << binaryTree2.GetBalance() << endl;

	BinaryTree<int> binaryTree3(binaryTree2);
	cout << "binaryTree3 : 복사생성자" << endl;
	Print(binaryTree3.GetRoot());
	cout << "length:" << binaryTree2.GetLength() << endl;
	cout << "balance:" << binaryTree2.GetBalance() << endl;

	binaryTree3 = binaryTree;
	cout << "binaryTree3 : 치환연산자" << endl;
	Print(binaryTree3.GetRoot());
	cout << "length:" << binaryTree3.GetLength() << endl;
	cout << "balance:" << binaryTree3.GetBalance() << endl;

#if 0
	cout << "MakeKeys" << endl;
	binaryTree.MakeKeys(&keys, &count);

	while (i < count) {
		cout << keys[i] << endl;
		i++;
	}

	cout << "DeleteAllItems" << endl;
	binaryTree.DeleteAllItems();
	Print(binaryTree.GetRoot());


	cout << "MakeTree" << endl;
	binaryTree.MakeTree(keys, count);
	Print(binaryTree.GetRoot());

	cout << binaryTree.GetLength() << endl;
	cout << binaryTree.GetBalance() << endl;
#endif


#if 0
	cout << "binaryTree1: 복사생성자 출력 " << endl;
	BinaryTree<int> binaryTree1(binaryTree);
	Print(binaryTree1.GetRoot());

	cout << "binaryTree2: 치환연산자 전용 " << endl;
	BinaryTree<int> binaryTree2;

	node = binaryTree2.Insert(7, Compare);

	node = binaryTree2.Insert(1, Compare);

	node = binaryTree2.Insert(2, Compare);

	node = binaryTree2.Insert(10, Compare);

	Print(binaryTree2.GetRoot());

	cout << "binaryTree 치환 후 " << endl;
	binaryTree1 = binaryTree2;
	Print(binaryTree1.GetRoot());
#endif

	return 0;
}



int Compare(void* one, void* other) {
	int* one_ = (int*)one;
	int* other_ = (int*)other;
	int ret;

	if(*one_ == *other_) {
		ret = 0;
	}
	else if (*one_ > * other_) {
		ret = 1;
	}
	else if (*one_ < *other_) {
		ret = -1;
	}
	return ret;
}

void Print(BinaryTree<int>::Node* node) {
	if (node != 0) {
		Print(node->GetLeft());
		cout << node->GetKey() << endl;
		Print(node->GetRight());
	}
}


#endif



