#!/usr/bin/env python

import numpy as np
import matplotlib.pyplot as plt
from scipy.special import gamma

def V(d):
    return np.pi ** (d / 2) / gamma(1 + d / 2)

ds = np.linspace(1, 200)
plt.plot(ds, V(ds))
plt.xlabel('$d$')
plt.ylabel('$V_S(d)$')
plt.yscale('log')
plt.minorticks_on()
plt.savefig('analytical.pdf')

print('V_S(1) = {}'.format(V(1)))
print('V_S(200) = {}'.format(V(200)))
