#!/usr/bin/env python

import numpy as np
import matplotlib.pyplot as plt
from scipy.special import gamma
import uncertainties as u
import uncertainties.unumpy as unp

def V(d):
    return np.pi ** (d / 2) / gamma(1 + d / 2)

d = np.arange(1, 201)
Qs, Qs_ = np.loadtxt('Qs.txt', unpack=True)
Qs = unp.uarray(Qs, Qs_)
Vs = [2]
for i in range(0, 199):
    Vs.append(Qs[i] * 2 * Vs[i])

f, (axplot, axpull) = plt.subplots(2, sharex=True)
axplot.errorbar(d, unp.nominal_values(Vs), yerr=unp.std_devs(Vs), fmt='k.', label='MCMC')
axplot.plot(d, V(d), 'r-', label='Analytical')
axplot.set_yscale('log')
axplot.set_ylabel('$V_S(d)$')
axplot.legend(loc='best')
axpull.plot(d, (unp.nominal_values(Vs) - V(d)) / unp.std_devs(Vs), 'k.')
axpull.set_xlabel('$d$')
axpull.set_ylabel('Pull')
plt.savefig('plot.pdf')
