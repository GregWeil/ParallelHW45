# Generate sbatch files for BG/Q

nodes = [32, 64, 128]
configs = [(64, 0), (16, 4), (4, 16), (1, 64)]

template = """#!/bin/sh
#SBATCH --job-name NODE{0!s}RANK{1!s}THREAD{2!s}
#
#SBATCH -D /gpfs/u/home/PCP5/PCP5crls/scratch/hw4/run/{0!s}-{1!s}-{2!s}/
#SBATCH --nodes {0!s}
#SBATCH --time 1:00:00
#
#SBATCH --mail-type ALL
#SBATCH --mail-user carlsc2@rpi.edu

module load xl

echo {0!s}

srun --ntasks {1!s} ../assignment4.xl 131072 {2!s} 1 0 ./1/
srun --ntasks {1!s} ../assignment4.xl 131072 {2!s} 4 0 ./4/
srun --ntasks {1!s} ../assignment4.xl 131072 {2!s} 16 0 ./16/
srun --ntasks {1!s} ../assignment4.xl 131072 {2!s} 32 0 ./32/
"""

for node in nodes:
	for config in configs:
		file = open('batch/batch-'+str(node)+'-'+str(config[0])+'-'+str(config[1])+'.sh', 'w', newline='')
		file.write(template.format(node, config[0], config[1]))
		file.close()
