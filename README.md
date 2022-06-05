# OpenMPPoliciesTest
Comparisson between various OpenMP policies for multi-thread loop distribution based on the Gaussian Blur algorithm using box blur approximation.

# Algorithm overview. 

Gaussian blur is an algorithm that blurs the image using the gaussian function:
$ g(x, y) = e^\frac{x^2+y^2}{2\sigma^2} $. Result of the function are used to create a convolution kernel of size $6\sigma -1$ which is moved through the picture and changes each pixel. $\sigma$ is the standart deviation of pixel colour.
