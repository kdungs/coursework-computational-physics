#!/usr/bin/env python

import matplotlib
import matplotlib.colors as col
import matplotlib.pyplot as plt
import numpy as np

labels = np.loadtxt('data.txt')
fig, ax = plt.subplots()
colors = [('white')] + [(matplotlib.cm.jet(i)) for i in range(1,256)]
new_map = col.LinearSegmentedColormap.from_list('new_map', colors, N=256)

heatmap = ax.pcolor(labels, cmap=new_map)
plt.show()
