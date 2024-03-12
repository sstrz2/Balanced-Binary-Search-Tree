#ifndef _BST_H
#define _BST_H

#include <iostream>
#include <vector>

using namespace std;

template <typename T>
class bst {

  private: 
    struct bst_node {
      T      val;
      bst_node *left;
      bst_node *right;
      int leftCount = 0;
      int rightCount = 0;

      bst_node ( const T & _val = T{}, bst_node * l = nullptr, bst_node *r = nullptr)
        : val { _val },  left { l }, right {r} 
      { }
    };



  public:
    // constructor:  initializes an empty tree
    bst(){
      root = nullptr;
    }

  private:
    // helper function which recursively deallocates nodes
    //   in a tree.
    static void delete_nodes(bst_node *r){
      if(r==nullptr) return;
      delete_nodes(r->left);
      delete_nodes(r->right);
      delete r;
    }

  public:
    // destructor
    ~bst() {
      delete_nodes(root);
    }

  private:

/*
 * function:  insert()
 * desc:      recursive helper function inserting x into
 *            binary search tree rooted  at r.
 *
 * returns:   pointer to root of tree after insertion.
 *
 * notes:     if x is already in tree, no modifications are made.
 */
    static bst_node * _insert(bst_node *r, T & x, bool &success, bst_node *&lowestRight){
      if(r == nullptr){
        success = true;
        return new bst_node(x, nullptr, nullptr);
      }

      if(r->val == x){
        success = false;
        return r;
      }
      if(x < r->val){

        r->left = _insert(r->left, x, success,lowestRight);
        if(success){
          r->leftCount++;
        }
        if(std::max(r->left->leftCount,r->left->rightCount) > (2*std::min(r->left->leftCount,r->left->rightCount)+1)){
          lowestRight=r;
        }
        return r;
      }
      else {
        r->right = _insert(r->right, x, success,lowestRight);
        if(success){
          r->rightCount++;
        }
        if(std::max(r->right->leftCount,r->right->rightCount) > (2*std::min(r->right->leftCount,r->right->rightCount)+1)){
          lowestRight=r;
        }
        return r;
      }
    }


  public:
  /*
   * function:  insert
   * desc:      inserts x into BST given by t.  Note that
   *            a BST stores a SET -- no duplicates.  Thus,
   *            if x is already in t when call made, no 
   *            modifications to tree result.
   *
   * note:      helper function does most of the work.
   *
   */
   bool insert(T & x){
      bool success;
      bst_node* wrong=nullptr;
      root = _insert(root, x, success,wrong);
      if(std::max(root->leftCount,root->rightCount) > (2*std::min(root->leftCount,root->rightCount)+1)){
        vector<T>* vec=to_vector();
        bst_node* temp=_from_vec(*vec,0,size()-1);
        delete_nodes(root);
        root=temp;
      }
      else if(wrong != nullptr){
        if(std::max(wrong->left->leftCount,wrong->left->rightCount) > (2*std::min(wrong->left->leftCount,wrong->left->rightCount)+1)){
        vector<T>* vec=to_vector_node(wrong->left);
        bst_node* temp=_from_vec(*vec,0,_size(wrong->left)-1);
        delete_nodes(wrong->left);
        wrong->left=temp;
        }
        if(std::max(wrong->right->leftCount,wrong->right->rightCount) > (2*std::min(wrong->right->leftCount,wrong->right->rightCount)+1)){
          vector<T>* vec=to_vector_node(wrong->right);
          bst_node* temp=_from_vec(*vec,0,_size(wrong->right)-1);
          delete_nodes(wrong->right);
          wrong->right=temp;
        }
      }
      return success;
   }

/*
 * function:  contains()
 * desc:      returns true or false depending on whether x is an
 *            element of BST (calling object)
 *
 */
    bool contains(const T & x){
      bst_node *p = root;

      while(p != nullptr){

        if(p->val == x)
          return true;
        if(x < p->val){
          p = p->left;
        }
        else
          p = p->right;
      }
      return false;  
    }

  private:
    // returns pointer to node containing
    //   smallest value in tree rooted at r
    static bst_node * _min_node(bst_node *r ){
      if(r==nullptr)
        return nullptr; // should never happen!
      while(r->left != nullptr)
        r = r->left;
      return r;
    }

