/*The function countingMergeSort sorts a vector and returns the amount of inversions needed to do so.
The program takes as input on the first line the size N of the array and on the next line,
N space seperated integers, the elements of the array. */

#include <vector>
#include <iostream>
using namespace std;

int countingMerge(vector <int> &v, int start, int mid, int end)
{
    vector <int> aux(v.size());
    int i = start;    // start of the left subvector
    int j = mid + 1; // start of the right subvector
    int k = start;  // start of the auxiliary vector
    int count = 0;  // total inversions needed for the merge

    /* Take the smallest element of each subvector and compare them.
    If the element of the right subvector is the smallest we need 
    to move it to the beginning of the aux vector. The number of 
    inversions required are equal to the amount of remaining 
    elements in the left subvector.*/

    while (i <= mid && j <= end) {
        if (v[i] < v[j]) aux[k++] = v[i++];
        else {
            aux[k++] = v[j++];
            count+=mid-i+1;
        }
    }

    // Remaining elements of left subvector
    while (i <= mid) {
        aux[k] = v[i++];
        k++;
    }

    // Remaining elements of right subvector
    while (j <= end) {
        aux[k] = v[j++];
        k++;
    }

    // Copy the auxiliary vector
    for (int i = start; i <= end; i++) {
        v[i] = aux[i];
    }

    return count;
}

// Merge sort
int countingMergeSort(vector <int> &v, int start, int end) {

    /*Total inversions needed = inversions to sort left part + inversions to sort right part + inversions to merge.
    Since addition is left-assosiative, we know that the function calls will be executed in the correct order:
    sort left -> sort right -> merge. */

    if (start < end) {
        int m = (start + end) / 2 ;
        return countingMergeSort(v, start, m)+
        countingMergeSort(v, m + 1, end)+
        countingMerge(v, start, m, end);
    }
    //Inversions to sort a vector with a single element
    return 0;
}

int main(){
    int N, i, x, inversions;
    cin>>N;
    vector<int> vect(N);
    for(i=0; i<N; i++){
        cin>>x;
        vect[i]=x;   
    }
    inversions = countingMergeSort(vect, 0, N-1);
    cout<<"The sorted array:\n";
    for(i=0; i<N; i++){
        cout<<vect[i]<<' ';   
    }
    cout<<endl<<"Inversions needed:"<<endl<<inversions;
}