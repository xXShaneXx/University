# Paweł Grzegory 282211

using JuMP
using HiGHS
using LinearAlgebra

model = Model(HiGHS.Optimizer)

num_products = 4
num_machines = 3

# availabity for machines 60h per week
machine_hours = [60, 60, 60]

# production times per hour/kg for products P1, P2, P3, P4 on machines M1, M2, M3
production_times = [
    5 10 6
    3 6 4
    4 5 3
    4 2 1
] / 60  # convert to hours

max_demand = [400, 100, 150, 500]

# cost for machine work per hour
machine_costs = [2, 2, 3]

# production cost for each kg of product
production_costs = [4, 1, 1, 1]

# profit for each kg of product
product_profits = [9, 7, 6, 5]

# Decision variables: time spent on each machine for each product
@variable(model, x[1:num_products, 1:num_machines] >= 0)

# Objective function: Maximize profit
@objective(model, Max, 
    sum(x[i, j] / production_times[i, j] * (product_profits[i] - production_costs[i]) 
        for i in 1:num_products, j in 1:num_machines) 
    - sum(machine_costs[j] * sum(x[i, j] for i in 1:num_products) for j in 1:num_machines)
)

# Machine time constraints
@constraint(model, sum(x, dims=1) .<= machine_hours)

# Demand constraints
@constraint(model, [i=1:num_products], sum(x[i, j] / production_times[i, j] for j in 1:num_machines) <= max_demand[i])

println(model)

optimize!(model)

if termination_status(model) == MOI.OPTIMAL
    println("Optimal solution found.")
    println("Total profit: ", objective_value(model))
    println("Production plan:")
    for i in 1:num_products
        for j in 1:num_machines
            if value(x[i, j]) > 0
                println("  Produce $(value(x[i, j])) hours of Product $i on Machine $j")
            end
        end
    end
else
    println("No optimal solution found. Status: ", termination_status(model))
end