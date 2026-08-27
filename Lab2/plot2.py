import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("results.csv")

df = df.sort_values("threads")

# Baseline: 1-thread execution
t1 = df[df["threads"] == 1]["time"].iloc[0]

# Speedup
df["speedup"] = t1 / df["time"]

# Efficiency
df["efficiency"] = df["speedup"] / df["threads"]


# ==========================================
# Part 2(a): Threads vs Parallel Execution Time
# ==========================================

plt.figure(figsize=(8, 6))

plt.plot(
    df["threads"],
    df["time"],
    marker="o"
)

plt.xlabel("Number of Threads")
plt.ylabel("Execution Time (seconds)")
plt.title("Part 2: Threads vs Parallel Execution Time")

plt.xticks(df["threads"])
plt.grid(True)

plt.tight_layout()
plt.savefig("part2_threads_vs_time.png", dpi=300)
plt.show()


# ==========================================
# Part 2(b): Threads vs Speedup
# ==========================================

plt.figure(figsize=(8, 6))

plt.plot(
    df["threads"],
    df["speedup"],
    marker="o",
    label="Measured Speedup"
)

# Ideal speedup
plt.plot(
    df["threads"],
    df["threads"],
    linestyle="--",
    label="Ideal Speedup"
)

plt.xlabel("Number of Threads")
plt.ylabel("Speedup")
plt.title("Part 2: Threads vs Speedup")

plt.xticks(df["threads"])
plt.legend()
plt.grid(True)

plt.tight_layout()
plt.savefig("part2_threads_vs_speedup.png", dpi=300)
plt.show()


# ==========================================
# Part 2(c): Threads vs Efficiency
# ==========================================

plt.figure(figsize=(8, 6))

plt.plot(
    df["threads"],
    df["efficiency"] * 100,
    marker="o"
)

plt.axhline(
    100,
    linestyle="--",
    label="Ideal Efficiency"
)

plt.xlabel("Number of Threads")
plt.ylabel("Efficiency (%)")
plt.title("Part 2: Threads vs Efficiency")

plt.xticks(df["threads"])
plt.legend()
plt.grid(True)

plt.tight_layout()
plt.savefig("part2_threads_vs_efficiency.png", dpi=300)
plt.show()