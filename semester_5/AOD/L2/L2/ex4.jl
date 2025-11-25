# Paweł Grzegory 282211

using JuMP
using HiGHS

println("=== Zadanie 4(a) ===")

# Data
N = 1:10
T_limit = 15

# edges given as (i, j, cij, tij)
edges = [
    (1, 2, 3, 4), (1, 3, 4, 9), (1, 4, 7, 10), (1, 5, 8, 12),
    (2, 3, 2, 3), (3, 4, 4, 6), (3, 5, 2, 2), (3, 10, 6, 11),
    (4, 5, 1, 1), (4, 7, 3, 5), (5, 6, 5, 6), (5, 7, 3, 3), (5, 10, 5, 8),
    (6, 1, 5, 8), (6, 7, 2, 2), (6, 10, 7, 11), (7, 3, 4, 6), (7, 8, 3, 5),
    (7, 9, 1, 1), (8, 9, 1, 2), (9, 10, 2, 2)
]

# Build arc set and parameter dictionaries
arcs = [(i,j) for (i,j,_,_) in edges]
c = Dict{Tuple{Int,Int},Float64}()
t = Dict{Tuple{Int,Int},Float64}()
for (i,j,ci,ti) in edges
    c[(i,j)] = float(ci)
    t[(i,j)] = float(ti)
end

start = 1
destination = 10

model = Model(HiGHS.Optimizer)
@variable(model, x[arcs] >= 0, Bin)

# Objective: minimize total cost of the path
@objective(model, Min, sum(c[(i,j)] * x[(i,j)] for (i,j) in arcs))

# Time constraint
@constraint(model, sum(t[(i,j)] * x[(i,j)] for (i,j) in arcs) <= T_limit)

# Flow conservation constraints
for n in N
    if n == start
        @constraint(model, sum(x[(n,j)] for j in N if (n,j) in arcs) - sum(x[(i,n)] for i in N if (i,n) in arcs) == 1)
    elseif n == destination
        @constraint(model, sum(x[(n,j)] for j in N if (n,j) in arcs) - sum(x[(i,n)] for i in N if (i,n) in arcs) == -1)
    else
        @constraint(model, sum(x[(n,j)] for j in N if (n,j) in arcs) - sum(x[(i,n)] for i in N if (i,n) in arcs) == 0)
    end
end

optimize!(model)
st = termination_status(model)
pr = primal_status(model)
println("Termination status: ", st)
println("Primal status: ", pr)

if st == MOI.OPTIMAL || st == MOI.LOCALLY_SOLVED
    println("\nOptimal cost (C*) = ", objective_value(model))
    println("\nOptimal path:")
    for (i,j) in arcs
        if value(x[(i,j)]) > 0.5
            println("  From node $i to node $j")
        end
    end
else
    println("Model did not find an optimal solution.")
end

println("\n=== Zadanie 4(b) ===")

# Own example
N2 = 1:10
T_limit2 = 5

edges2 = [
    (1,2,1,1), (1,6,2,1), (1,10,6,10), (2,10,4,5), (6,7,2,1), (7,10,2,1)
]

arcs2 = [(i,j) for (i,j,_,_) in edges2]
c2 = Dict{Tuple{Int,Int},Float64}()
t2 = Dict{Tuple{Int,Int},Float64}()
for (i,j,ci,ti) in edges2
    c2[(i,j)] = float(ci)
    t2[(i,j)] = float(ti)
end

start2 = 1
destination2 = 10

model2 = Model(HiGHS.Optimizer)
@variable(model2, x2[arcs2] >= 0, Bin)

@objective(model2, Min, sum(c2[(i,j)] * x2[(i,j)] for (i,j) in arcs2))

@constraint(model2, sum(t2[(i,j)] * x2[(i,j)] for (i,j) in arcs2) <= T_limit2)

for n in N2
    if n == start2
        @constraint(model2, sum(x2[(n,j)] for j in N2 if (n,j) in arcs2) - sum(x2[(i,n)] for i in N2 if (i,n) in arcs2) == 1)
    elseif n == destination2
        @constraint(model2, sum(x2[(n,j)] for j in N2 if (n,j) in arcs2) - sum(x2[(i,n)] for i in N2 if (i,n) in arcs2) == -1)
    else
        @constraint(model2, sum(x2[(n,j)] for j in N2 if (n,j) in arcs2) - sum(x2[(i,n)] for i in N2 if (i,n) in arcs2) == 0)
    end
end

optimize!(model2)
st2 = termination_status(model2)
pr2 = primal_status(model2)
println("Termination status: ", st2)
println("Primal status: ", pr2)

if st2 == MOI.OPTIMAL || st2 == MOI.LOCALLY_SOLVED
    println("\nOptimal cost (C*) = ", objective_value(model2))
    println("\nOptimal path:")
    for (i,j) in arcs2
        if value(x2[(i,j)]) > 0.5
            println("  From node $i to node $j")
        end
    end
