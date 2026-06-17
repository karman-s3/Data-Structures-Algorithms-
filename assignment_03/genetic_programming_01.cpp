
/*
Karman Sidhu
400578131
sidhuk29

MECTRON 2MD3
Assignment 4
Last Edit: March 17 at 9:52 P.M
*/
#include <math.h>
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <random>
#include <stack>
#include <vector>

#include "cartCentering.h"

using namespace std;

// return a double unifomrly sampled in (0,1)
double randDouble(mt19937& rng) {
  return std::uniform_real_distribution<>{0, 1}(rng);
}
// return uniformly sampled 0 or 1
bool randChoice(mt19937& rng) {
  return std::uniform_int_distribution<>{0, 1}(rng);
}
// return a random integer uniformly sampled in (min, max)
int randInt(mt19937& rng, const int& min, const int& max) {
  return std::uniform_int_distribution<>{min, max}(rng);
}

// return true if op is a suported operation, otherwise return false
bool isOp(string op) {
  if (op == "+")
    return true;
  else if (op == "-")
    return true;
  else if (op == "*")
    return true;
  else if (op == "/")
    return true;
  else if (op == ">")
    return true;
  else if (op == "abs")
    return true;
  else
    return false;
}

int arity(string op) {
  if (op == "abs")
    return 1;
  else
    return 2;
}

typedef string Elem;

class LinkedBinaryTree {
 public:
  friend void crossover(LinkedBinaryTree& t1, LinkedBinaryTree& t2, mt19937& rng); //PART 3: friend function so its easier to access 
  struct Node {
    Elem elt;
    string name;
    Node* par;
    Node* left;
    Node* right;
    Node() : elt(), par(NULL), name(""), left(NULL), right(NULL) {}
    int depth() {
      if (par == NULL) return 0;
      return par->depth() + 1;
    }
  };

  class Position {
   private:
    Node* v;

   public:
    Position(Node* _v = NULL) : v(_v) {}
    Node *getNode() const {return v;} // PART 3 
    Elem& operator*() { return v->elt; }
    Position left() const { return Position(v->left); }
    void setLeft(Node* n) { v->left = n; }
    Position right() const { return Position(v->right); }
    void setRight(Node* n) { v->right = n; }
    Position parent() const  // get parent
    {
      return Position(v->par);
    }
    bool isRoot() const  // root of the tree?
    {
      return v->par == NULL;
    }
    bool isExternal() const  // an external node?
    {
      return v->left == NULL && v->right == NULL;
    }
    friend class LinkedBinaryTree;  // give tree access
  };
  typedef vector<Position> PositionList;

 public:
  LinkedBinaryTree() : _root(NULL), score(0), steps(0), generation(0) {}

  // copy constructor
  LinkedBinaryTree(const LinkedBinaryTree& t) {
    _root = copyPreOrder(t.root());
    score = t.getScore();
    steps = t.getSteps();
    generation = t.getGeneration();
  }

  // copy assignment operator
  LinkedBinaryTree& operator=(const LinkedBinaryTree& t) {
    if (this != &t) {
      // if tree already contains data, delete it
      if (_root != NULL) {
        PositionList pl = positions();
        for (auto& p : pl) delete p.v;
      }
      _root = copyPreOrder(t.root());
      score = t.getScore();
      steps = t.getSteps();
      generation = t.getGeneration();
    }
    return *this;
  }

  // destructor
  ~LinkedBinaryTree() {
    if (_root != NULL) {
      PositionList pl = positions();
      for (auto& p : pl) delete p.v;
    }
  }

  int size() const { return size(_root); }
  int size(Node* root) const;
  int depth() const;
  bool empty() const { return size() == 0; };
  Node* root() const { return _root; }
  PositionList positions() const;
  void addRoot() { _root = new Node; }
  void addRoot(Elem e) {
    _root = new Node;
    _root->elt = e;
  }
  void nameRoot(string name) { _root->name = name; }
  void addLeftChild(const Position& p, const Node* n);
  void addLeftChild(const Position& p);
  void addRightChild(const Position& p, const Node* n);
  void addRightChild(const Position& p);
  void printExpression() { printExpression(_root); }
  void printExpression(Node* v);
  double evaluateExpression(double a, double b) {
    return evaluateExpression(Position(_root), a, b);
  };
  double evaluateExpression(const Position& p, double a, double b);
  long getGeneration() const { return generation; }
  void setGeneration(int g) { generation = g; }
  double getScore() const { return score; }
  void setScore(double s) { score = s; }
  double getSteps() const { return steps; }
  void setSteps(double s) { steps = s; }
  void randomExpressionTree(Node* p, const int& maxDepth, mt19937& rng);
  void randomExpressionTree(const int& maxDepth, mt19937& rng) {
    randomExpressionTree(_root, maxDepth, rng);
  }
  void deleteSubtreeMutator(mt19937& rng);
  void addSubtreeMutator(mt19937& rng, const int maxDepth);

