import sys

def compute_transition_function(pattern):
    """
    Oblicza funkcję przejścia dla automatu skończonego.
    """
    m = len(pattern)
    alphabet = set(pattern)
    delta = {}

    for q in range(m + 1):
        delta[q] = {}
        for a in alphabet:
            k = min(m, q + 1)
            while k > 0 and not (pattern[:q] + a).endswith(pattern[:k]):
                k -= 1
            delta[q][a] = k
    return delta, alphabet

def finite_automaton_matcher(text, pattern):
    """
    Wyszukuje wszystkie wystąpienia wzorca w tekście przy użyciu automatu skończonego.
    """
    m = len(pattern)
    if m == 0:
        return

    transition_function, alphabet = compute_transition_function(pattern)
    
    q = 0
    for i in range(len(text)):
        char = text[i]
        # Jeśli znak nie jest w alfabecie wzorca, stan wraca do 0
        q = transition_function.get(q, {}).get(char, 0)
        if q == m:
            print(f"Wzorzec znaleziony na pozycji {i - m + 1}")

def main():
    if len(sys.argv) != 3:
        print("Wywołanie: python FA.py <wzorzec> <nazwa pliku>")
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

    finite_automaton_matcher(text, pattern)

if __name__ == "__main__":
    main()