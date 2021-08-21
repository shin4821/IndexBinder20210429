//BinaryTree.h
#ifndef _BINARYTREE_H
#define _BINARYTREE_H
typedef signed long int Long;

template<typename T>
class BinaryTree {
public:
	class Node { //Nested Class
		friend class BinaryTree;
	    public:
			Node() {
				this->left = 0;
				this->right = 0;
			}
			Node(T key) {
				this->left = 0;
				this->key = key;
				this->right = 0;
			}
			void MakeKeys(T(*keys), Long* count, Long* i) {
				if (this->left != 0) {
					this->left->MakeKeys(keys, count, i);
				}

				keys[*i] = this->key;
				(*count)++;
				(*i)++;

				if(this->right != 0) {
					this->right->MakeKeys(keys, count, i);
				}
			}

			void DeleteAllItems() {
				if (this->left != 0) {
					this->left->DeleteAllItems();
				}
				if (this->right != 0) {
					this->right->DeleteAllItems();
				}
				if (this != 0) {
					this->left = 0;
					this->right = 0;
					delete this;
				}
			}
			static Node* MakeTree(T(*keys), Long low, Long high) {
				Long middle;
				Node* node=0;

				//1. keys, low, high를 읽는다.
				if (low <= high) {
					middle = (low + high) / 2;
					node = new Node;
					node->key = keys[middle];

					node->left = MakeTree(keys, low, middle - 1);
					node->right = MakeTree(keys, middle + 1, high);
				}
				return node;
			}

			Node& operator=(const Node& other) {//치환연산자
				this->key = other.key;

				if (other.left != 0) {
					this->left = new Node;
					*this->left = *other.left;
				}
				if (other.right != 0) {
					this->right = new Node;
					*this->right = *other.right;
				}
				return *this;
			}
			Node(const Node& source) {//복사생성자
				this->left = 0;
				this->key = source.key;
				this->right = 0;

				if (source.left != 0) {
					this->left = new Node(*source.left);
				}
				if (source.right != 0) {
					this->right = new Node(*source.right);
				}
			}
			~Node() {
				//1. 노드를 읽는다.
				if (this->left != 0) {
					delete this->left;
				}
				if (this->right != 0) {
					delete this->right;
				}
			}

			T& GetKey() const {
				return const_cast<T&>(this->key);
			}
			Node* GetLeft() const {
				return const_cast<Node*>(this->left);
			}
			Node* GetRight() const {
				return const_cast<Node*>(this->right);
			}
	        private:
				Node* left;
				T key;
				Node* right;
	};

public:
	BinaryTree();
	~BinaryTree();
	BinaryTree(const BinaryTree& source); //복사생성자
	BinaryTree& operator=(const BinaryTree& other); //치환연산자
	Node* Insert(T key, int(*compare)(void*, void*));
	Node* Search(T key, int(*compare)(void*, void*));
	Node* Delete(T key, int(*compare)(void*, void*));
	void MakeKeys(T* (*keys), Long* count);
	void MakeBalance();
	void DeleteAllItems();
	void MakeTree(T(*keys), Long count);
	Node* GetRoot() const;
	Long GetLength() const;
	Long GetBalance() const;
	T& GetAt(Node* index);

private:
	Node* root;
	Long length;
	Long balance;
};

template<typename T>
BinaryTree<T>::BinaryTree() {
	this->root = 0;
	this->length = 0;
	this->balance = 0;
}

template<typename T>
BinaryTree<T>::~BinaryTree() {
	delete this->root;
}

template<typename T>
typename BinaryTree<T>::Node* BinaryTree<T>::Insert(T key, int(*compare)(void*, void*)) {
	Node* index;
	Node* node;
	Node* parent=0;

	//1. 노드를 생성한다.
	index = new Node(key);
	node = this->root;

	//2. 부모를 찾는다.
	while (node != 0) {
		parent = node;

		if (compare(&node->key, &key) > 0) {
			node = node->left;
		}
		else {
			node = node->right;
		}
	}

	//3. 부모와의 관계를 짓는다.
	if (parent != 0) {
		if (compare(&parent->key, &key) > 0) {
			parent->left = index;
		}
		else {
			parent->right = index;
		}
	}
	else {
		this->root = index;
	}

	//4. 개수를 증가시킨다.
	this->length++;

	//5. balance를 맞춘다.
	if (compare(&this->root->key, &key) > 0) {
		this->balance--;
	}
	else if (compare(&this->root->key, &key) < 0) {
		this->balance++;
	}

	return index;
}

template<typename T>
typename BinaryTree<T>::Node* BinaryTree<T>::Search(T key, int(*compare)(void*, void*)) {
	Node* it;
	Node* index = 0;

	//1. key를 읽는다.
	it = this->root;

	//2. 노드의 끝이 아니고 key가 아닌동안 반복한다.
	while (it != 0 && compare(&it->key, &key) != 0) {
		if (compare(&it->key, &key) > 0) {
			it = it->left;
		}
		else {
			it = it->right;
		}
	}
	index = it;

	//3. index를 출력한다.
	return index;
}

