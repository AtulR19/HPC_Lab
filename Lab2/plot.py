import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# ============================================================
# Load data
# ============================================================

df = pd.read_csv("results.csv")

# Make sure data is sorted
df = df.sort_values(["points", "threads"])

# ============================================================
# 1. Number of points vs Estimated Pi
# ============================================================

plt.figure(figsize=(9, 6))

for threads in sorted(df["threads"].unique()):
    data = df[df["threads"] == threads]
    plt.plot(
        data["points"],
        data["pi"],
        marker="o",
        label=f"{threads} threads"
    )

plt.axhline(np.pi, linestyle="--", label="Actual Pi")

plt.xscale("log")
plt.xlabel("Number of points")
plt.ylabel("Estimated Pi")
plt.title("Number of Points vs Estimated Pi")
plt.legend()
plt.grid(True, which="both", alpha=0.3)
plt.tight_layout()
plt.savefig("points_vs_pi.png", dpi=300)
plt.show()


# ============================================================
# 2. Number of points vs Error
# ============================================================

plt.figure(figsize=(9, 6))

for threads in sorted(df["threads"].unique()):
    data = df[df["threads"] == threads]
    plt.plot(
        data["points"],
        data["error"],
        marker="o",
        label=f"{threads} threads"
    )

plt.xscale("log")
plt.yscale("log")

plt.xlabel("Number of points")
plt.ylabel("Absolute Error")
plt.title("Number of Points vs Error")
plt.legend()
plt.grid(True, which="both", alpha=0.3)
plt.tight_layout()
plt.savefig("points_vs_error.png", dpi=300)
plt.show()


# ============================================================
# 3. Number of threads vs Execution Time
# ============================================================

# Choose the largest number of points for the performance graph
max_points = df["points"].max()

performance = df[df["points"] == max_points].copy()
performance = performance.sort_values("threads")

plt.figure(figsize=(9, 6))

plt.plot(
    performance["threads"],
    performance["time"],
    marker="o"
)

plt.xlabel("Number of Threads")
plt.ylabel("Execution Time (seconds)")
plt.title(f"Number of Threads vs Execution Time ({max_points:,} points)")
plt.xticks(sorted(df["threads"].unique()))
plt.grid(True, alpha=0.3)
plt.tight_layout()
plt.savefig("threads_vs_time.png", dpi=300)
plt.show()


# ============================================================
# 4. Number of threads vs Speedup
# ============================================================

# Time with one thread is the baseline
t1 = performance[performance["threads"] == 1]["time"].iloc[0]

performance["speedup"] = t1 / performance["time"]

plt.figure(figsize=(9, 6))

plt.plot(
    performance["threads"],
    performance["speedup"],
    marker="o",
    label="Measured Speedup"
)

# Ideal speedup
threads = performance["threads"].values
plt.plot(
    threads,
    threads,
    linestyle="--",
    label="Ideal Speedup"
)

plt.xlabel("Number of Threads")
plt.ylabel("Speedup")
plt.title(f"Number of Threads vs Speedup ({max_points:,} points)")
plt.xticks(threads)
plt.legend()
plt.grid(True, alpha=0.3)
plt.tight_layout()
plt.savefig("threads_vs_speedup.png", dpi=300)
plt.show()


# ============================================================
# 5. Number of threads vs Efficiency
# ============================================================

performance["efficiency"] = (
    performance["speedup"] / performance["threads"]
)

plt.figure(figsize=(9, 6))

plt.plot(
    performance["threads"],
    performance["efficiency"] * 100,
    marker="o"
)

plt.axhline(
    100,
    linestyle="--",
    label="Ideal Efficiency"
)

plt.xlabel("Number of Threads")
plt.ylabel("Efficiency (%)")
plt.title(f"Number of Threads vs Efficiency ({max_points:,} points)")
plt.xticks(threads)
plt.legend()
plt.grid(True, alpha=0.3)
plt.tight_layout()
plt.savefig("threads_vs_efficiency.png", dpi=300)
plt.show()


# ============================================================
# Print performance table
# ============================================================

print("\nPerformance Results")
print("=" * 65)

print(
    performance[
        ["points", "threads", "time", "speedup", "efficiency"]
    ].to_string(index=False)
)

print("\nGraphs saved:")
print("  points_vs_pi.png")
print("  points_vs_error.png")
print("  threads_vs_time.png")
print("  threads_vs_speedup.png")
print("  threads_vs_efficiency.png")