#include <iostream>
#include <vector>
#include <chrono>
#include <omp.h>
using namespace std;
using namespace std::chrono;

void bubbleSortSeq(vector<int>& arr){
    int n = arr.size();
    for(int i = 0;i<n-1;i++){
        for(int j = 0;j<n-i-1;j++){
            if(arr[j] > arr[j+1]){
                swap(arr[j],arr[j+1]);
            }
        }
    }
}


void bubbleSortPar(vector<int>& arr){
    int n = arr.size();
    for(int i = 0;i<n;i++){
        #pragma omp parallel for
        for(int j = i%2;j<n-1;j+=2){
            if(arr[j]>arr[j+1]){
                swap(arr[j],arr[j+1]);
            }
        }
    }
}

void merge(vector<int>& arr,int l, int m, int r){
    vector<int> temp;
    int i = l, j = m+1;
    while(i <= m && j <= r){
        if(arr[i] < arr[j]){
            temp.push_back(arr[i]);
            i++;
        }else{
            temp.push_back(arr[j]);
            j++;
        }
    }
    
    while(i<=m){
        temp.push_back(arr[i]);
        i++;
    }
    
    while(j<=r){
        temp.push_back(arr[j]);
        j++;
    }
    
    for(int k = 0;k<temp.size();k++){
        arr[l+k] = temp[k];
    }
}

void mergeSortSeq(vector<int>& arr, int l, int r){
    if(l>=r) return;
    int m = (l+r)/2;
    mergeSortSeq(arr,l,m);
    mergeSortSeq(arr,m+1,r);
    merge(arr,l,m,r);
}

void mergeSortPar(vector<int>& arr, int l, int r, int depth = 0){
    if(l>=r) return;
    int m = (l+r)/2;
    if(depth <= 3){
        #pragma omp parallel
        {      
           #pragma omp single
           {
            	#pragma omp task
            	mergeSortPar(arr,l,m,depth+1);
            	#pragma omp task
            	mergeSortPar(arr,m+1,r,depth+1);
            	#pragma omp taskwait
        }
}
    }else{
        mergeSortSeq(arr,l,m);
        mergeSortSeq(arr,m+1,r);
    }
    merge(arr,l,m,r);
}

void display(const vector<int>& arr){
    for(auto x: arr){
        cout<<x<<" ";
    }
    cout<<endl;
}

int main(){
    int n;
    cout<<"Enter the number of element : ";
    cin>>n;
    
    vector<int> arr;
    cout<<"Enter element : "<<endl;
    for(int i = 0;i<n;i++){
        int num;
        cin>>num;
        arr.push_back(num);
    }
    
    vector<int> bseq = arr, bpar = arr, mseq = arr, mpar = arr;
    high_resolution_clock::time_point start,end;
    
    start = high_resolution_clock::now();
    bubbleSortSeq(bseq);
    end = high_resolution_clock::now();
    cout<<"\nSequential Bubble Sort Time : "<<duration_cast<milliseconds>(end-start).count()<<"ms";
    cout<<"\nSorted Array(Sequential Buble Sort) : ";
    display(bseq);
    
    start = high_resolution_clock::now();
    bubbleSortPar(bpar);
    end = high_resolution_clock::now();
    cout<<"\nParallel Bubble Sort Time : "<<duration_cast<milliseconds>(end-start).count()<<"ms";
    cout<<"\nSorted Array (Parallel Bubble Sort) : ";
    display(bpar);
    
    start = high_resolution_clock::now();
    mergeSortSeq(mseq, 0, n-1);
    end = high_resolution_clock::now();
    cout<<"\nSequential Merge Sort Time : "<<duration_cast<milliseconds>(end-start).count()<<"ms";
    cout<<"\nSorted Array(Sequential Merge Sort) : ";
    display(mseq);
    
    start = high_resolution_clock::now();
    mergeSortSeq(mpar, 0, n-1);
    end = high_resolution_clock::now();
    cout<<"\nParallel Merge Sort Time : "<<duration_cast<milliseconds>(end-start).count()<<"ms";
    cout<<"\nSorted Array(Parallel Merge Sort) : ";
    display(mpar);
    
}
    
