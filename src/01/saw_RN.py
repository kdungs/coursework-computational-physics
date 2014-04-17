#!/usr/bin/env python

import matplotlib.pyplot as plt
import numpy as np
from scipy.optimize import curve_fit
from sys import stdin
import uncertainties as u
import uncertainties.unumpy as unp

nom = unp.nominal_values
std = unp.std_devs

N, RN2, RN2_ = np.loadtxt(stdin, dtype=np.float, unpack=True)
RN = unp.sqrt(unp.uarray(RN2, RN2_))

f = lambda x, a, b, c: a * x ** b + c
popt, pcov = curve_fit(f, N, nom(RN), sigma=std(RN))
params = unp.uarray(popt, np.sqrt(np.diag(pcov)))

n = np.linspace(15, 65, 10000)
plt.errorbar(N, nom(RN), yerr=std(RN), fmt='ko', label='Data')
plt.plot(n, f(n, *popt), label='Fit: $ax^b+c$')
print('a = {:L}\nb = {:L}\nc = {:L}'.format(*params))
plt.text(50, 0.6, '$a = {:L}$\n$b = {:L}$\n$c = {:L}$'.format(*params))
plt.xlabel('$N$')
plt.ylabel('$R_N$')
plt.legend(loc='best')
plt.savefig('RN.pdf')
