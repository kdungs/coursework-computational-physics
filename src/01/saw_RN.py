#!/usr/bin/env python

import matplotlib.pyplot as plt
import numpy as np
from scipy.optimize import curve_fit
from scipy.stats import linregress
import uncertainties.unumpy as unp


N, RN2 = np.loadtxt('data.txt', unpack=True)
RN = np.sqrt(RN2)
RN_ = RN ** (4/3)


print('With $R_N^{4/3}$')
a, b, r, p, err = linregress(RN_, N)
print('a = {}'.format(a))
print('b = {}'.format(b))
print('p = {}'.format(p))
print('err = {}'.format(err))
print('r^2 = {}'.format(r ** 2))
plt.plot(RN_, N, 'ko', label='Data')
n = np.linspace(0, max(RN_), 10000)
plt.plot(n, a * n + b)
plt.xlabel('$R_N^{4/3}$')
plt.ylabel('$N$')
plt.minorticks_on()
plt.savefig('saw_N_RN43.pdf')
plt.clf()

print('With $R_N$')
a, b, r, p, err = linregress(RN, N)
print('a = {}'.format(a))
print('b = {}'.format(b))
print('p = {}'.format(p))
print('err = {}'.format(err))
print('r^2 = {}'.format(r ** 2))
plt.plot(RN, N, 'ko', label='Data')
n = np.linspace(0, max(RN), 10000)
plt.plot(n, a * n + b)
plt.xlabel('$R_N$')
plt.ylabel('$N$')
plt.minorticks_on()
plt.savefig('saw_N_RN.pdf')
plt.clf()
