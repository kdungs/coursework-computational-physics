#!/usr/bin/env python

import numpy as np
import matplotlib.pyplot as plt
from scipy.integrate import dblquad
from scipy.special import jn

def analytical(a, q):
    return -2 * np.pi * a * (jn(1, a * q) - 2  * jn(1, 2 * a * q)) / q

xs = np.linspace(0, 10, 10000)
plt.plot(xs, analytical(1, xs) ** 2)
plt.show()
