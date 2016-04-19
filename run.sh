srun -N1 -t 10 --ntasks 1 --overcommit -o results/run_1_1_.log /gpfs/u/scratch/PCP5/PCP5crls/hw4/assignment4.xl 16 0 1 0 /gpfs/u/scratch/PCP5/PCP5crls/hw4/results/


srun -N128 --partition medium -t 20 --ntasks 32 --overcommit -o results/run_1_1_.log /gpfs/u/scratch/PCP5/PCP5crls/hw4/assignment4.xl 16 0 1 0 /gpfs/u/scratch/PCP5/PCP5crls/hw4/results/

