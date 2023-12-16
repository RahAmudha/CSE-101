/***
* Rahul Amudhasgaran
* ramudhas
* 2023 Fall CSE101 PA{7}
* Dictionary.cpp
* Dictionary ADT Program
***/ 

#include "Dictionary.h"
#include <cstddef>
#include <ostream>
#include <stdexcept>

using namespace std;

#define RED 0
#define BLACK 1

// Private Constructor --------------------------------------------------------

    // Node constructor
    Dictionary::Node::Node(keyType k, valType v){
        key = k;
        val = v;
        parent = nullptr;
        right = nullptr;
        left = nullptr;
        color = RED;
    }

// Class Constructors & Destructors -------------------------------------------

    // Default Constructor
    Dictionary::Dictionary(){
        nil = new Node("", -1);
        nil->color = BLACK;
        root = nil;
        current = nullptr;
        num_pairs = 0;
    }

    // Copy Constructor
    Dictionary::Dictionary(const Dictionary& D){
        nil = new Node("", -1);
        nil->color = BLACK;
        root = nil;
        current = D.current;
        num_pairs = 0;

        preOrderCopy(D.root, D.nil);
    }

    // Destructor
    Dictionary::~Dictionary() {
        clear();
        root = nullptr;
        delete nil;
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
    // string will consist of keys only, with the order determined by a pre-order
    // tree walk. The keys stored in black Nodes will be appended as "key\n", and
    // the keys stored in red Nodes will be appended as "key (RED)\n". The output 
    // of this function is demonstrated in /Examples/pa8/DictionaryClient-out.  
    void Dictionary::preOrderString(string& s, Node* R) const{
        if (R != nil){
            R->color == RED ? s += R->key + " (RED)\n" : s += R->key + "\n";
            preOrderString(s, R->left);
            preOrderString(s, R->right);
        }
    }

    // preOrderCopy()
    // Recursively inserts a deep copy of the subtree rooted at R into this 
    // Dictionary. Recursion terminates at N.
    void Dictionary::preOrderCopy(Node* R, Node* N){
        if (R != N){
            BST_insert(R->key, R->val, R->color);
            preOrderCopy(R->left, N);
            preOrderCopy(R->right, N);
        }
    }

    // BST_insert()
    // Inserts a copy of the Node *M into this Dictionary. Used by preOrderCopy().
    void Dictionary::BST_insert(keyType k, valType v, int c){
        Node* R = root;
        while (R != nil){
            if (k == R->key){
                R->val = v;
                return;
            } else if (k < R->key){
                if (R->left == nil){
                    Node* N = new Node(k, v);
                    R->left = N;
                    N->parent = R;
                    N->right = nil;
                    N->left = nil;
                    N->color = c;
                    ++num_pairs;
                    return;
                } R = R->left;
            } else {
                if (R->right == nil){
                    Node* N = new Node(k, v);
                    R->right = N;
                    N->parent = R;
                    N->right = nil;
                    N->left = nil;
                    N->color = c;
                    ++num_pairs;
                    return;
                } R = R->right;
            }
        }        
        Node* N = new Node(k, v);
        root = N;
        N->parent = nil;
        N->left = nil;
        N->right = nil;
        N->color = c;
        ++num_pairs;
    }

    // postOrderDelete()
    // Deletes all Nodes in the subtree rooted at R.
    void Dictionary::postOrderDelete(Node* R){
        if (R != nil){
            postOrderDelete(R->left);
            postOrderDelete(R->right);
            delete R;
        }
    }

    // search()
    // Searches the subtree rooted at R for a Node with key==k. Returns
    // the address of the Node if it exists, returns nil otherwise.
    Dictionary::Node* Dictionary::search(Node* R, keyType k) const{
        while (R != nil){
            if (k == R->key) return R;
            else if (k < R->key) R = R->left;
            else R = R->right;
        } return nil;
    }

    // findMin()
    // If the subtree rooted at R is not empty, returns a pointer to the 
    // leftmost Node in that subtree, otherwise returns nil.
    Dictionary::Node* Dictionary::findMin(Node* R){
        if (R == nil) return nil; //if subroot doesn't exist, return nil
        while (R->left != nil) R = R->left; //move node left until node has no more left children
        return R;
    }

    // findMax()
    // If the subtree rooted at R is not empty, returns a pointer to the 
    // rightmost Node in that subtree, otherwise returns nil.
    Dictionary::Node* Dictionary::findMax(Node* R){
        if (R == nil) return nil; //if subroot doesn't exist, return nil
        while (R->right != nil) R = R->right; //move node right until node has no more right children
        return R;
    }

    // findNext()
    // If N does not point to the rightmost Node, returns a pointer to the
    // Node after N in an in-order tree walk.  If N points to the rightmost 
    // Node, or is nil, returns nil. 
    Dictionary::Node* Dictionary::findNext(Node* N){
        if (N == findMax(root) || N == nil) return nil; //if N is nil or max
        if (N->right != nil) return findMin(N->right); //if N has right child find min of that child
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
        if (N->left != nil) return findMax(N->left); //if N has left child find max of that child
        if (N->parent->right == N) return N->parent; //return parent if N is right child of parent
        do {N = N->parent;} while (N->parent->left == N); //moves node up until N is right child
        return N->parent;
    }

// RBT Helper Functions (Optional) -----------------------------------------
   
    // LeftRotate()
    void Dictionary::LeftRotate(Node* N){
        // set y
        Node* rot = N->right;

        // turn y's left subtree into x's right subtree
        N->right = rot->left;

        // not necessary if using sentinal nil node
        if (rot->left != nil) rot->left->parent = N;

        // link y's parent to x
        rot->parent = N->parent;
        if (N->parent == nil) root = rot;
        else if (N == N->parent->left) N->parent->left = rot;
        else N->parent->right = rot;

        // put x on y's left
        rot->left = N;
        N->parent = rot;
    }

    // RightRotate()
    void Dictionary::RightRotate(Node* N){
        // set y
        Node* rot = N->left;

        // turn y's right subtree into N's left subtree
        N->left = rot->right;

        // not necessarry if using sentinal nil node
        if (rot->right != nil) rot->right->parent = N;

        // link rot's parent to N
        rot->parent = N->parent;
        if (N->parent == nil) root = rot;
        else if (N == N->parent->right) N->parent->right = rot;
        else N->parent->left = rot;

        // put N on rot's right
        rot->right = N;
        N->parent = rot;
    }

    // RB_InsertFixUP()
    void Dictionary::RB_InsertFixUp(Node* N){
        Node* diag;
        while (N->parent->color == RED){
            if (N->parent->parent == nil) break;
            if (N->parent == N->parent->parent->left){
                diag = N->parent->parent->right;
                if (diag->color == RED){ //case 1
                    N->parent->color = BLACK;              
                    diag->color = BLACK;                    
                    N->parent->parent->color = RED;   
                    N = N->parent->parent;
                } else {
                    if (N == N->parent->right){ //case 2
                        N = N->parent;                     
                        LeftRotate(N);               
                    } N->parent->color = BLACK; //case 3
                    N->parent->parent->color = RED;     
                    RightRotate(N->parent->parent);    
                }
            } else {
                diag = N->parent->parent->left;
                if (diag->color == RED){ //case 4
                    N->parent->color = BLACK;             
                    diag->color = BLACK;                  
                    N->parent->parent->color = RED;        
                    N = N->parent->parent;                
                } else {
                    if (N == N->parent->left){ //case 5
                        N = N->parent;                  
                        RightRotate(N);              
                    } N->parent->color = BLACK; //case 6
                    N->parent->parent->color = RED;         
                    LeftRotate(N->parent->parent);  
                }   
            }
        } root->color = BLACK;
    }

    // RB_Transplant()
    void Dictionary::RB_Transplant(Node* u, Node* v){
        if (u->parent == nil) root = v;
        else if (u == u->parent->left) u->parent->left = v;
        else u->parent->right = v;
        v->parent = u->parent;
    }

    // RB_Delete()
    void Dictionary::RB_Delete(Node* N){
        Node* y = N;
        Node* x;
        int y_original_color = y->color;
        if (N->left == nil){
            x = N->right;
            RB_Transplant(N, N->right);
        } else if (N->right == nil){
            x = N->left;
            RB_Transplant(N, N->left);
        } else {
            y = findMin(N->right);
            y_original_color = y->color;
            x = y->right;
            if (y->parent == N) x->parent = y;
            else {
                RB_Transplant(y, y->right);
                y->right = N->right;
                y->right->parent = y;
            } RB_Transplant(N, y);
            y->left = N->left;
            y->left->parent = y;
            y->color = N->color;
        } if (y_original_color == BLACK) RB_DeleteFixUp(x);
        --num_pairs;
        if (current == N) current = nullptr;
        delete N;
    }

    // RB_DeleteFixUp()
    void Dictionary::RB_DeleteFixUp(Node* N){
        Node* diag;
        while (N != root && N->color == BLACK){
            if (N == N->parent->left){
                diag = N->parent->right;
                if (diag->color == RED){ //case 1
                    diag->color = BLACK;                        
                    N->parent->color = RED;                   
                    LeftRotate(N->parent);               
                    diag = N->parent->right;                    
                } if (diag->left->color == BLACK and diag->right->color == BLACK){ //case 2
                    diag->color = RED;               
                    N = N->parent;                      
                } else {
                    if (diag->right->color == BLACK){ //case 3
                        diag->left->color = BLACK;             
                        diag->color = RED;                    
                        RightRotate(diag);                
                        diag = N->parent->right;               
                    } diag->color = N->parent->color; // case 4
                    N->parent->color = BLACK;        
                    diag->right->color = BLACK;                
                    LeftRotate(N->parent);            
                    N = root;                         
                }
            } else {
                diag = N->parent->left;
                if (diag->color == RED){ //case 5
                    diag->color = BLACK;                    
                    N->parent->color = RED;               
                    RightRotate(N->parent);             
                    diag = N->parent->left;                      
                } if (diag->right->color == BLACK and diag->left->color == BLACK){ //case 6
                    diag->color = RED;                      
                    N = N->parent;                      
                } else {
                    if (diag->left->color == BLACK){ //case 7
                        diag->right->color = BLACK;            
                        diag->color = RED;                    
                        LeftRotate(diag);                    
                        diag = N->parent->left;                   
                    } diag->color = N->parent->color; //case 8          
                    N->parent->color = BLACK;                 
                    diag->left->color = BLACK;                 
                    RightRotate(N->parent);         
                    N = root;                           
                }
            }
        } N->color = BLACK;
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
        return contains(k) ? search(root, k)->val : throw logic_error("Dictionary: getValue(): key \"" + k + "\" does not exist");
    }

    // hasCurrent()
    // Returns true if the current iterator is defined, and returns false 
    // otherwise.
    bool Dictionary::hasCurrent() const{
        return current != nullptr && current != nil;
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
        if (size() > 0) postOrderDelete(root);
        current = nullptr;
        root = nil;
        num_pairs = 0;
    }

    // setValue()
    // If a pair with key==k exists, overwrites the corresponding value with v, 
    // otherwise inserts the new pair (k, v).
    void Dictionary::setValue(keyType k, valType v){
        BST_insert(k, v, RED);
        RB_InsertFixUp(search(root, k));
    }

    // remove()
    // Deletes the pair for which key==k. If that pair is current, then current
    // becomes undefined.
    // Pre: contains(k).
    void Dictionary::remove(keyType k){
        Node* del = search(root, k);
        if (del == nil) throw logic_error("Dictionary: remove(): key \"" + k + "\" does not exist");
        RB_Delete(del);
    }

    // begin()
    // If non-empty, places current iterator at the first (key, value) pair
    // (as defined by the order operator < on keys), otherwise does nothing. 
    void Dictionary::begin(){
        if (size() > 0) current = findMin(root);
    }

    // end()
    // If non-empty, places current iterator at the last (key, value) pair
    // (as defined by the order operator < on keys), otherwise does nothing. 
    void Dictionary::end(){
        if (size() > 0) current = findMax(root);
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
    // Returns a string consisting of all keys in this Dictionary. The key order is 
    // given by a pre-order tree walk. The keys stored in black Nodes will appear in
    // the returned string as "key\n", and the keys stored in red Nodes will appear 
    // as "key (RED)\n".  The output of this function is demonstrated in 
    // /Examples/pa8/DictionaryClient-out.
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