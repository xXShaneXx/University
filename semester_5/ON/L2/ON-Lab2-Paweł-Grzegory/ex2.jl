#Paweł Grzegory 282211

using Plots

f(x) = exp(x) * log(1 + exp(-x))

# Zakres x
x = -100:0.1:100

y = f.(x)

plot(x, y,
    label="f(x) = exp(x) * log(1 + exp(-x))",
    xlabel="x",
    ylabel="f(x)",
    title="Wykres funkcji f(x)",
    legend=:topleft,
    grid=true
)

savefig("wykres_julia.png")
println("Wykres został zapisany do pliku wykre_julia.png")
