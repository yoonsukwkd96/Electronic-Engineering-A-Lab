

// Change the cars and run_time variable to the desired values.
//Directions: north: 1, south: 2, west: 3, east: 4
#include <iostream>
#include <queue>
#include <algorithm>
#include <cstdlib> //for rand()
#include <ctime> //for clock(), clock_t, CLOCKS_PER_SEC
#include <unistd.h> // for usleep()
#include <pthread.h>

//#include "intra_common.hpp"
//#include "intra_std.hpp"

using namespace std;

const int cars = 100; // # cars all together
const int run_time = 5; // seconds the program will wait for to allow all cars to pass
int car_id; // global ID for each car arriving at the intersection
//int direction;
int k = 0;
int direction = 0;
int d = 0;

int NtoS_conflict[] = {}, NtoW_conflict[] = {}, NtoE_conflict[] = {}, NtoN_conflict[] = {};
int StoS_conflict[] = {}, StoW_conflict[] = {}, StoE_conflict[] = {}, StoN_conflict[] = {};
int WtoS_conflict[] = {}, WtoW_conflict[] = {}, WtoE_conflict[] = {}, WtoN_conflict[] = {};
int EtoS_conflict[] = {}, EtoW_conflict[] = {}, EtoE_conflict[] = {}, EtoN_conflict[] = {};

// a queue for each direction.
// queue<int> north_lane;
// queue<int> east_lane;
// queue<int> south_lane;
// queue<int> west_lane;
queue<int> intersection_lane;

queue<int> wd;
queue<int> ed;
queue<int> sd;
queue<int> nd;
// conflict queues (incoming direction to desired destination) e.g. from north to south is 12 (north: 1, south: 2, west: 3, east: 4)
queue<int> NtoN; // 11  0
queue<int> NtoS; // 12  1
queue<int> NtoW; // 13  2
queue<int> NtoE; // 14  3
queue<int> StoN; // 21  4
queue<int> StoS; // 22  5
queue<int> StoW; // 23  6
queue<int> StoE; // 24  7
queue<int> WtoN; // 31  8
queue<int> WtoS; // 32  9   
queue<int> WtoW; // 33  10
queue<int> WtoE; // 34  11
queue<int> EtoN; // 41  12
queue<int> EtoS; // 42  13
queue<int> EtoW; // 43  14
queue<int> EtoE; // 44  15

pthread_t threadID;

// a lock for each queue/direction.
// pthread_mutex_t northLock;
// pthread_mutex_t eastLock;
// pthread_mutex_t southLock;
// pthread_mutex_t westLock;
pthread_mutex_t intersectionLaneLock;
pthread_mutex_t wdLock;
pthread_mutex_t edLock;
pthread_mutex_t sdLock;
pthread_mutex_t ndLock;

pthread_mutex_t NtoSLock;
pthread_mutex_t NtoWLock;
pthread_mutex_t NtoELock;
pthread_mutex_t NtoNLock;
pthread_mutex_t StoSLock;
pthread_mutex_t StoWLock;
pthread_mutex_t StoELock;
pthread_mutex_t StoNLock;
pthread_mutex_t WtoSLock;
pthread_mutex_t WtoELock;
pthread_mutex_t WtoNLock;
pthread_mutex_t WtoWLock;
pthread_mutex_t EtoSLock;
pthread_mutex_t EtoWLock;
pthread_mutex_t EtoNLock;
pthread_mutex_t EtoELock;

pthread_mutex_t global_id_Lock; // lock for changing the globalid (i.e. car id)
//pthread_mutex_t intersectionLock; // lock for one car passing through the intersection

// sorting the array in ascending order
void sort_array(int *arr){
    /*Here we take two parameters, the beginning of the
    array and the length n upto which we want the array to
    be sorted*/
    int n = sizeof(arr) / sizeof(arr[0]);

    sort(arr, arr + n);
  
    // for (int i = 0; i < n; ++i)
    //     cout << arr[i] << " ";
}

// deletes first element from array and shifts the others
void delete_element_array(int *arr){
    int n = sizeof(arr) / sizeof(arr[0]);

    for (int i = 0; i < n; ++i){
        arr[i] = arr[i + 1]; // copy next element left
    }
}

void add_element_array (int *arr, int element){
    int n = sizeof(arr) / sizeof(arr[0]);

    arr[n-1] = element;
}

int getrand(int min,int max) //return a number x s.t. min <= x < max
{
	     return(rand_r()%(max-min)+min);
}

int directions (int dir, int kk){
    int dd = 0;
    if(dir==1 && kk==1){
        dd = 11;
        return dd;
    }else if(dir==1 && kk==2){
        dd = 12;
        return dd;
    }else if(dir==1 && kk==3){
        dd = 13;
        return dd;
    }else if(dir==1 && kk==4){
        dd = 14;
        return dd;
    }else if(dir==2 && kk==1){
        dd = 21;
        return dd;
    }else if(dir==2 && kk==2){
        dd = 22;
        return dd;
    }else if(dir==2 && kk==3){
        dd = 23;
        return dd;
    }else if (dir==2 && kk==4)
    {
        dd = 24;
        return dd;
    }else if (dir==3 && kk==1)
    {
        dd = 31;
        return dd;
    }else if (dir==3 && kk==2)
    {
        dd = 32;
        return dd;
    }else if (dir==3 && kk==3)
    {
        dd = 33;
        return dd;
    }else if (dir==3 && kk==4)
    {
        dd = 34;
        return dd;
    }else if(dir==4 && kk==1){
        dd = 41;
        return dd;
    }else if(dir==4 && kk==2){
        dd = 42;
        return dd;
    }else if(dir==4 && kk==3){
        dd = 43;
        return dd;
    }else if(dir==4 && kk==4){
        dd = 44;
        return dd;
    }
    return 0;
}

void init()
{
    car_id = 1; //first car will have ID = 1
    // pthread_mutex_init(&northLock, NULL);
    // pthread_mutex_init(&eastLock, NULL);
    // pthread_mutex_init(&southLock, NULL);
    // pthread_mutex_init(&westLock, NULL);
    pthread_mutex_init(&intersectionLaneLock, NULL);
    
    //pthread_mutex_init(&intersectionLock, NULL);
    pthread_mutex_init(&wdLock, NULL);
    pthread_mutex_init(&edLock, NULL);
    pthread_mutex_init(&sdLock, NULL);
    pthread_mutex_init(&edLock, NULL);

    pthread_mutex_init(&global_id_Lock, NULL);

    pthread_mutex_init(&NtoSLock, NULL);
    pthread_mutex_init(&NtoELock, NULL);
    pthread_mutex_init(&NtoWLock, NULL);
    pthread_mutex_init(&NtoNLock, NULL);
    pthread_mutex_init(&StoELock, NULL);
    pthread_mutex_init(&StoSLock, NULL);
    pthread_mutex_init(&StoWLock, NULL);
    pthread_mutex_init(&StoNLock, NULL);
    pthread_mutex_init(&WtoSLock, NULL);
    pthread_mutex_init(&WtoELock, NULL);
    pthread_mutex_init(&WtoNLock, NULL);
    pthread_mutex_init(&WtoWLock, NULL);
    pthread_mutex_init(&EtoSLock, NULL);
    pthread_mutex_init(&EtoWLock, NULL);
    pthread_mutex_init(&EtoNLock, NULL);
    pthread_mutex_init(&EtoELock, NULL);


}

