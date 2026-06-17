// Karman Sidhu - sidhuk29 - 400578131
// Program Created: February 12, 2026
// Program Last Modified: February 12, 2026
/* Purpose: Build a program that uses linked lists, recursion, and alogrithim analysis to 
   work with grocery lists in terms of formatting, reversing, printing, etc. */

#include <iostream>
#include <string>

using namespace std;

// Part 1A) Create Your Grocery List


//Each grocery item stored as a node in a linked list
struct GroceryItem {
 std::string name; // Name of the item
 int quantity; // Quantity to purchase
 bool isPerishable; // True if the item can expire
 GroceryItem* next; // Pointer to the next item
};

                    //pointer to first node, item name, quantity to purchase, whether the item can expire
GroceryItem* addItem(GroceryItem* head, std::string name, int quantity, bool
isPerishable){

    if (head == nullptr){  // if the first node is empty
        GroceryItem* newItem = new GroceryItem; // create a pointer called new Iteam of type grocery item
        newItem->name = name; // name it
        newItem->quantity = quantity;// assign its quantity
        newItem->isPerishable = isPerishable; // true or not if its perishable
        newItem->next = nullptr; // setting the next next node as empty null
        return newItem;
    }

    head->next = addItem(head->next, name, quantity, isPerishable); // recursively calling the same function with respective info until last one reached 

    return head; 
}



// PART 1B) Print Your Grocery List

void printList(GroceryItem* head){

    GroceryItem* currentItem = head; // creating a new pointer called currentiteam of value grocery item

    while (currentItem !=nullptr){ // using a while loop to traverse the list

        cout << currentItem->name << " (" << currentItem->quantity << ")"; // print out the current items name and quantity

        if (currentItem -> isPerishable){  // if the current item is perishable
            cout << "*"; // going to print a star right beside the quantity
        }

        cout << " -> "; // arrow to indicate okay thats the next item
         
        currentItem = currentItem->next; // for the next iteration the current item will be the next item of the now current item :)

    }

    cout << "nullptr" << endl; // nullptr for the last item in the list if its next item is nullptr
}

// Part 2: Remove Expired Items

// a recursive function that will remove all the expired items from the grocery list
GroceryItem* removeExpiredItems(GroceryItem* head){

    if (head == nullptr){ // just a base case to check if the head (first node) is empty
        return nullptr;
    }

    head->next = removeExpiredItems(head->next); // recursively call until we reach the last item

    if (head-> isPerishable && head->quantity ==0){ // if its perishable and the quanity is zero (an assumption)
        GroceryItem* temp = head->next; // a new temporary pointer holding the next node 
        delete head; // delete the node
        return temp; // return the temporary, now new head of the list
    }

    return head; // return the head; 
}


// Part 3: Reverse the Grocery List Recursively 

// Using a recursive function, reverse the order of the grocery list
GroceryItem* reverseList(GroceryItem* head){

    // if current node is null and so is the next node (the list is empty or only has one node)

    if (head == nullptr || head->next == nullptr){
        return head;
    }

    // create a pointer of type grcoery item that calls the function recursively using the next head of the node/head
    GroceryItem* newHead = reverseList(head->next);

    head->next->next = head; // the head of the next next node is the current head

    head->next = nullptr; // the head next node is the null ptr

    return newHead;  // return the new head of the reversed list

}


// Part 4: The Big-O

enum Complexity {
    O_1,
    O_LOG_N,
    O_N,
    O_N_LOG_N,
    O_N_SQUARED
};

std::string complexityToString(Complexity c) {
    switch (c) {
        case O_1: return "O(1)";
        case O_LOG_N: return "O(log n)";
        case O_N: return "O(n)";
        case O_N_LOG_N: return "O(n log n)";
        case O_N_SQUARED:return "O(n^2)";
        default: return "UNKNOWN";
    }
}

// function that returns the correct enum value representing the complexity of countPerishablePairs
// no printing
// return one value from the complexity enum
// the returned value is the worst-case big-O complexity with respect to the number of grocery items n
Complexity getTimeComplexity(){
    return O_N_SQUARED; 
}

// function that prints a brief explaination in 1-3 sentences that prove my choice of time complexity
// also reference how the list is traversed in the function given
void printComplexityExplanation(){
    cout << "Each operation inside the loops run in O(1) in time, however the list is traversed "
         << "using 2 nested loops. Per node, the inner loop visits all next nodes, expressing"
         << " a quadratic number of operations and an overall time complexity of O(n^2)." << endl;
}



int main(){

    // main function as given

     GroceryItem* list = nullptr;
    // Build list (Part 1)
    list = addItem(list, "Milk", 10, true);
    list = addItem(list, "Bread", 5, true);
    list = addItem(list, "Eggs", 12, true);
    list = addItem(list, "Oatmeal", 2, false);
    list = addItem(list, "Cheese", 0, true); 
    list = addItem(list, "Apples", 0, false); 
    
    std::cout << "Initial list:" << std::endl;
    printList(list);
    
    // Remove expired items (Part 2)
    list = removeExpiredItems(list);
    std::cout << "\nAfter removing expired items:" << std::endl;
    printList(list);
 
   // Reverse list (Part 3)
    list = reverseList(list);
    std::cout << "\nAfter reversing list:" << std::endl;
    printList(list);
 
    // Cleanup remaining nodes to avoid memory leaks
 
    while (list != nullptr) {
 
        GroceryItem* temp = list;
        list = list->next;
        delete temp;
 
    }
 
 
    //Big-O (Part 4)
    Complexity c= getTimeComplexity();
    std::cout << "\nComplexity: " << complexityToString(c) << std::endl;
    std::cout << "\nExplanation:" << std::endl;
    printComplexityExplanation() ;
    return 0;
}

// for me
// compile it = g++ -std=c++17 -Wall asn2_sidhuk29_400578131.cpp -o asn2
// run it = ./asn2