    // returns pointer to node containing
    //   smallest value in tree rooted at r
    static bst_node * _max_node(bst_node *r ){
      if(r==nullptr)
        return nullptr; // should never happen!
      while(r->right != nullptr)
        r = r->right;
      return r;
    }

    // recursive helper function for node removal
    //   returns root of resulting tree after removal.
    static bst_node * _remove(bst_node *r, T & x, bool &success){
      bst_node *tmp;
      bool sanity;

      if(r==nullptr){
        success = false;
        return nullptr;
      }
      if(r->val == x){
        success = true;

        if(r->left == nullptr){
          tmp = r->right;

          delete r;
          return tmp;
        }
        if(r->right == nullptr){
          tmp = r->left;
          delete r;
          return tmp;
        }
        // if we get here, r has two children
        r->val = _min_node(r->right)->val;
        r->rightCount--;
        r->right = _remove(r->right, r->val, sanity);
        if(!sanity)
          std::cerr << "ERROR:  remove() failed to delete promoted value?\n";
        return r;
      }
      if(x < r->val){
        r->left = _remove(r->left, x, success);
        if(success){
          r->leftCount--;
        }
      }
      else {
        r->right = _remove(r->right, x, success);
        if(success){
          r->rightCount--;
        }
      }
      return r;
    }

  public:

    bool remove(T & x){
      bool success;
      root = _remove(root, x, success);
      return success;
    }


  private:
    // recursive helper function to compute size of
    //   tree rooted at r
    static int _size(bst_node *r){
      return r->leftCount+r->rightCount+1;
    }

  public: 
    int size() {
      return root->leftCount + root->rightCount + 1;
    }

  private:

    static int _height(bst_node *r){
      int l_h, r_h;

      if(r==nullptr) return -1;
      l_h = _height(r->left);
      r_h = _height(r->right);
      return 1 + (l_h > r_h ? l_h : r_h);
    }

  public:

    int height() {
      return _height(root);
    }

    bool min(T & answer) {
      if(root == nullptr){
        return false;
      }
      answer = _min_node(root)->val;
      return true;
    }

    T max() {
      return _max_node(root)->val;
    }

    /*
     * Function:  to_vector
     * Description:  allocates a vector of type T and populates
     *               it with the elements of the tree in sorted
     *               order.  A pointer to the vector is returned.
     *
     * runtime:  O(n) where n is the tree size.
     *
     */
    vector<T> * to_vector() {
      vector<T>* rtn = new vector<T>();
      _to_vector(root,rtn);
      return rtn;
    }


    void _to_vector(bst_node* node, vector<T>* vec){
      if(node!=nullptr){
        _to_vector(node->left,vec);
        vec->push_back(node->val);
        _to_vector(node->right,vec);
      }
    }
    vector<T>* to_vector_node(bst_node* node){
      vector<T>* rtn = new vector<T>();
      _to_vector(node,rtn);
      return rtn;
    }


    /*
     * Function:  get_ith
     * Description:  determines the ith smallest element in t and
     *    "passes it back" to the caller via the reference parameter x.  
     *    i ranges from 1..n where n is the number of elements in the 
     *    tree.
     *
     * Runtime:  O(h) where h is the tree height
     */
    bool get_ith(int i, T &x) {
      if(i > size() || i < 1){
        return false;
      }
      _get_ith(i,x,root, (root->leftCount+1));
      return true;
    }

    void _get_ith(int i, T &x, bst_node* node, int curPos ){
      if(node == nullptr){
        return;
      }
      if(curPos ==  i){
        x=node->val;
        return;
      }
      else if(curPos > i){
        if(node->left != nullptr){
          curPos-=(node->left->rightCount)+1;
          _get_ith(i,x,node->left,curPos);
        }
        else{
          cerr<<"Should never happen I think1\n";
        }
      }
      else{
        if(node->right != nullptr){
          curPos+=(node->right->leftCount)+1;
          _get_ith(i,x,node->right,curPos);
        }
        else{
          cerr<<"Should never happen I think2\n";
        }
      }
    }


