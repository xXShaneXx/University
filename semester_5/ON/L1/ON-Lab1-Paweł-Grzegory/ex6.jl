function f(x::Float64)
    return  sqrt(x^2 + Float64(1.0)) - Float64(1.0)
end

function g(x::Float64)
    return x^2 / (sqrt(x^2 + Float64(1.0)) + Float64(1.0))
end

for i in 1:180
    println("f($i) = ", f(Float64(8.0^-i)), ", g($i) = ", g(Float64(8.0^-i)))
    if f(Float64(8.0^-i)) == g(Float64(8.0^-i))
        println("f($i) == g($i)")
    else
        println("f($i) != g($i)")
    end
end