 protected:                                        // local utilities
  void preorder(Node* v, PositionList& pl) const;  // preorder utility
  Node* copyPreOrder(const Node* root);
  double score;     // mean reward over 20 episodes
  double steps;     // mean steps-per-episode over 20 episodes
  long generation;  // which generation was tree "born"
 private:
  Node* _root;  // pointer to the root
};

// add the tree rooted at node child as this tree's left child
void LinkedBinaryTree::addLeftChild(const Position& p, const Node* child) {
  Node* v = p.v;
  v->left = copyPreOrder(child);  // deep copy child
  v->left->par = v;
}

// add the tree rooted at node child as this tree's right child
void LinkedBinaryTree::addRightChild(const Position& p, const Node* child) {
  Node* v = p.v;
  v->right = copyPreOrder(child);  // deep copy child
  v->right->par = v;
}

void LinkedBinaryTree::addLeftChild(const Position& p) {
  Node* v = p.v;
  v->left = new Node;
  v->left->par = v;
}

void LinkedBinaryTree::addRightChild(const Position& p) {
  Node* v = p.v;
  v->right = new Node;
  v->right->par = v;
}

// return a list of all nodes
LinkedBinaryTree::PositionList LinkedBinaryTree::positions() const {
  PositionList pl;
  preorder(_root, pl);
  return PositionList(pl);
}

void LinkedBinaryTree::preorder(Node* v, PositionList& pl) const {
  pl.push_back(Position(v));
  if (v->left != NULL) preorder(v->left, pl);
  if (v->right != NULL) preorder(v->right, pl);
}

int LinkedBinaryTree::size(Node* v) const {
  int lsize = 0;
  int rsize = 0;
  if (v->left != NULL) lsize = size(v->left);
  if (v->right != NULL) rsize = size(v->right);
  return 1 + lsize + rsize;
}

int LinkedBinaryTree::depth() const {
  PositionList pl = positions();
  int depth = 0;
  for (auto& p : pl) depth = std::max(depth, p.v->depth());
  return depth;
}

LinkedBinaryTree::Node* LinkedBinaryTree::copyPreOrder(const Node* root) {
  if (root == NULL) return NULL;
  Node* nn = new Node;
  nn->elt = root->elt;
  nn->left = copyPreOrder(root->left);
  if (nn->left != NULL) nn->left->par = nn;
  nn->right = copyPreOrder(root->right);
  if (nn->right != NULL) nn->right->par = nn;
  return nn;
}

// body defined for createrandomtree 

void LinkedBinaryTree::randomExpressionTree(Node* p, const int& maxDepth, mt19937& rng){

  if (p==NULL){
    return; 
  }

  // if max depth reached assign a leaf
  if (maxDepth <=0){
    if(randChoice(rng)){
      p->elt = "a"; 
    }
    else{
      p->elt = "b"; 
    }
    return; 
  }

  //50% chance to make lead node if depth not reached yet
  if (randDouble(rng)<0.5){
    if(randChoice(rng)){
      p->elt = "a"; 
    }
    else{
      p->elt = "b"; 
    }
    return; 
  }

  //otherwise u get a random operator
  int opIndex = randInt(rng, 0, 5);
  string ops[6] = {"+", "-", "*", "/", ">", "abs"}; 
  p->elt = ops[opIndex]; 

  //children created based on operator type recursively
  if (arity(p->elt)==1){ // abs always 1
    if (!p->left){
      addLeftChild(Position(p));
    }
    randomExpressionTree(p->left, maxDepth -1, rng); 
  }
  else{ // basic operators
    if (!p->left){
      addLeftChild(Position(p)); 
    }
    if (!p->right){
      addRightChild(Position(p)); 
    }
    randomExpressionTree(p->left, maxDepth -1, rng); 
    randomExpressionTree(p->right, maxDepth -1, rng); 
  }
}



