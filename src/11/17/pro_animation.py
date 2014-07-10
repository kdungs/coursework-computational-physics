import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

fig = plt.figure()
fig.subplots_adjust(left=0, right=1, bottom=0, top=1)
ax = fig.add_subplot(111, aspect='equal', autoscale_on=False,
                     xlim=(0, 5), ylim=(0, 5))

particles, = ax.plot([], [], 'bo', ms=6)


def init():
    particles.set_data([], [])
    return particles


def animate(i):
    xs, ys = np.genfromtxt('./data/{:04d}.txt'.format(i), unpack=True)
    particles.set_data(xs, ys)
    return particles

ani = animation.FuncAnimation(
    fig,
    animate,
    frames=1800,
    interval=30,
    init_func=init
)

plt.show()
