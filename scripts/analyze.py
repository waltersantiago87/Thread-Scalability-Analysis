import csv
import statistics
from pathlib import Path
from collections import defaultdict

import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

ROOT = Path(__file__).resolve().parent.parent
RESULTS = ROOT / "results"
FIGURES = ROOT / "figures"

FIGURES.mkdir(exist_ok=True)

groups = defaultdict(list)

with open(RESULTS / "raw_data.csv", newline="") as f:
    for row in csv.DictReader(f):
        key = (
            row["mode"],
            int(row["n"]),
            int(row["threads"])
        )
        groups[key].append(float(row["time"]))

assert len(groups) == 88
assert sum(map(len, groups.values())) == 880
assert all(len(v) == 10 for v in groups.values())

summary = []

for (mode, n, threads), times in sorted(groups.items()):
    mean = statistics.mean(times)
    median = statistics.median(times)
    std = statistics.stdev(times)
    cv = 100 * std / mean

    order = mode.split("-")[1]
    baseline = statistics.median(
        groups[(f"seq-{order}", n, 1)]
    )

    speedup = baseline / median
    efficiency = 100 * speedup / threads

    summary.append({
        "mode": mode,
        "n": n,
        "threads": threads,
        "repetitions": len(times),
        "mean": mean,
        "median": median,
        "std": std,
        "cv_percent": cv,
        "speedup": speedup,
        "efficiency_percent": efficiency
    })

with open(RESULTS / "summary.csv", "w", newline="") as f:
    writer = csv.DictWriter(
        f, fieldnames=list(summary[0].keys())
    )
    writer.writeheader()
    writer.writerows(summary)

for order in ["ijk", "ikj"]:
    fig, ax = plt.subplots(figsize=(9, 5))

    for n in [128, 256, 512, 1024]:
        rows = sorted(
            [
                r for r in summary
                if r["mode"] == f"par-{order}"
                and r["n"] == n
            ],
            key=lambda r: r["threads"]
        )

        ax.plot(
            [r["threads"] for r in rows],
            [r["speedup"] for r in rows],
            marker="o",
            label=f"N={n}"
        )

    ax.axhline(
        1,
        linestyle=":",
        label="Referencia sequencial"
    )

    ax.set_xlabel("Threads solicitadas")
    ax.set_ylabel("Speedup")
    ax.set_xticks([1, 2, 4, 6, 8, 10, 12, 16, 24, 32])
    ax.grid(alpha=0.3)
    ax.legend()
    fig.tight_layout()

    fig.savefig(FIGURES / f"speedup_{order}.pdf")
    plt.close(fig)

print("Medicoes processadas: 880")
print("Configuracoes analisadas: 88")
print("Arquivo summary.csv gerado.")
print("Graficos IJK e IKJ gerados.")

print("\nMelhores configuracoes paralelas:")

for n in [128, 256, 512, 1024]:
    for order in ["ijk", "ikj"]:
        best = min(
            [
                r for r in summary
                if r["mode"] == f"par-{order}"
                and r["n"] == n
            ],
            key=lambda r: r["median"]
        )

        print(
            f"N={n:4} | {order.upper()} | "
            f"Threads={best['threads']:2} | "
            f"Mediana={best['median']:.9f}s | "
            f"Speedup={best['speedup']:.2f}x"
        )
