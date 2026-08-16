import subprocess
import re
import os
import matplotlib.pyplot as plt

config_path = "src/examples/mesh88_lat"
injection_rates = [0.02, 0.05, 0.08, 0.10, 0.12, 0.15, 0.18, 0.20, 0.22, 0.25]

def collect_data(topology, routing="dim_order"):
    latencies = []
    print(f"\n--- Collecting 10 data points for {topology.upper()} ---")
    for rate in injection_rates:
        cmd = f"./src/booksim {config_path} topology={topology} k=8 n=2 routing_function={routing} injection_rate={rate}"
        res = subprocess.run(cmd, shell=True, capture_output=True, text=True)
        
        match = re.search(r"(?:Packet latency average|Overall average packet latency)\s*=\s*([\d\.]+)", res.stdout)
        if match:
            lat = float(match.group(1))
            latencies.append(lat)
            print(f"Rate: {rate:.2f} -> Latency: {lat} cycles")
        else:
            latencies.append(None)
            print(f"Rate: {rate:.2f} -> Saturated / Parse Failed")
    return latencies

# Collect Mesh and Torus data
mesh_lat = collect_data("mesh", "dim_order")
torus_lat = collect_data("torus", "dim_order")

# Plot 1: 8x8 Mesh
plt.figure(figsize=(7, 5))
plt.plot(injection_rates, mesh_lat, marker='o', color='b', label='8x8 Mesh')
plt.xlabel('Offered Load (Injection Rate)')
plt.ylabel('Average Packet Latency (cycles)')
plt.title('Latency vs Offered Load (8x8 Mesh)')
plt.grid(True)
plt.legend()
plt.savefig('mesh_latency_plot.png')
plt.close()

# Plot 2: 8x8 Torus
plt.figure(figsize=(7, 5))
plt.plot(injection_rates, torus_lat, marker='s', color='r', label='8x8 Torus')
plt.xlabel('Offered Load (Injection Rate)')
plt.ylabel('Average Packet Latency (cycles)')
plt.title('Latency vs Offered Load (8x8 Torus)')
plt.grid(True)
plt.legend()
plt.savefig('torus_latency_plot.png')
plt.close()

# Combined Comparison Plot
plt.figure(figsize=(7, 5))
plt.plot(injection_rates, mesh_lat, marker='o', color='b', label='8x8 Mesh')
plt.plot(injection_rates, torus_lat, marker='s', color='r', label='8x8 Torus')
plt.xlabel('Offered Load (Injection Rate)')
plt.ylabel('Average Packet Latency (cycles)')
plt.title('Comparison: Mesh vs Torus Latency')
plt.grid(True)
plt.legend()
plt.savefig('mesh_vs_torus_plot.png')
plt.close()

print("\nDone! Plots saved successfully.")