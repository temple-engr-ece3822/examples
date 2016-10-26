'''
Created on Oct 24, 2016

@author: james
'''
import random

if __name__ == '__main__':
    num = []
    
    for i in range(0, 10):
        a = random.randint(1, 30)
        while not (a in num):
            num.append(a)
    #How large is the array?
    #print(num)
    
    #Lets change the array. 
    for i in range(0, 10):
        num[i] += num[i+1]
        
    #Will this work?
    #print(num)