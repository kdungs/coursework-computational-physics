#!/usr/bin/env python

import numpy as np
import matplotlib.pyplot as plt

t, Eu, Eu_, Er, Er_ = np.loadtxt('E.txt', unpack=True)

plt.plot(t, -Eu, label='Uniform')
plt.plot(t, -Er, label='Random')
plt.xlabel('$\log(t)$')
plt.ylabel('$\log (-E)$')
plt.xscale('log')
plt.yscale('log')
plt.legend(loc='best')
plt.savefig('E.png')

#plt.plot(t[t > 1e5], Eu[t > 1e5])
#plt.plot(t[t > 1e5], Er[t > 1e5])
