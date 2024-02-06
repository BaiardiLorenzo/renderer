import pandas as pd
from matplotlib import pyplot as plt


results_folder = 'csv/result-'

plot_folder = 'plots/'

plot_results_times = 'results_times.png'
plot_results_speedup = 'results_speedup.png'

test = "TEST"
t_seq = "T_SEQ"
t_par = "T_PAR"
speedup = "SPEEDUP"


def plot_csv(data, folder):
    # Read file CSV
    data = pd.read_csv(data, sep=';')
    # Plot Tempi
    plt.figure()
    plt.plot(data[test], data[t_seq], marker='o', label='Sequential')
    for i in range(2, len(data.columns) - 2, 2):
        plt.plot(data[test], data[t_par + str(i)], marker='o', label=f'{i} Threads')
    plt.xlabel('Planes')
    plt.ylabel('Time (seconds)')
    plt.title('Execution Times')
    plt.legend()
    plt.grid()
    plt.tight_layout()
    plt.savefig(folder + plot_results_times)

    # Plot Speedup
    plt.figure()
    plt.plot(data[test], data[t_seq]/data[t_seq], marker='o', label=f'Sequential')
    for i in range(2, len(data.columns) - 2, 2):
        plt.plot(data[test], data[speedup + str(i)], marker='o', label=f'{i} Threads')
    plt.xlabel('Planes')
    plt.ylabel('Speedup')
    plt.title('Speedups')
    plt.legend()
    plt.grid()
    plt.tight_layout()
    plt.savefig(folder + plot_results_speedup)
    # plt.show()


def main():
    img_sizes = [256, 512, 1024]
    for size in img_sizes:
        data = results_folder + str(size) + '-' + str(size) + '.csv'
        plot_csv(data, plot_folder + str(size) + "/")


if __name__ == '__main__':
    main()
