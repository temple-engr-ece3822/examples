
import numpy as np

def main():
    a = np.random.random_sample(10)
    for i in range(len(a)):
        a[i] += a[i]
    print(a)
#Guard the main functino call
if __name__ == '__main__':
    main()