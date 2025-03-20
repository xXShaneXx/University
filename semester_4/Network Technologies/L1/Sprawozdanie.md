# Sprawozdanie z testowania narzędzi sieciowych: Ping, Traceroute, WireShark

## Wstęp
Celem sprawozdania jest analiza działania narzędzi sieciowych `ping`, `traceroute` oraz `WireShark`. Testy obejmują badanie liczby węzłów na trasie do różnych serwerów, wpływ wielkości pakietu na czas propagacji, analizę fragmentacji pakietów oraz określenie maksymalnego niefragmentowanego pakietu.

## Opis programów

### 1. Ping
`ping` to narzędzie służące do testowania łączności sieciowej. Wysyła pakiety ICMP do wskazanego hosta i mierzy czas odpowiedzi.

#### Opcje używane w testach:
- `-c` – liczba wysyłanych pakietów,
- `-s` – rozmiar pakietu,
- `-i` – interwał czasu między pakietami,
- `-M` – kontrola fragmentacji pakietów,
- `-t` – czas życia (TTL) pakietów.

#### Przykładowe użycie:
```sh
ping -c 4 -s 64 -i 1 -t 128 www.example.com
ping -M do -s 64 -t 17 onet.pl
```

### 2. Traceroute
`traceroute` służy do śledzenia trasy pakietów między źródłem a docelowym serwerem. Pokazuje liczbę węzłów oraz czas ich przejścia.

#### Przykładowe użycie:
```sh
traceroute onet.pl
```

### 3. WireShark
`WireShark` to narzędzie do analizy ruchu sieciowego. Umożliwia przechwytywanie pakietów i analizę ich zawartości.

## Wyniki testów

| Strona | Liczba węzłów do | Liczba węzłów od | Czas propagacji |
|----------|-----------------|-----------------|-----------------|
| https://www.onet.pl/ | 17 | 11  | 97.753 ms|
| https://www.dw.com/ (Niemcy) | 18  | 16  | 88.778 ms |
| https://www.bbc.co.uk/ | 16  | 14 | 106.285 ms |
| https://www.cbc.ca/ | 18  | 16  | 109.912 ms |
| https://www.latercera.com/ (Chile) | 18  | 16  | 99.110 ms |
| https://www.chinadaily.com.cn/ | 17 | 15  | 89.204 ms|
| https://www.japantimes.co.jp/ | 15  | 13 | 88.124 ms |
| https://www.inquirer.net/ (Filipiny) | 13 | 11 | 97.692 ms |
| https://www.arabnews.com/ (Arabia Saudyjska) | 13  | 11 | 99.581 ms |

### Wpływ rozmiaru pakietu na czas propagacji i fragmentację

| Domena | Rozmiar pakietu | Czas (fragmentacja) | Czas (bez fragmentacji) |
|-----------------|-----------------|---------------------|-------------------------|
| onet.pl | 64 bajty | 90.064 ms | 90.682 ms |
| | 512 bajtów | 90.508 ms | 99.319 ms |
| | 1 472 bajty | 97.881 ms | 93.802 ms |
| bbc.co.uk | 64 bajty | 83.513 ms | 87.936 ms |
| | 512 bajtów | 94.836 ms | 94.551 ms |
| | 1 472 bajty | 99.990 ms | 104.266 ms |
| arabnews.com | 64 bajty | 95.032 ms | 88.203 ms |
| | 512 bajtów | 75.039 ms | 112.270 ms |
| | 1 472 bajty | 93.573 ms | 84.771 ms |

### Wpływ wielkości pakietu na liczbę skoków

| Domena          | Rozmiar pakietu | Skoki do/od (fragmentacja) | Skoki do/od (bez fragmentacji)|
|-----------------|-----------------|----------------------------|-------------------------------|
|  onet.pl        | 64 bajty        | 17/11                      | 17/11                         |
|                 | 512 bajtów      | 17/11                      | 17/11                         |
|                 | 1 472 bajty     | error                      | error                         |
| bbc.co.uk       | 64 bajty        | 16/14                      | 16/14                         |
|                 | 512 bajtów      | 16/14                      | 16/14                         |
|                 | 1 472 bajty     | 16/14                      | 16/14                         |
| japantimes.co.jp| 64 bajty        | 15/13                      | 15/13                         |
|                 | 512 bajtów      | 15/13                      | 15/13                         |
|                 | 1 452 bajty     | 15/13                      | 15/13                         |

### Najdłuższa trasa
Najdłuższa ścieżka miała 28 skoków:
```sh
ping -t 28 sydney.edu.au
```

## Wnioski
**Wpływ wielkości pakietu:**
   - wielkość pakietu ma nieznaczny wpływ na czas transmisji i zerowy na ilość węzłów.
   - Fragmentacja może prowadzić do większych opóźnień i zwiększonej podatności na utratę pakietów.

**Trasy do/z serwerów różnią się:**
   - Ilość skoków do i od danego hosta może się różnić.
   - Może to wynikać z asymetrycznych tras w Internecie.

**Średnica Internetu:**
   - W testach maksymalna liczba skoków wyniosła 28.
   - Oznacza to, że średnica Internetu może przekraczać 28 skoków w zależności od ścieżki.

**Wpływ lokalizacji:**
   - Serwery w odległych krajach mają  większą liczbę węzłów.
   - Czas propagacji zależy od odległości geograficznej między hostami.

**Sieci wirtualne:**
   - Trasy przez platformy chmurowe nie różnią się znacząco od tradycyjnych.
   - Liczba skoków może być mniejsza, jeśli dostawca ma zoptymalizowaną trasę.

**Przydatność narzędzi:**
   - `ping` jest użyteczny do podstawowej diagnostyki sieci.
   - `traceroute` pozwala na szczegółową analizę tras pakietów.
   - `WireShark` dostarcza najbardziej szczegółowe dane, ale wymaga bardziej zaawansowanej wiedzy do interpretacji.

## Podsumowanie
Narzędzia `ping`, `traceroute` i `WireShark` umożliwiają kompleksową analizę sieci. `Ping` pozwala na szybkie sprawdzenie dostępności hostów i czasu odpowiedzi. `Traceroute` umożliwia identyfikację ścieżki pakietów i liczby węzłów. `WireShark` zapewnia pełny wgląd w ruch sieciowy, co jest przydatne do zaawansowanej analizy. Wyniki pokazują, że wielkość pakietów ma wpływ na czas propagacji, a trasy do serwerów mogą się znacząco różnić w zależności od ich lokalizacji.