    /* 
     * get_ith_SLOW:  slow version of get_ith which gives the right answer, but
     *   does so by "walking the entire tree".
     */
    bool get_ith_SLOW(int i, T &x) {
      int n = size();
      int sofar=0;

      if(i < 1 || i > n) 
        return false;

      _get_ith_SLOW(root, i, x, sofar);
      return true;
    }

  private:
    // recursive helper function that does most of the work
    static void _get_ith_SLOW(bst_node *t, int i, T &x, int &sofar) {
      if(t==nullptr) 
        return;
      _get_ith_SLOW(t->left, i, x, sofar);

      if(sofar==i) 
        return;
      sofar++;
      if(sofar==i) {
        x = t->val;
        return;
      }
      _get_ith_SLOW(t->right, i, x, sofar);
    }

  public:

    /* 
     * Function: position_of 
     * Description:  this is like the inverse of
     *       get_ith:  given a value x, determine the 
     *       position ("i") where x would appear in a
     *       sorted list of the elements and return
     *       the position as an integer.
     *       If x is not in the tree, -1 is returned.
     *       
     * Return:  -1 if x is not in the tree; otherwise, returns the position where x 
     *          would appear in the sorted sequence of the elements of the tree (a
     *          value in {1..n}          
     *
     * Runtime:  O(h) where h is the tree height
     */
    int position_of(const T & x) {
      int rtn=-1;
      _position_of(root,x,(root->leftCount+1),rtn);
      return rtn;
    }

    void _position_of(bst_node* node, T x, int curPos, int &spot){
      if(node == nullptr){
        return;
      }
      if(node->val == x){
        spot=curPos;
      }
      else if(node->val > x){
        if(node->left != nullptr){
          curPos-=(node->left->rightCount)+1;
          _position_of(node->left,x,curPos,spot);
        }
      }
      else{
        if(node->right != nullptr){
          curPos+=(node->right->leftCount)+1;
          _position_of(node->right,x,curPos,spot);
        }

      }

    }

    /* 
     * Function:  num_geq
     * Description:  returns the number of elements in tree which are 
     *       greater than or equal to x.
     *
     * Runtime:  O(h) where h is the tree height
     */
    int num_geq(const T & x) {
      int rtn=0;
      _num_geq(x,root,rtn);
      return rtn;
    }

    void _num_geq(T x, bst_node* node,int &rtn){
      if(node == nullptr){
        return;
      }
      if(node->val >= x){
        rtn+=node->rightCount+1;
        _num_geq(x,node->left,rtn);
      }
      else{
        _num_geq(x,node->right,rtn);
      }
    }

    /*
     * function:     num_geq_SLOW
     * description:  same functionality as num_geq but sloooow (linear time)
     *                (private helper does most of the work)
     *
     */
    int num_geq_SLOW(const T & x) {
      return _num_geq_SLOW(root, x);
    }

  private:
    static int _num_geq_SLOW(bst_node * t, const T & x) {
      int total;

      if(t==nullptr) return 0;
      total =_num_geq_SLOW(t->left, x) + _num_geq_SLOW(t->right, x);

      if(t->val >= x)
        total++;
      return total;
    }
    
  public:

    /*
     * Function:  num_leq
     * Description:  returns the number of elements in tree which are less
     *      than or equal to x.
     *
     * Runtime:  O(h) where h is the tree height
     *
     **/
    int num_leq(const T & x) {
      int rtn=0;
      _num_leq(x,root,rtn);
      return rtn;
    }

    void _num_leq(T x, bst_node* node,int &rtn){
      if(node == nullptr){
        return;
      }
      if(node->val <= x){
        rtn+=node->leftCount+1;
        _num_leq(x,node->right,rtn);
      }
      else{
        _num_leq(x,node->left,rtn);
      }
    }

    /*
     * function:     num_leq_SLOW
     * description:  same functionality as num_leq but sloooow (linear time)
     *               (private helper _num_leq_SLOW does most of the work)
     */
    int num_leq_SLOW(const T & x) {
      return _num_leq_SLOW(root, x);
    }
  private:

    // helper for num_leq_SLOW
    static int _num_leq_SLOW(bst_node *t, const T &x) {
      int total;

      if(t==nullptr) return 0;
      total =_num_leq_SLOW(t->left, x) + _num_leq_SLOW(t->right, x);

      if(t->val <= x)
        total++;
      return total;
    }

