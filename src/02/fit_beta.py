import numpy as np
import matplotlib.pyplot as plt
from os import listdir
from scipy.optimize import curve_fit

files = listdir('./data/clustersize/')
for f in files:
    size = int(f.split('.')[0])

    p, maxSize = np.loadtxt('./data/clustersize/' + f, unpack=True)
    pIndex = np.where(p > 0.6)
    pFit = p[:pIndex[0][0]]
    maxSizeFit = maxSize[:pIndex[0][0]]

    func = lambda x, a, b: a * abs(x - 0.597) ** (-b)
    coeff, coeff_var = curve_fit(func, pFit, maxSizeFit,
                                 p0=[1.0, -0.1], maxfev=10000)

    plt.errorbar(p,
                 maxSize,
                 yerr=np.sqrt(maxSize), fmt='kx', label='Data')
    parr = np.arange(min(pFit), 0.597, 0.001)
    plt.plot(parr, func(parr, coeff[0], coeff[1]), 'r-', label='Fit')
    plt.xlabel('$p$')
    plt.ylabel('$M_{\\infty}$')
    plt.legend(['Data', 'Fit'], 'best')
    plt.text(0.2, 0.7 * max(maxSize),
             '$\\beta = {:.4f}\pm{:.4f}$'.format(coeff[1],
                                                 np.sqrt(coeff_var[1][1])))
    plt.ylim([0, max(maxSize)])
    plt.savefig('./plots/beta/' + f.replace('txt', 'pdf'))
    plt.clf()
