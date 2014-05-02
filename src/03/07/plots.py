import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

x = np.genfromtxt("data/histogram.txt", unpack=True)

hist = plt.subplot(111)
plt.hist(x, 50)
plt.savefig("plots/hist.pdf")
plt.clf()

dist2d = np.genfromtxt("data/2d_dist.txt", unpack=True)

plt.scatter(dist2d[0], dist2d[1], marker=",")
plt.savefig("plots/dist2d.pdf")
plt.clf()

dist2d = np.genfromtxt("data/2d_dist_mt.txt", unpack=True)

plt.scatter(dist2d[0], dist2d[1], marker=",")
plt.savefig("plots/dist2d_mt.pdf")
plt.clf()

dist2d = np.genfromtxt("data/2d_dist_rand.txt", unpack=True)

plt.scatter(dist2d[0], dist2d[1], marker=",")
plt.savefig("plots/dist2d_rand.pdf")
plt.clf()

dist3d = np.genfromtxt("data/3d_dist_mt.txt", unpack=True)

fig = plt.figure()
ax_mt = fig.add_subplot(111, projection="3d")

ax_mt.scatter(dist3d[0], dist3d[1], dist3d[2], marker=",")
ax_mt.azim = 17
ax_mt.elev = 21
plt.savefig("plots/dist3d_mt.pdf")

ax_rand = fig.add_subplot(111, projection="3d")
dist3d = np.genfromtxt("data/3d_dist_rand.txt", unpack=True)
ax_rand.scatter(dist3d[0], dist3d[1], dist3d[2], marker=",")
ax_rand.azim = 17
ax_rand.elev = 21
plt.savefig("plots/dist3d_rand.pdf")

ax_lin = fig.add_subplot(111, projection="3d")
dist3d = np.genfromtxt("data/3d_dist.txt", unpack=True)
ax_lin.scatter(dist3d[0], dist3d[1], dist3d[2], marker=",")
ax_lin.azim = 17
ax_lin.elev = 21
plt.savefig("plots/dist3d.pdf")
