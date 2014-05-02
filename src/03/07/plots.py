import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from matplotlib import rc
rc('text', usetex=True)

x = np.genfromtxt("data/histogram.txt", unpack=True)

hist = plt.subplot(111)
plt.xlabel(r"$x$")
plt.ylabel(r"\# Entries")
plt.hist(x, 50)
plt.savefig("plots/hist.pdf")
plt.clf()

dist2d = np.genfromtxt("data/2d_dist.txt", unpack=True)

plt.scatter(dist2d[0], dist2d[1], s=1, marker=",")
plt.xlabel("$x$")
plt.ylabel("$y$")
plt.xlim(0, 1)
plt.ylim(0, 1)
plt.title("Linear Congruential")
plt.savefig("plots/dist2d.pdf")
plt.clf()

dist2d = np.genfromtxt("data/2d_dist_mt.txt", unpack=True)

plt.scatter(dist2d[0], dist2d[1], s=1, marker=",")
plt.xlabel("$x$")
plt.ylabel("$y$")
plt.xlim(0, 1)
plt.ylim(0, 1)
plt.title("Mersenne Twister")
plt.savefig("plots/dist2d_mt.pdf")
plt.clf()

dist2d = np.genfromtxt("data/2d_dist_rand.txt", unpack=True)

plt.scatter(dist2d[0], dist2d[1], s=1, marker=",")
plt.xlabel("$x$")
plt.ylabel("$y$")
plt.xlim(0, 1)
plt.ylim(0, 1)
plt.title(r"\texttt{rand()}")
plt.savefig("plots/dist2d_rand.pdf")
plt.clf()

dist3d = np.genfromtxt("data/3d_dist_mt.txt", unpack=True)

fig = plt.figure()
ax_mt = fig.add_subplot(111, projection="3d")

ax_mt.scatter(dist3d[0], dist3d[1], dist3d[2], s=1, marker=",")
ax_mt.azim = 17
ax_mt.elev = 21
ax_mt.dist = 8
ax_mt.set_xlim(0, 1)
ax_mt.set_ylim(0, 1)
ax_mt.set_zlim(0, 1)
ax_mt.set_xlabel(r"$x$")
ax_mt.set_ylabel(r"$y$")
ax_mt.set_zlabel(r"$z$")
plt.savefig("plots/dist3d_mt.pdf")

ax_rand = fig.add_subplot(111, projection="3d")
dist3d = np.genfromtxt("data/3d_dist_rand.txt", unpack=True)
ax_rand.scatter(dist3d[0], dist3d[1], dist3d[2], s=1, marker=",")
ax_rand.azim = 17
ax_rand.elev = 21
ax_rand.set_xlim(0, 1)
ax_rand.set_ylim(0, 1)
ax_rand.set_zlim(0, 1)
ax_rand.set_xlabel(r"$x$")
ax_rand.set_ylabel(r"$y$")
ax_rand.set_zlabel(r"$z$")
plt.savefig("plots/dist3d_rand.pdf")

ax_lin = fig.add_subplot(111, projection="3d")
dist3d = np.genfromtxt("data/3d_dist.txt", unpack=True)
ax_lin.scatter(dist3d[0], dist3d[1], dist3d[2], s=1, marker=",")
ax_lin.azim = 17
ax_lin.elev = 21
ax_lin.set_xlim(0, 1)
ax_lin.set_ylim(0, 1)
ax_lin.set_zlim(0, 1)
ax_lin.set_xlabel(r"$x$")
ax_lin.set_ylabel(r"$y$")
ax_lin.set_zlabel(r"$z$")
plt.xlabel(r"$x$")
plt.savefig("plots/dist3d.pdf")
