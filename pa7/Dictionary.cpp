/***
* Rahul Amudhasgaran
* ramudhas
* 2023 Fall CSE101 PA{7}
* Dictionary.cpp
* Dictionary ADT Program
***/ 

#include"Dictionary.h"
#include <cstddef>
#include <ostream>
#include <stdexcept>

using namespace std;

// Private Constructor --------------------------------------------------------

    // Node constructor
    Dictionary::Node::Node(keyType k, valType v){
        key = k;
        val = v;
        parent = nullptr;
        right = nullptr;
        left = nullptr;
    }

// Class Constructors & Destructors -------------------------------------------

    // Default Constructor
    Dictionary::Dictionary(){
        nil = nullptr;
        root = nullptr;
        current = nullptr;
        num_pairs = 0;
    }

    // Copy Constructor
    Dictionary::Dictionary(const Dictionary& D){
        nil = nullptr;
        root = nullptr;
        current = D.current;
        num_pairs = 0;

        preOrderCopy(D.root, D.nil);
    }

    // Destructor
    Dictionary::~Dictionary() {
        clear();
    }


// Helper functions -----------------------------------------------------------

    // inOrderString()
    // Appends a string representation of the tree rooted at R to string s. The
    // string appended consists of: "key : value \n" for each key-value pair in
    // tree R, arranged in order by keys.
    void Dictionary::inOrderString(string& s, Node* R) const{
        if (R != nil){
            inOrderString(s, R->left);
            s += R->key + " : " + std::to_string(R->val) + "\n";
            inOrderString(s, R->right);
        }
    }

    // preOrderString()
    // Appends a string representation of the tree rooted at R to s. The appended
    // string consists of keys only, separated by "\n", with the order determined
    // by a pre-order tree walk.
    void Dictionary::preOrderString(string& s, Node* R) const{
        if (R){
            s += R->key + "\n";
            preOrderString(s, R->left);
            preOrderString(s, R->right);
        }
    }

    // preOrderCopy()
    // Recursively inserts a deep copy of the subtree rooted at R into this 
    // Dictionary. Recursion terminates at N.
    void Dictionary::preOrderCopy(Node* R, Node* N){
        if (R && R != N){
            setValue(R->key, R->val);
            preOrderCopy(R->left, N);
            preOrderCopy(R->right, N);
        }
    }

    // postOrderDelete()
    // Deletes all Nodes in the subtree rooted at R.
    void Dictionary::postOrderDelete(Node* R){
        if (R){
            postOrderDelete(R->left);
            postOrderDelete(R->right);
            delete R;
        }
    }

    // search()
    // Searches the subtree rooted at R for a Node with key==k. Returns
    // the address of the Node if it exists, returns nil otherwise.
    Dictionary::Node* Dictionary::search(Node* R, keyType k) const{
        while (R){
            if (k == R->key) return R;
            else if (k < R->key) R = R->left;
            else R = R->right;
        } return nil;
    }

    // findMin()
    // If the subtree rooted at R is not empty, returns a pointer to the 
    // leftmost Node in that subtree, otherwise returns nil.
    Dictionary::Node* Dictionary::findMin(Node* R){
        if (!R) return nil; //if subroot doesn't exist, return nil
        while (R->left) R = R->left; //move node left until node has no more left children
        return R;
    }

    // findMax()
    // If the subtree rooted at R is not empty, returns a pointer to the 
    // rightmost Node in that subtree, otherwise returns nil.
    Dictionary::Node* Dictionary::findMax(Node* R){
        if (!R) return nil; //if subroot doesn't exist, return nil
        while (R->right) R = R->right; //move node right until node has no more right children
        return R;
    }

    // findNext()
    // If N does not point to the rightmost Node, returns a pointer to the
    // Node after N in an in-order tree walk.  If N points to the rightmost 
    // Node, or is nil, returns nil. 
    Dictionary::Node* Dictionary::findNext(Node* N){
        if (N == findMax(root) || N == nil) return nil; //if N is nil or max
        if (N->right) return findMin(N->right); //if N has right child find min of that child
        if (N->parent->left == N) return N->parent; //return parent if N is left child of parent and has no children
        do {N = N->parent;} while (N->parent->right == N); //moves node up until N is left child
        return N->parent;
    }

    // findPrev()
    // If N does not point to the leftmost Node, returns a pointer to the
    // Node before N in an in-order tree walk.  If N points to the leftmost 
    // Node, or is nil, returns nil.
    Dictionary::Node* Dictionary::findPrev(Node* N){
        if (N == findMin(root) || N == nil) return nil; //if N is nil or min
        if (N->left) return findMax(N->left); //if N has left child find max of that child
        if (N->parent->right == N) return N->parent; //return parent if N is right child of parent
        do {N = N->parent;} while (N->parent->left == N); //moves node up until N is right child
        return N->parent;
    }

// Access functions -----------------------------------------------------------

    // size()
    // Returns the size of this Dictionary.
    int Dictionary::size() const{
        return num_pairs;
    }

    // contains()
    // Returns true if there exists a pair such that key==k, and returns false
    // otherwise.
    bool Dictionary::contains(keyType k) const{
        return search(root, k) != nil;
    }

    // getValue()
    // Returns a reference to the value corresponding to key k.
    // Pre: contains(k)
    valType& Dictionary::getValue(keyType k) const{
        return contains(k) ? search(root, k)->val : throw logic_error("Dictionary: getValue(): key " + k + " does not exist");
    }

    // hasCurrent()
    // Returns true if the current iterator is defined, and returns false 
    // otherwise.
    bool Dictionary::hasCurrent() const{
        return (current != nil);
    }

    // currentKey()
    // Returns the current key.
    // Pre: hasCurrent() 
    keyType Dictionary::currentKey() const{
        return hasCurrent() ? current->key : throw logic_error("Dictionary: currentKey(): current undefined");
    }

    // currentVal()
    // Returns a reference to the current value.
    // Pre: hasCurrent()
    valType& Dictionary::currentVal() const{
        return hasCurrent() ? current->val : throw logic_error("Dictionary: currentVal(): current undefined");
    }

