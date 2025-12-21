# Paweł Grzegory 282211

include("functions.jl")

f1(x) = exp(1 - x) - 1
df1(x) = -exp(1 - x)

f2(x) = x * exp(-x)
df2(x) = exp(-x) * (1 - x)

delta = 1e-5
epsilon = 1e-5
maxit = 100

println("------- Function f1(x) = exp(1 - x) - 1 -------")

println("Bisection [0, 2]:")
r, v, it, err = functions.bisection_method(f1, 0.0, 2.0, delta, epsilon)
println("r = $r, v = $v, it = $it, err = $err")

println("Newton x0 = 0.5:")
r, v, it, err = functions.newton_method(f1, df1, 0.5, delta, epsilon, maxit)
println("r = $r, v = $v, it = $it, err = $err")

println("Secant x0 = 0, x1 = 2:")
r, v, it, err = functions.secant_method(f1, 0.0, 2.0, delta, epsilon, maxit)
println("r = $r, v = $v, it = $it, err = $err")

println("\n-------- Function f2(x) = x * exp(-x) --------")

println("Bisection [-1, 1]:")
r, v, it, err = functions.bisection_method(f2, -1.0, 1.0, delta, epsilon)
println("r = $r, v = $v, it = $it, err = $err")

println("Newton x0 = -1.0:")
r, v, it, err = functions.newton_method(f2, df2,-1.0, delta, epsilon, maxit)
println("r = $r, v = $v, it = $it, err = $err")

println("Secant x0 = -1.0, x1 = 1:")
r, v, it, err = functions.secant_method(f2, -1.0, 1.0, delta, epsilon, maxit)
println("r = $r, v = $v, it = $it, err = $err")

println("\n-------- Newton Additional Tests --------")

println("Newton x0 = 2 (in (1, ∞]):")
r, v, it, err = functions.newton_method(f1, df1, 2.0, delta, epsilon, maxit)
println("r = $r, v = $v, it = $it, err = $err")

println("Newton x0 = 2 (>1):")
r, v, it, err = functions.newton_method(f2, df2, 2.0, delta, epsilon, maxit)
println("r = $r, v = $v, it = $it, err = $err")

println("Newton x0 = 1:")
r, v, it, err = functions.newton_method(f2, df2, 1.0, delta, epsilon, maxit)
println("r = $r, v = $v, it = $it, err = $err")



