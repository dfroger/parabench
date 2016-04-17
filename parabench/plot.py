import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import pandas as pd

def plot_benchmark(dataframes, filename):
    figure, axes = plt.subplots(len(dataframes), 1, sharex='col')
    axes = axes.flatten()

    for iaxe,df in enumerate(dataframes):
        size = len(df.index)
        df.plot.barh(stacked=True, ax=axes[iaxe],
                     title='{} process'.format(size),
                     legend=True if iaxe==len(dataframes)-1 else False)
        plt.sca(axes[iaxe])
        plt.ylabel('Process rank')

    plt.xlabel('Wall-clock time')

    plt.savefig(filename)
    print(filename)
