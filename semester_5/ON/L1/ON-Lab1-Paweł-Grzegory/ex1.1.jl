using Printf

function find_macheps(T::Type{<:AbstractFloat})
    macheps = T(1.0)
    # standart IEEE 754 x = +-(1.fraction) * 2^exponent 
    while T(1.0) + macheps / T(2.0) > T(1.0)
        macheps /= T(2.0)
    end
    return macheps
end

function main()
    float_types = [Float16, Float32, Float64]

    println("Porównanie iteracyjnie obliczonego epsilonu maszynowego z funkcją eps():")
    println("-"^60)
    println("Typ danych          Obliczony Epsilon      eps() z Julii          Różnica")
    println("-"^60)

    for T in float_types
        calculated_eps = find_macheps(T)
        julia_eps = eps(T) #eps gives the distance between 1.0 and the next larger representable floating-point value:
        difference = calculated_eps - julia_eps

        # Formatowanie wyjścia w celu wyrównania kolumn 
        println(@sprintf("%-18s  %22.15e  %22.15e  %22.15e", string(T), calculated_eps, julia_eps, difference))
    end
    println("-"^60)
end

main()