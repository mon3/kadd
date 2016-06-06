# -*- coding: utf-8 -*-
import numpy as np
import matplotlib.pyplot as plt





N = 1000
x = np.linspace(0, 2, N)
f_rozklad = np.ones(N ) * 0.5 # normalizacja do 1 po scalkowaniu
splot_1 = np.convolve(f_rozklad, f_rozklad, mode='same') #podaje się shape = 'same' czy coś takiego żeby nie zmieniał rozmiaru tablicy a przesamplował w już istniejącej
plt.plot(x, splot_1)
plt.show()



splot_2 = np.convolve(splot_1, f_rozklad, mode='same')
plt.plot(x, splot_2)
plt.show()