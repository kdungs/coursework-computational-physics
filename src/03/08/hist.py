#!/usr/bin/env python

import numpy as np
import matplotlib.pyplot as plt

x = np.loadtxt('data.txt')

plt.hist(x)
plt.show()
