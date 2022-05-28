
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
  
    for (int i = 0; i < n; ++i)
        cout << arr[i] << " ";
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