else
    println("Model did not find an optimal solution.")
end

# Unconstrained
model2_uncon = Model(HiGHS.Optimizer)
@variable(model2_uncon, x2u[arcs2] >= 0, Bin)
@objective(model2_uncon, Min, sum(c2[(i,j)] * x2u[(i,j)] for (i,j) in arcs2))
for n in N2
    if n == start2
        @constraint(model2_uncon, sum(x2u[(n,j)] for j in N2 if (n,j) in arcs2) - sum(x2u[(i,n)] for i in N2 if (i,n) in arcs2) == 1)
    elseif n == destination2
        @constraint(model2_uncon, sum(x2u[(n,j)] for j in N2 if (n,j) in arcs2) - sum(x2u[(i,n)] for i in N2 if (i,n) in arcs2) == -1)
    else
        @constraint(model2_uncon, sum(x2u[(n,j)] for j in N2 if (n,j) in arcs2) - sum(x2u[(i,n)] for i in N2 if (i,n) in arcs2) == 0)
    end
end

optimize!(model2_uncon)
if termination_status(model2_uncon) == MOI.OPTIMAL
    println("\nUnconstrained optimal cost = ", objective_value(model2_uncon))
    println("Unconstrained path:")
    for (i,j) in arcs2
        if value(x2u[(i,j)]) > 0.5
            println("  From node $i to node $j")
        end
    end
end

println("\n=== Zadanie 4(c) ===")

# Counterexample for integer necessity
# Two parallel paths with same cost and time
edges_c = [
    (1,2,5,5), (1,3,5,5), (2,4,0,0), (3,4,0,0)
]
arcs_c = [(i,j) for (i,j,_,_) in edges_c]
c_c = Dict((i,j)=>float(ci) for (i,j,ci,_) in edges_c)
t_c = Dict((i,j)=>float(ti) for (i,j,_,ti) in edges_c)
T_c = 5
start_c = 1
dest_c = 4
N_c = 1:4

model_c = Model(HiGHS.Optimizer)
@variable(model_c, x_c[arcs_c] >= 0)  # No Bin, LP relaxation
@objective(model_c, Min, sum(c_c[(i,j)] * x_c[(i,j)] for (i,j) in arcs_c))
@constraint(model_c, sum(t_c[(i,j)] * x_c[(i,j)] for (i,j) in arcs_c) <= T_c)
for n in N_c
    if n == start_c
        @constraint(model_c, sum(x_c[(n,j)] for j in N_c if (n,j) in arcs_c) - sum(x_c[(i,n)] for i in N_c if (i,n) in arcs_c) == 1)
    elseif n == dest_c
        @constraint(model_c, sum(x_c[(n,j)] for j in N_c if (n,j) in arcs_c) - sum(x_c[(i,n)] for i in N_c if (i,n) in arcs_c) == -1)
    else
        @constraint(model_c, sum(x_c[(n,j)] for j in N_c if (n,j) in arcs_c) - sum(x_c[(i,n)] for i in N_c if (i,n) in arcs_c) == 0)
    end
end

optimize!(model_c)
if termination_status(model_c) == MOI.OPTIMAL
    println("LP solution:")
    for (i,j) in arcs_c
        val = value(x_c[(i,j)])
        if val > 0
            println("  x[($i,$j)] = $val")
        end
    end
    println("Total cost: ", objective_value(model_c))
    println("Total time: ", sum(t_c[(i,j)] * value(x_c[(i,j)]) for (i,j) in arcs_c))
end

println("\n=== Zadanie 4(d) ===")

# Without time constraint, LP
model_d = Model(HiGHS.Optimizer)
@variable(model_d, x_d[arcs] >= 0)  # No Bin
@objective(model_d, Min, sum(c[(i,j)] * x_d[(i,j)] for (i,j) in arcs))
for n in N
    if n == start
        @constraint(model_d, sum(x_d[(n,j)] for j in N if (n,j) in arcs) - sum(x_d[(i,n)] for i in N if (i,n) in arcs) == 1)
    elseif n == destination
        @constraint(model_d, sum(x_d[(n,j)] for j in N if (n,j) in arcs) - sum(x_d[(i,n)] for i in N if (i,n) in arcs) == -1)
    else
        @constraint(model_d, sum(x_d[(n,j)] for j in N if (n,j) in arcs) - sum(x_d[(i,n)] for i in N if (i,n) in arcs) == 0)
    end
end

optimize!(model_d)
if termination_status(model_d) == MOI.OPTIMAL
    println("LP solution without time constraint:")
    for (i,j) in arcs
        val = value(x_d[(i,j)])
        if val > 0
            println("  x[($i,$j)] = $val")
        end
    end
    println("Total cost: ", objective_value(model_d))
    println("Total time: ", sum(t[(i,j)] * value(x_d[(i,j)]) for (i,j) in arcs))
end