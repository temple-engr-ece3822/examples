#A Bunch of Imports
from mpl_toolkits.mplot3d import axes3d
from matplotlib import cm
from matplotlib.ticker import LinearLocator
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd 

def main():
    ##Function to show some basic plotting functions
    ##Uncomment to use
    simple_plot()
    
    ##Function to show simple sinusid with a spines
    ##Uncomment to use
    spine_placement_example()
    
    ##Functin to show a simple wireframe example
    ##Uncomment to use
    wireframe_example()
    
    ##Function to show a 3d surface plot
    ##Uncomment to use
    surface_example()
    
    ##Functin to show a filled contour map
    ##Uncomment to use
    contour_example()
    
    ##Function to generate data into a CSV file and then plot data
    ##Uncomment to use
    plot_from_file("titanic.csv")

#Name: simple_plot
#Input: N/A
#Output: N/A
#SideEffects: Displays a Quad Plot
#Summary: Will generate data for a linear, log, 
def simple_plot():
    # make up some data in the interval [0, 1]
    y = np.random.normal(loc=0.5, scale=0.4, size=1000)
    y = y[(y > 0) & (y < 1)]
    y.sort()
    x = np.arange(len(y))
    
    # plot with various axes scales
    plt.figure()
    
    # linear
    plt.subplot(221) #Create Subplot Right
    plt.plot(x, y)
    plt.yscale('linear') #Set ccale of Y-Axis
    plt.title('linear')
    plt.grid(True)
    
    
    # log
    plt.subplot(222) #Create Subplot left
    plt.plot(x, y)
    plt.yscale('log') #Set ccale of Y-Axis
    plt.title('log')
    plt.grid(True) #Turn on Grid Lines
    
    plt.show() #Show Figure
    
    # plot with variious lines 
    plt.figure()
    # evenly sampled time at 200ms intervals
    t = np.arange(0., 5., 0.2)
    
    plt.title("Some title")
    # red dashes, blue squares and green triangles
    plt.plot(t, t, 'r--', t, t**2, 'bs', t, t**3, 'g^')
    plt.show() #Show Figure
    
#spine_placement_example
#Input: N/A
#Output: N/A
#SideEffects: Generates a Sin plot with a spine
#Summary:
def spine_placement_example():
    #Create a figure example f using figure size
    fig = plt.figure(figsize=(4, 5))
    ax = fig.add_subplot(111)
    #Generate the sin data. 
    x = np.arange(-2*np.pi, 2*np.pi, 0.01)
    y = np.sin(x)
    #Create spines
    ax.spines['top'].set_color('none')
    ax.spines['right'].set_color('none')
    
    ax.xaxis.set_ticks_position('bottom')
    ax.yaxis.set_ticks_position('left')
    
    ax.plot(x, y,linewidth="3", alpha=0.3)
    
    #Plot Y Spine, in green at 1
    ax.plot([0, np.pi/2], [1, 1], ls="--", color="green", linewidth="3",alpha=0.5)
    #Plot X Spine in Red at pi/2
    ax.plot([np.pi/2, np.pi/2], [1, 0], ls="--", color="red", linewidth="3",alpha=0.5)
    #Set spine positions
    ax.spines['left'].set_position('center')
    ax.spines['bottom'].set_position('center')
    
    #Set x axis boundary
    ax.set_xlim(-2*np.pi, 2*np.pi)
    #Set x-axis tick value to count by pi/2
    xticker = np.arange(-np.pi-np.pi/2, np.pi+np.pi, np.pi/2)
    #Check out the inline R, isn't that fun.
    xlabels = [r"$\frac{-3\pi}{2}$", r"${-\pi}$",r"$\frac{-\pi}{2}$","",r"$\frac{pi}{2}$",r"${\pi}$",r"$\frac{3\pi}{2}$"]
    #Print labels for x-axis
    ax.set_xticks(xticker)
    ax.set_xticklabels(xlabels, size=17)
    #Place text for the plot
    ax.text(np.pi, 1.1, "y=sin(x)")
    #Set Y-Axis limit for the plot
    ax.set_ylim(-1.5, 1.5)
    yticker = np.arange(-1, 2, 1)
    ax.set_yticks(yticker)
    
    plt.show() #Show figure

