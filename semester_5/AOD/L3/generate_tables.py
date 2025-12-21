import pandas as pd
import sys
import os

INPUT_TIME = "results/times.csv"
INPUT_PATH = "results/paths.csv"

def generate_tables():
    # 1. Wczytanie danych czasowych
    if not os.path.exists(INPUT_TIME):
        print(f"BŁĄD: Brak pliku {INPUT_TIME}. Uruchom najpierw run_experiments.py")
        return

    df = pd.read_csv(INPUT_TIME)
    
    # Konwersja czasu na liczby
    df['AvgTime_ms'] = pd.to_numeric(df['AvgTime_ms'], errors='coerce')

    # Rozdzielenie algorytmów na Single i Random
    # Zakładamy, że w kolumnie Algorithm mamy np. 'dijkstra-single' i 'dijkstra-random'
    
    # --- TABELA 1: SINGLE SOURCE ---
    print("\n" + "="*80)
    print("TABELA 1: Czas obliczeń - Źródło ustalone (Single Source) [ms]")
    print("="*80)
    
    df_single = df[df['Algorithm'].str.endswith('-single')].copy()
    # Usuwamy sufiks z nazwy, żeby nagłówki kolumn były ładne
    df_single['Algorithm'] = df_single['Algorithm'].str.replace('-single', '')
    

    pivot_single = df_single.pivot_table(
        index=['Family', 'File'], 
        columns='Algorithm', 
        values='AvgTime_ms'
    )
    
    # Wyświetlenie tabeli (możesz skopiować wynik do Excela/Sprawozdania)
    print(pivot_single.to_string())
    
    # Opcjonalnie zapis do CSV
    pivot_single.to_csv("results/table_single_source.csv")


    # --- TABELA 2: RANDOM SOURCES ---
    print("\n\n" + "="*80)
    print("TABELA 2: Czas obliczeń - Średnia z 5 losowych źródeł [ms]")
    print("="*80)
    
    df_random = df[df['Algorithm'].str.endswith('-random')].copy()
    df_random['Algorithm'] = df_random['Algorithm'].str.replace('-random', '')
    
    pivot_random = df_random.pivot_table(
        index=['Family', 'File'], 
        columns='Algorithm', 
        values='AvgTime_ms'
    )
    
    print(pivot_random.to_string())
    pivot_random.to_csv("results/table_random_sources.csv")


    # --- TABELA 3: DŁUGOŚCI ŚCIEŻEK (P2P) ---
    print("\n\n" + "="*80)
    print("TABELA 3: Poprawność - Długości ścieżek (P2P) dla największych instancji")
    print("="*80)

    if os.path.exists(INPUT_PATH):
        df_path = pd.read_csv(INPUT_PATH)
        
        df_path.sort_values(by=['Family', 'Start', 'End'], inplace=True)
        
        # Formatowanie do wyświetlenia
        pivot_path = df_path.pivot_table(
            index=['Family', 'File', 'Start', 'End'],
            columns='Algorithm',
            values='Distance',
            aggfunc='first' # Distance to string lub int
        )
        print(pivot_path.to_string())
        pivot_path.to_csv("results/table_p2p_distances.csv")
    else:
        print("Brak pliku paths.csv - pomijam tabelę P2P.")

    print("\n\nGenerowanie zakończone. Wyniki zapisano również w folderze results/ jako pliki .csv")

if __name__ == "__main__":
    generate_tables()