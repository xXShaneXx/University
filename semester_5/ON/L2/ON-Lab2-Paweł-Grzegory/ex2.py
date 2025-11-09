#Paweł Grzegory 282211

import numpy as np
import matplotlib.pyplot as plt

def f(x):
    return np.exp(x) * np.log(1 + np.exp(-x))

x = np.arange(-100, 100, 0.1)

y = f(x)

plt.figure(figsize=(10, 6))
plt.plot(x, y, label="f(x) = exp(x) * log(1 + exp(-x))")
plt.xlabel("x")
plt.ylabel("f(x)")
plt.title("Wykres funkcji f(x)")
plt.legend()
plt.grid(True)

plt.savefig("wykres_python.png")
print("Wykres został zapisany do pliku wykres_python.png")