// This is a function for a car moving from north to south.
void *northToSouth(void *null){
    int localID;
    double duration; //for checking how long a car will be waiting at the front of its lane
    clock_t start;

    pthread_mutex_lock(&NtoSLock); // locking the queue
    pthread_mutex_lock(&global_id_Lock); // locking globalIDLock mutex in order to update globalID
    localID = car_id++; // update globalID after reserving that ID for a car in NtoS
    pthread_mutex_unlock(&global_id_Lock);
    NtoS.push(localID); // pushing the local car into NtoS.
    pthread_mutex_unlock(&NtoSLock);

    add_element_array(EtoW_conflict, localID);
    sort_array(EtoW_conflict);
    add_element_array(EtoS_conflict, localID);
    sort_array(EtoS_conflict);
    add_element_array(WtoE_conflict, localID);
    sort_array(WtoE_conflict);
    add_element_array(WtoS_conflict, localID);
    sort_array(WtoS_conflict);
    add_element_array(WtoW_conflict, localID);
    sort_array(WtoW_conflict);
    add_element_array(WtoN_conflict, localID);
    sort_array(WtoN_conflict);
    add_element_array(StoW_conflict, localID);
    sort_array(StoW_conflict);
    add_element_array(StoS_conflict, localID);
    sort_array(StoS_conflict);

    while(1){
        if(EtoW_conflict[0] == localID && EtoS_conflict[0] == localID && WtoE_conflict[0] == localID && WtoS_conflict[0] == localID && WtoW_conflict[0] == localID 
            && WtoN_conflict[0] == localID && StoW_conflict[0] == localID && StoS_conflict[0] == localID){
                sleep(5); //say it takes 5 seconds to get to the intersection
                pthread_mutex_lock(&intersectionLaneLock);
                intersection_lane.push(localID);
                cout << "Car from NORTH lane with ID: " << localID << " ENTERING the intersection." << endl;
                pthread_mutex_unlock(&intersectionLaneLock);
                start = clock(); // Now the car has arrived at intersection. Let's start the timer.

                while(1) //Checking cars properties here
                {
                    if ((localID == NtoS.front()) && (localID == intersection_lane.front())) // Current Car is in the front of the lane... Lets Proceed
                    {
                        break;
                    }
                    else //Current car is not in front on its lane. Lets wait
                    {
                        usleep(2); // sleep for 2 microsecond to allow for other cars to proceed if they must
                        continue;
                    }
                }
            
                // Car is in the front so let's proceed to allow it to pass through intersection.
                pthread_mutex_lock(&sdLock); // need to lock the intersection. Function call will block until mutex is available
                sd.push(localID);
                cout << "Car from the NORTH lane with ID: " << localID << " ENTERING the SOUTH LANE." << endl;
                duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
                NtoS.pop();
                intersection_lane.pop();
                delete_element_array(EtoW_conflict);
                delete_element_array(EtoS_conflict);
                delete_element_array(WtoE_conflict);
                delete_element_array(WtoS_conflict);
                delete_element_array(WtoW_conflict);
                delete_element_array(WtoN_conflict);
                delete_element_array(StoW_conflict);
                delete_element_array(StoS_conflict);
                cout << "It has been waiting for the permission for: "<< duration<<" seconds."<<endl;
                cout << "Car from NORTH lane is heading towards SOUTH direction." << endl;
                sleep(3);
                sd.pop();
                cout << "Car from NORTH lane with ID: " << localID << " LEAVING the intersection." << endl<<endl;
                pthread_mutex_unlock(&sdLock);

                break;
        }else{
            usleep(2);
            continue;
        }

    }    
}

// This is a function for a car moving north to west.
void *northToWest(void *null)
{

    int localID;
    double duration; //for checking how long a car will be waiting at the front of its lane
    clock_t start;

    pthread_mutex_lock(&NtoWLock); // locking the queue
    pthread_mutex_lock(&global_id_Lock); // locking globalIDLock mutex in order to update globalID
    localID = car_id++; // update globalID after reserving that ID for a car in NtoS
    pthread_mutex_unlock(&global_id_Lock);
    NtoW.push(localID); // pushing the local car into NtoW.
    pthread_mutex_unlock(&NtoWLock);

    add_element_array(EtoW_conflict, localID);
    sort_array(EtoW_conflict);
    add_element_array(WtoW_conflict, localID);
    sort_array(WtoW_conflict);
    add_element_array(StoW_conflict, localID);
    sort_array(StoW_conflict);

    while(1){
        if(EtoW_conflict[0] == localID && WtoW_conflict[0] == localID && StoW_conflict[0] == localID){
                sleep(5); //say it takes 5 seconds to get to the intersection
                pthread_mutex_lock(&intersectionLaneLock);
                intersection_lane.push(localID);
                cout << "Car from NORTH lane with ID: " << localID << " ENTERING the intersection." << endl;
                pthread_mutex_unlock(&intersectionLaneLock);
                start = clock(); // Now the car has arrived at intersection. Let's start the timer.

                while(1) //Checking cars properties here
                {
                    if ((localID == NtoW.front()) && (localID == intersection_lane.front())) // Current Car is in the front of the lane... Lets Proceed
                    {
                        break;
                    }
                    else //Current car is not in front on its lane. Lets wait
                    {
                        usleep(2); // sleep for 2 microsecond to allow for other cars to proceed if they must
                        continue;
                    }
                }
            
                // Car is in the front so let's proceed to allow it to pass through intersection.
                pthread_mutex_lock(&wdLock); // need to lock the intersection. Function call will block until mutex is available
                wd.push(localID);
                cout << "Car from the NORTH lane with ID: " << localID << " ENTERING the WEST LANE." << endl;
                duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
                NtoW.pop();
                intersection_lane.pop();
                delete_element_array(EtoW_conflict);
                delete_element_array(WtoW_conflict);
                delete_element_array(StoW_conflict);
                cout << "It has been waiting for the permission for: "<< duration<<" seconds."<<endl;
                cout << "Car from NORTH lane is heading towards WEST direction." << endl;
                sleep(3);
                wd.pop();
                cout << "Car from NORTH lane with ID: " << localID << " LEAVING the intersection." << endl<<endl;
                pthread_mutex_unlock(&wdLock);

                break;
        }else{
            usleep(2);
            continue;
        }
    }
}

