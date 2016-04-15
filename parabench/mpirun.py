import textwrap
import os
import subprocess
import shlex
from distutils.spawn import find_executable

def mpirun(mpirun_exe, exe_with_args, nproc, output_file, dry_run):
    """Run cmd in parallel and write stdout to file"""

    template = textwrap.dedent('''
    {mpirun_exe} 
        -n {nproc}
        {exe_with_args}
    ''').strip()

    cmd = template.format(
        mpirun_exe = mpirun_exe,
        nproc = nproc,
        exe_with_args = exe_with_args)

    print(cmd + ' > ' + str(output_file))

    args = shlex.split(cmd)
    if not dry_run:
        result = subprocess.run(args, stdout=subprocess.PIPE,
                                universal_newlines=True)
        with output_file.open('w') as f:
            f.write(result.stdout)
    print(output_file)