#Name: 3d_plot_example
#Input: None
#Output: None
#SideEffects: 
#Summary: 
def wireframe_example():
    #Create New Figure
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    #Get some test data
    X, Y, Z = axes3d.get_test_data(0.05)
    #Plot wireframe
    ax.plot_wireframe(X, Y, Z, rstride=10, cstride=10)
    
    plt.show() #Show Figure

#Name: surface_example
#Input: N/A
#Output: N/A
#SideEffects: 
#Summary: Creates a sphere plot
def surface_example():
    #Create new Figure
    fig = plt.figure()
    ax = fig.gca(projection='3d')
    #Generate Data
    X = np.arange(-5, 5, 0.25)
    xlen = len(X)
    Y = np.arange(-5, 5, 0.25)
    ylen = len(Y)
    X, Y = np.meshgrid(X, Y)
    R = np.sqrt(X**2 + Y**2)
    Z = np.sin(R)
    #Set Color values
    colortuple = ('y', 'b')
    colors = np.empty(X.shape, dtype=str)
    #Color Data Points to give 3d apperance
    for y in range(ylen):
        for x in range(xlen):
            colors[x, y] = colortuple[(x + y) % len(colortuple)]
    #Plot solid surface
    ax.plot_surface(X, Y, Z, rstride=1, cstride=1, facecolors=colors,
                           linewidth=0, antialiased=False)
    #Set Axis boundaries
    ax.set_zlim3d(-1, 1)
    ax.w_zaxis.set_major_locator(LinearLocator(6))
    
    plt.show() #Show Figure

#Name: contour_example()
#Input: N/A
#Output: N/A
#SideEffects:
#Summary: Generates a simple filled contour plot
def contour_example():
    #Create New Figure
    fig = plt.figure()
    ax = fig.gca(projection='3d')
    #Generate Data
    X, Y, Z = axes3d.get_test_data(0.05)
    #Plot Surface
    ax.plot_surface(X, Y, Z, rstride=8, cstride=8, alpha=0.3)
    #Set Axis Colors
    cset = ax.contourf(X, Y, Z, zdir='z', offset=-100, cmap=cm.coolwarm)
    cset = ax.contourf(X, Y, Z, zdir='x', offset=-40, cmap=cm.coolwarm)
    cset = ax.contourf(X, Y, Z, zdir='y', offset=40, cmap=cm.coolwarm)
    #Set Axis boundaries and labels
    ax.set_xlabel('X')
    ax.set_xlim(-40, 40)
    ax.set_ylabel('Y')
    ax.set_ylim(-40, 40)
    ax.set_zlabel('Z')
    ax.set_zlim(-100, 100)
    
    plt.show()#Show Figure
    
        
#Name: plot_from_file
#Input: filename (contains the filename, in current directory otherwise full path)
#Output: N/A
#SideEffects: Displays a two histograms
#Summary: Function will read in data from the titanic CSV and then plot the data using 
# matplotlib. 
def plot_from_file(filename):
    #Import data using pandas, because its easier based on data format
    data = pd.read_csv(filename)
    #Need to replace the null values with the median age to allow for plotting.
    data['Age'].fillna(data['Age'].median(), inplace=True)
    #Plot some informatin on survival rates based on age using a histogram
    figure = plt.figure(figsize=(10,5))
    #Isn't this one-liner wonderful, does all the data manipulation and plotting
    plt.hist([data[data['Survived']==1]['Age'],data[data['Survived']==0]['Age']], 
             stacked=True, color = ['g','r'],bins = 30,label = ['Survived','Dead'])
    #Apply the labels
    plt.xlabel('Age')
    plt.ylabel('Number of passengers')
    plt.legend()
    plt.show()
    
    #Plot some intresting information about passengers verus. fare. 
    figure = plt.figure(figsize=(10,5))
    #Create the histogram
    plt.hist([data[data['Survived']==1]['Fare'],data[data['Survived']==0]['Fare']],
             stacked=True, color = ['g','r'],bins = 30,label = ['Survived','Dead'])
    #Apply the labels
    plt.xlabel('Fare')
    plt.ylabel('Number of passengers')
    plt.legend()
    plt.show()

#Guard the main functino call
if __name__ == '__main__':
     main()