// This is a function for a car moving north to east.
void *northToEast(void *null)
{

    int localID;
    double duration; //for checking how long a car will be waiting at the front of its lane
    clock_t start;

    pthread_mutex_lock(&NtoELock); // locking the queue
    pthread_mutex_lock(&global_id_Lock); // locking globalIDLock mutex in order to update globalID
    localID = car_id++; // update globalID after reserving that ID for a car in NtoS
    pthread_mutex_unlock(&global_id_Lock);
    NtoE.push(localID); // pushing the local car into NtoE.
    pthread_mutex_unlock(&NtoELock);

    add_element_array(StoN_conflict, localID);
    sort_array(StoN_conflict);
    add_element_array(StoW_conflict, localID);
    sort_array(StoW_conflict);
    add_element_array(StoE_conflict, localID);
    sort_array(StoE_conflict);
    add_element_array(StoS_conflict, localID);
    sort_array(StoS_conflict);
    add_element_array(EtoW_conflict, localID);
    sort_array(EtoW_conflict);
    add_element_array(EtoE_conflict, localID);
    sort_array(EtoE_conflict);
    add_element_array(EtoS_conflict, localID);
    sort_array(EtoS_conflict);
    add_element_array(WtoE_conflict, localID);
    sort_array(WtoE_conflict);
    add_element_array(WtoW_conflict, localID);
    sort_array(WtoW_conflict);
    add_element_array(WtoN_conflict, localID);
    sort_array(WtoN_conflict);

    while(1){
        if(StoN_conflict[0] == localID && StoW_conflict[0] == localID && StoE_conflict[0] == localID && StoS_conflict[0] == localID && EtoW_conflict[0] == localID && 
            EtoE_conflict[0] == localID && EtoS_conflict[0] == localID && WtoE_conflict[0] == localID && WtoW_conflict[0] == localID && WtoN_conflict[0] == localID){
                sleep(5); //say it takes 5 seconds to get to the intersection
                pthread_mutex_lock(&intersectionLaneLock);
                intersection_lane.push(localID);
                cout << "Car from NORTH lane with ID: " << localID << " ENTERING the intersection." << endl;
                pthread_mutex_unlock(&intersectionLaneLock);
                start = clock(); // Now the car has arrived at intersection. Let's start the timer.

                while(1) //Checking cars properties here
                {
                    if ((localID == NtoE.front()) && (localID == intersection_lane.front())) // Current Car is in the front of the lane... Lets Proceed
                    {
                        break;
                    }
                    else //Current car is not in front on its lane. Lets wait
                    {
                        usleep(2); // sleep for 2 microsecond to allow for other cars to proceed if they must
                        continue;
                    }
                }
            
                // Car is in the front so let's proceed to allow it to pass through intersection.
                pthread_mutex_lock(&edLock); // need to lock the intersection. Function call will block until mutex is available
                ed.push(localID);
                cout << "Car from the NORTH lane with ID: " << localID << " ENTERING the EAST LANE." << endl;
                duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
                NtoE.pop();
                intersection_lane.pop();
                delete_element_array(StoN_conflict);
                delete_element_array(StoW_conflict);
                delete_element_array(StoE_conflict);
                delete_element_array(StoS_conflict);
                delete_element_array(EtoW_conflict);
                delete_element_array(EtoE_conflict);
                delete_element_array(EtoS_conflict);
                delete_element_array(WtoE_conflict);
                delete_element_array(WtoW_conflict);
                delete_element_array(WtoN_conflict);
                cout << "It has been waiting for the permission for: "<< duration<<" seconds."<<endl;
                cout << "Car from NORTH lane is heading towards EAST direction." << endl;
                sleep(3);
                ed.pop();
                cout << "Car from NORTH lane with ID: " << localID << " LEAVING the intersection." << endl<<endl;
                pthread_mutex_unlock(&edLock);

                break;
        }else{
            usleep(2);
            continue;
        }
    }
}

// This is a function for a car moving north to north.
void *northToNorth(void *null)
{

    int localID;
    double duration; //for checking how long a car will be waiting at the front of its lane
    clock_t start;

    pthread_mutex_lock(&NtoNLock); // locking the queue
    pthread_mutex_lock(&global_id_Lock); // locking globalIDLock mutex in order to update globalID
    localID = car_id++; // update globalID after reserving that ID for a car in NtoS
    pthread_mutex_unlock(&global_id_Lock);
    NtoN.push(localID); // pushing the local car into NtoN.
    pthread_mutex_unlock(&NtoNLock);

    add_element_array(StoN_conflict, localID);
    sort_array(StoN_conflict);
    add_element_array(EtoN_conflict, localID);
    sort_array(EtoN_conflict);
    add_element_array(EtoW_conflict, localID);
    sort_array(EtoW_conflict);
    add_element_array(WtoN_conflict, localID);
    sort_array(WtoN_conflict);
    add_element_array(WtoW_conflict, localID);
    sort_array(WtoW_conflict);

    while(1){
        if(StoN_conflict[0] == localID && EtoN_conflict[0] == localID && EtoW_conflict[0] == localID && WtoN_conflict[0] == localID && WtoW_conflict[0] == localID){
                sleep(5); //say it takes 5 seconds to get to the intersection
                pthread_mutex_lock(&intersectionLaneLock);
                intersection_lane.push(localID);
                cout << "Car from NORTH lane with ID: " << localID << " ENTERING the intersection." << endl;
                pthread_mutex_unlock(&intersectionLaneLock);
                start = clock(); // Now the car has arrived at intersection. Let's start the timer.

                while(1) //Checking cars properties here
                {
                    if ((localID == NtoN.front()) && (localID == intersection_lane.front())) // Current Car is in the front of the lane... Lets Proceed
                    {
                        break;
                    }
                    else //Current car is not in front on its lane. Lets wait
                    {
                        usleep(2); // sleep for 2 microsecond to allow for other cars to proceed if they must
                        continue;
                    }
                }
            
                // Car is in the front so let's proceed to allow it to pass through intersection.
                pthread_mutex_lock(&ndLock); // need to lock the intersection. Function call will block until mutex is available
                nd.push(localID);
                cout << "Car from the NORTH lane with ID: " << localID << " ENTERING the NORTH LANE." << endl;
                duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
                NtoN.pop();
                intersection_lane.pop();
                delete_element_array(StoN_conflict);
                delete_element_array(EtoN_conflict);
                delete_element_array(EtoW_conflict);
                delete_element_array(WtoN_conflict);
                delete_element_array(WtoW_conflict);
                cout << "It has been waiting for the permission for: "<< duration<<" seconds."<<endl;
                cout << "Car from NORTH lane is heading towards NORTH direction." << endl;
                sleep(3);
                nd.pop();
                cout << "Car from NORTH lane with ID: " << localID << " LEAVING the intersection." << endl<<endl;
                pthread_mutex_unlock(&ndLock);

                break;
        }else{
            usleep(2);
            continue;
        }
    }
}

// This is a function for a car moving south to north.
void *southToNorth(void *null)
{

    int localID;
    double duration; //for checking how long a car will be waiting at the front of its lane
    clock_t start;

    pthread_mutex_lock(&StoNLock); // locking the queue
    pthread_mutex_lock(&global_id_Lock); // locking globalIDLock mutex in order to update globalID
    localID = car_id++; // update globalID after reserving that ID for a car in NtoS
    pthread_mutex_unlock(&global_id_Lock);
    StoN.push(localID); // pushing the local car into StoN.
    pthread_mutex_unlock(&StoNLock);

    add_element_array(WtoE_conflict, localID);
    sort_array(WtoE_conflict);
    add_element_array(WtoN_conflict, localID);
    sort_array(WtoN_conflict);
    add_element_array(EtoN_conflict, localID);
    sort_array(EtoN_conflict);
    add_element_array(EtoW_conflict, localID);
    sort_array(EtoW_conflict);
    add_element_array(EtoE_conflict, localID);
    sort_array(EtoE_conflict);
    add_element_array(EtoS_conflict, localID);
    sort_array(EtoS_conflict);
    add_element_array(NtoE_conflict, localID);
    sort_array(NtoE_conflict);
    add_element_array(NtoN_conflict, localID);
    sort_array(NtoN_conflict);

    while(1){
        if(WtoE_conflict[0] == localID && WtoN_conflict[0] == localID && EtoN_conflict[0] == localID && EtoW_conflict[0] == localID && EtoE_conflict[0] == localID && 
            EtoS_conflict[0] == localID && NtoE_conflict[0] == localID && NtoN_conflict[0] == localID){
                sleep(5); //say it takes 5 seconds to get to the intersection
                pthread_mutex_lock(&intersectionLaneLock);
                intersection_lane.push(localID);
                cout << "Car from SOUTH lane with ID: " << localID << " ENTERING the intersection." << endl;
                pthread_mutex_unlock(&intersectionLaneLock);
                start = clock(); // Now the car has arrived at intersection. Let's start the timer.

                while(1) //Checking cars properties here
                {
                    if ((localID == StoN.front()) && (localID == intersection_lane.front())) // Current Car is in the front of the lane... Lets Proceed
                    {
                        break;
                    }
                    else //Current car is not in front on its lane. Lets wait
                    {
                        usleep(2); // sleep for 2 microsecond to allow for other cars to proceed if they must
                        continue;
                    }
                }
            
                // Car is in the front so let's proceed to allow it to pass through intersection.
                pthread_mutex_lock(&ndLock); // need to lock the intersection. Function call will block until mutex is available
                nd.push(localID);
                cout << "Car from the SOUTH lane with ID: " << localID << " ENTERING the NORTH LANE." << endl;
                duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
                StoN.pop();
                intersection_lane.pop();
                delete_element_array(WtoE_conflict);
                delete_element_array(WtoN_conflict);
                delete_element_array(EtoW_conflict);
                delete_element_array(EtoN_conflict);
                delete_element_array(EtoE_conflict);
                delete_element_array(EtoS_conflict);
                delete_element_array(NtoE_conflict);
                delete_element_array(NtoN_conflict);
                cout << "It has been waiting for the permission for: "<< duration<<" seconds."<<endl;
                cout << "Car from SOUTH lane is heading towards NORTH direction." << endl;
                sleep(3);
                nd.pop();
                cout << "Car from SOUTH lane with ID: " << localID << " LEAVING the intersection." << endl<<endl;
                pthread_mutex_unlock(&ndLock);

                break;
        }else{
            usleep(2);
            continue;
        }
    }
}

