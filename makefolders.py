# Generate sbatch files for BG/Q

import os

nodes = [32, 64, 128]
configs = [(64, 0), (16, 4), (4, 16), (1, 64)]

template = """
run/{0!s}-{1!s}-{2!s}/1/
run/{0!s}-{1!s}-{2!s}/4/
run/{0!s}-{1!s}-{2!s}/16/
run/{0!s}-{1!s}-{2!s}/32/
"""

for node in nodes:
    for config in configs:
        for s in template.splitlines():
            if s == "":
                continue
            directory = s.format(node, config[0]*node, config[1])
            if not os.path.exists(directory):
                os.makedirs(directory)
