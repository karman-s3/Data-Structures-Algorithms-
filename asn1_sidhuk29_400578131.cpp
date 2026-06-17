// Karman Sidhu - sidhuk29 - 400578131
// Program Created: January 25, 2026
// Program Last Modified: January 26, 2026
/* Purpose: Build a simulation to test the Birthday Paradox in relation
   to sensor bin collisions to see how often collisions occurs. */



#include <iostream> //library for output
using namespace std; // std namespace to avoid repeated writing 

unsigned int custom_rand_state = 0;

//starting point of random seequence (same seed, same seq every time)
void custom_srand(unsigned int seed) {
 custom_rand_state = seed;
}

//returns the next random integer in the seq
int custom_rand() {
 custom_rand_state = custom_rand_state * 1103515245 + 12345;
 return (custom_rand_state / 65536) % 32768;
}





//PART 1: To store data for a reading

// Each sensor reading has 1. Which bin it lands in 2. Which sensor produced reading

class SensorReading {
private:// not accessible outside of class, only member functions
 int bin_value; // Which bin (0 to NUM_BINS-1)
 int sensor_id; // Which sensor produced this reading
public:
 SensorReading(); //default constructor declared
 SensorReading(int bin, int id); //parameterized constructor
 
 //get functions that read (dont change any data)
 int getBinValue() const;
 int getSensorId() const;
 
 // Declare these as friends: functions that are external but can read the private variables of this class
 friend bool operator==(const SensorReading& a, const SensorReading& b);
 friend ostream& operator<<(ostream& out, const SensorReading& r);
};

//constructor intializing using list. Syntax = ( the name (value in brackets) )
//remember, values intialized is corresponding order to how they are declared in
//the class definition
SensorReading::SensorReading()
    : bin_value(-1),
      sensor_id(-1){}

//parameterized constructor - takes inputs 

SensorReading::SensorReading(int bin, int id)
    :bin_value(bin), // assigning input bin and id to data members
     sensor_id(id) {}

// the get functions return integer values 
int SensorReading::getBinValue() const{
    return bin_value;
}
//returns the sensor id
int SensorReading::getSensorId() const{
    return sensor_id;
}

//friend functions (don't write friend infront)
//using references(nicknames), (not pointers) so . works to access class 
//defines to the compiler what the == means
bool operator==(const SensorReading& a, const SensorReading& b){
    return a.bin_value == b.bin_value; 
}
// to tell the compiler to do cout<<... (review example in textbook)
ostream& operator<<(ostream& out, const SensorReading& r){
    out << "S" << r.sensor_id << ":B" << r.bin_value; 
    return out; 
}








// PART 2: The sensor family (base and derived) = polymorphism

// creating family with polymorphism. Creating 2 different sensors that are sensors, but the sensor cannot be those 2
// base class created, create the derived ones now

//base class
class Sensor {
protected: //derived classes can access the given data members
 int sensor_id;
 int num_bins;
public:
 Sensor(int id, int bins); //default constructor for base
 virtual ~Sensor(); // virtual destructor that helps clean up base and derived
 
 int getSensorId() const;
 virtual SensorReading generateReading() = 0; // Pure virtual given by =0, has to be defined for every derived class tho
}; // essentially and abstract class

Sensor::Sensor(int id, int bins) //defualt constructor and intializing
    : sensor_id(id), num_bins(bins){}
Sensor::~Sensor(){} // destructor

// read (constant doesnt change anything) get function, returns the id of the sensor
int Sensor::getSensorId()const{
    return sensor_id;
}

//Uniform sensor derived class
class UniformSensor : public Sensor  { //inheritence telling complier that the uniform sensor is derived
public:
 UniformSensor(int id, int bins);
 SensorReading generateReading() override; //replaces the base (review this)
};

//calling base sensor with id and bins
UniformSensor::UniformSensor(int id, int bins)
    :Sensor(id, bins){}

//return type   class owner     function  this is for equal chance of any bin
SensorReading UniformSensor:: generateReading(){
    int bin = custom_rand() % num_bins; //function given in the very top (instructions)
    return SensorReading(bin, sensor_id); 
}

//Biased sensor class (similar(not exact) implementation as uniform)

class BiasedSensor : public Sensor {
public:
 BiasedSensor(int id, int bins);
 SensorReading generateReading() override;
};

BiasedSensor:: BiasedSensor(int id, int bins)
    :Sensor(id, bins){}

//middle bins 
SensorReading BiasedSensor::generateReading(){
    int bin = (custom_rand() % (num_bins/2)) + (num_bins /4); 
    return SensorReading(bin, sensor_id); 
}






//PART 3: The entire experiment

class CollisionExperiment {
private:
 Sensor** sensors; // Array of pointers to sensors (double pointer) - pointer to an array of sensor pointers (diff types allowed as we use base in type definition)
 int num_sensors;
 int num_bins;
public:
 CollisionExperiment(int n_sensors, int n_bins, unsigned int seed);
 ~CollisionExperiment();
 