// This is a function for a car moving south to west.
void *southToWest(void *null)
{

    int localID;
    double duration; //for checking how long a car will be waiting at the front of its lane
    clock_t start;

    pthread_mutex_lock(&StoWLock); // locking the queue
    pthread_mutex_lock(&global_id_Lock); // locking globalIDLock mutex in order to update globalID
    localID = car_id++; // update globalID after reserving that ID for a car in NtoW
    pthread_mutex_unlock(&global_id_Lock);
    StoW.push(localID); // pushing the local car into StoN.
    pthread_mutex_unlock(&StoWLock);

    add_element_array(NtoS_conflict, localID);
    sort_array(NtoS_conflict);
    add_element_array(NtoE_conflict, localID);
    sort_array(NtoE_conflict);
    add_element_array(NtoW_conflict, localID);
    sort_array(NtoW_conflict);
    add_element_array(NtoN_conflict, localID);
    sort_array(NtoN_conflict);
    add_element_array(WtoE_conflict, localID);
    sort_array(WtoE_conflict);
    add_element_array(WtoW_conflict, localID);
    sort_array(WtoW_conflict);
    add_element_array(WtoN_conflict, localID);
    sort_array(WtoN_conflict);
    add_element_array(EtoW_conflict, localID);
    sort_array(EtoW_conflict);
    add_element_array(EtoE_conflict, localID);
    sort_array(EtoE_conflict);
    add_element_array(EtoS_conflict, localID);
    sort_array(EtoS_conflict);

    while(1){
        if(NtoS_conflict[0] == localID && NtoE_conflict[0] == localID && NtoW_conflict[0] == localID && NtoN_conflict[0] == localID && WtoE_conflict[0] == localID && 
            WtoW_conflict[0] == localID && WtoN_conflict[0] == localID && EtoW_conflict[0] == localID && EtoE_conflict[0] == localID && EtoS_conflict[0] == localID){
                sleep(5); //say it takes 5 seconds to get to the intersection
                pthread_mutex_lock(&intersectionLaneLock);
                intersection_lane.push(localID);
                cout << "Car from SOUTH lane with ID: " << localID << " ENTERING the intersection." << endl;
                pthread_mutex_unlock(&intersectionLaneLock);
                start = clock(); // Now the car has arrived at intersection. Let's start the timer.

                while(1) //Checking cars properties here
                {
                    if ((localID == StoW.front()) && (localID == intersection_lane.front())) // Current Car is in the front of the lane... Lets Proceed
                    {
                        break;
                    }
                    else //Current car is not in front on its lane. Lets wait
                    {
                        usleep(2); // sleep for 2 microsecond to allow for other cars to proceed if they must
                        continue;
                    }
                }
            
                // Car is in the front so let's proceed to allow it to pass through intersection.
                pthread_mutex_lock(&wdLock); // need to lock the intersection. Function call will block until mutex is available
                wd.push(localID);
                cout << "Car from the SOUTH lane with ID: " << localID << " ENTERING the WEST LANE." << endl;
                duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
                StoW.pop();
                intersection_lane.pop();
                delete_element_array(NtoS_conflict);
                delete_element_array(NtoE_conflict);
                delete_element_array(NtoW_conflict);
                delete_element_array(NtoN_conflict);
                delete_element_array(WtoE_conflict);
                delete_element_array(WtoW_conflict);
                delete_element_array(WtoN_conflict);
                delete_element_array(EtoW_conflict);
                delete_element_array(EtoE_conflict);
                delete_element_array(EtoS_conflict);
                cout << "It has been waiting for the permission for: "<< duration<<" seconds."<<endl;
                cout << "Car from SOUTH lane is heading towards WEST direction." << endl;
                sleep(3);
                wd.pop();
                cout << "Car from SOUTH lane with ID: " << localID << " LEAVING the intersection." << endl<<endl;
                pthread_mutex_unlock(&wdLock);

                break;
        }else{
            usleep(2);
            continue;
        }
    }
}

// This is a function for a car moving south to east.
void *southToEast(void *null)
{

    int localID;
    double duration; //for checking how long a car will be waiting at the front of its lane
    clock_t start;

    pthread_mutex_lock(&StoELock); // locking the queue
    pthread_mutex_lock(&global_id_Lock); // locking globalIDLock mutex in order to update globalID
    localID = car_id++; // update globalID after reserving that ID for a car in StoE
    pthread_mutex_unlock(&global_id_Lock);
    StoE.push(localID); // pushing the local car into StoE.
    pthread_mutex_unlock(&StoELock);

    add_element_array(WtoE_conflict, localID);
    sort_array(WtoE_conflict);
    add_element_array(EtoE_conflict, localID);
    sort_array(EtoE_conflict);
    add_element_array(NtoE_conflict, localID);
    sort_array(NtoE_conflict);

    while(1){
        if(WtoE_conflict[0] == localID && EtoE_conflict[0] == localID && NtoE_conflict[0] == localID){
                sleep(5); //say it takes 5 seconds to get to the intersection
                pthread_mutex_lock(&intersectionLaneLock);
                intersection_lane.push(localID);
                cout << "Car from SOUTH lane with ID: " << localID << " ENTERING the intersection." << endl;
                pthread_mutex_unlock(&intersectionLaneLock);
                start = clock(); // Now the car has arrived at intersection. Let's start the timer.

                while(1) //Checking cars properties here
                {
                    if ((localID == StoE.front()) && (localID == intersection_lane.front())) // Current Car is in the front of the lane... Lets Proceed
                    {
                        break;
                    }
                    else //Current car is not in front on its lane. Lets wait
                    {
                        usleep(2); // sleep for 2 microsecond to allow for other cars to proceed if they must
                        continue;
                    }
                }
            
                // Car is in the front so let's proceed to allow it to pass through intersection.
                pthread_mutex_lock(&edLock); // need to lock the intersection. Function call will block until mutex is available
                ed.push(localID);
                cout << "Car from the SOUTH lane with ID: " << localID << " ENTERING the EAST LANE." << endl;
                duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
                StoE.pop();
                intersection_lane.pop();
                delete_element_array(WtoE_conflict);
                delete_element_array(EtoE_conflict);
                delete_element_array(NtoE_conflict);
                cout << "It has been waiting for the permission for: "<< duration<<" seconds."<<endl;
                cout << "Car from SOUTH lane is heading towards EAST direction." << endl;
                sleep(3);
                ed.pop();
                cout << "Car from SOUTH lane with ID: " << localID << " LEAVING the intersection." << endl<<endl;
                pthread_mutex_unlock(&edLock);

                break;
        }else{
            usleep(2);
            continue;
        }
    }
}

