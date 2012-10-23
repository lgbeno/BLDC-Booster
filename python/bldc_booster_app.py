import os
import time
import sys
from PyQt4 import QtGui,QtCore
from bldc_booster_api import *
from bldc_booster_ui import *

class MyForm(QtGui.QMainWindow):
    def __init__(self, parent=None):
        #build parent user interface
        QtGui.QWidget.__init__(self, parent)
        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)
        
        #Connect Buttons
        self.connect(self.ui.connectBtn, QtCore.SIGNAL('clicked()'),self.connectClick)
        self.connect(self.ui.disconnectBtn, QtCore.SIGNAL('clicked()'),self.disconnectClick)
        self.connect(self.ui.captureBtn, QtCore.SIGNAL('clicked()'),self.captureClick)
        self.connect(self.ui.dutyCycle, QtCore.SIGNAL('editingFinished()'),self.dutyClick)
        self.connect(self.ui.dutyCycle, QtCore.SIGNAL('clicked()'),self.dutyClick)
        self.connect(self.ui.fluxThreshold, QtCore.SIGNAL('editingFinished()'),self.thresholdClick)
        self.connect(self.ui.fluxThreshold, QtCore.SIGNAL('clicked()'),self.thresholdClick)
        self.connect(self.ui.fluxSlider, QtCore.SIGNAL('valueChanged(int)'),self.thresholdSlide)
        self.connect(self.ui.dutySlider, QtCore.SIGNAL('valueChanged(int)'),self.dutySlide)
        self.connect(self.ui.enableChk, QtCore.SIGNAL('stateChanged(int)'),self.enableClick)
        
        self.ui.dutySlider.setTracking(True)
        self.ui.fluxSlider.setTracking(True)
        
        self.ui.disconnectBtn.setEnabled(False)
        
    def enableClick(self,state):
      if state:
        self.p.startup(10)
        self.p.set_duty(50)
        time.sleep(0.01)
        self.p.set_duty(self.ui.dutyCycle.value())
      else:
        self.p.set_duty(0)
        
    def thresholdSlide(self):
      self.ui.fluxThreshold.setValue(self.ui.fluxSlider.value())
      self.p.set_threshold(self.ui.fluxThreshold.value())
      
    def dutySlide(self):
      self.ui.dutyCycle.setValue(self.ui.dutySlider.value())
      self.p.set_duty(self.ui.dutyCycle.value())
    
    def captureClick(self):
      data=self.p.get_data(self.ui.plotSelect.currentIndex()+1)
      self.ui.plotWidget.axes.plot(data,'ro')
      self.ui.plotWidget.draw()
      self.ui.plotWidget.axes.plot(data)
      self.ui.plotWidget.draw()
    
    def dutyClick(self):
      self.ui.dutySlider.setValue(self.ui.dutyCycle.value())
      
    def thresholdClick(self):
      self.ui.fluxSlider.setValue(self.ui.fluxThreshold.value())
      
    def connectClick(self):
      try:
        if self.p.isOpen() != True:
          self.p.open()
      except:
        self.p=bldc_booster(str(self.ui.comPort.text()))
      self.ui.connectBtn.setEnabled(False)
      self.ui.disconnectBtn.setEnabled(True)
      self.enableClick(False)
    
    def disconnectClick(self):
      try:
        if self.p.isOpen() == True:
          self.p.close()
      except:
        pass
        
      self.ui.connectBtn.setEnabled(True)
      self.ui.disconnectBtn.setEnabled(False)

if __name__ == "__main__":
    app = QtGui.QApplication(sys.argv)
    myapp = MyForm()
    myapp.show()
    sys.exit(app.exec_())