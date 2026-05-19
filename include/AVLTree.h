#pragma once
#include <stdexcept>
#include <algorithm>

template<typename K, typename V>
struct Node {
	K key;
	V value;
	int height;
	Node* left;
	Node* right;
	Node(const K& _key) :key(_key), value(V()), height(1), left(nullptr), right(nullptr) {}
	Node(const K& _key, const V& _value) :key(_key), value(_value), height(1), left(nullptr), right(nullptr) {}
	~Node() {}
};

template<typename K, typename V>
class AVLTree {
	Node<K, V>* root;
private:
	Node<K, V>* insert(Node<K, V>* curr, const K& _key, const V& _value) {
		if (!curr) return new Node<K, V>(_key, _value);
		if (_key < curr->key) {
			curr->left = insert(curr->left, _key, _value);
		}
		else if (_key > curr->key) {
			curr->right = insert(curr->right, _key, _value);
		}
		else {
			curr->value = _value;
		}
		return balance(curr);
	}

	Node<K, V>* findMin(Node<K, V>* curr) {
		if (!curr) return NULL;
		if (curr->left) {
			return findMin(curr->left);
		}
		return curr;
	}

	Node<K, V>* removeMin(Node<K, V>* curr) {
		if (!curr) return NULL;
		if (curr->left == 0) {
			return curr->right;
		}
		curr->left = removeMin(curr->left);
		return balance(curr);
	}

	Node<K, V>* remove(Node<K, V>* curr, const K& _key) {
		if (!curr) return 0;
		if (_key < curr->key) {
			curr->left = remove(curr->left, _key);
		}
		else if (_key > curr->key) {
			curr->right = remove(curr->right, _key);
		}
		else {
			Node<K, V>* q = curr->left;
			Node<K, V>* r = curr->right;
			delete curr;
			if (!r) return q;
			Node<K, V>* min = findMin(r);
			min->right = removeMin(r);
			min->left = q;
			return balance(min);
		}
		return balance(curr);
	}

	Node<K, V>* copy(Node<K, V>* other) {
		if (!other) return nullptr;
		Node<K, V>* res = new Node(other->key);
		res->value = other->value;
		res->left = copy(other->left);
		res->right = copy(other->right);
		return res;
	}

	Node<K, V>* rotateSmallLeft(Node<K, V>* curr) {
		Node<K, V>* y = curr->right;
		Node<K, V>* t = y->left;

		curr->right = t;
		y->left = curr;
		updateHeight(curr);
		updateHeight(y);
		return y;
	}

	Node<K, V>* rotateSmallRight(Node<K, V>* curr) {
		Node<K, V>* x = curr->left;
		Node<K, V>* t = x->right;

		x->right = curr;
		curr->left = t;
		updateHeight(curr);
		updateHeight(x);
		return x;
	}

	Node<K, V>* balance(Node<K, V>* curr) {
		updateHeight(curr);
		if (getDiff(curr) == 2) {
			if (getDiff(curr->right) < 0) {
				curr->right = rotateSmallRight(curr->right);
			}
			return rotateSmallLeft(curr);
		}
		else if (getDiff(curr) == -2) {
			if (getDiff(curr->left) > 0) {
				curr->left = rotateSmallLeft(curr->left);
			}
			return rotateSmallRight(curr);
		}
		return curr;
	}

	int getHeight(Node<K, V>* curr) {
		if (curr == 0) return 0;
		int hl = 0;
		int hr = 0;
		if (curr->left != 0) {
			hl = curr->left->height;
		}
		if (curr->right != 0) {
			hr = curr->right->height;
		}
		return (std::max(hl, hr)) + 1;
	}

	int updateHeight(Node<K, V>* curr) {
		if (!curr) return 0;
		int lh = 0, rh = 0;
		if (curr->left) {
			lh = curr->left->height;
		}
		if (curr->right) {
			rh = curr->right->height;
		}
		curr->height = std::max(rh, lh) + 1;
		return curr->height;
	}

	int getDiff(Node<K, V>* curr) {
		if (!curr) return 0;
		return updateHeight(curr->right) - updateHeight(curr->left);
	}

	void clear(Node<K, V>* curr) {
		if (!curr) return;
		clear(curr->left);
		clear(curr->right);
		delete curr;
	}

	void printInOrder(Node<K, V>* node) const {
		if (node) {
			printInOrder(node->left);
			std::cout << node->key << " = " << node->value << std::endl;
			printInOrder(node->right);
		}
	}

public:
	AVLTree() { root = nullptr; }

	void insert(const K& _key, const V& _value) {
		root = insert(root, _key, _value);
	}

	bool isFound(const K& _key) {
		if (!root) return false;
		Node<K, V>* t = root;
		while (t) {
			if (t->key == _key) return true;
			else if (t->key > _key) {
				t = t->left;
			}
			else {
				t = t->right;
			}
		}
		return false;
	}

	V* find(const K& _key) {
		if (!root) return nullptr;
		Node<K, V>* t = root;
		while (t) {
			if (t->key == _key) return &t->value;
			else if (t->key > _key) {
				t = t->left;
			}
			else {
				t = t->right;
			}
		}
		return nullptr;
	}

	AVLTree(const AVLTree<K, V>& other) {
		root = copy(other.root);
	}
	void delKey(const K& key) { root = remove(root, key); }

	~AVLTree() { clear(root); }

	void printInOrder() const {
		printInOrder(root);
	}

	void clear() {
		clear(root);
		root = nullptr;
	}

};