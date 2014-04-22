#!/usr/bin/env python

import matplotlib.pyplot as plt
import numpy as np
from scipy.stats import linregress
import uncertainties as u


N, RN2 = np.loadtxt('data.txt', unpack=True)
RN = np.sqrt(RN2)
logN = np.log(N)
logRN = np.log(RN)

a, b, r, p, err = linregress(logN, logRN)
mx = logN.mean()
sx2 = ((logN - mx) ** 2).sum()
sd_a = err * np.sqrt(1 / len(logN) + mx ** 2 / sx2)
sd_b = err * np.sqrt(1 / sx2)
a_ = u.ufloat(a, sd_a)
b_ = u.ufloat(b, sd_b)
print('a = {:L}'.format(a_))
print('b = {:L}'.format(b_))

plt.plot(logN, logRN, 'ko')
n = np.linspace(0, max(logN), 1000)
plt.plot(n, a * n + b)
plt.xlabel('$\log N$')
plt.ylabel('$\log R_N$')
plt.minorticks_on()
plt.savefig('saw_log.pdf')
