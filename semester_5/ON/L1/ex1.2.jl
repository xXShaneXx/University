function find_eta(T::Type{<:AbstractFloat})
    eta = T(1.0)
    
    while (eta / T(2.0)) > T(0.0)
        eta = eta / T(2.0)
    end
    
    return eta
end

float_types = [Float16, Float32, Float64]

println("Wyznaczanie najmniejszej dodatniej liczby maszynowej (eta)")
println("-"^60)

for T in float_types
    println("Analiza dla typu: $T")
    
    # Wyznaczenie eta metodą iteracyjną
    iterative_eta = find_eta(T)
    println("  > Wartość wyznaczona iteracyjnie: $iterative_eta")
    
    # Pobranie wartości eta za pomocą funkcji wbudowanej
    library_eta = nextfloat(T(0.0))
    println("  > Wartość z funkcji nextfloat($(T)(0.0)): $library_eta")
    
    # Porównanie wyników
    if iterative_eta == library_eta
        println("  Wyniki są identyczne.")
    else
        println("  Wyniki są różne.")
    end
    println("-"^60)
end