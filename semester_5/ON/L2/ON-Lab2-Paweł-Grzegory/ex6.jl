#Paweł Grzegory 282211

using Plots

function iterate_equation(c::Float64, x0::Float64, n::Int)
    results = Vector{Float64}(undef, n)
    x = x0
    for i in 1:n
        x = x^2 + c
        results[i] = x
    end
    return results
end

iterations = 40
n_range = 1:iterations

# 1. c = -2 i x0 = 1
x1 = iterate_equation(-2.0, 1.0, iterations)

# 2. c = -2 i x0 = 2
x2 = iterate_equation(-2.0, 2.0, iterations)

# 3. c = -2 i x0 = 1.99999999999999
x3 = iterate_equation(-2.0, 1.99999999999999, iterations)

# 4. c = -1 i x0 = 1
x4 = iterate_equation(-1.0, 1.0, iterations)

# 5. c = -1 i x0 = -1
x5 = iterate_equation(-1.0, -1.0, iterations)

# 6. c = -1 i x0 = 0.75
x6 = iterate_equation(-1.0, 0.75, iterations)

# 7. c = -1 i x0 = 0.25
x7 = iterate_equation(-1.0, 0.25, iterations)

plot(n_range, x1, c=:red, label="\$c = -2, x_0 = 1\$")
plot!(n_range, x2, c=:blue, label="\$c = -2, x_0 = 2\$")
plot!(n_range, x3, c=:green, label="\$c = -2, x_0 = 1.99999999999999\$")
plot!(n_range, x4, c=:yellow, label="\$c = -1, x_0 = 1\$")
plot!(n_range, x5, c=:magenta, label="\$c = -1, x_0 = -1\$")
plot!(n_range, x6, c=:cyan, label="\$c = -1, x_0 = 0.75\$")
plot!(n_range, x7, c=:black, label="\$c = -1, x_0 = 0.25\$")

plot!(legend=:outertopright,
    title="Graph of \$x_n\$, for \$n = 1,\\dots,40\$ \n \$x_{n+1} = x_n^2 + c\$")

savefig("ex6.png")