function khanEps(T::Type{<:AbstractFloat})
    return T(3.0) * (T(4.0) / T(3.0) - T(1.0)) - T(1.0)
end

println("Wyznaczanie epsilonu maszynowego metodą Khana")
for T in [Float16, Float32, Float64]
    println("Typ: $T, Khan Epsilon: $(khanEps(T)), eps() z Julii: $(eps(T))")
end