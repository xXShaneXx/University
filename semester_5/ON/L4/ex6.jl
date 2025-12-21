#Paweł Grzegory 282211

include("InterpolacjaNewtona.jl")

f_a(x) = abs(x)
f_b(x) = 1 / (1 + x^2)

interval_a = (-1.0, 1.0)
interval_b = (-5.0, 5.0)
degrees = [5, 10, 15]

println("Testowanie dla f(x) = |x| na przedziale [-1, 1]")
for n in degrees
    println("n = $n, węzły równoodległe")
    InterpolacjaNewtona.rysujNnfx(f_a, interval_a[1], interval_a[2], n, wezly=:rownoodlegle, filename="interpolacja_n_abs_$n")
    println("n = $n, węzły Czebyszewa")
    InterpolacjaNewtona.rysujNnfx(f_a, interval_a[1], interval_a[2], n, wezly=:czebyszew, filename="interpolacja_n_abs_$n")
end

println("\nTestowanie dla f(x) = 1/(1+x^2) na przedziale [-5, 5]")
for n in degrees
    println("n = $n, węzły równoodległe")
    InterpolacjaNewtona.rysujNnfx(f_b, interval_b[1], interval_b[2], n, wezly=:rownoodlegle, filename="interpolacja_1_(1+x^2)_$n")
    println("n = $n, węzły Czebyszewa")
    InterpolacjaNewtona.rysujNnfx(f_b, interval_b[1], interval_b[2], n, wezly=:czebyszew, filename="interpolacja_1_(1+x^2)_$n")
end
