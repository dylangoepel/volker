import os
import matplotlib.pyplot as plt

def readDat(path):
    ret = {}
    counts = {}
    for line in open(path, "r").readlines():
        row = line.split("\t")
        if len(row) != 2: # skip invalid lines
            continue
        if row[0] not in ret:
            ret[row[0]] = int(row[1])
            counts[row[0]] = 1
        else:
            ret[row[0]] += int(row[1])
            counts[row[0]] += 1
    for k in ret:
        ret[k] = ret[k] / counts[k]
    return ret

def sections(benchmarkData):
    sections = {}
    for key in benchmarkData.keys():
        section = key.split("_")[0]
        if section not in sections:
            sections[section] = []
        sections[section].append(key)
    return sections

diOld = None
if os.path.isfile("dat/bm.old.dat"):
    diOld = readDat("dat/bm.old.dat")

di = readDat("dat/bm.dat")

figures = sections(di)
for sec in figures:
    plt.figure()
    for i, key in enumerate(figures[sec]):
        if diOld is not None and key in diOld:
            plt.bar(i, diOld[key], color="red", width=0.25)
            plt.bar(i + 0.25, di[key], color="green", width=0.25)
        else:
            plt.bar(i, di[key], color="green", width=0.5)
    plt.xticks(range(len(figures[sec])), figures[sec])
    for sp in plt.gca().spines.values():
        sp.set_visible(False)
    plt.gca().tick_params(length=0)
    plt.title("Benchmarks times in $\\mu s$")

plt.show()