// This is a function for a car moving south to south.
void *southToSouth(void *null)
{

    int localID;
    double duration; //for checking how long a car will be waiting at the front of its lane
    clock_t start;

    pthread_mutex_lock(&StoSLock); // locking the queue
    pthread_mutex_lock(&global_id_Lock); // locking globalIDLock mutex in order to update globalID
    localID = car_id++; // update globalID after reserving that ID for a car in StoS
    pthread_mutex_unlock(&global_id_Lock);
    StoS.push(localID); // pushing the local car into StoN.
    pthread_mutex_unlock(&StoSLock);

    add_element_array(NtoS_conflict, localID);
    sort_array(NtoS_conflict);
    add_element_array(WtoS_conflict, localID);
    sort_array(WtoS_conflict);
    add_element_array(WtoE_conflict, localID);
    sort_array(WtoE_conflict);
    add_element_array(EtoS_conflict, localID);
    sort_array(EtoS_conflict);
    add_element_array(EtoE_conflict, localID);
    sort_array(EtoE_conflict);

    while(1){
        if(NtoS_conflict[0] == localID && WtoS_conflict[0] == localID && WtoE_conflict[0] == localID && EtoS_conflict[0] == localID && EtoE_conflict[0] == localID){
                sleep(5); //say it takes 5 seconds to get to the intersection
                pthread_mutex_lock(&intersectionLaneLock);
                intersection_lane.push(localID);
                cout << "Car from SOUTH lane with ID: " << localID << " ENTERING the intersection." << endl;
                pthread_mutex_unlock(&intersectionLaneLock);
                start = clock(); // Now the car has arrived at intersection. Let's start the timer.

                while(1) //Checking cars properties here
                {
                    if ((localID == StoS.front()) && (localID == intersection_lane.front())) // Current Car is in the front of the lane... Lets Proceed
                    {
                        break;
                    }
                    else //Current car is not in front on its lane. Lets wait
                    {
                        usleep(2); // sleep for 2 microsecond to allow for other cars to proceed if they must
                        continue;
                    }
                }
            
                // Car is in the front so let's proceed to allow it to pass through intersection.
                pthread_mutex_lock(&sdLock); // need to lock the intersection. Function call will block until mutex is available
                sd.push(localID);
                cout << "Car from the SOUTH lane with ID: " << localID << " ENTERING the SOUTH LANE." << endl;
                duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
                StoS.pop();
                intersection_lane.pop();
                delete_element_array(NtoS_conflict);
                delete_element_array(WtoS_conflict);
                delete_element_array(WtoE_conflict);
                delete_element_array(EtoS_conflict);
                delete_element_array(EtoE_conflict);
                cout << "It has been waiting for the permission for: "<< duration<<" seconds."<<endl;
                cout << "Car from SOUTH lane is heading towards SOUTH direction." << endl;
                sleep(3);
                sd.pop();
                cout << "Car from SOUTH lane with ID: " << localID << " LEAVING the intersection." << endl<<endl;
                pthread_mutex_unlock(&sdLock);

                break;
        }else{
            usleep(2);
            continue;
        }
    }
}

// This is a function for a car moving west to east.
void *westToEast(void *null)
{

    int localID;
    double duration; //for checking how long a car will be waiting at the front of its lane
    clock_t start;

    pthread_mutex_lock(&WtoELock); // locking the queue
    pthread_mutex_lock(&global_id_Lock); // locking globalIDLock mutex in order to update globalID
    localID = car_id++; // update globalID after reserving that ID for a car in WtoE
    pthread_mutex_unlock(&global_id_Lock);
    WtoE.push(localID); // pushing the local car into StoN.
    pthread_mutex_unlock(&WtoELock);

    add_element_array(NtoS_conflict, localID);
    sort_array(NtoS_conflict);
    add_element_array(NtoE_conflict, localID);
    sort_array(NtoE_conflict);
    add_element_array(StoN_conflict, localID);
    sort_array(StoN_conflict);
    add_element_array(StoE_conflict, localID);
    sort_array(StoE_conflict);
    add_element_array(StoS_conflict, localID);
    sort_array(StoS_conflict);
    add_element_array(StoW_conflict, localID);
    sort_array(StoW_conflict);
    add_element_array(EtoS_conflict, localID);
    sort_array(EtoS_conflict);
    add_element_array(EtoE_conflict, localID);
    sort_array(EtoE_conflict);

    while(1){
        if(NtoS_conflict[0] == localID && NtoE_conflict[0] == localID && StoN_conflict[0] == localID && StoE_conflict[0] == localID && StoS_conflict[0] == localID && 
            StoW_conflict[0] == localID && EtoS_conflict[0] == localID && EtoE_conflict[0] == localID){
                sleep(5); //say it takes 5 seconds to get to the intersection
                pthread_mutex_lock(&intersectionLaneLock);
                intersection_lane.push(localID);
                cout << "Car from WEST lane with ID: " << localID << " ENTERING the intersection." << endl;
                pthread_mutex_unlock(&intersectionLaneLock);
                start = clock(); // Now the car has arrived at intersection. Let's start the timer.

                while(1) //Checking cars properties here
                {
                    if ((localID == WtoE.front()) && (localID == intersection_lane.front())) // Current Car is in the front of the lane... Lets Proceed
                    {
                        break;
                    }
                    else //Current car is not in front on its lane. Lets wait
                    {
                        usleep(2); // sleep for 2 microsecond to allow for other cars to proceed if they must
                        continue;
                    }
                }
            
                // Car is in the front so let's proceed to allow it to pass through intersection.
                pthread_mutex_lock(&edLock); // need to lock the intersection. Function call will block until mutex is available
                ed.push(localID);
                cout << "Car from the WEST lane with ID: " << localID << " ENTERING the EAST LANE." << endl;
                duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
                WtoE.pop();
                intersection_lane.pop();
                delete_element_array(NtoS_conflict);
                delete_element_array(NtoE_conflict);
                delete_element_array(StoN_conflict);
                delete_element_array(StoE_conflict);
                delete_element_array(StoS_conflict);
                delete_element_array(StoW_conflict);
                delete_element_array(EtoS_conflict);
                delete_element_array(EtoE_conflict);
                cout << "It has been waiting for the permission for: "<< duration<<" seconds."<<endl;
                cout << "Car from WEST lane is heading towards EAST direction." << endl;
                sleep(3);
                ed.pop();
                cout << "Car from WEST lane with ID: " << localID << " LEAVING the intersection." << endl<<endl;
                pthread_mutex_unlock(&edLock);

                break;
        }else{
            usleep(2);
            continue;
        }
    }
}

// This is a function for a car moving west to south.
void *westToSouth(void *null)
{

    int localID;
    double duration; //for checking how long a car will be waiting at the front of its lane
    clock_t start;

    pthread_mutex_lock(&WtoSLock); // locking the queue
    pthread_mutex_lock(&global_id_Lock); // locking globalIDLock mutex in order to update globalID
    localID = car_id++; // update globalID after reserving that ID for a car in WtoS
    pthread_mutex_unlock(&global_id_Lock);
    WtoS.push(localID); // pushing the local car into StoN.
    pthread_mutex_unlock(&WtoSLock);

    add_element_array(NtoS_conflict, localID);
    sort_array(NtoS_conflict);
    add_element_array(StoS_conflict, localID);
    sort_array(StoS_conflict);
    add_element_array(EtoS_conflict, localID);
    sort_array(EtoS_conflict);

    while(1){
        if(NtoS_conflict[0] == localID && StoS_conflict[0] == localID && EtoS_conflict[0] == localID){
                sleep(5); //say it takes 5 seconds to get to the intersection
                pthread_mutex_lock(&intersectionLaneLock);
                intersection_lane.push(localID);
                cout << "Car from WEST lane with ID: " << localID << " ENTERING the intersection." << endl;
                pthread_mutex_unlock(&intersectionLaneLock);
                start = clock(); // Now the car has arrived at intersection. Let's start the timer.

                while(1) //Checking cars properties here
                {
                    if ((localID == WtoS.front()) && (localID == intersection_lane.front())) // Current Car is in the front of the lane... Lets Proceed
                    {
                        break;
                    }
                    else //Current car is not in front on its lane. Lets wait
                    {
                        usleep(2); // sleep for 2 microsecond to allow for other cars to proceed if they must
                        continue;
                    }
                }
            
                // Car is in the front so let's proceed to allow it to pass through intersection.
                pthread_mutex_lock(&sdLock); // need to lock the intersection. Function call will block until mutex is available
                sd.push(localID);
                cout << "Car from the WEST lane with ID: " << localID << " ENTERING the SOUTH LANE." << endl;
                duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
                WtoS.pop();
                intersection_lane.pop();
                delete_element_array(NtoS_conflict);
                delete_element_array(StoS_conflict);
                delete_element_array(EtoS_conflict);
                cout << "It has been waiting for the permission for: "<< duration<<" seconds."<<endl;
                cout << "Car from WEST lane is heading towards SOUTH direction." << endl;
                sleep(3);
                sd.pop();
                cout << "Car from WEST lane with ID: " << localID << " LEAVING the intersection." << endl<<endl;
                pthread_mutex_unlock(&sdLock);

                break;
        }else{
            usleep(2);
            continue;
        }
    }
}

