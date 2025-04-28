#include <iostream>
#include <vector>
#include <omp.h>
using namespace std;

int main(){
    int n;
    cout<<"Enter the number of element : ";
    cin>>n;
    
    vector<int> arr(n);
    cout<<"Enter the element :"<<endl;
    for(int i = 0;i<n;i++){
        cin>>arr[i];
    }
    
    int minVal = arr[0];
    int maxVal = arr[0];
    int sum = 0;
    
    #pragma omp parallel for reduction(min:minVal) reduction(max:maxVal) reduction(+:sum)
    for(int i = 0;i<n;i++){
        if(arr[i] < minVal) minVal = arr[i];
        if(arr[i] > maxVal) maxVal = arr[i];
        sum += arr[i];
    }
    
    double avg  = (double)sum/n;
    cout<<"\n-------Result--------";
    cout<<"\nMinimum : "<<minVal;
    cout<<"\nMaximum : "<<maxVal;
    cout<<"\nSum     : "<<sum;
    cout<<"\nAverage : "<<avg;
    
}