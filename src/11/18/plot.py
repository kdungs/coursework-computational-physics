#!/usr/bin/env python

import glob
import matplotlib.pyplot as plt
import numpy as np
import re
import sys

def plot(xi, fname, i):
    psi = np.genfromtxt('data/{}'.format(fname), unpack=True)
    t = float(re.match(r'psi_([\d\.]+).txt', fname).group(1))
    plt.plot(xi, psi, 'm-')
    plt.xlabel(r'$\xi$', ha='right', x=1)
    plt.ylabel(r'$|\psi(\xi)|^2$', ha='right', y=1)
    plt.ylim(0., 1.)
    plt.minorticks_on()
    plt.grid()
    plt.title(r'$\tau = {}$'.format(t))
    plt.savefig('plots/{:04d}.png'.format(i))
    plt.clf()


if __name__ == '__main__':
    xi = np.genfromtxt('data/xi.txt', unpack=True)
    files = sorted(glob.glob('data/psi*.txt'))
    for i, fname in enumerate(files):
            print(fname)
            plot(xi, fname[5:], i)