// This is a function for a car moving west to north.
void *westToNorth(void *null)
{

    int localID;
    double duration; //for checking how long a car will be waiting at the front of its lane
    clock_t start;

    pthread_mutex_lock(&WtoNLock); // locking the queue
    pthread_mutex_lock(&global_id_Lock); // locking globalIDLock mutex in order to update globalID
    localID = car_id++; // update globalID after reserving that ID for a car in WtoN
    pthread_mutex_unlock(&global_id_Lock);
    WtoN.push(localID); // pushing the local car into StoN.
    pthread_mutex_unlock(&WtoNLock);

    add_element_array(EtoW_conflict, localID);
    sort_array(EtoW_conflict);
    add_element_array(EtoS_conflict, localID);
    sort_array(EtoS_conflict);
    add_element_array(EtoN_conflict, localID);
    sort_array(EtoN_conflict);
    add_element_array(EtoE_conflict, localID);
    sort_array(EtoE_conflict);
    add_element_array(NtoS_conflict, localID);
    sort_array(NtoS_conflict);
    add_element_array(NtoN_conflict, localID);
    sort_array(NtoN_conflict);
    add_element_array(NtoE_conflict, localID);
    sort_array(NtoE_conflict);
    add_element_array(StoN_conflict, localID);
    sort_array(StoN_conflict);
    add_element_array(StoS_conflict, localID);
    sort_array(StoS_conflict);
    add_element_array(StoW_conflict, localID);
    sort_array(StoW_conflict);

    while(1){
        if(EtoW_conflict[0] == localID && EtoS_conflict[0] == localID && EtoN_conflict[0] == localID && EtoE_conflict[0] == localID && NtoS_conflict[0] == localID && 
            NtoN_conflict[0] == localID && NtoE_conflict[0] == localID && StoN_conflict[0] == localID && StoS_conflict[0] == localID && StoW_conflict[0] == localID){
                sleep(5); //say it takes 5 seconds to get to the intersection
                pthread_mutex_lock(&intersectionLaneLock);
                intersection_lane.push(localID);
                cout << "Car from WEST lane with ID: " << localID << " ENTERING the intersection." << endl;
                pthread_mutex_unlock(&intersectionLaneLock);
                start = clock(); // Now the car has arrived at intersection. Let's start the timer.

                while(1) //Checking cars properties here
                {
                    if ((localID == WtoN.front()) && (localID == intersection_lane.front())) // Current Car is in the front of the lane... Lets Proceed
                    {
                        break;
                    }
                    else //Current car is not in front on its lane. Lets wait
                    {
                        usleep(2); // sleep for 2 microsecond to allow for other cars to proceed if they must
                        continue;
                    }
                }
            
                // Car is in the front so let's proceed to allow it to pass through intersection.
                pthread_mutex_lock(&ndLock); // need to lock the intersection. Function call will block until mutex is available
                nd.push(localID);
                cout << "Car from the WEST lane with ID: " << localID << " ENTERING the NORTH LANE." << endl;
                duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
                WtoN.pop();
                intersection_lane.pop();
                delete_element_array(EtoW_conflict);
                delete_element_array(EtoS_conflict);
                delete_element_array(EtoN_conflict);
                delete_element_array(EtoE_conflict);
                delete_element_array(NtoS_conflict);
                delete_element_array(NtoN_conflict);
                delete_element_array(NtoE_conflict);
                delete_element_array(StoN_conflict);
                delete_element_array(StoS_conflict);
                delete_element_array(StoW_conflict);
                cout << "It has been waiting for the permission for: "<< duration<<" seconds."<<endl;
                cout << "Car from WEST lane is heading towards NORTH direction." << endl;
                sleep(3);
                nd.pop();
                cout << "Car from WEST lane with ID: " << localID << " LEAVING the intersection." << endl<<endl;
                pthread_mutex_unlock(&ndLock);

                break;
        }else{
            usleep(2);
            continue;
        }
    }
}

// This is a function for a car moving west to west.
void *westToWest(void *null)
{

    int localID;
    double duration; //for checking how long a car will be waiting at the front of its lane
    clock_t start;

    pthread_mutex_lock(&WtoWLock); // locking the queue
    pthread_mutex_lock(&global_id_Lock); // locking globalIDLock mutex in order to update globalID
    localID = car_id++; // update globalID after reserving that ID for a car in WtoW
    pthread_mutex_unlock(&global_id_Lock);
    WtoW.push(localID); // pushing the local car into StoN.
    pthread_mutex_unlock(&WtoWLock);

    add_element_array(EtoW_conflict, localID);
    sort_array(EtoW_conflict);
    add_element_array(NtoW_conflict, localID);
    sort_array(NtoW_conflict);
    add_element_array(NtoS_conflict, localID);
    sort_array(NtoS_conflict);
    add_element_array(StoW_conflict, localID);
    sort_array(StoW_conflict);
    add_element_array(StoS_conflict, localID);
    sort_array(StoS_conflict);

    while(1){
        if(EtoW_conflict[0] == localID && NtoW_conflict[0] == localID && NtoS_conflict[0] == localID && StoW_conflict[0] == localID && StoS_conflict[0] == localID){
                sleep(5); //say it takes 5 seconds to get to the intersection
                pthread_mutex_lock(&intersectionLaneLock);
                intersection_lane.push(localID);
                cout << "Car from WEST lane with ID: " << localID << " ENTERING the intersection." << endl;
                pthread_mutex_unlock(&intersectionLaneLock);
                start = clock(); // Now the car has arrived at intersection. Let's start the timer.

                while(1) //Checking cars properties here
                {
                    if ((localID == WtoW.front()) && (localID == intersection_lane.front())) // Current Car is in the front of the lane... Lets Proceed
                    {
                        break;
                    }
                    else //Current car is not in front on its lane. Lets wait
                    {
                        usleep(2); // sleep for 2 microsecond to allow for other cars to proceed if they must
                        continue;
                    }
                }
            
                // Car is in the front so let's proceed to allow it to pass through intersection.
                pthread_mutex_lock(&wdLock); // need to lock the intersection. Function call will block until mutex is available
                wd.push(localID);
                cout << "Car from the WEST lane with ID: " << localID << " ENTERING the WEST LANE." << endl;
                duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
                WtoW.pop();
                intersection_lane.pop();
                delete_element_array(EtoW_conflict);
                delete_element_array(NtoW_conflict);
                delete_element_array(NtoS_conflict);
                delete_element_array(StoW_conflict);
                delete_element_array(StoS_conflict);
                cout << "It has been waiting for the permission for: "<< duration<<" seconds."<<endl;
                cout << "Car from WEST lane is heading towards WEST direction." << endl;
                sleep(3);
                wd.pop();
                cout << "Car from WEST lane with ID: " << localID << " LEAVING the intersection." << endl<<endl;
                pthread_mutex_unlock(&wdLock);

                break;
        }else{
            usleep(2);
            continue;
        }
    }
}

