#!/bin/sh
#SBATCH --job-name NODE128RANK128THREAD64
#
#SBATCH -D /gpfs/u/home/PCP5/PCP5crls/scratch/hw4/run/128-128-64/
#SBATCH --nodes 128
#SBATCH --time 1:00:00
#
#SBATCH --mail-type ALL
#SBATCH --mail-user carlsc2@rpi.edu

module load xl

echo 128

srun --ntasks 128  /gpfs/u/home/PCP5/PCP5crls/scratch/hw4/assignment4.xl 131072 64 128 0 ./1/
srun --ntasks 128  /gpfs/u/home/PCP5/PCP5crls/scratch/hw4/assignment4.xl 131072 64 4 0 ./4/
srun --ntasks 128  /gpfs/u/home/PCP5/PCP5crls/scratch/hw4/assignment4.xl 131072 64 16 0 ./16/
srun --ntasks 128  /gpfs/u/home/PCP5/PCP5crls/scratch/hw4/assignment4.xl 131072 64 32 0 ./32/
