#!/usr/bin/env python

import numpy as np
import matplotlib.pyplot as plt

Tc_anal = 2 / np.log(1 + np.sqrt(2))

Ls = np.arange(25, 101, 25)
Tc = []

for L in Ls:
    T, Cv = np.loadtxt("critical_temperature_{}.txt".format(L), unpack=True)
    Tc.append(T[np.argmax(Cv)])
    plt.plot(T, Cv, label='L = {}'.format(L))
plt.xlabel('$T$')
plt.ylabel('$C_v$')
plt.legend(loc='best')
plt.savefig("crit.pdf")
plt.clf()

plt.loglog(1 / Ls, Tc)
plt.plot([1e-2, 1e-1], [Tc_anal, Tc_anal])
plt.xlabel('$L^{-1}$')
plt.ylabel('$T_c$')
plt.savefig('Tc_L.pdf')
