# Paweł Grzegory 282211

using JuMP
using HiGHS
using LinearAlgebra # For dot product

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
# This is the dot product of the cost matrix and the variable matrix
@objective(model, Min, dot(C, x))

# Demand constraints: sum over columns for each row
@constraint(model, sum(x, dims=2) .== L_demand)

# Supply constraints: sum over rows for each column
@constraint(model, sum(x, dims=1)' .<= F_max)

# Solve the model
optimize!(model)

# Print the results
if termination_status(model) == MOI.OPTIMAL
    println("Optimal solution found.")
    println("Total cost: ", objective_value(model))
    println("Solution:")
    x_val = value.(x)
    for i in 1:num_L
        for j in 1:num_F
            if x_val[i, j] > 1e-6 # Use a tolerance for floating point comparison
                println("  Buy $(x_val[i, j]) for L$i from F$j")
            end
        end
    end
else
    println("No optimal solution found. Status: ", termination_status(model))
end