 void addSensor(int index, Sensor* s);
 bool runOnce();
 int runMultiple(int experiments);
};

// constructor to allocate an array of n_sensors that point to a sensor (intially null, just allocating memory)
CollisionExperiment:: CollisionExperiment (int n_sensors, int n_bins, unsigned int seed){

    num_sensors = n_sensors; // accessing the private data in the same class to store the total number of sensors
    num_bins = n_bins;  //store the bin amount

    sensors = new Sensor*[num_sensors]; // allocating memory for the array of pointers

    for (int i=0; i<num_sensors; i++){
        sensors[i]=nullptr;  //safety always good to use null in case
    }

    custom_srand(seed);  // setting the random seed
}

//destructor to ensure no memory leaks
CollisionExperiment:: ~CollisionExperiment(){
    for (int i=0; i<num_sensors; i++){
        delete sensors[i]; //deleting the contents of elements
    }
    delete [] sensors; //deleting entire array same way the memory was allocated 
}

// adding a sensor object into the array
void CollisionExperiment:: addSensor(int index, Sensor* s){ // s is a pointer (rmbr: just a definition)
    if ((index >=0) && (index < num_sensors) ){
        sensors[index] = s;
    }
}

// per 1 experiment
bool CollisionExperiment:: runOnce(){
    //need to dyanmically allocate (instructions), so we can create a new variable whose type is sensorreading
    //and store inside the readings the sensors generating
    //reason for it is cause is exactly the thing im comparing for collisions
    //what two readings hit in the same bin

    //allocating memory for the sensor readings
    SensorReading* previous_readings = new SensorReading[num_sensors]; //num_sensors given in collision class

    for (int i=0; i<num_sensors; i++){
        

        if (sensors[i] == nullptr){
            delete[]previous_readings;
            return false;  //safety incase not all sensors defined (just incase although its not written explicity in the assignment handout)
        }

        //calling the appropriate version of generating a reading happens auto cause derived are essentially the base
        SensorReading new_reading = sensors[i]->generateReading(); // pointer so using -> instead of . 

        //checking/comparing all sensors that already have a reading
        for (int b=0; b<i; b++){
            if (new_reading == previous_readings[b]){ //using the earlier defined overload '=='
                delete[] previous_readings; // deleting the temp array 
                return true; // collision is found :)
            }
        }
        previous_readings[i] = new_reading; // collisions not found so save the new reading as previous and reiterate a loop
    }
    delete [] previous_readings; // deleting the temp array
    return false; // no collisions found after all have been checked
}

//run the experiment as many times indicated (i.e. 100 in the assigned case)
int CollisionExperiment::runMultiple(int experiments){
    int count =0;  // to return the amount of experiments with 1+ collisions

    for (int i =0; i<experiments; i++){ // if a collision found increase the count by one (post)
        if (runOnce()){ // if the functions returns true
            count ++;
        }
    }
    return count; 
}




// PART 4 using polymorphism and object composition to reproduce results

int main (){
   
    /*SensorReading r1(42, 3); // Sensor 3 hit bin 42
    SensorReading r2(42, 7); // Sensor 7 also hit bin 42 - collision!
    SensorReading r3(10, 3); // Sensor 3 hit bin 10 (different reading)
    cout << r1 << endl; // Output: S3:B42
    cout << (r1 == r2) << endl; // Output: 1 (same bin = collision)
    cout << (r1 == r3) << endl; // Output: 0 (different bins)

    return 0; */

    //defining constants (better for safety indicated in textbook)
    const int N_SENSORS = 23;
    const int N_BINS = 365; 
    const int SEED = 12345; 
    const unsigned int N_EXPERIMENTS = 100;

    const int N_UNIFORM = 11; //uses the amount of uniform so if it changes its a quick and easy fix
    // helps to automatically determine number of biased

    //intializing the object experiment 
    CollisionExperiment experiment(N_SENSORS, N_BINS, SEED);

    //first n senseors are unfirom
    for (int i=0; i< N_UNIFORM; i++){
        experiment.addSensor(i, new UniformSensor (i, N_BINS));
    }

    //last sensors-n are biased
    for (int i=N_UNIFORM; i<N_SENSORS; i++){
        experiment.addSensor(i, new BiasedSensor(i, N_BINS));
    }

    //runs the experiment for the amount of times indicated
    int collisions = experiment.runMultiple(N_EXPERIMENTS);
    //calculates the rate of the collisions
    double rate = collisions / 100.0; 

    //output formated according to the assignment
    cout << "Sensors: " << N_SENSORS << endl;
    cout << "Bins: " << N_BINS << endl;
    cout << "Experiments: " << N_EXPERIMENTS << endl;
    cout << "Collisions: " << collisions << endl;
    cout << "Rate: " << rate << endl;

    return 0; // end of the program

    // the memory doesnt have to be deleted in the main function cause the class takes care of it
}







// for me
// compile it = g++ -std=c++17 -Wall asn1_sidhuk29_400578131.cpp -o asn1
// run it = ./asn1