#!/usr/bin/env python

import numpy as np
import matplotlib.pyplot as plt

for kbT in ['1', '2.25', '3', '50', '1000', 'decoupled']:
    t, Eu, Eu_, Er, Er_ = np.loadtxt('{}.txt'.format(kbT), unpack=True)
    plt.plot(t, Eu, label='Uniform')
    plt.plot(t, Er, label='Random')
    plt.xlabel('$t$')
    plt.xscale('log')
    plt.ylabel(r'$E = \langle\mathcal{H}\rangle$')
    plt.legend(loc='best')
    plt.title('$k_BT = {}$'.format(kbT))
    plt.savefig('{}.png'.format(kbT))
    plt.clf()
