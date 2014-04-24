#!/usr/bin/env python

import itertools as it
import numpy as np
#import prettyplotlib as ppl  # https://github.com/olgabot/prettyplotlib
import matplotlib as mpl
import matplotlib.pyplot as plt
import random

def flat_for(a, f):
    a = a.reshape(-1)
    for i, v in enumerate(a):
        a[i] = f(v)

labels = np.loadtxt('data.txt')

# this helps having better colors
unique = set(labels.flatten())
unique_map = {label: i for label, i in zip(unique, it.count())}
random_indices = np.linspace(1, 3, len(unique))
random.shuffle(random_indices)
flat_for(labels, lambda l: random_indices[unique_map[l]] if l != 0 else 0)

mask = np.zeros(labels.shape)
mask[labels == 0] = 1
masked_labels = np.ma.masked_array(labels, mask)

plt.cm.jet.set_bad(color='white', alpha=None)
plt.pcolor(masked_labels, cmap=plt.cm.jet)
plt.savefig('cluster.pdf')
