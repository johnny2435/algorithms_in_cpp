## Closest Pair Of Points
  
The problem is solved for D dimensions where D is a parameter.  
The closestPairDim function uses divide and conquer to find the distance between the closest pair of points.  
The recursive formula for D>2 is:  
    T(n,d)=2T(n/2,d)+T(n,d-1)+Θ(n) ,d>1  
    T(n,1)=Θ(n)  
which gives a total time complexity T(n,D) = O(n*(logn)^(D-1)).  
(Note that this can be improved to O(nlogn) by selecting the midPoint in a smarter way.)
