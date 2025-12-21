#Paweł Grzegory 282211

include("functions.jl")
using Test

@testset "Bisection Method Tests" begin
    f1(x) = x^2 - 4
    f2(x) = x^3 - x - 2
    f3(x) = sin(x)
    delta = 1e-5
    epsilon = 1e-5

    # --- Test Cases with Sign Change (Expected Success: err = 0) ---

    r1, v1, it1, err1 = functions.bisection_method(f1, 0.0, 5.0, delta, epsilon)
    @test isapprox(r1, 2.0, atol=1e-5)
    @test isapprox(v1, 0.0, atol=1e-5)
    @test err1 == 0

    r2, v2, it2, err2 = functions.bisection_method(f2, 1.0, 2.0, delta, epsilon)
    @test abs(v2) < epsilon
    @test err2 == 0

    r3, v3, it3, err3 = functions.bisection_method(f3, 3.0, 4.0, delta, epsilon)
    @test abs(v3) < epsilon
    @test err3 == 0

    # --- Test Cases with No Sign Change (Expected Failure: err = 1) ---
    
    @test functions.bisection_method(f1, 5.0, 10.0, delta, epsilon) == (7.5, 52.25, 0, 1) 
    
    @test functions.bisection_method(f2, -2.0, -1.0, delta, epsilon) == (-1.5, -3.875, 0, 1)
end

@testset "Newton Method Tests" begin
    f1(x) = x^2 - 4
    df1(x) = 2*x
    f2(x) = x^3 - x - 2
    df2(x) = 3*x^2 - 1
    f3(x) = sin(x)
    df3(x) = cos(x)
    delta = 1e-5
    epsilon = 1e-5
    maxit = 100

    # Test case 1: Convergent to root
    r1, v1, it1, err1 = functions.newton_method(f1, df1, 3.0, delta, epsilon, maxit)
    @test isapprox(r1, 2.0, atol=1e-5)
    @test isapprox(v1, 0.0, atol=1e-5)
    @test err1 == 0

    # Test case 2: Initial guess is root
    r2, v2, it2, err2 = functions.newton_method(f1, df1, 2.0, delta, epsilon, maxit)
    @test r2 == 2.0
    @test v2 == 0.0
    @test it2 == 0
    @test err2 == 0

    # Test case 3: Derivative close to zero
    # For f(x) = x^3 + 1, df(x)=3x^2, at x=0, df=0, f=1
    f4(x) = x^3 + 1
    df4(x) = 3*x^2
    r3, v3, it3, err3 = functions.newton_method(f4, df4, 0.0, delta, epsilon, maxit)
    @test err3 == 2

    # Test case 4: Not converged in maxit
    r4, v4, it4, err4 = functions.newton_method(f2, df2, 10.0, delta, epsilon, 5) 
    @test err4 == 1
end

@testset "Secant Method Tests" begin
    f1(x) = x^2 - 4
    f2(x) = x^3 - x - 2
    delta = 1e-5
    epsilon = 1e-5
    maxit = 100

    # Test case 1: Convergent to root
    r1, v1, it1, err1 = functions.secant_method(f1, 0.0, 5.0, delta, epsilon, maxit)
    @test isapprox(r1, 2.0, atol=1e-5)
    @test isapprox(v1, 0.0, atol=1e-5)
    @test err1 == 0

    # Test case 2: Initial x1 is root
    r2, v2, it2, err2 = functions.secant_method(f1, 0.0, 2.0, delta, epsilon, maxit)
    @test r2 == 2.0
    @test v2 == 0.0
    @test it2 == 0
    @test err2 == 0

    # Test case 3: Not converged in maxit
    r3, v3, it3, err3 = functions.secant_method(f2, 10.0, 10.1, delta, epsilon, 5)  
    @test err3 == 1
    @test it3 == 5
end