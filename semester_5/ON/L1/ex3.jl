function demonstrate_distribution(start_val::Float64, steps::Int)
    println("Analiza dla przedziału zaczynającego się od: ", start_val)
    println("-"^50)
    
    x = start_val
    # ULP (Unit in the Last Place) dla danej liczby, czyli krok
    delta = eps(x) 
    println("Krok (delta) = eps($start_val) = $delta")
    println("Oczekiwany format liczb: $start_val + k * $delta")
    println("-"^50)

    for k in 0:steps
        current_val = start_val + k * delta
        # Sprawdzenie, czy obliczona wartość zgadza się z k-tą następną liczbą float
        # nextfloat(x, k) zwraca k-tą reprezentowalną liczbę po x
        @assert current_val == nextfloat(start_val, k)

        println("k = $k:")
        println("  x = ", current_val)
        println("  bitstring: ", bitstring(current_val))
    end
    println("\n")
end

function run_analysis()
    # Przedział [1.0, 2.0]
    # Krok delta = eps(1.0) = 2^-52
    demonstrate_distribution(1.0, 4)

    # Przedział [0.5, 1.0]
    # Krok delta = eps(0.5) = 2^-53
    demonstrate_distribution(0.5, 4)

    # Przedział [2.0, 4.0]
    # Krok delta = eps(2.0) = 2^-51
    demonstrate_distribution(2.0, 4)
end

run_analysis()