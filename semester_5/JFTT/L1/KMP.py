import sys

def compute_lps_array(pattern):
    """
    Oblicza tablicę LPS (Longest Proper Prefix which is also Suffix).
    LPS[i] przechowuje długość najdłuższego właściwego prefiksu wzorca pattern[0...i],
    który jest jednocześnie sufiksem tego podciągu.
    """
    m = len(pattern)
    lps = [0] * m
    length = 0  # Długość poprzedniego najdłuższego prefiksu-sufiksu
    i = 1

    while i < m:
        if pattern[i] == pattern[length]:
            length += 1
            lps[i] = length
            i += 1
        else:
            if length != 0:
                length = lps[length - 1]
            else:
                lps[i] = 0
                i += 1
    return lps

def kmp_search(text, pattern):
    """
    Wyszukuje wszystkie wystąpienia wzorca w tekście przy użyciu algorytmu KMP.
    """
    n = len(text)
    m = len(pattern)

    if m == 0:
        return
    
    lps = compute_lps_array(pattern)
    i = 0  # Indeks dla tekstu
    j = 0  # Indeks dla wzorca

    while i < n:
        if pattern[j] == text[i]:
            i += 1
            j += 1

        if j == m:
            print(f"Wzorzec znaleziony na pozycji {i - j}")
            j = lps[j - 1]
        elif i < n and pattern[j] != text[i]:
            if j != 0:
                j = lps[j - 1]
            else:
                i += 1

def main():
    if len(sys.argv) != 3:
        print("Wywołanie: python KMP.py <wzorzec> <nazwa pliku>")
        sys.exit(1)

    pattern = sys.argv[1]
    filename = sys.argv[2]

    try:
        with open(filename, 'r', encoding='utf-8') as f:
            text = f.read()
    except FileNotFoundError:
        print(f"Błąd: Plik '{filename}' nie został znaleziony.")
        sys.exit(1)
    except Exception as e:
        print(f"Wystąpił błąd podczas odczytu pliku: {e}")
        sys.exit(1)

    kmp_search(text, pattern)

if __name__ == "__main__":
    main()
