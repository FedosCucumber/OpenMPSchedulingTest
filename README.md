# OpenMPPoliciesTest
Comparisson between various OpenMP policies for multi-thread loop distribution based on the Gaussian Blur algorithm using box blur approximation.

# Algorithm overview. 

Gaussian blur is an algorithm that blurs the image using the gaussian function:
$ g(x, y) = e^\frac{x^2+y^2}{2\sigma^2} $. Result of the function are used to create a convolution kernel of size $6\sigma -1$ which is moved through the picture and changes each pixel. $\sigma$ is the standart deviation of pixel colour.

Applying gaussian blur directly can proove to be rather time-consuming. What we need is some form of approximation of the blur. This example uses the box blur aaproximation technique. Lets define the horizontal and total blur:

$$ b_h[i, j] $$
