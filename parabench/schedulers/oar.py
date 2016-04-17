mpirun_oar = '''
{mpirun} 
    -machinefile {machinefile}
    -mca plm_rsh_agent "oarsh"
    -n {nproc}
    -npernode 1
    {cmd}
'''

    parser.add_argument('-o', '--oar', action='store_true',
                        help='Submit run with OAR scheduler')

        full_cmd = mpirun_oar.format(mpirun = find_executable('mpirun'),
                                     machinefile = os.getenv('OAR_FILE_NODES'),
                                     nproc=nproc,
                                     cmd=cmd)

    #if args.oar and not args.scheduled and (args.writing_benchmark or args.reading_benchmark):
        #cmd = 'oarsub "python {} --scheduled"'.format(' '.join(sys.argv))
        #args = shlex.split(cmd)
        #subprocess.run(args)
