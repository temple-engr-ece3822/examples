import numpy as np
from timeit import default_timer as timer

def demo1():
    n = 100
    x = np.random.rand(n,) #Generate array of random numbers
    y = np.zeros(n); z = np.zeros(n); a = np.zeros(n) #Preallocate memory
    b = np.zeros(n*n); c = np.zeros(n*n); d = np.zeros(n*n)
    
    print('--- iterative loops vs. map ---')
    start = timer() # Start Timer
    for i in range(len(x)):
        y[i] = x[i] * 5 
    end = timer()  
    print('Itterative: ', end-start, ' s ' ) 
    
    start = timer()
    a = [v * 5 for v in x]  
    end = timer()
    print('List Comprehension :', end-start, ' s ')
    
    start = timer()
    z = map(lambda v : v * 5, x) #returns itterable object
    #z = [*map(lambda v: v * 5, x)] #returns array
    end = timer()  
    print('Functional: ', end-start, ' s ' ) 
    print("Are results the same?", set(list(z)) == set(y) == set(a))
    print()
    
    print('--- iterative loops vs. filter ---')
    start = timer()    
    for i in range(len(x)) :
        if x[i] % 2 :
            y[i] = x[i] * 5
    end = timer()
    print('Itterative: ', end-start, ' s ')
    
    start = timer()
    a = [v * 5 for v in x if v % 2]
    end = timer()
    print('List Comprehension: ', end-start, ' s ')
    
    start = timer()
    z = map(lambda v : v * 5, filter(lambda u : u % 2, x))
    end = timer()
    print('Functional:', end-start, ' s ')
    print("Are results the same?", set(list(z)) == set(y) == set(a))
    print()  

	from timeit import default_timer as timer

def demo2():
    n = 10000
    print('--- Filtered List Comprehension vs. Filter/Map ---')    
    start = timer()
    special_squares_l = [ x**2 for x in range(n) if x**2 > 1 and x**2 < n ]
    end = timer()
    print('Filtered List comprehension: ', end-start, ' s ')
    
    start = timer()
    squares_f = map(lambda x: x**2, range(n))
    special_squares_f = filter(lambda x: x > 1 and x < n, squares_f) #Returns a filter object
    end = timer()
    print('Functional:' , end-start, ' s ')   
    print('Are the results equal', set(list(special_squares_f)) == set(special_squares_l))
    
demo1()
demo2()
