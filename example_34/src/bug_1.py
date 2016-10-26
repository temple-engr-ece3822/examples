'''
Created on Oct 24, 2016

@author: james
'''
   
def fib(n):
    """Print a Fibonacci series up to n."""
    a, b = 0, 1
    while a < n:
        print(a, end=' ')
        a, b  = b, a+b
    print()
    
if __name__ == '__main__':
    fib(200)
    