from audioop import avg
from cProfile import label
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import re

def read_data():
    with open("./time_using.txt","r") as file:
        rows=file.readlines()

        time_using=pd.DataFrame(columns=['width','height','window_size','time_using'])
        total_time=[]
        for row in rows:
            cur=len(time_using)
            if row.find("参数")!=-1:
                if len(time_using)!=0:
                    time_using.loc[cur-1,'time_using']=total_time.copy()
                    total_time.clear()
                width,height,window_size=re.findall(r"\d+",row)
                time_using.loc[cur,'width']=int(width)
                time_using.loc[cur,'height']=int(height)
                time_using.loc[cur,'window_size']=int(window_size)
            else:
                time=re.findall(r"\d+.\d+",row)
                total_time.append(float(time[0]))

        time_using.loc[len(time_using)-1,'time_using']=total_time.copy()
        print(time_using)

        return time_using

def draw(time_using:pd.DataFrame):
    min_time_using=[]
    max_time_using=[]
    avg_time_using=[]

    for data in time_using['time_using']:
        # print(data)
        min_time_using.append(min(data))
        max_time_using.append(max(data))
        avg_time_using.append(sum(data)/len(data))
    
    width=time_using['width']
    window_size=time_using['window_size']

    X=[]
    for i in range(len(width)):
        X.append(str((width[i],window_size[i])))
    
    print(min_time_using)
    print(max_time_using)
    print(X)

    fig, ax = plt.subplots()

    ax.plot(X,min_time_using,label="min time using")
    ax.plot(X,avg_time_using,label="avg time using")
    ax.plot(X,max_time_using,label="max time using")

    ax.fill_between(X, min_time_using, avg_time_using, interpolate=True, color='red', alpha=0.2)
    ax.fill_between(X, avg_time_using, max_time_using, interpolate=True, color='green', alpha=0.2)

    ax.set_xlabel('width & window size')
    ax.set_ylabel('time using')
    ax.legend()

    plt.savefig("original.png")

    

if __name__=="__main__":
    time_using=read_data()
    draw(time_using)