void LinkedBinaryTree::printExpression(Node* v) {
  if (v==NULL){ //empty node
    return;
  }

  //if lead node just print its element
  if (v->left ==NULL && v->right == NULL){
    cout << v->elt; 
    return; 
  }

  //checking type of operator
  int n = arity (v->elt);

  if (n==1){ // if abs
    cout << v->elt << "("; // print operator and opening (
    printExpression(v->left);  // print left child recursively
    cout << ")"; //close brackets
  }
  else{
    cout << "("; // is basic operaters
    printExpression(v->left); // print l child recusively
    cout << v->elt; //print operator between children
    printExpression(v->right); // print r child
    cout << ")";  //close brackets
  }
}

double evalOp(string op, double x, double y = 0) {
  double result;
  if (op == "+")
    result = x + y;
  else if (op == "-")
    result = x - y;
  else if (op == "*")
    result = x * y;
  else if (op == "/") {
    result = x / y;
  } else if (op == ">") {
    result = x > y ? 1 : -1;
  } else if (op == "abs") {
    result = abs(x);
  } else
    result = 0;
  return isnan(result) || !isfinite(result) ? 0 : result;
}

double LinkedBinaryTree::evaluateExpression(const Position& p, double a, double b) {
  
  // if an empty node
  if (p.v ==NULL){
    return 0.0; 
  }
  
  string value = p.v->elt; 

  if (p.isExternal()){ //leaf node
    if(value=="a"){
      return a;
    }
    if(value=="b"){
      return b;
    }

    return 0.0;  //unknown leaf
  }

  if (arity(value)==1){ //operators abs
    double x = evaluateExpression(p.left(), a,b );
    return evalOp(value, x); 
  }

  //regular operators
  double x = evaluateExpression(p.left(), a,b );
  double y = evaluateExpression(p.right(), a,b );

  return evalOp(value, x, y); 
}





void LinkedBinaryTree::deleteSubtreeMutator(mt19937& rng) {

  PositionList pList = positions();  // grab all them nodes

  if (pList.size() <=1){ // dont delete if only single root
    return; 
  }

  int index = randInt(rng, 1, (int)pList.size()-1);  //grabing random non root nodes
  Position target = pList[index];  // deleting this one dmlemefmer

  Position parent = target.parent(); 
  bool isLeftChild = (parent.v->left == target.v); // grabbing parent of target and checking is it a left child? 

  //collect the nodes in subtree so we can FINALLY delte them
  PositionList subtree;
  preorder(target.v, subtree); 

  //remove subtree from the parent
  if (isLeftChild){
    parent.v->left = nullptr;
  }
  else{
    parent.v->right = nullptr; 
  }

 

  //dete all nodes in the subtreeeee
  for (auto& p: subtree){ // stated in class we can use auto starting 2 weeks ago
    delete p.v; 
  }

  //creatibng a mnew small rand subtree to replace the deleted node
  Node* newNode = new Node; 
  newNode->par = parent.v;
  randomExpressionTree(newNode, 2, rng); //given it a small depthh

  //finally atatch the new subtree :DDDDDD
  if (isLeftChild){
    parent.v->left = newNode; 
  }
  else{
    parent.v->right = newNode; 
  }
}



void LinkedBinaryTree::addSubtreeMutator(mt19937& rng, const int maxDepth) {
  PositionList pList = positions(); //grabbing all nodes in the tree

  int index = randInt(rng, 0, (int)pList.size()-1);  //pick a rand node
  Position target = pList[index]; //grab its index

  int remainingDepth = maxDepth - target.v->depth(); //reamining depth
  if (remainingDepth < 1){ //cant grow beyond max
    return; 
  }

  // if its a leaf, expand it
  if (target.isExternal()){
    string ops[6] = {"+", "-", "*", "/", ">", "abs"}; 
    target.v->elt = ops[randInt(rng, 0, 5)];  //asign a rand operator

    if (arity(target.v->elt)==1){ // if abs
      if (!target.v->left){ //add a left child if missing
        addLeftChild(target);

      }
      randomExpressionTree(target.left().v, remainingDepth -1, rng); //grow left subtree
    }
    else{ // basic oppperator
      if (!target.v->left){ //ad left child if missing
        addLeftChild(target);
      }
      if (!target.v->right){ //add right child if missing
        addRightChild(target);
      }
      //grow both of dem recursively 
      randomExpressionTree(target.left().v, remainingDepth -1, rng); 
      randomExpressionTree(target.right().v, remainingDepth -1, rng); 
    }
  }
}






bool operator<(const LinkedBinaryTree& x, const LinkedBinaryTree& y) {
  return x.getScore() < y.getScore();
}






