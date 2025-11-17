# Paweł Grzegory 282211

using JuMP
using HiGHS

model = Model(HiGHS.Optimizer)

# Data
# Costs Cij
C = [
    10 7 8
    10 11 14
    9 12 4
    11 13 9
]

# Supplier capacity
F_max = [275000, 550000, 660000]

# Demand
L_demand = [110000, 220000, 330000, 440000]

num_L = 4
num_F = 3

# Decision variables Xij >= 0
@variable(model, x[1:num_L, 1:num_F] >= 0)

# Objective function: Minimize sum(Cij * Xij)
@objective(model, Min, sum(C[i, j] * x[i, j] for i in 1:num_L, j in 1:num_F))

# Demand constraints: sum(Xij) for each L must equal its demand
@constraint(model, demand[i in 1:num_L], sum(x[i, j] for j in 1:num_F) == L_demand[i])

# Supply constraints: sum(Xij) for each F cannot exceed its capacity
@constraint(model, supply[j in 1:num_F], sum(x[i, j] for i in 1:num_L) <= F_max[j])

println("Model constructed successfully.")
println(model)

# Solve the model
optimize!(model)

# Print the results
if termination_status(model) == MOI.OPTIMAL
    println("Optimal solution found.")
    println("Total cost: ", objective_value(model))
    println("Solution:")
    for i in 1:num_L
        for j in 1:num_F
            if value(x[i, j]) > 0
                println("  Buy $(value(x[i, j])) for L$i from F$j")
            end
        end
    end
else
    println("No optimal solution found. Status: ", termination_status(model))
end