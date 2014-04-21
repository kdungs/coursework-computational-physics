import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as patches
from os import listdir

files = listdir('./data/')
colorarray = np.random.random_sample((100, 3))
for f in files:
    size = int(f.split('-')[0])

    x, y, c = np.loadtxt('./data/' + f, unpack=True)

    fig = plt.figure(figsize=(5, 5))
    ax = fig.add_subplot(111)

    for px, py, col in zip(x, y, c):
        rect = patches.Rectangle((px, py), 1, 1, color=colorarray[col])
        ax.add_patch(rect)
    plt.xlim([0, size])
    plt.ylim([0, size])
    print('./plots/' + f.replace('.txt', '.pdf'))
    plt.savefig('./plots/' + f.replace('.txt', '.pdf'))
    plt.clf()
