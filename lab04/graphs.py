import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("results.csv")

plt.figure()

plt.plot(df["Size_KB"], df["Sequential"], label="Sequential")
plt.plot(df["Size_KB"], df["Random"], label="Random")
plt.plot(df["Size_KB"], df["RandomIndex"], label="RandomIndex")

plt.xlabel("Размер массива (KB)")
plt.ylabel("Время (нс на итерацию)")
plt.xscale("log")

plt.legend()
plt.grid()

plt.savefig("graph.png")
plt.show()