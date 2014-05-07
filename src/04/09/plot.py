#!/usr/bin/env python

import numpy as np
import matplotlib.gridspec as gridspec
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

gs = gridspec.GridSpec(2, 1, height_ratios=[4,1])
gs.update(hspace=0)
axplot = plt.subplot(gs[0])
axpull = plt.subplot(gs[1])

axplot.errorbar(d, unp.nominal_values(Vs), yerr=unp.std_devs(Vs), fmt='k.', label='MCMC')
axplot.plot(d, V(d), 'r-', label='Analytical')
axplot.set_yscale('log')
axplot.set_ylabel('$V_S(d)$')
axplot.set_xticklabels([])
axplot.legend(loc='best')
axplot.minorticks_on()

pulls = (unp.nominal_values(Vs) - V(d)) / unp.std_devs(Vs)
axpull.vlines(d, 0, pulls)
axpull.set_xlabel('$d$')
axpull.set_ylabel('Pull [$\sigma$]')
axpull.set_ylim(-4, 4)
axpull.set_yticks([-2, 0, 2])
axpull.minorticks_on()
plt.savefig('plot.pdf')

print('V_S(200)')
print('Analytical: {}'.format(V(200)))
print('MCMC: {}'.format(Vs[199]))
print('Pull: {}'.format(pulls[199]))
