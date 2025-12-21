# Paweł Grzegory 282211

using JuMP
using HiGHS

# sizes of the matrix
m, n = 5, 5

# camera view
k = 2

# position of contanainers
containers = [(2, 3), (4, 1), (5, 5)]

model = Model(HiGHS.Optimizer)

@variable(model, x[1:m, 1:n], Bin)

@objective(model, Min, sum(x))

# + area constraint
@constraints(model, begin
    # each container must be fully covered
    [i = 1:length(containers)],
        sum(x[r, containers[i][2]] for r in max(1, containers[i][1]-k):min(m, containers[i][1]+k)) +
        sum(x[containers[i][1], c] for c in max(1, containers[i][2]-k):min(n, containers[i][2]+k)) >= 1
end)

# alternative constraint(squared area)
# @constraints(model, begin
#     # each container must be fully covered
#     [i=1:length(containers)], 
#         sum(x[r, c] for r in max(containers[i][1] - k, 1):min(m, containers[i][1] + k),
#                         c in max(containers[i][2] - k, 1):min(n, containers[i][2] + k)) >= 1
# end)

@constraints(model, begin
# prevent placing camera directly on container
    [i = 1:length(containers)],
    x[containers[i][1], containers[i][2]] == 0
end)

optimize!(model)

st = termination_status(model)
pr = primal_status(model)
println("Termination status: ", st)
println("Primal status: ", pr)

if st == MOI.OPTIMAL || st == MOI.LOCALLY_SOLVED
    println("\nMinimal number of cameras (Z*) = ", objective_value(model))
    println("\nCamera placement:")
    for r in 1:m
        for c in 1:n
            if value(x[r, c]) > 0.5
                println("  Place camera at row $r, column $c")
            end
        end
    end
else
    println("Model did not find an optimal solution.")
end