// This is a function for a car moving east to west.
void *eastToWest(void *null)
{

    int localID;
    double duration; //for checking how long a car will be waiting at the front of its lane
    clock_t start;

    pthread_mutex_lock(&EtoWLock); // locking the queue
    pthread_mutex_lock(&global_id_Lock); // locking globalIDLock mutex in order to update globalID
    localID = car_id++; // update globalID after reserving that ID for a car in EtoW
    pthread_mutex_unlock(&global_id_Lock);
    EtoW.push(localID); // pushing the local car into EtoW.
    pthread_mutex_unlock(&EtoWLock);

    add_element_array(StoN_conflict, localID);
    sort_array(StoN_conflict);
    add_element_array(StoW_conflict, localID);
    sort_array(StoW_conflict);
    add_element_array(NtoS_conflict, localID);
    sort_array(NtoS_conflict);
    add_element_array(NtoW_conflict, localID);
    sort_array(NtoW_conflict);
    add_element_array(NtoN_conflict, localID);
    sort_array(NtoN_conflict);
    add_element_array(NtoE_conflict, localID);
    sort_array(NtoE_conflict);
    add_element_array(WtoN_conflict, localID);
    sort_array(WtoN_conflict);
    add_element_array(WtoW_conflict, localID);
    sort_array(WtoW_conflict);

    while(1){
        if(StoN_conflict[0] == localID && StoW_conflict[0] == localID && NtoS_conflict[0] == localID && NtoW_conflict[0] == localID && NtoN_conflict[0] == localID && 
            NtoE_conflict[0] == localID && WtoN_conflict[0] == localID && WtoW_conflict[0] == localID){
                sleep(5); //say it takes 5 seconds to get to the intersection
                pthread_mutex_lock(&intersectionLaneLock);
                intersection_lane.push(localID);
                cout << "Car from EAST lane with ID: " << localID << " ENTERING the intersection." << endl;
                pthread_mutex_unlock(&intersectionLaneLock);
                start = clock(); // Now the car has arrived at intersection. Let's start the timer.

                while(1) //Checking cars properties here
                {
                    if ((localID == EtoW.front()) && (localID == intersection_lane.front())) // Current Car is in the front of the lane... Lets Proceed
                    {
                        break;
                    }
                    else //Current car is not in front on its lane. Lets wait
                    {
                        usleep(2); // sleep for 2 microsecond to allow for other cars to proceed if they must
                        continue;
                    }
                }
            
                // Car is in the front so let's proceed to allow it to pass through intersection.
                pthread_mutex_lock(&wdLock); // need to lock the intersection. Function call will block until mutex is available
                wd.push(localID);
                cout << "Car from the EAST lane with ID: " << localID << " ENTERING the WEST LANE." << endl;
                duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
                EtoW.pop();
                intersection_lane.pop();
                delete_element_array(StoN_conflict);
                delete_element_array(StoW_conflict);
                delete_element_array(NtoS_conflict);
                delete_element_array(NtoW_conflict);
                delete_element_array(NtoN_conflict);
                delete_element_array(NtoE_conflict);
                delete_element_array(WtoN_conflict);
                delete_element_array(WtoW_conflict);
                cout << "It has been waiting for the permission for: "<< duration<<" seconds."<<endl;
                cout << "Car from EAST lane is heading towards WEST direction." << endl;
                sleep(3);
                wd.pop();
                cout << "Car from EAST lane with ID: " << localID << " LEAVING the intersection." << endl<<endl;
                pthread_mutex_unlock(&wdLock);

                break;
        }else{
            usleep(2);
            continue;
        }
    }
}

// This is a function for a car moving east to north.
void *eastToNorth(void *null)
{

    int localID;
    double duration; //for checking how long a car will be waiting at the front of its lane
    clock_t start;

    pthread_mutex_lock(&EtoNLock); // locking the queue
    pthread_mutex_lock(&global_id_Lock); // locking globalIDLock mutex in order to update globalID
    localID = car_id++; // update globalID after reserving that ID for a car in EtoN
    pthread_mutex_unlock(&global_id_Lock);
    EtoN.push(localID); // pushing the local car into EtoN.
    pthread_mutex_unlock(&EtoWLock);

    add_element_array(StoN_conflict, localID);
    sort_array(StoN_conflict);
    add_element_array(NtoN_conflict, localID);
    sort_array(NtoN_conflict);
    add_element_array(WtoN_conflict, localID);
    sort_array(WtoN_conflict);

    while(1){
        if(StoN_conflict[0] == localID && NtoN_conflict[0] == localID && WtoN_conflict[0] == localID){
                sleep(5); //say it takes 5 seconds to get to the intersection
                pthread_mutex_lock(&intersectionLaneLock);
                intersection_lane.push(localID);
                cout << "Car from EAST lane with ID: " << localID << " ENTERING the intersection." << endl;
                pthread_mutex_unlock(&intersectionLaneLock);
                start = clock(); // Now the car has arrived at intersection. Let's start the timer.

                while(1) //Checking cars properties here
                {
                    if ((localID == EtoN.front()) && (localID == intersection_lane.front())) // Current Car is in the front of the lane... Lets Proceed
                    {
                        break;
                    }
                    else //Current car is not in front on its lane. Lets wait
                    {
                        usleep(2); // sleep for 2 microsecond to allow for other cars to proceed if they must
                        continue;
                    }
                }
            
                // Car is in the front so let's proceed to allow it to pass through intersection.
                pthread_mutex_lock(&ndLock); // need to lock the intersection. Function call will block until mutex is available
                nd.push(localID);
                cout << "Car from the EAST lane with ID: " << localID << " ENTERING the NORTH LANE." << endl;
                duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
                EtoN.pop();
                intersection_lane.pop();
                delete_element_array(StoN_conflict);
                delete_element_array(NtoN_conflict);
                delete_element_array(WtoN_conflict);
                cout << "It has been waiting for the permission for: "<< duration<<" seconds."<<endl;
                cout << "Car from EAST lane is heading towards NORTH direction." << endl;
                sleep(3);
                nd.pop();
                cout << "Car from EAST lane with ID: " << localID << " LEAVING the intersection." << endl<<endl;
                pthread_mutex_unlock(&ndLock);

                break;
        }else{
            usleep(2);
            continue;
        }
    }
}

