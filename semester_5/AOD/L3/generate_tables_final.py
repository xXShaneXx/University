import pandas as pd
import os
import sys

INPUT_TIME = "results/times.csv"
INPUT_PATH = "results/paths.csv"
OUTPUT_TXT = "results/tabele_do_sprawozdania.txt"

def format_time_value(val):
    """Formatuje czas: zaokrągla floaty, zostawia stringi (np. TIMEOUT)."""
    try:
        f = float(val)
        return f"{f:.2f}"
    except ValueError:
        return str(val)

def generate_styled_tables():
    if not os.path.exists(INPUT_TIME):
        print("Brak pliku results/times.csv. Uruchom najpierw run_experiments.py")
        return

    # 1. Wczytanie i przygotowanie danych CZASOWYCH
    df_time = pd.read_csv(INPUT_TIME)
    
    
    largest_instances = df_time.groupby(['Family', 'Algorithm']).tail(1).copy()
    
    # Tworzenie kolumny łączonej "Rodzina - Algorytm"
    largest_instances['Rodzina - Algorytm'] = largest_instances['Family'] + " - " + largest_instances['Algorithm']
    
    # Wybieramy tylko interesujące kolumny
    table_time = largest_instances[['Rodzina - Algorytm', 'AvgTime_ms']].copy()
    
    # Formatowanie czasu
    table_time['AvgTime_ms'] = table_time['AvgTime_ms'].apply(format_time_value)
    table_time.rename(columns={'AvgTime_ms': 'Czas (ms)'}, inplace=True)


    # 2. Wczytanie i przygotowanie danych ŚCIEŻEK (P2P)
    table_p2p = pd.DataFrame()
    if os.path.exists(INPUT_PATH):
        df_path = pd.read_csv(INPUT_PATH)
        
        # Filtrujemy, żeby mieć tylko "największe" (teoretycznie plik paths.csv 
        # zawiera już tylko te największe, jeśli użyłeś mojego skryptu)
        
        df_path['Rodzina - Algorytm'] = df_path['Family'] + " - " + df_path['Algorithm']
        
        # Wybieramy kolumny
        # Jeśli Dial miał timeout, może go tu nie być, trzeba to obsłużyć ręcznie przy łączeniu,
        # ale w CSV zazwyczaj są tylko pomyślne wyniki.
        table_p2p = df_path[['Rodzina - Algorytm', 'Start', 'End', 'Distance']].copy()
        table_p2p.rename(columns={
            'Start': 'Start',
            'End': 'Koniec', 
            'Distance': 'Odległość'
        }, inplace=True)
    
    # --- WYPISYWANIE WYNIKÓW ---
    
    with open(OUTPUT_TXT, 'w') as f:
        # Helper do zapisu i druku
        def write_print(text):
            print(text)
            f.write(text + "\n")

        write_print("="*60)
        write_print("TABELA 1: Długości ścieżek (Poprawność)")
        write_print("Dla największej instancji w każdej rodzinie")
        write_print("="*60)
        
        if not table_p2p.empty:
            # Sortowanie, żeby rodziny były obok siebie
            table_p2p.sort_values(by='Rodzina - Algorytm', inplace=True)
            write_print(table_p2p.to_string(index=False))
            
            # Generowanie LaTeX dla chętnych
            write_print("\n--- Kod LaTeX (Tabela 1) ---")
            write_print(table_p2p.to_latex(index=False, longtable=False))
        else:
            write_print("Brak danych P2P.")

        write_print("\n\n" + "="*60)
        write_print("TABELA 2: Czas wykonania (Wydajność)")
        write_print("Dla największej instancji (dla 'Random' to 'dijkstra-random' itp.)")
        write_print("="*60)
        
        # Sortowanie
        table_time.sort_values(by='Rodzina - Algorytm', inplace=True)
        write_print(table_time.to_string(index=False))
        
        write_print("\n--- Kod LaTeX (Tabela 2) ---")
        write_print(table_time.to_latex(index=False, longtable=False))

    print(f"\nWyniki zapisano w pliku: {OUTPUT_TXT}")

if __name__ == "__main__":
    generate_styled_tables()