template<typename T>
typename BinaryTree<T>::Node* BinaryTree<T>::Delete(T key, int(*compare)(void*, void*)) {
	Node* index;
	Node* parent = 0;
	Node* replace = 0;
	Node* replaceKid = 0;
	Node* replaceParent = 0;
	Node* it=0;
	T rootKey;
	Node* right;

	//1. key를 읽는다
	//2. key와 parent의 위치를 찾는다.
	it = this->root;
	while (compare(&it->key, &key) != 0) {
		parent = it;
		if (compare(&it->key, &key) > 0) {
			it = it->left;
		}
		else {
			it = it->right;
		}
	}
	//3. 오른쪽에서 먼저 replace를 찾는다.
	index = it;
	right = index->right;
	it = index->right;
	while (it != 0) {
		replace = it;
		it = it->left;
	}

	//4. 오른쪽 replace없으면 왼쪽에서 찾아준다.
	if (right == 0) {
		it = index->left;
		while (it != 0) {
			replace = it;
			it = it->right;
		}
	}

	//5. replaceParent 위치를 찾는다.
	it = index;
	while (replace != 0 && it != replace) {
		replaceParent = it;

		if (compare(&it->key, &replace->key) > 0) {
			it = it->left;
		}
		else {
			it = it->right;
		}
	}
	//6. replaceParent의 관계 설정한다.
	if (replace != 0 && right != 0 && replaceParent != index) {
		replaceKid = replace->right;
		replaceParent->left = replaceKid;
	}
	else if (replace != 0 && right == 0 && replaceParent != index) {
		replaceKid = replace->left;
		replaceParent->right = replaceKid;
	}
	//7. replace의 관계 설정한다.
	if (replace != 0 && right != 0 && replaceParent != index) {
		replace->left = index->left;
		replace->right = index->right;
	}
	else if (replace != 0 && right == 0 && replaceParent != index) {
		replace->left = index->left;
	}
	else if (replace != 0 && right != 0 && replaceParent == index) {
		replace->left = index->left;
	}
	rootKey = this->root->key;

	//8. parent의 관계설정 한다.
	if (parent != 0) {
		if (compare(&parent->key, &index->key) > 0) {
			parent->left = replace;
		}
		else {
			parent->right = replace;
		}

	}
	else {
		this->root = replace;
	}
	//9. index를 지워준다.
	if (index != 0) {

		index->left = 0;
		index->right = 0;
		delete index;
	}
	this->length--;

	//10. balance를 설정한다.
	if (compare(&rootKey, &key) > 0) {
		this->balance++;
	}
	else if (compare(&rootKey, &key) < 0) {
		this->balance--;
	}
	else if (replace != 0 && compare(&rootKey, &replace->key) > 0) {
		this->balance++;
	}
	else if (replace != 0 && compare(&rootKey, &replace->key) < 0) {
		this->balance--;
	}

	//10. index(=null)를 출력한다.
	return index;
}


template<typename T>
void BinaryTree<T>::MakeKeys(T* (*keys), Long* count) {
	Long i = 0;
	(*count) = 0;
	*keys = new T[this->length];

	this->root->MakeKeys(*keys, count, &i);	
}


template<typename T>
void BinaryTree<T>::MakeBalance() {
	Long count;
	T(*keys);

	MakeKeys(&keys, &count);
	DeleteAllItems();
	MakeTree(keys, count);

}


template<typename T>
void BinaryTree<T>::DeleteAllItems() {
	this->root->DeleteAllItems();

	this->root = 0;
	this->length = 0;
	this->balance = 0;
}

template<typename T>
void BinaryTree<T>::MakeTree(T(*keys), Long count) {
	Node* node;
	Long low = 0;
	Long high = count-1;
	Long remainder;

	node = this->root->MakeTree(keys, low, high);
	this->root = node;
	this->length = count;

	remainder = count - count / 2 * 2;
	if (remainder >= 1) {
		this->balance = 0;
	}
	else {
		this->balance = 1;
	}


}


template<typename T>
BinaryTree<T>::BinaryTree(const BinaryTree& source) {//복사생성자
	this->root = new Node(*source.root);

	this->length = source.length;
	this->balance = source.balance;

}

template<typename T>
BinaryTree<T>& BinaryTree<T>::operator=(const BinaryTree& other) {//치환연산자

	delete this->root;
	
	this->root = new Node;
	*this->root = *other.root;

	this->length = other.length;
	this->balance = other.balance;

	return *this;
}



template<typename T>
T& BinaryTree<T>::GetAt(Node* index) {
	return const_cast<T&>(index->key);
}

template<typename T>
inline typename BinaryTree<T>::Node* BinaryTree<T>::GetRoot() const {
	return const_cast<Node*>(this->root);
}

template<typename T>
inline Long BinaryTree<T>::GetLength() const {
	return this->length;
}

template<typename T>
inline Long BinaryTree<T>::GetBalance() const {
	return this->balance;
}



#endif//_BINARYTREE_H


