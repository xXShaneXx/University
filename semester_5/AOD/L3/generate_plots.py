import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import os
import re
import sys

# --- KONFIGURACJA ---
INPUT_CSV = "results/times.csv"
OUTPUT_DIR = "plots"

sns.set_theme(style="whitegrid")
plt.rcParams.update({'figure.figsize': (12, 7)}) 

def extract_parameter_c(filename):
    """
    POPRAWIONE: Wyciąga C z nazwy pliku, ignorując końcowy seed.
    """
    numbers = re.findall(r'\d+', filename)
    if not numbers: return 0
    if len(numbers) >= 2:
        return int(numbers[-2]) # Bierzemy przedostatnią liczbę (Parametr C)
    return int(numbers[-1])

def generate_plots():
    if not os.path.exists(INPUT_CSV):
        print(f"Brak pliku {INPUT_CSV}")
        sys.exit(1)
    if not os.path.exists(OUTPUT_DIR):
        os.makedirs(OUTPUT_DIR)

    try:
        df = pd.read_csv(INPUT_CSV)
    except Exception as e:
        print(f"Błąd odczytu CSV: {e}")
        sys.exit(1)

    # Czyszczenie danych
    df = df[pd.to_numeric(df['AvgTime_ms'], errors='coerce').notnull()]
    df['AvgTime_ms'] = df['AvgTime_ms'].astype(float)
    
    families = df['Family'].unique()
    print(f"Rodziny: {families}")

    for family in families:
        print(f"Rysowanie: {family}...")
        subset = df[df['Family'] == family].copy()

        # Ustalanie osi X
        if family.endswith("-C"):
            x_axis = "Parameter C"
            subset[x_axis] = subset['File'].apply(extract_parameter_c)
            subset = subset.sort_values(by=x_axis)
            xlabel = "Parametr C (koszt krawędzi)"
            log_scale_x = True 
        else:
            x_axis = "Nodes"
            subset = subset.sort_values(by=x_axis)
            xlabel = "Liczba wierzchołków (n)"
            log_scale_x = False

        plt.figure()
        
        plot = sns.lineplot(
            data=subset,
            x=x_axis,
            y="AvgTime_ms",
            hue="Algorithm",
            style="Algorithm",
            markers=True,
            dashes=False,
            markersize=7
        )

        plt.title(f"Czas działania: {family}")
        plt.xlabel(xlabel)
        plt.ylabel("Czas [ms]")
        
        if log_scale_x and subset[x_axis].min() > 0:
            plt.xscale('log') # Logarytmiczna oś X dla parametru C 
        

        plt.legend(title="Wariant")
        plt.grid(True, which="both", ls="--", linewidth=0.5)
        
        out_file = os.path.join(OUTPUT_DIR, f"plot_{family}.png")
        plt.savefig(out_file, dpi=300, bbox_inches='tight')
        plt.close()

    print(f"Wykresy zapisane w {OUTPUT_DIR}")

if __name__ == "__main__":
    generate_plots()