#Paweł Grzegory 282211

using Plots

include("InterpolacjaNewtona.jl")

println("Testowanie funkcji rysujNnfx...")

# Przykład (a)
f_a(x) = exp(x)
a_a = 0.0
b_a = 1.0
ns_a = [5, 10, 15]

println("\n--- Przykład (a): f(x) = e^x na [0, 1] ---")
for n in ns_a
    println("Generowanie wykresu dla n = $n...")
    InterpolacjaNewtona.rysujNnfx(f_a, a_a, b_a, n, wezly=:rownoodlegle, filename="interpolacja_n_exp_$n")
    println("Wykres dla f(x)=e^x, n=$n został zapisany.")
end

# Przykład (b)
f_b(x) = x^2 * sin(x)
a_b = -1.0
b_b = 1.0
ns_b = [5, 10, 15]

println("\n--- Przykład (b): f(x) = x^2*sin(x) na [-1, 1] ---")
for n in ns_b
    println("Generowanie wykresu dla n = $n...")
    InterpolacjaNewtona.rysujNnfx(f_b, a_b, b_b, n, wezly=:rownoodlegle, filename="interpolacja_n_sin_$n")
    println("Wykres dla f(x)=x^2*sin(x), n=$n został zapisany.")
end