LinkedBinaryTree createExpressionTree(string RPN) {
  stack<LinkedBinaryTree> tree_stack;  // Keep this line
  
  string token = ""; //temp string to build each variable and operator

  //iterating through the rpn string
  for (int i=0; i<=RPN.length(); i++){
    //reach end of string/space we have the entirety
    if (i==RPN.length() || RPN[i] ==' '){
      if (token!=""){ //ignoring empty tokens
        if (isOp(token)){ // if its an oeprator 

          LinkedBinaryTree t; //create new tree
          t.addRoot(token); //make it the root

          if (arity(token)==1){ //something like abs
            LinkedBinaryTree child = tree_stack.top(); 
            tree_stack.pop(); // remove it from the stack
            
            t.addLeftChild(t.root(), child.root()); //attach it as left child
          }
          else{ //regular operators
            LinkedBinaryTree right = tree_stack.top();  //right operand
            tree_stack.pop();

            LinkedBinaryTree left = tree_stack.top(); //left oeprand
            tree_stack.pop(); 

            t.addLeftChild(t.root(), left.root()); //attach left child
            t.addRightChild(t.root(), right.root());  //atttach right child
          }

          tree_stack.push(t); //push the final tree on stack
        }
        else{ // incase anything else
          LinkedBinaryTree t;
          t.addRoot(token);
          tree_stack.push(t); 
        }

        token =""; //strings rest for next iteration
      }
    }
    else{
      token = token + RPN[i]; // build the token by single chars
    }
  }
  return tree_stack.top();  // Keep this line
}







LinkedBinaryTree createRandExpressionTree(int max_depth, mt19937& rng) {
  LinkedBinaryTree t; //create a new tree
  t.addRoot();  // add a root 
  t.randomExpressionTree(t.root(), max_depth, rng); // fit the tree with random expression node
  // I defined the body of this already defined function 
  return t; // return the final constructed rand tree
}


// Evaluate tree t in the cart centering task
void evaluate(mt19937& rng, LinkedBinaryTree& t, const int& num_episode,
              bool animate) {
  cartCentering env;
  double mean_score = 0.0;
  double mean_steps = 0.0;
  for (int i = 0; i < num_episode; i++) {
    double episode_score = 0.0;
    int episode_steps = 0;
    env.reset(rng);
    while (!env.terminal()) {
      double value = t.evaluateExpression(env.getCartXPos(), env.getCartXVel());
      int action = (value>=0)? 1:-1; 
      episode_score += env.update(action, animate);
      episode_steps++;
    }
    mean_score += episode_score;
    mean_steps += episode_steps;
  }
  t.setScore(mean_score / num_episode);
  t.setSteps(mean_steps / num_episode);
}



// Part 2: The LexLessThan ADT

struct LexLessThan {
  bool operator()(const LinkedBinaryTree& T1, const LinkedBinaryTree& T2) const {
    double scoreDiff = T1.getScore() - T2.getScore(); 

    if (scoreDiff < 0.01 && scoreDiff > -0.01){
      //smaller tree better
      if (T1.size() > T2.size()){
        return true; 
      }
      else{
        return false; 
      }

    }

    //if scores nto close to equal, compare them by the scoreee

    if (T1.getScore() < T2.getScore()){
      return true; 
    }
    else {
      return false; 
    }
  }
}; 


void crossover (LinkedBinaryTree& t1, LinkedBinaryTree& t2, mt19937& rng){
  //once again, stated we can u auto in lecture

  // from both trees getting their nodes 
  auto nodes1 = t1.positions();
  auto nodes2 = t2.positions(); 

  // if 1 or both ahve only 1 node no crossovers gonna happen
  if (nodes1.size() <=1 || nodes2.size() <=1){
    return;
  }

  //picking a random index from each tree (it will not be the very first parent)
  int index1 = randInt(rng, 1, nodes1.size()-1);
  int index2 = randInt(rng, 1, nodes2.size()-1); 

  // the roots of the selected nodes
  auto subtree1 = nodes1[index1];
  auto subtree2 = nodes2[index2]; 

  // getting the parents of the  nodes
  auto parent1 = subtree1.parent(); 
  auto parent2 = subtree2.parent(); 


  // checking it each node is L or R child
  bool left1 = (parent1.getNode()->left == subtree1.getNode()); 
  bool left2 = (parent2.getNode()->left ==subtree2.getNode()); 

  //copying subtrees
  LinkedBinaryTree::Node* copy1 = t1.copyPreOrder(subtree1.getNode()); 
  LinkedBinaryTree::Node* copy2 = t2.copyPreOrder(subtree2.getNode()); 



  //storing old subtrees to be DELETEDDDD
  LinkedBinaryTree::Node* old1 = subtree1.getNode(); 
  LinkedBinaryTree::Node* old2 = subtree2.getNode(); 

  // t2 subtree into t1
  if (left1){

    parent1.getNode()->left = copy2; 
  }
  else{

    parent1.getNode()->right = copy2;  
  }
  copy2->par = parent1.getNode(); 

  // t1 subtree into t2
  if (left2){

    parent2.getNode()->left = copy1; 
  }
  else{

    parent2.getNode()->right = copy1;  
  }
  copy1->par = parent2.getNode(); 

  //Deleting old subtrees
  LinkedBinaryTree:: PositionList sub1;
  t1.preorder(old1, sub1); 
  for(auto& p: sub1){
    delete p.getNode(); 
  }

  LinkedBinaryTree:: PositionList sub2;
  t2.preorder(old2, sub2); 
  for(auto& p: sub2){
    delete p.getNode(); 
  }

}


