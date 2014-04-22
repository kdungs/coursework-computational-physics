import numpy as np
import matplotlib.pyplot as plt
from os import listdir
from scipy.optimize import curve_fit

files = listdir('./data/')
p_c = []
p_c_err = []
sizes = []
for f in files:
    size = int(f.split('.')[0])
    sizes.append(size)

    p, N = np.loadtxt('./data/' + f, unpack=True)

    p_N = N/max(N)

    func = lambda x, a, b: max(p_N)/2.0 * (np.tanh(a*(x - b)) + 1)

    coeff, coeff_var = curve_fit(func, p, p_N, p0=[1.0, 0.55])
    p_c.append(coeff[1])
    p_c_err.append(coeff_var[1][1])

    plt.errorbar(p,
                 p_N,
                 yerr=np.sqrt((p_N*(1.0-p_N)/max(N))), fmt='kx', label='Data')
    parr = np.arange(min(p), max(p) * 1.1, 0.001)
    plt.plot(parr, func(parr, coeff[0], coeff[1]), 'r-', label='Fit')
    plt.xlabel('$p$')
    plt.ylabel('$N/N_{max}$')
    plt.legend(['Data', 'Fit'], 'best')
    plt.xlim([min(p) * 0.9, max(p) * 1.1])
    plt.ylim([0, max(p_N) * 1.1])
    plt.savefig('./plots/' + f.replace('txt', 'pdf'))
    plt.clf()

p_c_func = lambda x, a, b: a/x + b
coeff, coeff_var = curve_fit(p_c_func, sizes, p_c)
print('{:.4f}({:.4f})'.format(coeff[1], np.sqrt(coeff_var[1][1])))

sizearr = np.arange(min(sizes), max(sizes), 0.001)
plt.errorbar(sizes, p_c, yerr=p_c_err, fmt='kx')
plt.plot(sizearr, p_c_func(sizearr, coeff[0], coeff[1]), 'r-')
plt.xlim([0.9*min(sizes), 1.1*max(sizes)])
plt.xlabel('$N$')
plt.ylabel('$p_c$')
plt.savefig('./plots/compare.pdf')
plt.clf()
