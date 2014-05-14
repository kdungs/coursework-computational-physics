#!/usr/bin/env python

import numpy as np
import matplotlib.pyplot as plt

t, Eu, Eu_, Er, Er_ = np.loadtxt('E.txt', unpack=True)

plt.plot(t, Eu, label='Uniform')
plt.plot(t, Er, label='Random')
plt.xlabel('$t$')
plt.xscale('log')
plt.ylabel(r'$E = \langle\mathcal{H}\rangle$')
plt.legend(loc='best')
plt.savefig('E.png')