int main() {
  mt19937 rng(42);
  // Experiment parameters
  const int NUM_TREE =50;
  const int MAX_DEPTH_INITIAL =1;
  const int MAX_DEPTH = 20;
  const int NUM_EPISODE = 20;
  const int MAX_GENERATIONS = 100;

  // Create an initial "population" of expression trees
  vector<LinkedBinaryTree> trees;
  for (int i = 0; i < NUM_TREE; i++) {
    LinkedBinaryTree t = createRandExpressionTree(MAX_DEPTH_INITIAL, rng);
    trees.push_back(t);
  }

  // Genetic Algorithm loop
  LinkedBinaryTree best_tree;
  std::cout << "generation,fitness,steps,size,depth" << std::endl;
  for (int g = 1; g <= MAX_GENERATIONS; g++) {

    // Fitness evaluation
    for (auto& t : trees) {
      if (t.getGeneration() < g - 1) continue;  // skip if not new
      evaluate(rng, t, NUM_EPISODE, false);
    }

    // Sort trees using overloaded "<" op (worst->best)
    //std::sort(trees.begin(), trees.end());

    // // Sort trees using comparaor class (worst->best)
     std::sort(trees.begin(), trees.end(), LexLessThan());

    // Erase worst 50% of trees (first half of vector)
    trees.erase(trees.begin(), trees.begin() + NUM_TREE / 2);

    // Print stats for best tree
    best_tree = trees[trees.size() - 1];
    std::cout << g << ",";
    std::cout << best_tree.getScore() << ",";
    std::cout << best_tree.getSteps() << ",";
    std::cout << best_tree.size() << ",";
    std::cout << best_tree.depth() << std::endl;

    // Selection and mutation
    while (trees.size() < NUM_TREE) {
      // Selected random "parent" tree from survivors
      LinkedBinaryTree parent1 = trees[randInt(rng, 0, (NUM_TREE / 2) - 1)];
      LinkedBinaryTree parent2 = trees[randInt(rng, 0, (NUM_TREE/2)-1)];
      
      // Create child tree with copy constructor
      LinkedBinaryTree child1(parent1);
      LinkedBinaryTree child2(parent2); 
      
      child1.setGeneration(g);
      child2.setGeneration(g); 

      //adding in the cross over 
      crossover(child1, child2, rng); 
      
      // Mutation
      // Delete a randomly selected part of the child's tree
      child1.deleteSubtreeMutator(rng);
      // Add a random subtree to the child
      child1.addSubtreeMutator(rng, MAX_DEPTH);
      
      // Delete a randomly selected part of the child's tree
      child2.deleteSubtreeMutator(rng);
      // Add a random subtree to the child
      child2.addSubtreeMutator(rng, MAX_DEPTH);
      
      trees.push_back(child1);
      if (trees.size() < NUM_TREE){
        trees.push_back(child2); 
      }
    }
  }

  // // Evaluate best tree with animation
  const int num_episode = 3;
  evaluate(rng, best_tree, num_episode, true);

  // Print best tree info
  std::cout << std::endl << "Best tree:" << std::endl;
  best_tree.printExpression();
  std::cout << endl;
  std::cout << "Generation: " << best_tree.getGeneration() << endl;
  std::cout << "Size: " << best_tree.size() << std::endl;
  std::cout << "Depth: " << best_tree.depth() << std::endl;
  std::cout << "Fitness: " << best_tree.getScore() << std::endl << std::endl;
}

// g++ genetic_programming_01.cpp cartCentering.h -o gp


