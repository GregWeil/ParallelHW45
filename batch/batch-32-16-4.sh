#!/bin/sh
#SBATCH --job-name NODE32RANK512THREAD4
#
#SBATCH -D /gpfs/u/home/PCP5/PCP5crls/scratch/hw4/run/32-512-4/
#SBATCH --nodes 32
#SBATCH --time 1:00:00
#
#SBATCH --mail-type ALL
#SBATCH --mail-user carlsc2@rpi.edu

module load xl

echo 32

srun --ntasks 512  /gpfs/u/home/PCP5/PCP5crls/scratch/hw4/assignment4.xl 131072 4 512 0 ./1/
srun --ntasks 512  /gpfs/u/home/PCP5/PCP5crls/scratch/hw4/assignment4.xl 131072 4 4 0 ./4/
srun --ntasks 512  /gpfs/u/home/PCP5/PCP5crls/scratch/hw4/assignment4.xl 131072 4 16 0 ./16/
srun --ntasks 512  /gpfs/u/home/PCP5/PCP5crls/scratch/hw4/assignment4.xl 131072 4 32 0 ./32/
