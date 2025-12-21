# Paweł Grzegory 282211

using JuMP
using HiGHS


# row: = dzistricts p1,p2,p3; column: = shitfs 1,2,3
min_mat = [2 4 3;    # p1
		   3 6 5;    # p2
		   5 7 6]    # p3

max_mat = [3 7 5;    # p1
		   5 7 10;   # p2
		   8 12 10]  # p3

# Minimum requirements: sum by shifts (columns) and sum by districts (rows)
shift_min = [10, 20, 18]   # shift 1,2,3
district_min = [10, 14, 13] # p1,p2,p3

n_districts, n_shifts = size(min_mat)

model = Model(HiGHS.Optimizer)

# Integer variable x[i,j] = number of police cars in district i during shift j
@variable(model, x[1:n_districts, 1:n_shifts] >= 0, Int)

# Cell constraints min/max
for i in 1:n_districts, j in 1:n_shifts
	@constraint(model, x[i,j] >= min_mat[i,j])
	@constraint(model, x[i,j] <= max_mat[i,j])
end

# Sum constraints by shifts (columns)
for j in 1:n_shifts
	@constraint(model, sum(x[i,j] for i in 1:n_districts) >= shift_min[j])
end

# Sum constraints by districts (rows)
for i in 1:n_districts
	@constraint(model, sum(x[i,j] for j in 1:n_shifts) >= district_min[i])
end

# Objective: minimize total number of police cars
@objective(model, Min, sum(x[i,j] for i in 1:n_districts, j in 1:n_shifts))

optimize!(model)

status = termination_status(model)
println("Status optymalizacji: ", status)

if status == MOI.OPTIMAL || status == MOI.LOCALLY_SOLVED
	sol = Int.(value.(x))
	println("\nOptymalny przydział (wiersze = p1,p2,p3; kolumny = zmiany 1,2,3):")
	for i in 1:n_districts
		println(join(sol[i, :], ", "))
	end
	total = sum(sol)
	println("\nSuma całkowita radiowozów = ", total)

	# Dodatkowe sprawdzenia
	println("\nSprawdzenie ograniczeń:")
	for j in 1:n_shifts
		s = sum(sol[:, j])
		println("  zmiana ", j, ": suma = ", s, " (wymagane ≥ ", shift_min[j], ")")
	end
	for i in 1:n_districts
		s = sum(sol[i, :])
		println("  dzielnica p", i, ": suma = ", s, " (wymagane ≥ ", district_min[i], ")")
	end
else
	println("Nie znaleziono optymalnego rozwiązania. Status: ", status)
end

