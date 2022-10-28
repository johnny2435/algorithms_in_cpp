#include <iostream>
#include <utility>
#include <vector>
#include <cmath>
#include <limits>
#include <bits/stdc++.h>
#include <chrono>
using namespace std;
using namespace std::chrono;    //Only needed for chronometer

const double INF = numeric_limits<double>::max();   //infinity

double min(double x, double y)
{
    return (x < y)? x : y;
}

// A structure to represent a Point in D-dimensional space
struct point{
    vector<double> x;
};

//Only needed for testing
void printPoint(point p){
    int D = p.x.size();
    cout<<'(';
    for (int d=0; d<D; d++) cout<<p.x[d]<<',';
    cout<<')';
}

//Only needed for testing
void printP(vector <point> &P){
    int N = P.size();
    for (int i=0; i<N; i++) printPoint(P[i]);
    cout<<endl;
}

//Comparisons needed to sort the vector by coordinate 0, which needed for initial sorting.
bool compare0(point p1, point p2){
    return p1.x[0]<p2.x[0];
}

//Distance between two points in D-dimensional space
double dist(point p1, point p2)
{
    int dist = 0, D = p1.x.size();
    for (int d=0; d<D; d++){
        dist+=(p1.x[d]-p2.x[d])*(p1.x[d]-p2.x[d]);
    }
    return sqrt(dist);
}

//Brute Force solution used to find minimum distance for 2 or 3 points
double closestPairBrute(vector <point> &P){
    int i, j, N=P.size();
    double distance, minimum = INF;
    for(i=0; i<N-1; i++){
        for(j=i+1; j<N; j++){
            distance = dist(P[i], P[j]);
            if (distance < minimum) minimum = distance;
        }
    }
    return minimum;
}

//Sort P vector with 2 or 3 elements by dimension d
void Vsort(vector <point> &P, int d){
    int N=P.size();
    int i, j;
    point temp;
    for (i = 0; i < N - 1; i++)
        // Last i elements are already in place
        for (j = 0; j < N - i - 1; j++)
            if (P[j].x[d] > P[j+1].x[d]){
                temp = P[j];
                P[j]=P[j+1];
                P[j+1]=temp;
            }
}

//Merge v1 and v2 into v, sorted by coordinate d
void merge(vector <point> &v1, vector <point> &v2, vector <point> &v, int d){

    int N1 = v1.size(), N2 = v2.size();  //size of each vector
    int i1 = 0, i2 = 0, i = 0;    // index for each vector

    while (i1 < N1 && i2 < N2) {
        if (v1[i1].x[d] < v2[i2].x[d]) v[i++] = v1[i1++];
        else {
            v[i++] = v2[i2++];
        }
    }

    // Remaining elements of left subvector
    while (i1 < N1) v[i++] = v1[i1++];

    // Remaining elements of right subvector
    while (i2 < N2) v[i++] = v2[i2++];
}

/*Recursive function that calculates the minimum distance
N is the number of points in the current subproblem
d is the current coordinate
D is the number of dimensions
minDist is the minimum distance up to that point,
needed to find the minimum distance in the strip,
minDist==INF when we don't have one*/
double closestUtil(vector <point> &P, int d, double minDist){

    int N=P.size(); 

    //If there is 0 or 1 points, minDist is the minimum
    if (N <= 1){
        return minDist;
    }
    // If there are 2 or 3 points use brute force
    // We need to sort vector P
    if (N <= 3){
        Vsort(P, d+1);
        return min(minDist,closestPairBrute(P));
    }

    int D=P[0].x.size(); //total dimensions

    // If we have one coordinate left, we can find the minimum distance in the strip in O(n)
    if (d==D-1){
        double minimum = minDist, distance;
        // Special case: 1-dimensional problem
        if (D==1){
            for (int i = 0; i < N-1; i++){
                distance = P[i+1].x[0]-P[i].x[0];
                if (distance < minimum) minimum = distance;
            }
            return minimum;
        }
        /* General case: Pick all points one by one and try the next points while 
           the difference between coordinates is smaller than minDist.
           This loop runs O(1) times */
        for (int i = 0; i < N-1; i++){
            for (int j = i+1; j < N && (P[j].x[d] - P[i].x[d]) < minDist; j++){
                distance = dist(P[i],P[j]);
                if (distance < minimum) minimum = distance;
            }
        }
        return minimum;
    }

    // Find the middle point
    int mid = N/2;
    point midPoint = P[mid];

    // Divide points to P-left and P-right depending on their d-coordinate.
    vector <point> Pl(mid), Pr(N-mid);
    for (int i = 0; i < mid; i++) Pl[i]=P[i];
    for (int i = mid; i < N; i++) Pr[i-mid]=P[i];

    // Find minimum distance for Pl and Pr recursivly
    // Pl and Pr will be sorted by coordinate d+1
    double dist = min(closestUtil(Pr, d, minDist), closestUtil(Pl, d, minDist));

    /* Merge sorted subvectors by coordinate d+1 into P
       By doing this, sorting P by coordinate d+1
       only costs O(N) per closestUtil call*/
    merge(Pl, Pr, P, d+1);

    // Pnew only contains points for which: distance from the hyperplane < dist
    vector <point> Pnew;

    for (int i = 0; i < N; i++)
        if (abs(P[i].x[d] - midPoint.x[d]) < dist)
            Pnew.push_back(P[i]);
    
    /* We call closestUtil with coordinate d+1 since we have restricted
       the points regarding their d-coordinate.
       This is like reducing the dimensions of the problem by 1 */
    return closestUtil(Pnew, d+1, min(dist,minDist));
}

//closestPair uses the recursive function closestUtil
//to find the minimum distance between any two points
double closestPairDim(vector <point> &P, int D){
    int i, N=P.size();

    //sort points by 0-coordinate
    sort(P.begin(), P.end(), compare0);

    // Use closestUtil() to find the minimum distance
    return closestUtil(P, 0, INF);
}




/* Driver program that tests speed and correctness
   Input: On the first line integers N and D
   On the following N lines, D space seperated doubles*/
int main(){
    //Redirect cin to input file
    ifstream in("ClosestPairDim-In.txt");
    cin.rdbuf(in.rdbuf());

    int N,D;
    cin>>N>>D;
    vector <point> P(N);
    double x;
    for(int i=0; i<N; i++){
        point p;
        for(int d=0; d<D; d++){
            cin>>x;
            p.x.push_back(x);
        }
        P[i]=p;
    }
    
    //chronometer
    auto start = high_resolution_clock::now();

    //We call the closestPairBrute function here

    cout<<"BruteForce found: "<<closestPairBrute(P)<<endl; 

    //chronometer
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Time taken by BruteForce: "
         << duration.count() << " microseconds" << endl;

    start = high_resolution_clock::now();   

    //We call the closestPairDim function here
    cout<<"DAC found: "<<closestPairDim(P, D)<<endl;

    //chronometer
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);
    cout << "Time taken by DAC: "
         << duration.count() << " microseconds" << endl;
}
