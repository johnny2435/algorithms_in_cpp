#program used to create test cases in ClosestPairDim-In.txt 
#which is used as input for ClosestPairDim.cpp
import random
#amount of points
N=10
#amount of dimensions
D=2
#coordinate range: low
low=0 
#coordinate range: high
high=10000
f = open("ClosestPairDim-In.txt", "w")
f.write(str(N))
f.write(' ')
f.write(str(D))
f.write('\n')
for i in range(N):
    for j in range(D):
        f.write(str(random.uniform(low,high)))
        f.write(' ')
    f.write('\n')

f.close()