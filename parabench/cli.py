import argparse
from pathlib import Path

from .bench import run_benchmark
from .reader import read_files

def bench_func(args):
    nproc_list = [int(nproc) for nproc in args.nproc_list.split(',')]
    run_benchmark(args.executable, args.problem_size, args.debug, 
                  nproc_list, args.output_directory, args.dry_run)

def plot_func(args):
    dataframes = read_files(args.csv_files)
    from .plot import plot_benchmark
    plot_benchmark(dataframes, args.output_filename)

def parse_command_line():
    parser = argparse.ArgumentParser()
    subparsers = parser.add_subparsers(dest='cmd', help='subcommands')
    subparsers.required = True

    # Bench
    bench = subparsers.add_parser('bench', help='Run benchmark')
    bench.add_argument('executable', type=Path,
                       help='Executable to run benchmark on')
    bench.add_argument('-g', '--debug', action='store_true',
                     help='Run executable in debug mode (performance penality)')
    bench.add_argument('-s', '--problem-size', type=int, default=10000,
                     help='Problem size')
    bench.add_argument('-n', '--nproc-list', default='1,2,4,8',
                     help='Comma separated list of number of processus')
    bench.add_argument('-j', '--job-scheduled', action='store_true',
                     help='Job has been scheduled, and can be run.')
    bench.add_argument('-e', '--conda-environment', default='',
                     help='Conda enviroment into which run benchmark.')
    bench.add_argument('-o', '--output-directory', default='.', type=Path,
                     help='Directory to store stdout.')
    bench.add_argument('-d', '--dry-run', action='store_true',
                       help='Print what would have been done')
    bench.set_defaults(func=bench_func)

    # Plot
    plot = subparsers.add_parser('plot', help='Plot benchmark')
    plot.add_argument('-o', '--output-filename', default='times.png',
                     help='Image file name.')
    plot.add_argument('csv_files', type=Path, nargs='+', 
                      help='CSV files containing time measures')
    plot.set_defaults(func=plot_func)

    args = parser.parse_args()

    return args
