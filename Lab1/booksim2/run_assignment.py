import subprocess
import re
import matplotlib.pyplot as plt

direct_rates = [0.02, 0.05, 0.08, 0.10, 0.12, 0.15, 0.18, 0.20, 0.22, 0.25]

def run_sim(config_file, rates, extra_args=""):
    latencies = []
    for rate in rates:
        cmd = f"./src/booksim {config_file} injection_rate={rate} {extra_args}"
        res = subprocess.run(cmd, shell=True, capture_output=True, text=True)
        
        match = re.search(r"(?:Packet latency average|Overall average packet latency|latency average)\s*=\s*([\d\.]+)", res.stdout, re.IGNORECASE)
        if match:
            lat = float(match.group(1))
            latencies.append(lat)
        else:
            latencies.append(None)
    return latencies

print("Generating plots for collected data...")

mesh_lat = [186.032, 260.121, 329.802, 357.8, 391.369, 417.895, 433.655, 440.753, 446.627, 459.975]
torus_lat = [182.307, 290.313, 347.691, 384.33, 407.709, 423.708, 444.586, 447.861, 453.687, 463.098]
fly_lat = [71.0164, 251.509, 348.582, 374.893, 408.152, 432.288, 448.569, 455.075, 464.62, 465.53]

# Plot Generation
plt.figure(figsize=(9, 6))

plt.plot(direct_rates, mesh_lat, marker='o', color='b', label='8x8 Mesh')
plt.plot(direct_rates, torus_lat, marker='s', color='r', label='8x8 Torus')
plt.plot(direct_rates, fly_lat, marker='d', color='m', label='Butterfly (k=4, n=3)')

plt.xlabel('Offered Load (Injection Rate)')
plt.ylabel('Average Packet Latency (cycles)')
plt.title('Topology Latency vs Offered Load Comparison')
plt.grid(True)
plt.legend()
plt.savefig('all_topologies_plot.png')
plt.close()

print("Plot updated and saved as all_topologies_plot.png")
