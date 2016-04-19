#!/bin/sh
#SBATCH --job-name NODE128RANK8192THREAD0
#
#SBATCH -D /gpfs/u/home/PCP5/PCP5crls/scratch/hw4/run/128-8192-0/
#SBATCH --nodes 128
#SBATCH --time 1:00:00
#
#SBATCH --mail-type ALL
#SBATCH --mail-user carlsc2@rpi.edu

module load xl

echo 128

srun --ntasks 8192 --overcommit /gpfs/u/home/PCP5/PCP5crls/scratch/hw4/assignment4.xl 131072 0 8192 0 ./1/
srun --ntasks 8192 --overcommit /gpfs/u/home/PCP5/PCP5crls/scratch/hw4/assignment4.xl 131072 0 4 0 ./4/
srun --ntasks 8192 --overcommit /gpfs/u/home/PCP5/PCP5crls/scratch/hw4/assignment4.xl 131072 0 16 0 ./16/
srun --ntasks 8192 --overcommit /gpfs/u/home/PCP5/PCP5crls/scratch/hw4/assignment4.xl 131072 0 32 0 ./32/
