# Generate sbatch files for BG/Q

npar = [(32,"small"), (64,"small"), (128,"medium")]
configs = [(64, 0), (16, 4), (4, 16), (1, 64)]



template = """
srun -N{0!s} --partition {1!s} --time 60 --ntasks {2!s} {3!s} -o /gpfs/u/home/PCP5/PCP5crls/scratch/hw4/run/{0!s}-{2!s}-{4!s}/ /gpfs/u/scratch/PCP5/PCP5crls/hw4/assignment4.xl 131072 {4!s} {2!s} 0 ./1/ &
srun -N{0!s} --partition {1!s} --time 60 --ntasks {2!s} {3!s} -o /gpfs/u/home/PCP5/PCP5crls/scratch/hw4/run/{0!s}-{2!s}-{4!s}/ /gpfs/u/scratch/PCP5/PCP5crls/hw4/assignment4.xl 131072 {4!s} 4 0 ./4/ &
srun -N{0!s} --partition {1!s} --time 60 --ntasks {2!s} {3!s} -o /gpfs/u/home/PCP5/PCP5crls/scratch/hw4/run/{0!s}-{2!s}-{4!s}/ /gpfs/u/scratch/PCP5/PCP5crls/hw4/assignment4.xl 131072 {4!s} 16 0 ./16/ &
srun -N{0!s} --partition {1!s} --time 60 --ntasks {2!s} {3!s} -o /gpfs/u/home/PCP5/PCP5crls/scratch/hw4/run/{0!s}-{2!s}-{4!s}/ /gpfs/u/scratch/PCP5/PCP5crls/hw4/assignment4.xl 131072 {4!s} 32 0 ./32/ &
"""




file = open('superbatch.sh', 'w', newline='')

for node,partition in npar:
    for config in configs:
        ayylmao = "--overcommit" if config[0] == 64 else ""
        file.write(template.format(node, partition, config[0]*node, ayylmao, config[1]))
file.close()
