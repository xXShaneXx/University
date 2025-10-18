
# Arytmetyka Float64 w standardzie IEEE 754 używa 64 bitów:
# 1 bit znaku (S), 11 bitów wykładnika (E), 52 bity mantysy (M).
# Wartość liczby normalnej: (-1)^S * (1.M)_2 * 2^(E - 1023)

println("Analiza rozkładu liczb Float64 w standardzie IEEE 754")
println("-"^60)

# --- Przedział [1, 2] ---
println("Przedział [1, 2]:")

one_float = 1.0
two_float = 2.0
next_after_one = nextfloat(one_float)
delta_1_2 = next_after_one - one_float
k_max_1_2 = (two_float - one_float) / delta_1_2

println("Liczba 1.0: ", bitstring(one_float))
println("Następna po 1.0: ", bitstring(next_after_one))
println("Krok (delta) w [1, 2]: ", delta_1_2)
println("Teoretyczny krok 2.0^-52: ", 2.0^-52)
println("Liczba kroków w [1, 2]: ", k_max_1_2)
println("Teoretyczna liczba kroków 2^52: ", 2.0^52)

println("\nWniosek dla [1, 2]:")
println("W przedziale [1, 2] wykładnik jest stały (E=1023). Zmienia się tylko 52-bitowa mantysa.")
println("Powoduje to równomierne rozmieszczenie liczb co δ = 2.0^-52.")
println("Każdą liczbę x z tego przedziału można przedstawić jako x = 1.0 + k * δ, gdzie k = 0, 1, ..., 2^52.")
println("-"^60)


# --- Przedział [0.5, 1] ---
println("Przedział [0.5, 1]:")

half_float = 0.5
one_float = 1.0
next_after_half = nextfloat(half_float)
delta_05_1 = next_after_half - half_float
k_max_05_1 = (one_float - half_float) / delta_05_1

println("Liczba 0.5: ", bitstring(half_float))
println("Następna po 0.5: ", bitstring(next_after_half))
println("Krok (delta) w [0.5, 1]: ", delta_05_1)
println("Teoretyczny krok 2.0^-53: ", 2.0^-53)
println("Liczba kroków w [0.5, 1]: ", k_max_05_1)
println("Teoretyczna liczba kroków 2^52: ", 2.0^52)

println("\nWniosek dla [0.5, 1]:")
println("W przedziale [0.5, 1] wykładnik jest stały (E=1022). Krok jest o połowę mniejszy niż w [1, 2].")
println("Rozmieszczenie jest równomierne co δ = 2.0^-53.")
println("Każdą liczbę x z tego przedziału można przedstawić jako x = 0.5 + k * δ, gdzie k = 0, 1, ..., 2^52.")
println("-"^60)


# --- Przedział [2, 4] ---
println("Przedział [2, 4]:")

two_float = 2.0
four_float = 4.0
next_after_two = nextfloat(two_float)
delta_2_4 = next_after_two - two_float
k_max_2_4 = (four_float - two_float) / delta_2_4

println("Liczba 2.0: ", bitstring(two_float))
println("Następna po 2.0: ", bitstring(next_after_two))
println("Krok (delta) w [2, 4]: ", delta_2_4)
println("Teoretyczny krok 2.0^-51: ", 2.0^-51)
println("Liczba kroków w [2, 4]: ", k_max_2_4)
println("Teoretyczna liczba kroków 2^52: ", 2.0^52)

println("\nWniosek dla [2, 4]:")
println("W przedziale [2, 4] wykładnik jest stały (E=1024). Krok jest dwa razy większy niż w [1, 2].")
println("Rozmieszczenie jest równomierne co δ = 2.0^-51.")
println("Każdą liczbę x z tego przedziału można przedstawić jako x = 2.0 + k * δ, gdzie k = 0, 1, ..., 2^52.")
println("-"^60)

println("Podsumowanie:")
println("Gęstość liczb zmiennoprzecinkowych maleje wraz ze wzrostem ich wielkości.")
println("W każdym przedziale [2^n, 2^(n+1)] znajduje się tyle samo (2^52) liczb maszynowych,")
println("ale przedziały te mają różną długość, co powoduje zmianę kroku (delty).")
