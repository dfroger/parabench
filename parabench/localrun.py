import textwrap
import os
import subprocess
import shlex
from distutils.spawn import find_executable

MPIRUN_TEMPLATE = '''
{mpirun_exe} 
    -n {nproc}
    {exe_with_args}
'''

def mpirun(mpirun_exe, exe_with_args, nproc, dry_run):
    """Run cmd in parallel and write stdout to file"""

    cmd = MPIRUN_TEMPLATE.format(
        mpirun_exe = mpirun_exe,
        nproc = nproc,
        exe_with_args = exe_with_args)

    print(cmd)

    args = shlex.split(cmd)
    if not dry_run:
        result = subprocess.run(args)
