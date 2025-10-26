function findx(start::Float64, finish::Float64)
    x = start
    while x < finish
        if x * (1.0 / x) != 1.0
            return x
        end
        x = nextfloat(x)
    end

    return nothing
end

function print_result(prefix::String, result::Union{Float64, Nothing})
    if isnothing(result)
        println(prefix, " Nie znaleziono takiej liczby x.")
    else
        println(prefix, " x = ", result)
    end
end

println("Znajdowanie liczby x, dla której x * (1/x) != 1")

let result = findx(nextfloat(1.0), 2.0)
    print_result("[1,2]:", result)
    if !isnothing(result)
        println(" wartość x * (1/x) = ", result * (1.0 / result))
    end
end

let result = findx(nextfloat(0.0), 1.0)
    print_result("Najmniejszy x:", result)
    if !isnothing(result)
        println(" wartość x * (1/x) = ", result * (1.0 / result))
    end
end