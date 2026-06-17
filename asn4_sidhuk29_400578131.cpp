/*
MECHTRON 2MD3
ASSIGNMENT 4
Karman Sidhu
400578131
Last Updated: March 30, 2026
*/

/*
DESIGN CHOICES:

Hash Function:
- hashA is the sum of the ASCII codes, that is then the modulus of N 
- hash B is the sum of (i+1)*ASCII code, then the modulus of (N-1), then + 1
- hash b is the secondary function thus it cant have any zero values, so thats why the +1 is there
- the table size (N) is a primary # to allow the probing of all cells
- table size (N) is relatively large to have a small load factor
- the unique functions help to reduce collisions

Collision Handling Mechanism: 
- Done through the use of Double Hashing
- Collisions handled by placing an item in the first available cell of the series
- Better than linear, as you dont have to walk past a longer sequence of probes


Sorting Algorithm: 
- Algorithm used is Merge sort
- Has O(n*log(n)) running time, meaning its fast and has sequential data access making it perfect
- Dividing and conquring 
- using generic sequence abstract data type (ADT) and memory allocation/deletion 
- chosen cause of the "noramlized results of a friendly competition of runtimes ;)"

*/




#include <iostream>
#include <fstream>
#include <string>

using namespace std;

const int N =2003; // prime number cause instrucitons say max matches are 1000


class HashTable{
private:
    string keys[N];  //stores the word
    int values [N];  // stores the value (count)
    bool occupied[N];  // tracks if currently in use
    int n;  // total number of elements currently in the ht

    //1st hash function
    // total of all ascii values
    int hashA(string key){
        int sum =0;
        for (char c: key){
            sum += c; 
        }
        return sum%N; 
    }

    //2nd hash function
    // to prevent simialr words with same ascii value totals from having same step size
    int hashB(string key){
        int sum =0; 
        for (int i=0; i< (int)key.length(); i++){
            sum += (i+1)* key[i]; 
        }
        return (sum%(N-1))+1; // cant be 0, and has to be prime
    }

public:
    //constructor
    // intializing all empty
    HashTable(){
        n=0; 
        for (int i=0; i<N; i++){
            occupied[i] = false; 
        }
    }

    // put (k,v)
    // to insert a key value pair using double hashing logic
    void put(string key, int value){
        int h1 = hashA(key); 
        int h2 = hashB(key); 

        int i =0; 

        while (i<N){
            //double hash, hB formula
            int index = (h1 + i*h2)%N; 

            // if key exists, update the value
            if (occupied[index] && keys[index]==key){
                values[index] = value; 
                return; 
            }

            // if empty, insert
            if (!occupied[index]){
                keys[index] = key; 
                values[index] = value; 
                occupied[index] = true; 
                n = n+1; 
                return; 
            }
            i++; 
        }
        cout << "HT full" << endl; 
    }

    // search for a key, return true if it found; 
    bool find (string key){
        int h1 = hashA(key); 
        int h2 = hashB(key); 
        int i =0; 

        while (i<N){
            int index = (h1 + i*h2)%N; 

            if (!occupied[index]){
                return false; // if we reeach an empty index, its not in the ht
            }
            if (keys[index]==key){
                return true; 
            }
            i++; 
        }
        return false; // table full, went through all indexs, but not found
    }

    void print(){ // for testing earlier, not used in the code 
        for (int i=0; i<N; i++){
            if (occupied[i]){
                cout <<i<<":(" << keys[i] << ", "<<values[i] << ")" <<endl; 
            }
        }
        cout << "totallll entries : " << n << endl; 
    }

}; 

//adt generic to manage the dynamic arrays 
struct Sequence {
    string* data; 
    int size; 
}; 

//merging of merge sort to combine two sorted sequences into 1
Sequence merge (const Sequence& A, const Sequence& B){
    int totalSize = A.size + B.size; 
    string* mergedData = new string[totalSize]; 

    int i =0;
    int j =0; 
    int k =0; 

    // in alphabetical order
    while (i<A.size && j<B.size){
        if (A.data[i] <= B.data[j]){
            mergedData[k++] = A.data[i++]; 
        }
        else {
            mergedData[k++] = B.data[j++]; 
        }
    }
    //append the remaining elements from the list
    while (i < A.size){
        mergedData[k++] = A.data[i++]; 
    }
    while (j < B.size){
        mergedData[k++] = B.data[j++]; 
    }

    Sequence s; 
    s.data = mergedData; 
    s.size = totalSize; 
    return s;    
}

// divide and conquer part of merge sort
Sequence mergeSort (const Sequence& S){
    // base case if just 0 or 1
    if (S.size <=1){
        string* base = new string[S.size];
        if (S.size == 1){
            base[0] = S.data[0]; 
        }

            Sequence s; 
            s.data = base;
            s.size = S.size; 
            return s; 
        
    }

    int mid  = S.size /2;  // half the size

    // make to sequences by spliiting
    Sequence S1; 
    S1.data = new string[mid]; 
    S1.size = mid; 

    Sequence S2; 
    S2.data = new string[S.size - mid]; 
    S2.size = S.size - mid; 

    for (int i =0; i <mid; i++){
        S1.data[i] = S.data[i]; 
    }
    for (int i=mid; i<S.size; i++){
         S2.data[i-mid] = S.data[i]; 
    }

    // recursively calling, to make smaller and smaller sub sequences
    Sequence sortedS1 = mergeSort(S1); 
    Sequence sortedS2 = mergeSort(S2); 

    // just cleaning up allocated memory
    delete[] S1.data; 
    delete[] S2.data; 

    Sequence result = merge (sortedS1, sortedS2); 
    delete[] sortedS1.data; 
    delete[] sortedS2.data; 

    return result; 

}



int main() {

    // reading words from first text file
    ifstream fileA("f1.txt");

    if (!fileA){
        cout << "Error" << endl; 
        return 1 ; // error in opening
    }

    HashTable ht; 

    string word;


    // storing ht with words from f1
    while (fileA >> word){
        ht.put(word, 1); 
    }

    // read from file 2
    ifstream fileB("f2.txt"); 
    if(!fileB){
        cout << "Error 2" << endl; 
        return 1; //if error in opening 
    }


    const int MAX_MATCHES = 2003; // just to match N at the top to know when to stop cause of ~1000
    Sequence matches; 

    matches.data = new string [MAX_MATCHES]; 
    matches.size =0; 

    while (fileB>>word){
        // look in hash table for each word in f2 and store respectively
        if (ht.find(word) && matches.size < MAX_MATCHES){ // if a match found
            matches.data[matches.size] = word;  // store the match
            matches.size ++; 
        }
    }

    fileB.close(); 

    cout <<"Total matches before sorting: " << matches.size << endl; 


    // applying merge sort to the unsorted matches to get alphabetical order
    // will return a new sequence, so delete the old one after
    Sequence sortedMatches =  mergeSort(matches); 

    delete[] matches.data; 

    // output file to store it
    ofstream outFile("out.txt"); 
    if (!outFile){
        cout << "Error in out.txt" << endl; 
        delete[] sortedMatches.data; 
        return 1; 
    }

    // iterating through the sorted sequence and writing each word to the output file
    for (int i =0; i < sortedMatches.size; i++){
        outFile << sortedMatches.data[i] << endl; 
    }
    outFile.close(); 

    //deleting the dynamic memory 
    delete[] sortedMatches.data; 

    // all done, great, it worked!
    return 0; 
}


