#import serial

#class bldc_booster:
  
  #def __init__(self, com_port):
  #  self.p=serial.Serial(com_port,baudrate='9600')
  
def get_adc(p, num_samples):
  cnt=0
  go=0
  last_data="00"
  upper=1
  list=[]
  while(cnt<num_samples):
    data=hex(ord(p.read(1)))[2:]
    if len(data)==1:
      data='0'+data
    if go==1:
      if upper==1:
        upper=0
      else:
        upper=1
        big_data=last_data+data
        if big_data!="ffff":
          list.append(int(big_data,16))
          cnt=cnt+1
    if last_data=="ff" and data=="ff":
      go=1
    last_data=data
    
  return list

def set_duty(p, duty):
  p.write(chr(0x80))
  p.write(chr((duty>>12)&0x0F))
  p.write(chr((duty>>8)&0x0F))
  p.write(chr((duty>>4)&0x0F))
  p.write(chr(duty&0x0F))
  
def set_threshold(p, thresh):
  p.write(chr(0x81))
  p.write(chr((thresh>>12)&0x0F))
  p.write(chr((thresh>>8)&0x0F))
  p.write(chr((thresh>>4)&0x0F))
  p.write(chr(thresh&0x0F))

def get_integrals(p, num_samples):
  cnt=0
  go=0
  last_data=["00","00","00"]
  MSB=4
  list=[]
  while(cnt<num_samples):
    data=hex(ord(p.read(1)))[2:]
    if data!='0':
      if len(data)==1:
        data='0'+data
      big_data=last_data[2]+last_data[1]+last_data[0]+data
      print big_data
      if big_data!="ffffffff":
        if go==1:
          if MSB==1:
            list.append(int(big_data,16))
            MSB=4
          else:
            MSB=MSB-1
      else:
          MSB=4
          if go==0:
            go=1
          else:
            cnt=cnt+1
      last_data[2]=last_data[1]
      last_data[1]=last_data[0]
      last_data[0]=data
  return list