  public:

    /*
     * Function:  num_range
     * Description:  returns the number of elements in tree which are
     *       between min and max (inclusive).
     *
     * Runtime:  O(h) where h is the tree height
     *
     **/
    int num_range(const T & min, const T & max) {
      int less=num_geq(max);
      int more=num_leq(min);
      return size()-(less+more);
    }

    /*
     * function:     num_range_SLOW
     * description:  same functionality as num_range but sloooow (linear time)
     *               (private helper _num_range_SLOW does most of the work)
     *
     */
    int num_range_SLOW(const T & min, const T & max) {
      return _num_range_SLOW(root, min, max);
    }

  private:
    static int _num_range_SLOW(bst_node *t, const T &min, const T &max) {
      int total;

      if(t==nullptr) return 0;
      total =_num_range_SLOW(t->left, min, max) + 
                _num_range_SLOW(t->right, min, max);

      if(t->val >= min && t->val <= max)
        total++;
      return total;
    }

  public:

    /*
     * function:     extract_range
     * Description:  allocates a vector of element type T
     *               and populates it with the tree elements
     *               between min and max (inclusive) in order.  
     *               A pointer to the allocated and populated
     *               is returned.
     *
     * runtime:  the runtime requirement is "output dependent".
     *           Let k be the number of elements in the specified range
     *           (and so the length of the resulting vector) and let h
     *           be the height of the tree.  The runtime must be:
     *
     *                  O(h + k)
     *
     *
     */
    vector<T> * extract_range(const T & min, const T & max) {
      vector<T>* rtn= new vector<T>();
      _extract_range(root,min,max,rtn);
      return rtn;
    }

    void _extract_range(bst_node* node, T min, T max, vector<T>* vec){
      if(node==nullptr){
        return;
      }
      if(node->val > min){
        _extract_range(node->left,min,max,vec);
      }
      if (node->val >= min && node->val <= max) {
        vec->push_back(node->val);
      }
      if(node->val < max){
        _extract_range(node->right,min,max,vec);
      }
    }
  private:
    static void indent(int m){
      int i;
      for(i=0; i<m; i++)
        std::cout << "-";
    }

    static void _inorder(bst_node *r){
      if(r==nullptr) return;
      _inorder(r->left);
      std::cout << "[ " << r->val << " ]\n";
      _inorder(r->right);
    }

    static void _preorder(bst_node *r, int margin){
      if(r==nullptr) {
        indent(margin);
        std::cout << " nullptr \n";
      } 
      else {
        indent(margin);
        std::cout << "[ " << r->val << " ]\n";
        _preorder(r->left, margin+3);
        _preorder(r->right, margin+3);
      }
    }

  public:
    void inorder() {
      std::cout << "\n======== BEGIN INORDER ============\n";
      _inorder(root);
      std::cout << "\n========  END INORDER  ============\n";
    }


    void preorder() {

      std::cout << "\n======== BEGIN PREORDER ============\n";
      _preorder(root, 0);
      std::cout << "\n========  END PREORDER  ============\n";

    }

  private:
    static bst_node * _from_vec(const std::vector<T> &a, int low, int hi){
      int m;
      bst_node *root;

      if(hi < low) return nullptr;
      m = (low+hi)/2;
      root = new bst_node(a[m]);
      root->left  = _from_vec(a, low, m-1);
      root->right = _from_vec(a, m+1, hi);
      root->leftCount=((m-low));
      root->rightCount=((hi-m));
      return root;
    }

  public:
    static bst * from_sorted_vec(const std::vector<T> &a, int n){

      bst * t = new bst();
      t->root = _from_vec(a, 0, n-1);
      return t;
    }

  private:
    static void _to_preorder_vec(bst_node *t, std::vector<T> &vec) {
      if(t==nullptr) return;
      vec.push_back(t->val);
      _to_preorder_vec(t->left, vec);
      _to_preorder_vec(t->right, vec);
    }
    std::vector<T> * to_preorder_vec() {
      std::vector<T> *vec = new std::vector<T>();
      _to_preorder_vec(root, *vec);
      return vec;
    }

  private:
    bst_node *root;


}; // end class bst

#endif
