#Paweł Grzegory 282211

using LinearAlgebra, Printf, MatrixDepot



function run_experiments()
    println("--- Eksperyment 1: Macierz Hilberta Hn ---")
    @printf("%-5s | %-22s | %-5s | %-22s | %-22s\n", "n", "cond(Hn)", "rank", "Błąd wzgl. (Gauss)", "Błąd wzgl. (inv)")
    println(repeat("-", 85))

    for n in 2:20
        A = matrixdepot("hilb", n)
        x = ones(Float64, n)
        b = A * x

        # Rozwiązanie za pomocą eliminacji Gaussa (A \ b)
        x_gauss = A \ b
        
        # Rozwiązanie za pomocą macierzy odwrotnej (inv(A) * b)
        x_inv = inv(A) * b

        # Obliczenie błędów względnych
        err_gauss = norm(x_gauss - x) / norm(x)
        err_inv = norm(x_inv - x) / norm(x)

        # Wskaźnik uwarunkowania i rząd
        cond_A = cond(A)
        rank_A = rank(A)

        @printf("%-5d | %-22.4e | %-5d | %-22.4e | %-22.4e\n", n, cond_A, rank_A, err_gauss, err_inv)
    end

    println("\n--- Eksperyment 2: Losowa macierz Rn z zadanym cond(A) ---")
    ns = [5, 10, 20]
    cs = [1.0, 10.0, 1e3, 1e7, 1e12, 1e16]

    for n in ns
        println("\n--- Dla n = $n ---")
        @printf("%-10s | %-22s | %-5s | %-22s | %-22s\n", "c (zadane)", "cond(Rn)", "rank", "Błąd wzgl. (Gauss)", "Błąd wzgl. (inv)")
        println(repeat("-", 95))
        
        for c in cs
            A = matrixdepot("randsvd", n, c)
            x = ones(Float64, n)
            b = A * x

            # Rozwiązanie za pomocą eliminacji Gaussa (A \ b)
            x_gauss = A \ b
            
            # Rozwiązanie za pomocą macierzy odwrotnej (inv(A) * b)
            x_inv = inv(A) * b

            # Obliczenie błędów względnych
            err_gauss = norm(x_gauss - x) / norm(x)
            err_inv = norm(x_inv - x) / norm(x)

            # Wskaźnik uwarunkowania i rząd
            cond_A = cond(A)
            rank_A = rank(A)

            @printf("%-10.1e | %-22.4e | %-5d | %-22.4e | %-22.4e\n", c, cond_A, rank_A, err_gauss, err_inv)
        end
    end
end

run_experiments()
