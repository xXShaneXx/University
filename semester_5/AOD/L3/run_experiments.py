import os
import subprocess
import random
import csv
import re
import glob

# --- KONFIGURACJA ---
BUILD_DIR = "./build"      # Folder z plikami wykonywalnymi
INPUTS_DIR = "./inputs"    # Folder z podkatalogami danych
RESULTS_DIR = "./results"
TIMEOUT_SEC = 120          
LIMIT_TESTS = 22           # Limit plików na rodzinę (np. 22 pierwsze)

ALGORITHMS = ["dijkstra", "radixheap", "dial"]

FAMILIES = [
    "Random4-n", "Random4-C",
    "Long-n", "Long-C",
    "Square-n", "Square-C",
    "USA-road-d"
]

def parse_graph_n(filepath):
    """Wyciąga liczbę wierzchołków n z nagłówka"""
    try:
        with open(filepath, 'r') as f:
            for line in f:
                if line.startswith('p sp'):
                    return int(line.split()[2])
    except:
        return 0
    return 0

def get_c_param(filename):
    """
    POPRAWIONE: Pobiera parametr C (ignoruje końcowy seed/index).
    Np. z Random4-C.1000.0.gr wyciągnie 1000.
    """
    nums = re.findall(r'\d+', filename)
    if not nums: return 0
    # Jeśli są >= 2 liczby, bierzemy przedostatnią (C), a nie ostatnią (seed)
    if len(nums) >= 2:
        return int(nums[-2])
    return int(nums[-1])

def write_ss_file(filename, sources):
    """Zapisuje listę źródeł do pliku .ss"""
    with open(filename, 'w') as f:
        f.write(f"p aux sp ss {len(sources)}\n")
        for s in sources:
            f.write(f"s {s}\n")

def generate_p2p_file(filename, n, count=4):
    max_node = max(1, n)
    pairs = [(1, max_node)]
    for _ in range(count):
        u = random.randint(1, max_node)
        v = random.randint(1, max_node)
        pairs.append((u, v))
        
    with open(filename, 'w') as f:
        f.write(f"p aux sp p2p {len(pairs)}\n")
        for u, v in pairs:
            f.write(f"q {u} {v}\n")

def parse_time_result(res_file):
    if not os.path.exists(res_file): return None
    with open(res_file, 'r') as f:
        for line in f:
            if line.startswith('t '):
                return float(line.split()[1])
    return None

def parse_dist_results(res_file):
    dists = []
    if os.path.exists(res_file):
        with open(res_file, 'r') as f:
            for line in f:
                if line.startswith('d '):
                    parts = line.split()
                    dists.append((parts[1], parts[2], parts[3]))
    return dists

def run():
    if not os.path.exists(RESULTS_DIR): os.makedirs(RESULTS_DIR)
    
    csv_time = open(f"{RESULTS_DIR}/times.csv", 'w', newline='')
    writer_time = csv.writer(csv_time)
    writer_time.writerow(["Family", "File", "Nodes", "Algorithm", "AvgTime_ms"])

    csv_p2p = open(f"{RESULTS_DIR}/paths.csv", 'w', newline='')
    writer_p2p = csv.writer(csv_p2p)
    writer_p2p.writerow(["Family", "File", "Algorithm", "Start", "End", "Distance"])

    print(f"Start testów (limit {LIMIT_TESTS} plików/rodzinę)...")

    for family in FAMILIES:
        family_path = os.path.join(INPUTS_DIR, family)
        if not os.path.exists(family_path):
            print(f"[SKIP] Brak: {family_path}")
            continue
            
        print(f"\n--- Rodzina: {family} ---")
        files = glob.glob(os.path.join(family_path, "*.gr"))
        
        # Sortowanie i przygotowanie metadanych plików
        files_data = []
        for f in files:
            n = parse_graph_n(f)
            c_param = get_c_param(os.path.basename(f))
            files_data.append({'path': f, 'name': os.path.basename(f), 'n': n, 'c': c_param})
            
        if family.endswith("-C"):
            files_data.sort(key=lambda x: x['c'])
        else:
            files_data.sort(key=lambda x: x['n'])

        if not files_data: continue

        # Ograniczenie liczby plików
        files_data = files_data[:LIMIT_TESTS]

        # 1. TESTY CZASU (Dwa warianty: Single i Random)
        for item in files_data:
            print(f"  File: {item['name']} (N={item['n']}, C={item['c']})")
            
            # Wariant A: Single Source (min index = 1)
            ss_single = "temp_single.ss"
            res_single = "temp_single.res"
            write_ss_file(ss_single, [1])

            # Wariant B: 5 Random Sources
            ss_random = "temp_random.ss"
            res_random = "temp_random.res"
            max_node = max(1, item['n'])
            rnd_sources = [random.randint(1, max_node) for _ in range(5)]
            write_ss_file(ss_random, rnd_sources)

            # Uruchamiamy algorytmy dla obu wariantów
            for base_algo in ALGORITHMS:
                # --- Uruchomienie SINGLE ---
                cmd_s = [
                    os.path.join(BUILD_DIR, base_algo),
                    "-d", item['path'], "-ss", ss_single, "-oss", res_single
                ]
                try:
                    subprocess.run(cmd_s, check=True, timeout=TIMEOUT_SEC, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
                    t = parse_time_result(res_single)
                    label = f"{base_algo}-single"
                    writer_time.writerow([family, item['name'], item['n'], label, t if t is not None else "ERR"])
                except:
                    writer_time.writerow([family, item['name'], item['n'], f"{base_algo}-single", "TIMEOUT/ERR"])

                # --- Uruchomienie RANDOM (5 avg) ---
                cmd_r = [
                    os.path.join(BUILD_DIR, base_algo),
                    "-d", item['path'], "-ss", ss_random, "-oss", res_random
                ]
                try:
                    subprocess.run(cmd_r, check=True, timeout=TIMEOUT_SEC, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
                    t = parse_time_result(res_random)
                    label = f"{base_algo}-random"
                    writer_time.writerow([family, item['name'], item['n'], label, t if t is not None else "ERR"])
                except:
                    writer_time.writerow([family, item['name'], item['n'], f"{base_algo}-random", "TIMEOUT/ERR"])

        # 2. TESTY P2P (tylko dla największego pliku)
        largest = files_data[-1]
        p2p_file = "temp.p2p"
        res_p2p = "temp_p2p.res"
        generate_p2p_file(p2p_file, largest['n'])
        
        for algo in ALGORITHMS:
            # P2P jest niezależne od source mode, uruchamiamy "czysty" algorytm
            cmd = [
                os.path.join(BUILD_DIR, algo),
                "-d", largest['path'], "-p2p", p2p_file, "-op2p", res_p2p
            ]
            try:
                subprocess.run(cmd, check=True, timeout=TIMEOUT_SEC, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
                distances = parse_dist_results(res_p2p)
                for (u, v, dist) in distances:
                    writer_p2p.writerow([family, largest['name'], algo, u, v, dist])
            except:
                pass

    csv_time.close()
    csv_p2p.close()
    
    # Sprzątanie
    for f in ["temp_single.ss", "temp_single.res", "temp_random.ss", "temp_random.res", "temp.p2p", "temp_p2p.res"]:
        if os.path.exists(f): os.remove(f)
    print("\nGotowe. Uruchom generate_plots.py")

if __name__ == "__main__":
    run()