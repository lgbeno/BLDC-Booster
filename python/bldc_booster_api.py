import serial
import matplotlib

class bldc_booster:
  
  def __init__(self, com_port):
    self.p=serial.Serial(com_port,baudrate='9600',timeout=2)
    
  def send_command(self,cmd,data):
    self.p.write(chr(0x80|(0x3F&cmd)))
    self.p.write(chr((data>>12)&0x0F))
    self.p.write(chr((data>>8)&0x0F))
    self.p.write(chr((data>>4)&0x0F))
    self.p.write(chr(data&0x0F))
    
  def set_duty(self, duty):
    self.send_command(0x00,duty)
      
  def startup(self, comms):
    self.send_command(0x03,comms)
    
  def set_threshold(self, thresh):
    self.send_command(0x01,thresh)
  
  def get_data(self,buffer_id):
    self.send_command(0x02,buffer_id)
    
    num_samples = 169
    cnt=0
    go=0
    last_data="00"
    upper=1
    list=[]
    while(cnt<num_samples):
      data=hex(ord(self.p.read(1)))[2:]
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