# OpenMP Scheduling Policies Test
Comparisson between various OpenMP policies for multi-thread loop distribution based on the Gaussian Blur algorithm using box blur approximation.

## Algorithm overview. 

Gaussian blur is an algorithm that blurs the image using the gaussian function:
$ g(x, y) = e^\frac{x^2+y^2}{2\sigma^2} $. Result of the function are used to create a convolution kernel of size $6\sigma -1$ which is moved through the picture and changes each pixel. $\sigma$ is the standart deviation of pixel colour.

Applying gaussian blur directly can proove to be rather time-consuming. What we need is some form of approximation of the blur. This example uses the box blur aaproximation technique. Lets define the horizontal and total blur:

$$ b_h[i, j] = \sum_{x=j-br}^{j+br}{\frac{f[i,x]}{2*br}}$$

$$ b_t[i, j] = \sum_{y=j-br}^{j+br}{\frac{b_h[i, x]}{2*br}}$$

Where $br$ is the box filter radius. seeing that, we can boil down $b_t[i, j]$ to

$$b_t[i,j] = \sum_{y=i-br}^{i+br}{\sum_{x=j-br}^{j+br}{f[x,y]/(2*br)^2}} $$

This makes the time complexity of the algoritm to be *O(nr)*.

## Sheduling comparisson 

One of the most popular ways to use OpenMP is to parallelize loops. This can be achieve using different verious scheduling options 
