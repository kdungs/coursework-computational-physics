#!/usr/bin/env python

from itertools import product
import numpy as np
import matplotlib.pyplot as plt

for kbT, c in product(['1', '2.25', '3', '50', '1000'],
                      ['u', 'r']):
    sc = np.loadtxt('sc_{}_{}.txt'.format(c, kbT), unpack=True)
    plt.pcolor(sc, cmap=plt.cm.copper)
    plt.savefig('sc_{}_{}.png'.format(c, kbT))