// Manipulation procedures -------------------------------------------------

    // clear()
    // Resets this Dictionary to the empty state, containing no pairs.
    void Dictionary::clear(){
        if (size()) postOrderDelete(root);
        nil = nullptr;
        current = nullptr;
        root = nullptr;
        num_pairs = 0;
    }

    // setValue()
    // If a pair with key==k exists, overwrites the corresponding value with v, 
    // otherwise inserts the new pair (k, v).
    void Dictionary::setValue(keyType k, valType v){
        Node* R = root;
        while (R){
            if (k == R->key){
                R->val = v;
                return;
            } else if (k < R->key){
                if (!R->left){
                    Node* N = new Node(k, v);
                    R->left = N;
                    N->parent = R;
                    ++num_pairs;
                    return;
                } R = R->left;
            } else {
                if (!R->right){
                    Node* N = new Node(k, v);
                    R->right = N;
                    N->parent = R;
                    ++num_pairs;
                    return;
                } R = R->right;
            }
        } Node* N = new Node(k, v);
        root = N;
        ++num_pairs;
    }

    // remove()
    // Deletes the pair for which key==k. If that pair is current, then current
    // becomes undefined.
    // Pre: contains(k).
    void Dictionary::remove(keyType k){
        Node* R = root;
        while (R){
            if (k == R->key) break;
            else if (k < R->key) R = R->left;
            else R = R->right;
        } if (!R) throw logic_error("Dictionary: remove(): key " + k + " does not exist");
        if (current == R) current = nullptr;
        if (R->right){
            Node* s = findMin(R->right);
            R->val = s->val;
            R->key = s->key;
            if (s->right){
                s->parent->right = s->right;
                s->right->parent = s->parent;
            } else s->parent->right == s ? s->parent->right = s->left : s->parent->left = s->left;          
            delete s;
        } else if (R->left){
            if (root == R){
                root = R->left;
                R->left->parent = nil;
            } else {
                R->parent->right == R ? R->parent->right = R->left : R->parent->left = R->left;
                R->left->parent = R->parent;
            } delete R;
        } else {
            if (R != root) R->parent->right == R ? R->parent->right = nil : R->parent->left = nil;
            else root = nil;
            delete R;
        } --num_pairs;
    }

    // begin()
    // If non-empty, places current iterator at the first (key, value) pair
    // (as defined by the order operator < on keys), otherwise does nothing. 
    void Dictionary::begin(){
        if (size()) current = findMin(root);
    }

    // end()
    // If non-empty, places current iterator at the last (key, value) pair
    // (as defined by the order operator < on keys), otherwise does nothing. 
    void Dictionary::end(){
        if (size()) current = findMax(root);
    }

    // next()
    // If the current iterator is not at the last pair, advances current 
    // to the next pair (as defined by the order operator < on keys). If 
    // the current iterator is at the last pair, makes current undefined.
    // Pre: hasCurrent()
    void Dictionary::next(){
        if (hasCurrent()) current = findNext(current);
        else {
            current = nullptr;
            throw logic_error("Dictionary: next(): current undefined");
        }
    }

    // prev()
    // If the current iterator is not at the first pair, moves current to  
    // the previous pair (as defined by the order operator < on keys). If 
    // the current iterator is at the first pair, makes current undefined.
    // Pre: hasCurrent()
    void Dictionary::prev(){
        if (hasCurrent()) current = findPrev(current);
        else {
            current = nullptr;
            throw logic_error("Dictionary: prev(): current undefined");
        }
    }

// Other Functions ------------------------------------------------------------

    // to_string()
    // Returns a string representation of this Dictionary. Consecutive (key, value)
    // pairs are separated by a newline "\n" character, and the items key and value 
    // are separated by the sequence space-colon-space " : ". The pairs are arranged 
    // in order, as defined by the order operator <.
    string Dictionary::to_string() const{
        string s = "";
        inOrderString(s, root);
        return s;
    }

    // pre_string()
    // Returns a string consisting of all keys in this Dictionary. Consecutive
    // keys are separated by newline "\n" characters. The key order is given
    // by a pre-order tree walk.
    string Dictionary::pre_string() const{
        string s = "";
        preOrderString(s, root);
        return s;
    }

    // equals()
    // Returns true if and only if this Dictionary contains the same (key, value)
    // pairs as Dictionary D.
    bool Dictionary::equals(const Dictionary& D) const{
        if (to_string() != D.to_string()) return false;
        return true;
    }

// Overloaded Operators --------------------------------------------------------

    // operator<<()
    // Inserts string representation of Dictionary D into stream, as defined by
    // member function to_string().
    ostream& operator<<(ostream& stream, Dictionary& D){
        return stream << D.to_string();
    }

    // operator==()
    // Returns true if and only if Dictionary A equals Dictionary B, as defined
    // by member function equals(). 
    bool operator==(const Dictionary& A, const Dictionary& B){
        return A.equals(B);
    }

    // operator=()
    // Overwrites the state of this Dictionary with state of D, and returns a
    // reference to this Dictionary.
    Dictionary& Dictionary::operator=(const Dictionary& D){
        if (this != &D){
            Dictionary temp = D;

            swap(nil, temp.nil);
            swap(root, temp.root);
            swap(current, temp.current);
            swap(num_pairs, temp.num_pairs);
        }
        return *this;
    }