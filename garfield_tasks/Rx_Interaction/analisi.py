import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

def gauss(x, A, mu, sigma):
    return A * np.exp(-(x - mu)**2 / (2 * sigma**2))

Energie = [1000, 2000, 4000, 5000, 6000, 8000]
ampiezze = [13446.1, 9403.35, 2469.49, 1270.97, 747.069, 277.974]
centroidi = [36.534, 73.2584, 147.286, 184.338, 221.264, 295.3]
deviazioni = [2.75013, 3.88668, 5.53875, 6.30561, 6.79664, 7.89123]

x_plot = np.linspace(0, 350, 1000)

plt.figure(figsize=(8, 6))

for egamma, A, mu, sigma in zip(Energie, ampiezze, centroidi, deviazioni):
    y = gauss(x_plot, A, mu, sigma)
    plt.plot(x_plot, y, label=f'{egamma} eV')
    plt.axvline(mu, 0, A, color='k', linestyle='--', alpha=0.5)
plt.title('Photopeak Gaussian Fits (Number of Electrons)')
plt.xlabel('Number of electrons')
plt.ylabel('Counts')
plt.xticks(centroidi)
plt.xlim(0, 350)
plt.legend()
plt.grid()
plt.savefig('hElectronsEnergy_python.png', dpi=300)