import os

from distutils.spawn import find_executable

from .mpirun import mpirun

def run_benchmark(executable, problem_size, debug, nproc_list, output_dir,
                  dry_run):
    exepath = find_executable(str(executable))
    if not exepath:
        raise IOError("Can't find executable: {}".format(executable))

    mpirun_executable = find_executable('mpirun')
    if not mpirun_executable:
        raise IOError("Can't find mpirun")

    print('mkdir -p {}'.format(output_dir))
    if not dry_run:
        output_dir.mkdir(exist_ok=True)

    exe_with_args = '{exepath} {problem_size} {debug_int}'.format(
        exepath = exepath,
        problem_size = problem_size,
        debug_int = 1 if debug else 0)

    for nproc in nproc_list:
        output_file = output_dir / 'times-{executable}-{nproc}.txt'.format(
            executable = executable.stem,
            nproc = nproc)
        mpirun(mpirun_executable, exe_with_args, nproc,
               output_file, dry_run)
