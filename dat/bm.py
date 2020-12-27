import os
import matplotlib.pyplot as plt

di = {}

for line in open("dat/bm.dat", "r").readlines():
    row = line.split("\t")
    di[row[0]] = int(row[1])

plt.bar(range(len(di)), di.values(), align="center")
plt.xticks(range(len(di)), di.keys())
for sp in plt.gca().spines.values():
    sp.set_visible(False)
plt.gca().tick_params(length=0)
plt.title("Benchmarks times in $\mu s$")
plt.show()
