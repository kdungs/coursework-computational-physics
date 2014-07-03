#!/usr/bin/env python

import glob
import matplotlib.pyplot as plt
import numpy as np
import re
import sys

def plot(xi, fname):
    psi = np.genfromtxt('data/{}'.format(fname), unpack=True)
    t = float(re.match(r'psi_([\d\.]+).txt', fname).group(1))
    plt.plot(xi, psi, 'm-')
    plt.xlabel(r'$\xi$', ha='right', x=1)
    plt.ylabel(r'$\psi(\xi)$', ha='right', y=1)
    plt.minorticks_on()
    plt.grid()
    plt.title('$t = {}$'.format(t))
    plt.savefig('plots/{}.png'.format(fname[:-4]))
    plt.clf()


if __name__ == '__main__':
    xi = np.genfromtxt('data/xi.txt', unpack=True)
    files = glob.glob('data/psi*.txt')
    for fname in files:
        plot(xi, fname[5:])
