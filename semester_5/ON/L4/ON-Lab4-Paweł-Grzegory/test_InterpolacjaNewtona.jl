#Paweł Grzegory 282211

include("InterpolacjaNewtona.jl")
using .InterpolacjaNewtona
using Test

@testset "Interpolacja Newtona" begin

    @testset "ilorazyRoznicowe" begin
        # Przykład 1: Wielomian f(x) = x^2
        x1 = [1.0, 2.0, 3.0, 4.0]
        f1 = [1.0, 4.0, 9.0, 16.0]
        expected1 = [1.0, 3.0, 1.0, 0.0]
        @test ilorazyRoznicowe(x1, f1) ≈ expected1

        # Przykład 2: Wielomian f(x) = x^3 - 2x + 5
        x2 = [-1.0, 0.0, 1.0, 2.0, 3.0]
        f2 = [6.0, 5.0, 4.0, 9.0, 26.0]
        expected2 = [6.0, -1.0, 0.0, 1.0, 0.0]
        @test ilorazyRoznicowe(x2, f2) ≈ expected2

        # Przykład 3: Funkcja f(x) = 1/x
        x3 = [1.0, 2.0, 4.0]
        f3 = [1.0, 0.5, 0.25]
        expected3 = [1.0, -0.5, 0.125]
        @test ilorazyRoznicowe(x3, f3) ≈ expected3
    end

    @testset "warNewton" begin
        # Test dla wielomianu f(x) = x^2
        x1 = [1.0, 2.0, 3.0]
        f1 = [1.0, 4.0, 9.0]
        fx1 = ilorazyRoznicowe(x1, f1) # [1.0, 3.0, 1.0]
        # p(t) = 1 + 3(t-1) + 1(t-1)(t-2)
        # p(1.5) = 1 + 3(0.5) + 1(0.5)(-0.5) = 1 + 1.5 - 0.25 = 2.25
        @test warNewton(x1, fx1, 1.5) ≈ 1.5^2
        @test warNewton(x1, fx1, 2.5) ≈ 2.5^2
        @test warNewton(x1, fx1, 1.0) ≈ 1.0
        @test warNewton(x1, fx1, 2.0) ≈ 4.0
        @test warNewton(x1, fx1, 3.0) ≈ 9.0

        # Test dla wielomianu f(x) = x^3 - 2x + 5
        x2 = [-1.0, 0.0, 1.0, 2.0]
        f2 = [6.0, 5.0, 4.0, 9.0]
        fx2 = ilorazyRoznicowe(x2, f2) # [6.0, -1.0, 0.0, 1.0]
        # p(t) = 6 - (t+1) + 0(t+1)t + 1(t+1)t(t-1)
        # p(0.5) = 6 - 1.5 + (1.5)(0.5)(-0.5) = 4.5 - 0.375 = 4.125
        # f(0.5) = 0.5^3 - 2*0.5 + 5 = 0.125 - 1 + 5 = 4.125
        @test warNewton(x2, fx2, 0.5) ≈ 0.5^3 - 2*0.5 + 5
        @test warNewton(x2, fx2, -1.0) ≈ 6.0
    end

    @testset "naturalna" begin
        # p(x) = x^2 (na węzłach 0,1,2)
        x = [0.0, 1.0, 2.0]
        # f(x)=x^2: c0=0, c1=1, c2=1
        fx = [0.0, 1.0, 1.0]
        a = naturalna(x, fx)
        #a0=0, a1=0, a2=1  (0 + 0*x + 1*x^2)
        @test a ≈ [0.0, 0.0, 1.0]

        # p(x) = 2 + 3x  (węzły 0,1)
        x2 = [0.0, 1.0]
        fx2 = [2.0, 3.0]
        a2 = naturalna(x2, fx2)
        @test a2 ≈ [2.0, 3.0]

        # p(x) = 6 - (t+1) + (t+1)t(t-1) = 6 - t - 1 + t^3 - t = t^3 - 2t + 5
        x3 = [-1.0, 0.0, 1.0, 2.0]
        fx3 = [6.0, -1.0, 0.0, 1.0]
        a3 = naturalna(x3, fx3)
        # p(x) = 5 - 2x + 0x^2 + 1x^3
        @test a3 ≈ [5.0, -2.0, 0.0, 1.0]
    end

end

println("Wszystkie testy w `test_InterpolacjaNewtona.jl` przeszły pomyślnie.")