// This is a function for a car moving east to south.
void *eastToSouth(void *null)
{

    int localID;
    double duration; //for checking how long a car will be waiting at the front of its lane
    clock_t start;

    pthread_mutex_lock(&EtoSLock); // locking the queue
    pthread_mutex_lock(&global_id_Lock); // locking globalIDLock mutex in order to update globalID
    localID = car_id++; // update globalID after reserving that ID for a car in EtoS
    pthread_mutex_unlock(&global_id_Lock);
    EtoS.push(localID); // pushing the local car into EtoS.
    pthread_mutex_unlock(&EtoSLock);

    add_element_array(WtoE_conflict, localID);
    sort_array(WtoE_conflict);
    add_element_array(WtoN_conflict, localID);
    sort_array(WtoN_conflict);
    add_element_array(WtoS_conflict, localID);
    sort_array(WtoS_conflict);
    add_element_array(WtoW_conflict, localID);
    sort_array(WtoW_conflict);
    add_element_array(StoN_conflict, localID);
    sort_array(StoN_conflict);
    add_element_array(StoS_conflict, localID);
    sort_array(StoS_conflict);
    add_element_array(StoW_conflict, localID);
    sort_array(StoW_conflict);
    add_element_array(NtoS_conflict, localID);
    sort_array(NtoS_conflict);
    add_element_array(NtoN_conflict, localID);
    sort_array(NtoN_conflict);
    add_element_array(NtoE_conflict, localID);
    sort_array(NtoE_conflict);

    while(1){
        if(WtoE_conflict[0] == localID && WtoN_conflict[0] == localID && WtoS_conflict[0] == localID && WtoW_conflict[0] == localID && StoN_conflict[0] == localID && 
            StoS_conflict[0] == localID && StoW_conflict[0] == localID && NtoS_conflict[0] == localID && NtoN_conflict[0] == localID && NtoE_conflict[0] == localID){
                sleep(5); //say it takes 5 seconds to get to the intersection
                pthread_mutex_lock(&intersectionLaneLock);
                intersection_lane.push(localID);
                cout << "Car from EAST lane with ID: " << localID << " ENTERING the intersection." << endl;
                pthread_mutex_unlock(&intersectionLaneLock);
                start = clock(); // Now the car has arrived at intersection. Let's start the timer.

                while(1) //Checking cars properties here
                {
                    if ((localID == EtoS.front()) && (localID == intersection_lane.front())) // Current Car is in the front of the lane... Lets Proceed
                    {
                        break;
                    }
                    else //Current car is not in front on its lane. Lets wait
                    {
                        usleep(2); // sleep for 2 microsecond to allow for other cars to proceed if they must
                        continue;
                    }
                }
            
                // Car is in the front so let's proceed to allow it to pass through intersection.
                pthread_mutex_lock(&sdLock); // need to lock the intersection. Function call will block until mutex is available
                sd.push(localID);
                cout << "Car from the EAST lane with ID: " << localID << " ENTERING the SOUTH LANE." << endl;
                duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
                EtoS.pop();
                intersection_lane.pop();
                delete_element_array(WtoE_conflict);
                delete_element_array(WtoN_conflict);
                delete_element_array(WtoS_conflict);
                delete_element_array(WtoW_conflict);
                delete_element_array(StoN_conflict);
                delete_element_array(StoS_conflict);
                delete_element_array(StoW_conflict);
                delete_element_array(NtoS_conflict);
                delete_element_array(NtoN_conflict);
                delete_element_array(NtoE_conflict);
                cout << "It has been waiting for the permission for: "<< duration<<" seconds."<<endl;
                cout << "Car from EAST lane is heading towards SOUTH direction." << endl;
                sleep(3);
                sd.pop();
                cout << "Car from EAST lane with ID: " << localID << " LEAVING the intersection." << endl<<endl;
                pthread_mutex_unlock(&sdLock);

                break;
        }else{
            usleep(2);
            continue;
        }
    }
}

// This is a function for a car moving east to east.
void *eastToEast(void *null)
{

    int localID;
    double duration; //for checking how long a car will be waiting at the front of its lane
    clock_t start;

    pthread_mutex_lock(&EtoELock); // locking the queue
    pthread_mutex_lock(&global_id_Lock); // locking globalIDLock mutex in order to update globalID
    localID = car_id++; // update globalID after reserving that ID for a car in EtoE
    pthread_mutex_unlock(&global_id_Lock);
    EtoE.push(localID); // pushing the local car into EtoE.
    pthread_mutex_unlock(&EtoELock);

    add_element_array(WtoE_conflict, localID);
    sort_array(WtoE_conflict);
    add_element_array(StoE_conflict, localID);
    sort_array(StoE_conflict);
    add_element_array(StoN_conflict, localID);
    sort_array(StoN_conflict);
    add_element_array(NtoE_conflict, localID);
    sort_array(NtoE_conflict);
    add_element_array(NtoN_conflict, localID);
    sort_array(NtoN_conflict);

    while(1){
        if(WtoE_conflict[0] == localID && StoE_conflict[0] == localID && StoN_conflict[0] == localID && NtoE_conflict[0] == localID && NtoN_conflict[0] == localID){
                sleep(5); //say it takes 5 seconds to get to the intersection
                pthread_mutex_lock(&intersectionLaneLock);
                intersection_lane.push(localID);
                cout << "Car from EAST lane with ID: " << localID << " ENTERING the intersection." << endl;
                pthread_mutex_unlock(&intersectionLaneLock);
                start = clock(); // Now the car has arrived at intersection. Let's start the timer.

                while(1) //Checking cars properties here
                {
                    if ((localID == EtoE.front()) && (localID == intersection_lane.front())) // Current Car is in the front of the lane... Lets Proceed
                    {
                        break;
                    }
                    else //Current car is not in front on its lane. Lets wait
                    {
                        usleep(2); // sleep for 2 microsecond to allow for other cars to proceed if they must
                        continue;
                    }
                }
            
                // Car is in the front so let's proceed to allow it to pass through intersection.
                pthread_mutex_lock(&edLock); // need to lock the intersection. Function call will block until mutex is available
                ed.push(localID);
                cout << "Car from the EAST lane with ID: " << localID << " ENTERING the EAST LANE." << endl;
                duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
                EtoE.pop();
                intersection_lane.pop();
                delete_element_array(WtoE_conflict);
                delete_element_array(StoE_conflict);
                delete_element_array(StoN_conflict);
                delete_element_array(NtoE_conflict);
                delete_element_array(NtoN_conflict);
                cout << "It has been waiting for the permission for: "<< duration<<" seconds."<<endl;
                cout << "Car from EAST lane is heading towards EAST direction." << endl;
                sleep(3);
                ed.pop();
                cout << "Car from EAST lane with ID: " << localID << " LEAVING the intersection." << endl<<endl;
                pthread_mutex_unlock(&edLock);

                break;
        }else{
            usleep(2);
            continue;
        }
    }
}

int main()
{
    init();
    srand(time(0));
    // int k = 0;
    // int direction = 0;
    // int d = 0;
    
    for(int i = 0; i < cars; i++) //first car will be car with ID 1
    {
       
        k = getrand(1,5); // outgoing direction
        direction = getrand(1,5);

        d = directions(direction, k);

        if(d == 11)
        {   
            pthread_create (&threadID, NULL, northToNorth, NULL);
        }
        else if (d == 12)
        {
            pthread_create (&threadID, NULL, northToSouth, NULL);
        }
        else if (d == 13)
        {
            pthread_create (&threadID, NULL, northToWest, NULL);
        }
        else if (d == 14)
        {
            pthread_create (&threadID, NULL, northToEast, NULL);
        }
        else if (d == 21)
        {
            pthread_create (&threadID, NULL, southToNorth, NULL);
        }
        else if (d == 22)
        {
            pthread_create (&threadID, NULL, southToSouth, NULL);
        }
        else if (d == 23)
        {
            pthread_create (&threadID, NULL, southToWest, NULL);
        }
        else if (d == 24)
        {
            pthread_create (&threadID, NULL, southToEast, NULL);
        }
        else if (d == 31)
        {
            pthread_create (&threadID, NULL, westToNorth, NULL);
        }
        else if (d == 32)
        {
            pthread_create (&threadID, NULL, westToSouth, NULL);
        }
        else if (d == 33)
        {
            pthread_create (&threadID, NULL, westToWest, NULL);
        }
        else if (d == 34)
        {
            pthread_create (&threadID, NULL, westToEast, NULL);
        }
        else if (d == 41)
        {
            pthread_create (&threadID, NULL, eastToNorth, NULL);
        }
        else if (d == 42)
        {
            pthread_create (&threadID, NULL, eastToSouth, NULL);
        }
        else if (d == 43)
        {
            pthread_create (&threadID, NULL, eastToWest, NULL);
        }
        else if (d == 44)
        {
            pthread_create (&threadID, NULL, eastToEast, NULL);
        }
        else{
            cout << "Test" << endl;
        }
        sleep(1); //a car will be created every # seconds.
    }
    sleep(run_time); //sleep for sufficient times to allow for all threads to finish running.
    cout << "Finished." << endl;
    return 0;
}   
