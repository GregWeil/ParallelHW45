#!/bin/sh
#SBATCH --job-name NODE32RANK128THREAD16
#
#SBATCH -D /gpfs/u/home/PCP5/PCP5crls/scratch/hw4/run/32-128-16/
#SBATCH --nodes 32
#SBATCH --time 1:00:00
#
#SBATCH --mail-type ALL
#SBATCH --mail-user carlsc2@rpi.edu

module load xl

echo 32

srun --ntasks 128  /gpfs/u/home/PCP5/PCP5crls/scratch/hw4/assignment4.xl 131072 16 128 0 ./1/
srun --ntasks 128  /gpfs/u/home/PCP5/PCP5crls/scratch/hw4/assignment4.xl 131072 16 4 0 ./4/
srun --ntasks 128  /gpfs/u/home/PCP5/PCP5crls/scratch/hw4/assignment4.xl 131072 16 16 0 ./16/
srun --ntasks 128  /gpfs/u/home/PCP5/PCP5crls/scratch/hw4/assignment4.xl 131072 16 32 0 ./32/
