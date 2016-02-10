'''
    This Source code contains the material used in the Provant Simulator.
    Author: Arthur Viana Lara
    Version: 1.0
    Data: -- 
'''


#!/usr/bin/env python
import wx

#import rospy

class HomeFrame(wx.Dialog):
    '''
    This Class has the content of Initial Frame of the application.
    It has: 
    1- the name of simulator
    2- the images of ProVant and Macro 
    3- a button to start the application 
    '''
    Nextframe = 0;
    
    def __init__(self, parent, title = "Home"):
       
            wx.Dialog.__init__(self, 
                        parent, 
                        title=title, 
                        size=(700,700),
                        pos = (0,0),
                        style = wx.DEFAULT_FRAME_STYLE & ~wx.MAXIMIZE_BOX ^ wx.RESIZE_BORDER )
       
        
            self.panel = wx.Panel(self)
            #Title of Application
            self.text = wx.StaticText(self.panel,
                                      label = "Simulador ProVANT",
                                      pos = (220,50))
            font = wx.Font(18, wx.DECORATIVE,wx.FONTWEIGHT_NORMAL, wx.NORMAL)
            self.text.SetFont(font)
            
            #Next Button 
            self.Next = wx.Button(self.panel,
                                  label="Next",
                                  pos = (600,630),
                                  size = (60,30) )
            self.Bind(wx.EVT_BUTTON,self.NextButton,self.Next)
            
            self.Show(True)
        

    def NextButton(self,event):
        self.NextFrame = ModelFrame(self)
        self.Show(False)

class ModelFrame(wx.Frame):
    '''
    This Class has:
    1- a field to choose the model of application
    2- a box with the mechanical parameters of chosen model
    3- two buttons: back button and next button
    '''
    NextFrame = 0;
    BackFrame = 0;
    
    def __init__(self, parent, title = "Choose a model"):
        wx.Frame.__init__(self, 
                        parent, 
                        title=title, 
                        size=parent.GetSize(),
                        pos = parent.GetPosition(),
                        style = wx.DEFAULT_FRAME_STYLE & ~wx.MAXIMIZE_BOX ^ wx.RESIZE_BORDER )
        
        panel=wx.Panel(self)
        
        #Next Button
        Next = wx.Button(panel,
                        label="Next",
                        pos = (600,630),
                        size = (60,30) )
        self.Bind(wx.EVT_BUTTON,self.NextButton,Next)
        
        #Back Button
        self.BackFrame = parent
        Back = wx.Button(panel,
                        label="Back",
                        pos = (40,630),
                        size = (60,30))
        self.Bind(wx.EVT_BUTTON,self.BackButton,Back)
        self.Show(True)

    def NextButton(self,event):
        self.NextFrame = SensorsFrame(self)
        self.Show(False)

    def BackButton(self,event):
        self.Show(False)
        self.BackFrame.SetPosition(self.GetPosition()) 
        self.BackFrame.Show(True)

class SensorsFrame(wx.Frame):
    '''
    This Class has:
    1- box with a list of sensors that the user has already included
    2- five buttons:
        - To include some sensor
        - To set some sensor up
        - To exclude some sensor
        - a back button
        - next button
    '''
    NextFrame = 0;
    BackFrame = 0;
    
    def __init__(self, parent, title = "Sensors setup"):
        wx.Frame.__init__(self, 
                        parent, 
                        title=title, 
                        size=parent.GetSize(),
                        pos = parent.GetPosition(),
                        style = wx.DEFAULT_FRAME_STYLE & ~wx.MAXIMIZE_BOX ^ wx.RESIZE_BORDER )
       
        panel=wx.Panel(self)
        
        #Next Button
        Next = wx.Button(panel,
                        label="Next",
                        pos = (600,630),
                        size = (60,30) )
        self.Bind(wx.EVT_BUTTON,self.NextButton,Next)
        
        #Back Button
        self.BackFrame = parent
        Back = wx.Button(panel,
                        label="Back",
                        pos = (40,630),
                        size = (60,30))
        self.Bind(wx.EVT_BUTTON,self.BackButton,Back)
        self.Show(True)

    def NextButton(self,event):
        self.NextFrame = SimulationFrame(self)
        self.Show(False)
        
    def BackButton(self,event):
        self.Show(False)
        self.BackFrame.SetPosition(self.GetPosition()) 
        self.BackFrame.Show(True)

class SimulationFrame(wx.Frame):
    '''
    This Class is responsible of the main configuration of the simulator
    It has a field to choose the initial position of the VANT, 
    a filed to choose an engine, a field  to choose the name of output file 
    with the input signals e and output signals of the model during the simulation
    and, finally, two button: back button and play simulation button 
    
    '''
    
    BackFrame = 0;
    
    def __init__(self, parent, title = "Simulation setup"):
        wx.Frame.__init__(self, 
                          parent, 
                          title=title, 
                          size=parent.GetSize(),
                          pos = parent.GetPosition(),
                          style = wx.DEFAULT_FRAME_STYLE & ~wx.MAXIMIZE_BOX ^ wx.RESIZE_BORDER )
        
        self.BackFrame = parent
        panel=wx.Panel(self)
        
        #Play Button
        Play = wx.Button(panel,
                        label="Play",
                        pos = (600,630),
                        size = (60,30) )
        self.Bind(wx.EVT_BUTTON,self.PlayButton,Play)
        
        #Back Button
        Back = wx.Button(panel,
                         label="Back",
                         pos = (40,630),
                         size = (60,30) )
        
        self.Bind(wx.EVT_BUTTON,self.BackButton,Back)
        self.Show(True)
        

    def PlayButton(self,event):
        
        return

    def BackButton(self,event):
        self.Show(False)
        self.BackFrame.SetPosition(self.GetPosition()) 
        self.BackFrame.Show(True)
        

if __name__=='__main__':
    app = wx.App(False)  # Create a new app, don't redirect stdout/stderr to a window.
    frame1 = HomeFrame(None) # A Frame is a top-level window.
    app.MainLoop()
    
    
