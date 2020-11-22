# Painting Based Image Compression

This is the portion of the project where we analyse what we have learnt to draw our own compression algorithm. In this case we plan on making our own Image Compression Algorithm, which we call `Painting based Image Compression`.

## Motivation

Motivation for this algorithm is from the fact that any picture is analogus to a painting, to some extent and in painting there are few programmable patterns : 
1. Painting consists of various small strokes.
2. Strokes are straight along x-axis, y-axis, x=y or x=-y. There is also a probability of a complete color fill in a portion.
3. The stroke can have variation of color along only one axis i.e. the width of the paint-brush.

## The Algorithm

The algorithm is as follows : 

1. Assign pre-defined codes to strokes 

```python
def f0(x,y,mat):
    for i in range(8):
        for j in range(8):
            if mat[x+i][y+j] != mat[x][y]:
                return False
    return True

def f1(x,y,mat):
    for i in range(8):
        for j in range(8):
            if mat[x+i][y+j] != mat[x+i][y]:
                return False
    return True

def f2(x,y,mat):
    for i in range(8):
        for j in range(8):
            if mat[x+i][y+j] != mat[x][y+j]:
                return False
    return True

def f3(x,y,mat):
    for i in range(8):
        for j in range(8):
            if mat[x+i][y+j] != mat[][]:
                return False
    return True


def f3(x,y,mat,seg_mat):    # segment_matrix is a 15 length 1-D matrix
    for i in range(8):
        for j in range(8):
            if mat[x+i][y+j] != seg_mat[7 + i - j]:
                return False
    return True


def f3(x,y,mat,seg_mat):    # segment_matrix is a 15 length 1-D matrix
    for i in range(8):
        for j in range(8):
            if mat[x+i][y+j] != seg_mat[14 - i - j]:
                return False
    return True

codes_dict = [f0,f1,f2,f3,f4]

```

2. Now divide the image into blocks of `8x8` and perform processing (step-3 onwards) on each of the blocks.
3. Now consider the 8x8 grid as stroke of type 0 (f0), 1 (f1) , ... , 4 (f4) and calculate loss function for each case (defined later).
4. Select minimum loss function value (given it is less than a pre-defined limit) and assign this type of distribution / stroke to the given grid.
5. Write onto a binary file the following :

|type of stroke 1 byte|mean matrix of stroke (1-15 bytes)|


## Loss function 

Loss function is defined somewhat similarly for all the types of strokes, but is same for {f0}, {f1,f2}, {f3,f4} seperately. 

### Loss function 

Calculate the mean &mu; for the entire grid. Now compute for each matrix element `elt(x,y)` of the grid : 

### LOSS FOR F0 : 

> &lambda; = &Sigma;(elt<sub>x,y</sub> - &mu;)<sup>2</sup>  
> &Delta; = &lambda; x size[0]

### LOSS FOR F1, F2 : 

> &lambda; = &Sigma;<sub>x</sub> &Sigma;<sub>y</sub> (elt<sub>x,y</sub> - &mu;)<sup>2</sup>     
> &Delta; = &lambda; x size[1]

(Similarly for f2 by interchanging x and y)

### LOSS FOR F3, F4 : 

> &lambda; = &Sigma;<sub>c</sub> &Sigma;<sub>(x-y=c)</sub> (elt<sub>x,y</sub> - &mu;)<sup>2</sup>   
> &Delta; = &lambda; x size[3]

Similarly for f4 the condition in c becomes c = x + y.

## Compression analysis : 

Consider that each of the type of file occours equiprobably i.e. `1/6` of the total occurances.
We obtain : 
1. 2 bytes from 64 bytes for f0 => 32 times compressed
2. 9 bytes from 64 bytes for f1,f2 => 64/9 times compressed
3. 16 bytes from 64 bytes for f3,f4 => 4 times compressed
4. 64 bytes from 64 bytes for uncompressed => 1 times compressed

Total compression is : 
> &phi;<sub>avg</sub> = 4x2/6 + 1/6 + 32/6 + 64x2/(6x9)

Clearly, in reality, the distrbution is not equiprobable and is more for case 4 and very less for case 1. This would mean that real-life compression rates are way lower than estimated compression rates. 

1. `For Pictures` : Compression is lower as there is large variation in gradient and very low consistency. Cases 3,4 are abundant.
2. `For Text Images` : Compression is good as maximum number of frames falls under category 1 or 2 and thus compression is atleast 8 times (estimated).

## Sample data 

We compressed a `Picture` and we would see the quality degradation before and after compression. 

> Before Compression 

![Real Image](https://github.com/vs666/PiedPiper/blob/master/Image%20Compression/PBIC/image.jpg)

> After Compression

![Compressed Image](https://github.com/vs666/PiedPiper/blob/master/Image%20Compression/PBIC/compressed_image.jpg)


## Author and Code 

The concept is authored and code written (currently in python) by : 

Varul Srivastava 