function find_max(T::Type{<:AbstractFloat})
    max_power_of_2 = T(1.0)
    while !isinf(max_power_of_2 * T(2.0))
        max_power_of_2 *= T(2.0)
    end

    max_val = max_power_of_2
    step = max_power_of_2
    while step > T(0.0)
        step /= T(2.0)
        if !isinf(max_val + step)
            max_val += step
        end
    end
    return max_val
end

float_types = [Float16, Float32, Float64]

println("Wyznaczanie największej liczby maszynowej")

for T in float_types
    max_value = find_max(T)
    println("Typ: $T, Wyznaczona największa liczba: $max_value")
    println("Wartość z funkcji floatmax:          $(floatmax(T))")
    println("-"^60)
end

println("Najmniejsze liczby maszynowe dla poszczególnych typów danych:")

for T in float_types
    println("Typ: $T")
    println("Wartość z funkcji floatmin:          $(floatmin(T))")
    println("-"^60)
end