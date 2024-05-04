#include <iostream>
#include <stdexcept>
#include "Vector.cpp"

template <typename Key, typename Value>
class Map { // Map class implemented based on a tree, written by Dumitru Nirca
private:
  struct Node {
    Key key;
    Value value;
    Node* left;
    Node* right;

    Node(const Key& k, const Value& v) : key(k), value(v), left(nullptr), right(nullptr) {};
  };

  Node* root;

  size_t countNodes (Node* node) const { //Function to count the number of nodes.
    if ( node == nullptr) {
      return 0;
    }
    return 1 + countNodes(node->left) + countNodes(node->right);
  }
  
  void insertHelper (Node*& node, const Key& key, const Value& value) { // Helper function to insert a key-value pair into the tree.
    if (node == nullptr) {
      node = new Node(key,value); // If the current node is null, create a new Node.
    } else if (key < node->key) {
      insertHelper (node->left, key, value); // If the key is less than the current's node key, insert recursively into the left subtree.
    } else if (key > node->key) {
      insertHelper(node->right, key, value); // If the key is greater than the currents node's key, recursively insert into the right subtree.
    } else {
      throw std::invalid_argument("Duplicate key");
    }
  }

  Value& findHelper(Node* node, const Key& key) const { // Helper function to recursively find a value corresponding to a key in the tree.
    if (node == nullptr) {
      throw std::out_of_range("Key not found");
    } else if (key < node->key) {
      return findHelper(node->left, key); // If the key is less than the current's node key, recursively look into the left subtree.
    } else if (key > node->key) {
      return findHelper(node->right, key); // If the key is greater than the current's node key, recursively look into the right subtree.
    } else {
      return node->value;
    }
  }

  Node* leftMostNode (Node* node) const { // Helper function to traverse the tree and find the leftmostnode (node with smallest value)
    if (node == nullptr) {
      return nullptr;
    }
    while (node->left != nullptr) {
      node = node->left;
    }
    return node;
  }

public :
  Map(): root(nullptr) {}

  struct KeyValue {
    Key key;
    Value value;
  };

  struct Iterator {
  private:
    Node* current;
    Vector<Node*> stack; // Vector to keep track of the nodes when incrementing or decrementing the iterator.
  public:
    Iterator (Node* root) : current(root) { // Custom iterator used to traverse the map.
      while (current != nullptr) {
	stack.push_back(current);
	current = current -> left;
      }
    }

    bool operator != (const Iterator& other) const { // Overloaded != operator.
      return current != other.current;
    }
    Iterator& operator++() {
      if (!stack.isEmpty()) {
	current = stack.back();
	stack.pop_back();
	Node* temp = current->right;
	while (temp != nullptr) {
	  stack.push_back(temp);
	  temp = temp->left;
	}
      } else {
	current = nullptr;
      }
      return *this;
    }
    KeyValue operator*() const {
      return {current->key, current->value};
    }
  };

  ~Map () {
    clear(root);
  }

  size_t getSize() const {
    return countNodes(root);
  }
  
  void insert (const Key& key, const Value& value) { // Function to insert a key-value pair into the map.
    insertHelper(root, key, value);
  }

 Value& operator[] (const Key& key) {
    try {
        return findHelper(root, key);
    } catch (const std::out_of_range&) {
        // Key not found, insert a new node with default-initialized value
        insert(key, Value{});  // Assuming Value is default-constructible
        return findHelper(root, key);
    }
}

  Value& find (const Key& key) const {
    return findHelper(root, key);
  }

  Iterator begin() const {
    return Iterator(leftMostNode(root));
  }

  Iterator end() const {
    return Iterator(nullptr);
  }
  void clear (Node* node) { // Function to delete all the nodes, called in the destructor.
    if (node != nullptr) {
      clear(node->left);
      clear(node->right);
      delete node;
    }
  }
};
