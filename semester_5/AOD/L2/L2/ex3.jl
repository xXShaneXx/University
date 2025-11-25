# Paweł Grzegory 282211

using JuMP
using HiGHS

K = 4
c = [6000, 4000, 8000, 9000]      # koszt produkcji normalnej cj
a = [60, 65, 70, 60]              # maks produkcji ponadwymiarowej aj
o = [8000, 6000, 10000, 11000]    # koszt ponadwymiarowy oj
d = [130, 80, 125, 195]           # zapotrzebowanie dj
h = 1500                          # koszt magazynowania za jednostkę na okres
s0 = 15                           # początkowy zapas
max_normal = 100                  # maks produkcji normalnej w każdym okresie
max_storage = 70                  # maksymalna pojemność magazynu

model = Model(HiGHS.Optimizer)

# zmienne
@variable(model, 0 <= x[i = 1:K] <= max_normal)       # produkcja normalna
@variable(model, 0 <= y[i = 1:K] <= a[i])             # produkcja ponadwymiarowa (ma górne ograniczenia a_j)
@variable(model, 0 <= s[i = 1:K] <= max_storage)      # zapas na koniec okresu j

# funkcja celu: minimalizacja kosztów produkcji i magazynowania
@objective(model, Min, sum(c[j]*x[j] + o[j]*y[j] + h*s[j] for j=1:K))

# ograniczenia bilansu zapasu: s_{j-1} + x_j + y_j - d_j = s_j
for j in 1:K
    if j == 1
        @constraint(model, s0 + x[1] + y[1] - d[1] == s[1])
    else
        @constraint(model, s[j-1] + x[j] + y[j] - d[j] == s[j])
    end
end

# zmienne całkowite
for j in 1:K
     set_integer(x[j])
     set_integer(y[j])
     set_integer(s[j])
end

optimize!(model)

st = termination_status(model)
pr = primal_status(model)
println("Termination status: ", st)
println("Primal status: ", pr)

if st == MOI.OPTIMAL || st == MOI.LOCALLY_SOLVED
    println("\nKoszt minimalny (Z*) = ", objective_value(model))
    println("\nRozkład produkcji i zapasów:")
    for j in 1:K
        println("Okres $j: x = ", value(x[j]),
                "   y = ", value(y[j]),
                "   s_end = ", value(s[j]),
                "   (demand = ", d[j], ")")
    end
else
    println("Model nie znaleziono optymalnego rozwiązania.")
end
