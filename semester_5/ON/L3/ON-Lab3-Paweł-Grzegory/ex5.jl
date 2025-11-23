# Paweł Grzegory 282211

include("functions.jl")

f(x) = 3x - exp(x)

println("Pierwszy pierwiastek w przedziale [0, 1]:")
r1, v1, it1, err1 = functions.bisection_method(f, 0.0, 1.0, 1e-4, 1e-4)
println("x = $r1, f(x) = $v1, iteracje = $it1, błąd = $err1")

println("Drugi pierwiastek w przedziale [1, 2]:")
r2, v2, it2, err2 = functions.bisection_method(f, 1.0, 2.0, 1e-4, 1e-4)
println("x = $r2, f(x) = $v2, iteracje = $it2, błąd = $err2")