import os
import sys
import shlex
import subprocess

from .. import conda

SUBMIT_TEMPLATE = '''
oarsub 
    -l /nodes=4/cpu=2/core=1,walltime=00:03:00
    "{python_exe} {inline_script}"
'''

DRY_RUN_SUBMIT_TEMPLATE = '{python_exe} {inline_script}'

MPIRUN_TEMPLATE = '''
{mpirun_exe}
    -machinefile {machinefile}
    -mca plm_rsh_agent "oarsh"
    -n {nproc}
    -npernode 2
    {exe_with_args}
'''

def submit(inline_script, dry_run, preserve_conda_env=False):
    inline_script  = inline_script[:]
    if preserve_conda_env:
        env_paths, active_env_path = conda.get_conda_environments()
        inline_script.append('--conda-environment='+active_env_path.name)

    inline_script.append('--job-scheduled')

    if dry_run:
        inline_script.append('--dry-run')

    cmd = SUBMIT_TEMPLATE.format(
        python_exe = sys.executable,
        inline_script = ' '.join(inline_script))

    print(cmd)

    if dry_run:
        cmd = DRY_RUN_SUBMIT_TEMPLATE.format(
            python_exe = sys.executable,
            inline_script = ' '.join(inline_script))

    args = shlex.split(cmd)
    subprocess.run(args)

def mpirun(mpirun_exe, exe_with_args, nproc, dry_run):
    """Run cmd in parallel and write stdout to file"""

    cmd = MPIRUN_TEMPLATE.format(
        mpirun_exe = mpirun_exe,
        machinefile = os.getenv('OAR_FILE_NODES'),
        nproc = nproc,
        exe_with_args = exe_with_args)

    print(cmd)

    args = shlex.split(cmd)
    if not dry_run:
        result = subprocess.run(args)
