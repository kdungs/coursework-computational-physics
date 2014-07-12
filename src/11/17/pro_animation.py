#! /usr/bin/env python3.3

import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import os
import sys

filelist = os.listdir('./data/')

fig = plt.figure(figsize=(12.8, 7.2))
fig.set_tight_layout(True)

ax_particle_box = plt.subplot2grid((4, 3), (0, 0), colspan=2, rowspan=4)
ax_particle_box.set_aspect('equal')
ax_particle_box.set_xlim(0, 5)
ax_particle_box.set_ylim(0, 5)

ax_E_pot = plt.subplot2grid((4, 3), (0, 2))
ax_E_pot.set_ylabel('$E_{\mathrm{pot}}$')

ax_E_kin = plt.subplot2grid((4, 3), (1, 2), sharex=ax_E_pot)
ax_E_kin.set_ylabel('$E_{\mathrm{kin}}$')
ax_E_kin.set_xticklabels('')

ax_T = plt.subplot2grid((4, 3), (2, 2), sharex=ax_E_pot)
ax_T.set_xticklabels('')

ax_corr = plt.subplot2grid((4, 3), (3, 2), sharex=ax_E_pot)
ax_corr.set_xticklabels('')

particles, = ax_particle_box.plot([], [], 'bo', ms=6)
E_kins, = ax_E_kin.plot([], [], 'r-')
E_pots, = ax_E_pot.plot([], [], 'r-')

E_kin, E_pot, size = np.genfromtxt('./energies.txt', unpack=True)
ax_E_pot.set_ylim(0, max(E_pot) * 1.1)


def init():
    particles.set_data([], [])
    E_kins.set_data([], [])
    E_pots.set_data([], [])
    return particles


def animate(i):
    first_frame = 1 if i < 100 else i - 100
    xs, ys = np.genfromtxt('./data/{:04d}.txt'.format(i), unpack=True)

    particles.set_data(xs, ys)
    ax_particle_box.set_xlim(0, size[i])
    ax_particle_box.set_ylim(0, size[i])

    E_kins.set_data(np.arange(first_frame, i), E_kin[first_frame:i])
    ax_E_kin.set_xlim(first_frame, i)
    ax_E_kin.set_ylim(0, max(E_kin[first_frame:first_frame+100]) * 1.1)

    E_pots.set_data(np.arange(first_frame, i), E_pot[first_frame:i])
    ax_E_pot.set_xlim(first_frame, i)
    ax_E_pot.set_ylim(min(E_pot[first_frame:first_frame+100]),
                      max(E_pot[first_frame:first_frame+100]) * 1.1)
    print '\rFrame {}'.format(i),
    sys.stdout.flush()
    return particles, E_kins, E_pots,

ani = animation.FuncAnimation(
    fig,
    animate,
    frames=len(filelist),
    interval=30,
    init_func=init
)

plt.show()
ani.save('animation.mp4', bitrate=1024, dpi=100)
