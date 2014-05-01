#!/usr/bin/env python

import numpy as np
import matplotlib.pyplot as plt

bm, cl = np.loadtxt('data.txt', unpack=True)

plt.hist(bm, alpha=.5, label='Box-Mueller')
plt.hist(cl, alpha=.5, label='Central Limit Theorem')
plt.legend(loc='best')
plt.savefig('hist.pdf')
