import csv
import os
import random
import subprocess
import time
from pathlib import Path

SIZES = [128, 256, 512, 1024]
THREADS = [1, 2, 4, 6, 8, 10, 12, 16, 24, 32]
REPETITIONS = 10
SEED = 42

OUTPUT = Path("results/raw_data.csv")

env = os.environ.copy()
env["OMP_DYNAMIC"] = "FALSE"
env["OMP_PROC_BIND"] = "FALSE"

configurations = []

for n in SIZES:
    for order in ["ijk", "ikj"]:
        configurations.append((f"seq-{order}", n, 1))

        for threads in THREADS:
            configurations.append((f"par-{order}", n, threads))

def execute(mode, n, threads):
    result = subprocess.run(
        ["./bin/matrix", mode, str(n), str(threads)],
        capture_output=True,
        text=True,
        check=True,
        env=env
    )

    values = result.stdout.strip().split(",")

    if len(values) != 5:
        raise RuntimeError(f"Saida invalida: {result.stdout}")

    return values

def format_time(seconds):
    seconds = int(seconds)
    hours, remainder = divmod(seconds, 3600)
    minutes, seconds = divmod(remainder, 60)
    return f"{hours:02d}:{minutes:02d}:{seconds:02d}"

OUTPUT.parent.mkdir(exist_ok=True)

if OUTPUT.exists():
    raise SystemExit(
        "raw_data.csv ja existe. Preserve ou renomeie o arquivo."
    )

print("Iniciando aquecimento...", flush=True)

for mode in ["seq-ijk", "seq-ikj", "par-ijk", "par-ikj"]:
    execute(mode, 256, 4 if mode.startswith("par") else 1)

print("Aquecimento concluido.", flush=True)

rng = random.Random(SEED)
total = len(configurations) * REPETITIONS
completed = 0
start = time.monotonic()

with OUTPUT.open("x", newline="") as file:
    writer = csv.writer(file)

    writer.writerow([
        "mode",
        "n",
        "threads",
        "time",
        "checksum",
        "repetition",
        "execution_order"
    ])

    for repetition in range(1, REPETITIONS + 1):
        batch = configurations.copy()
        rng.shuffle(batch)

        for mode, n, threads in batch:
            values = execute(mode, n, threads)

            completed += 1

            writer.writerow(
                values + [repetition, completed]
            )

            file.flush()

            elapsed = time.monotonic() - start
            remaining = elapsed / completed * (total - completed)
            progress = completed / total * 100

            print(
                f"[{completed}/{total}] "
                f"{progress:6.2f}% | "
                f"{mode:7s} | "
                f"N={n:4d} | "
                f"T={threads:2d} | "
                f"Tempo={float(values[3]):.6f}s | "
                f"Restante: {format_time(remaining)}",
                flush=True
            )

print("\nExperimentos concluidos!")
print(f"Total de medicoes: {completed}")
print(f"Duracao: {format_time(time.monotonic() - start)}")
print(f"Resultados: {OUTPUT}")
