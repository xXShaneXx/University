# Paweł Grzegory 282211

include("blocksys.jl")
using .blocksys
using Printf
using Plots
using Test
using LinearAlgebra

struct Result
    n::Int
    time_gauss::Float64
    err_gauss::Float64       
    time_gauss_pivot::Float64
    err_gauss_pivot::Float64 
    time_lu::Float64
    err_lu::Float64          
    time_lu_pivot::Float64
    err_lu_pivot::Float64   
end

function run_all_tests(tests_dir::String)
    files = readdir(tests_dir)
    data_folders = filter(f -> occursin(r"Dane\d+_", f), files)
    sort!(data_folders, by = f -> parse(Int, match(r"Dane(\d+)_", f).captures[1]))

    results = Result[]

    println("\n" * "-"^130)
    @printf("%-7s | %-28s | %-28s | %-28s | %-28s\n", 
            "N", "Gauss (Time / Err)", "Gauss Piv (Time / Err)", "LU (Time / Err)", "LU Piv (Time / Err)")
    println("-"^130)

    for folder in data_folders
        path_A = joinpath(tests_dir, folder, "A.txt")
        path_b = joinpath(tests_dir, folder, "b.txt")
        results_dir = joinpath(tests_dir, folder, "results")
        if !isdir(results_dir) mkdir(results_dir) end
        
        N_str = match(r"Dane(\d+)_", folder).captures[1]
        N = parse(Int, N_str)
        
        M = blocksys.load_matrix(path_A)
        
        x_exact = ones(Float64, M.n * M.l)
        b_calc = blocksys.multiply_matrix_vector(M, x_exact)

        function test_method(method_func, save_name)
            GC.gc()
            t = @elapsed begin
                x_res = method_func()
            end
            err = norm(x_res - x_exact) / norm(x_exact)
            blocksys.save_vector(joinpath(results_dir, save_name), x_res, err)
            return t, err
        end

        t_g, err_g     = test_method(() -> blocksys.solve_gauss(M, b_calc), "x_gen_Gauss.txt")
        t_gp, err_gp   = test_method(() -> blocksys.solve_gauss_pivot(M, b_calc), "x_gen_GaussPivot.txt")
        t_lu, err_lu   = test_method(() -> begin d=blocksys.compute_lu(M); blocksys.solve_lu_system(d, b_calc) end, "x_gen_LU.txt")
        t_lup, err_lup = test_method(() -> begin d=blocksys.compute_lu_pivot(M); blocksys.solve_lu_system(d, b_calc) end, "x_gen_LUPivot.txt")

        push!(results, Result(N, t_g, err_g, t_gp, err_gp, t_lu, err_lu, t_lup, err_lup))


        @printf("%-7d | %8.5f s   %8.2e | %8.5f s   %8.2e | %8.5f s   %8.2e | %8.5f s   %8.2e\n", 
                N, 
                t_g, err_g, 
                t_gp, err_gp, 
                t_lu, err_lu, 
                t_lup, err_lup)
    end
    println("-"^130)

    
    println("\n% Tabela Czasów:")
    println("\\begin{table}[H]")
    println("\\centering")
    println("\\caption{Czas wykonania algorytmów (s).}")
    println("\\begin{tabular}{|r|c|c|c|c|}")
    println("\\hline")
    println("\\textbf{N} & \\textbf{Gauss} & \\textbf{Gauss Piv} & \\textbf{LU} & \\textbf{LU Piv} \\\\ \\hline")
    for r in results
        @printf("%d & %.5f & %.5f & %.5f & %.5f \\\\ \\hline\n", r.n, r.time_gauss, r.time_gauss_pivot, r.time_lu, r.time_lu_pivot)
    end
    println("\\end{tabular}")
    println("\\end{table}")

    println("\n% Tabela Błędów:")
    println("\\begin{table}[H]")
    println("\\centering")
    println("\\caption{Błąd względny rozwiązania.}")
    println("\\begin{tabular}{|r|c|c|c|c|}")
    println("\\hline")
    println("\\textbf{N} & \\textbf{Gauss} & \\textbf{Gauss Piv} & \\textbf{LU} & \\textbf{LU Piv} \\\\ \\hline")
    for r in results
        @printf("%d & %.2e & %.2e & %.2e & %.2e \\\\ \\hline\n", r.n, r.err_gauss, r.err_gauss_pivot, r.err_lu, r.err_lu_pivot)
    end
    println("\\end{tabular}")
    println("\\end{table}")
    
    return results
end

function generate_plots(results::Vector{Result})
    ns = [r.n for r in results]
    
    # Time Plot
    p1 = plot(ns, [r.time_gauss for r in results], label="Gauss", marker=:circle, title="Czas wykonania", xlabel="N", ylabel="Czas (s)", legend=:topleft)
    plot!(p1, ns, [r.time_gauss_pivot for r in results], label="Gauss Pivot", marker=:square)
    plot!(p1, ns, [r.time_lu for r in results], label="LU", marker=:utriangle, linestyle=:dash)
    plot!(p1, ns, [r.time_lu_pivot for r in results], label="LU Pivot", marker=:dtriangle, linestyle=:dash)
    savefig(p1, "wykres_czasu.png")

    # Error Plot
    p2 = plot(ns, [r.err_gauss for r in results], label="Gauss", marker=:circle, yaxis=:log, title="Błąd względny", xlabel="N", ylabel="||x - x*|| / ||x*||")
    plot!(p2, ns, [r.err_gauss_pivot for r in results], label="Gauss Pivot", marker=:square)
    plot!(p2, ns, [r.err_lu for r in results], label="LU", marker=:utriangle)
    plot!(p2, ns, [r.err_lu_pivot for r in results], label="LU Pivot", marker=:dtriangle) # Opcjonalnie
    savefig(p2, "wykres_bledu.png")
    
    println("\nWykresy i pliki wynikowe zostały wygenerowane.")
end

if isdir("tests")
    res = run_all_tests("tests")
    generate_plots(res)
else
    println("Błąd: Katalog 'tests' nie istnieje.")
end