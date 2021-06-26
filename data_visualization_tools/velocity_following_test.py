#!/usr/bin/env python
# coding: utf-8


import bagpy
from bagpy import bagreader
import matplotlib.pyplot as plt
import pandas as pd
import sys


# In[3]:
n = len(sys.argv)
print("Total arguments passed:", n)

if n != 2:
  sys.exit("usage: script.py filepath")


b = bagreader(sys.argv[1])

# In[4]:


# b.topic_table


# In[5]:


CMD_ACC_MSG = b.message_by_topic('/timed_accel')
ACC_MSG = b.message_by_topic('/vehicle/accel')
GHOST_VEL_MSG = b.message_by_topic('/ghost/vehicle/vel')
VEL_MSG = b.message_by_topic('/vehicle/vel')


# In[8]:


df_cacc = pd.read_csv(CMD_ACC_MSG)
df_acc = pd.read_csv(ACC_MSG)
df_gvel = pd.read_csv(GHOST_VEL_MSG)
df_vel = pd.read_csv(VEL_MSG)
t0 = df_gvel['Time'][0]


# In[ ]:





# In[9]:


import plotly.graph_objects as go
from plotly.subplots import make_subplots
figr = make_subplots(rows=2, cols=1, subplot_titles=("", ""))

figr.append_trace(go.Scattergl(
    x=df_gvel['Time']-t0,
    y=df_gvel['twist.linear.x'],
     name="Vel ghost",
      line=dict(
        width=4,
          color="red",
     ),
), row=1, col=1)
figr.append_trace(go.Scattergl(
    x=df_vel['Time']-t0,
    y=df_vel['twist.linear.x'],
          line=dict(
        width=3,
          color="blue",
     ),
    name="Vel ego",
), row=1, col=1)


figr.append_trace(go.Scattergl(
    x=df_cacc['Time']-t0, 
#     y=speed_msg2["twist.linear.x"],
    y=df_cacc['data'],
       mode='markers',
          marker=dict(
            size=5,
          ),
    name="cmd accel",
), row=2, col=1)
figr.append_trace(go.Scattergl(
    x=df_acc['Time']-t0, 
#     y=speed_msg2["twist.linear.x"],
    y=df_acc['accel.linear.x'],
       mode='markers',
          marker=dict(
            size=5,
          ),
    name="accel",
), row=2, col=1)

figr.update_xaxes(title_text="Time(s)", row=2, col=1)

figr.update_yaxes(title_text="Vel (m/s)", row=1, col=1)
figr.update_yaxes(title_text="Accel (m/s^2)", row=2, col=1)



figr.update_layout(height=600, width=800, title_text="Low_speed_test")
figr.show()

