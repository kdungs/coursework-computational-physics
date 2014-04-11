#!/usr/bin/env python

import matplotlib.pyplot as plt
import numpy as np
from sys import stdin

x, y = np.loadtxt(stdin, dtype=np.int, unpack=True)
plt.plot(x, y)
xmin, xmax = plt.xlim()
plt.xlim(xmin - 1, xmax + 1)
ymin, ymax = plt.ylim()
plt.ylim(ymin - 1, ymax + 1)
plt.minorticks_on()
plt.grid()
plt.savefig('walk.pdf')
