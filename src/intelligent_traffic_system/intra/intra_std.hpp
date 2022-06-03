
//#include <bits/stdc++.h>
#include <iostream>
#include <queue>
#include <algorithm>
#include <cstdlib> //for rand()
#include <ctime> //for clock(), clock_t, CLOCKS_PER_SEC
#include <unistd.h> // for usleep()
#include <pthread.h>

using namespace std;

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
	     return(rand()%(max-min)+min);
}

int directions (int direction, int k){
    if(direction==1 && k==1){
        return 11;
    }else if(direction==1 && k==2){
        return 12;
    }else if(direction==1 && k==3){
        return 13;
    }else if(direction==1 && k==4){
        return 14;
    }else if(direction==2 && k==1){
        return 21;
    }else if(direction==2 && k==2){
        return 22;
    }else if(direction==2 && k==3){
        return 23;
    }else if (direction==2 && k==4)
    {
        return 24;
    }else if (direction==3 && k==1)
    {
        return 31;
    }else if (direction==3 && k==2)
    {
        return 32;
    }else if (direction==3 && k==3)
    {
        return 33;
    }else if (direction==3 && k==4)
    {
        return 34;
    }else if(direction==4 && k==1){
        return 41;
    }else if(direction==4 && k==2){
        return 42;
    }else if(direction==4 && k==3){
        return 43;
    }else if(direction==4 && k==4){
        return 44;
    }
}
