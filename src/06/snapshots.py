#!/usr/bin/env python

import numpy as np
import matplotlib.pyplot as plt

for i in range(0, 10):
    sc = np.loadtxt('m_{}.txt'.format(i), unpack=True)
    plt.pcolor(sc, cmap=plt.cm.copper)
    plt.savefig('m_{}.png'.format(i))
