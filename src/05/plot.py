#!/usr/bin/env python

import numpy as np
import matplotlib.pyplot as plt

sc = np.loadtxt('test.txt', unpack=True)

plt.pcolor(sc, cmap=plt.cm.copper)
plt.savefig('test.png')
