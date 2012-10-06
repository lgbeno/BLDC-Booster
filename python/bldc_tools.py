def load_adc(farg, *args):
  file=open(farg)
  go=0
  last_data="00"
  upper=1
  list=[]
  for line in file:
    s=line.find('[hex]')+5
    data=line[s:s+2]
    
    if go==1:
      if upper==1:
        upper=0
      else:
        upper=1
        big_data=last_data+data
        if big_data!="FFFF":
          list.append(int(big_data,16))
        else:
          return list
    
    if last_data=="FF" and data=="FF":
      go=1
    last_data=data
    
  return list
  
def load_integrals(farg, *args):
  file=open(farg)
  go=0
  last_data=["00","00","00"]
  MSB=4
  list=[]
  for line in file:
    s=line.find('[hex]')+5
    data=line[s:s+2]
    big_data=last_data[2]+last_data[1]+last_data[0]+data
    if big_data!="FFFFFFFF":
      if go==1:
        if MSB==1:
          list.append(int(big_data,16))
          MSB=4
        else:
          MSB=MSB-1
    else:
        go=1
    last_data[2]=last_data[1]
    last_data[1]=last_data[0]
    last_data[0]=data
    
  return list
  
def plot_adc(farg, *args):
  import matplotlib
  matplotlib.pyplot.plot(load_adc(farg))
  matplotlib.pyplot.plot(load_adc(farg),'ro')
  matplotlib.pyplot.title(farg)
  
def plot_integral(farg, *args):
  import matplotlib
  data=load_integrals(farg)
  matplotlib.pyplot.plot(data[:len(data)-1])
  matplotlib.pyplot.title(farg)
  
def hist_integral(farg, *args):
  import matplotlib
  data=load_integrals(farg)
  matplotlib.pyplot.hist(data[:len(data)-1], 50, normed=1, facecolor='blue')
  matplotlib.pyplot.title(farg)
  [t,d,u,l]=find_thresh(farg)
  matplotlib.pyplot.axvline(x=t,c='g',lw=5)
  matplotlib.pyplot.axvline(x=(t-d),c='b',lw=3)
  matplotlib.pyplot.axvline(x=(t+d),c='b',lw=3)
  matplotlib.pyplot.axvline(x=u,c='r',lw=1)
  matplotlib.pyplot.axvline(x=l,c='r',lw=1)
  matplotlib.pyplot.legend(["center: "+str(t),"delta: "+str(d)])
  
  
def subp_adc(farg, *args):
  import glob
  import math
  import matplotlib
  files=glob.glob(farg)
  sq=math.ceil(math.sqrt(len(files)))
  i=1
  for file in files:
    matplotlib.pyplot.subplot(sq,sq,i)
    plot_adc(file)
    i=i+1
    
def subp_integral(farg, *args):
  import glob
  import math
  import matplotlib
  files=glob.glob(farg)
  sq=math.ceil(math.sqrt(len(files)))
  i=1
  for file in files:
    matplotlib.pyplot.subplot(sq,sq,i)
    plot_integral(file)
    i=i+1
    
def subh_integral(farg, *args):
  import glob
  import math
  import matplotlib
  files=glob.glob(farg)
  sq=math.ceil(math.sqrt(len(files)))
  i=1
  for file in files:
    matplotlib.pyplot.subplot(sq,sq,i)
    hist_integral(file)
    i=i+1
    
def find_thresh(farg, *args):
  import numpy
  data=load_integrals(farg)
  middle=numpy.mean(data)
  lower=[]
  upper=[]
  for each in data:
    if each<middle:
      lower.append(each)
    else:
      upper.append(each)
  u=numpy.mean(upper)
  l=numpy.mean(lower)
  
  return [middle,numpy.mean([u-middle,middle-l]),u,l]
  
def thresh_array(farg, *args):
  import glob
  files=glob.glob(farg)
  mid_array=[]
  delta_array=[]
  u_array=[]
  l_array=[]
  for file in files:
    [t,d,u,l]=find_thresh(file)
    mid_array.append(t)
    delta_array.append(d)
    u_array.append(u)
    l_array.append(l)
  return [mid_array,delta_array,u_array,l_array]
  
def thresh_analysis(farg, *args):
  import glob
  import math
  import matplotlib
  [m,d,u,l]=thresh_array(farg)
  matplotlib.pyplot.subplot(2,2,1)
  matplotlib.pyplot.hist(m, 50, normed=1, facecolor='blue', alpha=0.75)
  matplotlib.pyplot.title("Center Point")
  matplotlib.pyplot.subplot(2,2,2)
  matplotlib.pyplot.hist(m, 50, normed=1, facecolor='blue', alpha=0.75)
  matplotlib.pyplot.title("Delta")
  matplotlib.pyplot.subplot(2,2,3)
  matplotlib.pyplot.hist(m, 50, normed=1, facecolor='blue', alpha=0.75)
  matplotlib.pyplot.title("Upper")
  matplotlib.pyplot.subplot(2,2,4)
  matplotlib.pyplot.hist(m, 50, normed=1, facecolor='blue', alpha=0.75)
  matplotlib.pyplot.title("Lower")