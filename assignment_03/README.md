## MECHTRON 2MD3: Data Structures and Algorithms for Mechatronics

### Assignment 3: Genetic Programming in Reinforcement Learning

## Instructions 

Download (or pull) the code directory from Gitlab in 2md3_2026/assignments/assignment_03

 This directory contains a partial implementation of a Genetic Programming experiment to solve the Cart Centering (isotropic rocket) problem. This application of trees is discussed in detail in lectures during the week of March 2.

The code extends the LinkedBinaryTree class from chapter 7 of the Goodrich text such that LinkedBinaryTree objects can represent expression trees. You must keep the the basic class structure and style. Your goal is to **extend** the code provided, not rewrite your own version from scratch.   

The `genetic_programming_01.cpp` program will evolve trees for 100 generations and print statistics on the best tree at each generation. Your task involves the following parts: 

### Part 1 (3 marks each).  Implement the following functions using the exact parameters and return types shown.
Place holders for these functions already exist. Your task is to fill in their code. You may reuse any code existing in the class. Implement the functions in order and keep testing you code. 

```cpp
LinkedBinaryTree createExpressionTree(string RPN)
```
This function creates a new expression tree from the RPN string using the stack method.

```cpp
double LinkedBinaryTree::evaluateExpression(const Position& p, double a, double b)
```
This function recursively evaluates the expression tree and returns the result. Variables $a$ and $b$ represent inputs to the expression. For example, if this function is called on a leaf node containing the Elem "a", then the return value is simply the value of the double parameter $a$.  

```cpp
void LinkedBinaryTree::printExpression(Node* v) 
```
This function must recursively print the expression tree with parentheses. For example, a tree representing RPN expression "1 2 + 3 *" is printed as "((1+2)*3)" 

```cpp
LinkedBinaryTree createRandExpressionTree(int max_depth, mt19937& rng)
```
This function should create and return a randomly generated expression tree with a depth no greater than max_depth. The function currently simply returns the expression tree $a + b$.

```cpp
void LinkedBinaryTree::deleteSubtreeMutator(mt19937& rng)
```
This function should delete a randomly selected subtree from the tree.

```cpp 
void addSubtreeMutator(mt19937& rng, const int maxDepth)
```
This function should add a randomly created subtree to the tree.

You should see a significant improvement in the best fitness after implementing createRandExpressionTree, and a further improvement after implementing the mutation operators. When plotted, the data should look something like the following plot. The blue line is the original code without modification. The red line shows results when the population is initialized with random trees. The yellow line shows results when the population is initialized with random trees and both mutation operators are implemented.

<img src="plots/a3_1.png" alt="GP Experiment" width="700">

Plot your results in the above format after implementing your functions. I used Excel but any plotting tool is fine. Your submission for part 1 must include your plot and the printout of the best individual evolved using mutation in exactly this format (note your tree will differ): 
```
Best tree: 
(((a > a) + (((b > b) * abs(((abs((b / abs(a))) * (((a / b) / a) / b)) > ((abs((a * (b / b))) - (abs(a) / a)) + a)))) * a)) - (b + a)) 
Generation: 99 
Size: 42 
Depth: 11 
Fitness: -0.0889936 
```

### Part 2 (3 marks).  
Implement a comparator ADT called LexLessThan which performs a lexicographic comparison of two trees $T1$ and $T2$ as follows: If the scores of $T1$ and $T2$ differ by less than 0.01, then $T1$ is “less than” $T2$ if $T1$ has more nodes than $T2$. Otherwise, compare the trees by their fitness score. The goal of using this comparator is to favour simpler trees only when their fitness scores are similar. Repeat the experiment from part 1 using the new comparator. To do so, you must uncomment the line under "sort using comparator class". After doing so, does evolution produce simpler trees? A line plot comparing the size of the best tree during evolution with and without the LexLessThan comparator might look like the following: 

<img src="plots/a3_2.png" alt="GP Experiment" width="700">

### Part 3 (5 marks).
Try adding a crossover operator in which you randomly sample two parents and create two children by swapping random subtrees from each parent. See lecture 2026-10-2MD3-RL-TreeGP.pdf slides for an example. Note there is no placeholder code for crossover. You must determine where and how to add this feature. Crossover is typically applied before mutation. When results are ready, add a line to your plot labelled “rand init + xover + mutation” to show the results of this experiment.

## Summary

| Part | Topic | Marks |
|------|-------|-------|
| 1 | Base expression tree functions | 18 |
| 2 | Comparator ADT | 3 |
| 3 | Crossover function and integration | 5 ||
| **Total** | | **26** |


## Submission Requirements

### Submit a single pdf file and name it:
```
asn3_macid_studentnumber.pdf
```

This document must contain the following sections:

* Part 1.1: A line plot like the one above (in part 1) showing the effect of your createRandExpressionTree function and mutation operators.

  Your plot should show 3 lines labelled as:
  * "default init" - Shows the default result of evolution (without your code modifications).
  * "rand init" - Shows the result after you add your createRandExpressionTree function.
  * "rand init + mutation" - Shows the result after you add your creatRandExpressionTree and your two mutation functions

* Part 1.2: A printout of the best tree and its statistics (Generation, Size, Depth, Fitness). 

* Part 2.1:  A line plot comparing the size of the best tree during evolution with and without the LexLessThan comparator (See part 2 example plot).

  Your plot should include 2 lines labelled as:
  * "default" - Shows evolution of the depth of the best individual **without** lexLessThan
  * "lecLessThan" - Show evolution of the depth of the best individual **with** lexLessThan

* Part 2.2: A printout of the best tree evolved using LexLessThan comparator ADT.

* Part 3: A fitness plot like the one above comparing the effect of your createRandExpressionTree function, mutations, **and** crossover operators.

  Your plot should show 4 lines labelled as:
  * "default init" - Shows the default result of evolution (without your code modifications).
  * "rand init" - Shows the result after you add your createRandExpressionTree function.
  * "rand init + mutation" - Shows the result after you add your creatRandExpressionTree and your two mutation functions
  * "rand init + xover+ mutation" - Shows the results after adding everythin above plus crossover.

### Submit your modified genetic_programming_01.cpp file and name it:
```
asn3_macid_studentnumber.cpp
``` 

### Pre-Submission Checklist

- [ ] File named `asn3_macid_studentnumber.cpp` (lowercase).
- [ ] File named `asn3_macid_studentnumber.pdf` (lowercase).
- [ ] Please ensure your source files compile and run properly before submitting. 
- [ ] Checked your code for memory leaks using valgrind (https://valgrind.org/). 

---

## Animation
If you uncomment the lines below “Evaluate best tree with animation“, the program will display a simple animation of the best tree interacting with the Cart Centering simulator. This can be very useful for debugging. However, note that animating the best tree will reset its score. For best results, do not animate the tree prior to generating its printout. 



