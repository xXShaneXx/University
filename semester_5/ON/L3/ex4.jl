#Paweł Grzegory 282211

include("functions.jl")

f(x) = sin(x) - (0.5 * x)^2
df(x) = cos(x) - 0.5 * x

delta = 0.5e-5
epsilon = 0.5e-5
maxit = 100

println("Wyznaczanie pierwiastka równania sin(x) - (1/2 x)^2 = 0")

println("\n1. Metoda bisekcji z przedziałem [1.5, 2]:")
r1, v1, it1, err1 = functions.bisection_method(f, 1.5, 2.0, delta, epsilon)
println("r = $r1, v = $v1, it = $it1, err = $err1")

println("\n2. Metoda Newtona z x0 = 1.5:")
r2, v2, it2, err2 = functions.newton_method(f, df, 1.5, delta, epsilon, maxit)
println("r = $r2, v = $v2, it = $it2, err = $err2")

println("\n3. Metoda siecznych z x0 = 1, x1 = 2:")
r3, v3, it3, err3 = functions.secant_method(f, 1.0, 2.0, delta, epsilon, maxit)
println("r = $r3, v = $v3, it = $it3, err = $err3")
