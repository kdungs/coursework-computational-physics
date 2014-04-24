#!/usr/bin/env python

import glob
import numpy as np
import matplotlib.pyplot as plt
import re

R = 1e3

files = glob.glob('data/q_*.txt')
for f in files:
    L = float(re.match(r'^data/q_([\d\.]+).txt', f).group(1))
    p, qL = np.loadtxt(f, unpack=True)
    qL_ = qL * (1 - qL) / np.sqrt(1000)  # Binomial errors
    plt.errorbar(p, qL, yerr=qL_, fmt='.', label='$L = {}$'.format(L))
plt.xlabel('$p$')
plt.ylabel('$q_L$')
plt.minorticks_on()
plt.grid()
plt.legend(loc='best')
plt.savefig('qL.pdf')
