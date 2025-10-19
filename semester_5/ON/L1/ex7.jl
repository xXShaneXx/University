using Printf

f(x) = sin(x) + cos(3*x)
df(x) = cos(x) - 3*sin(3*x)

x0 = Float64(1.0)

# Dokładna wartość pochodnej w punkcie x0
exact_derivative = df(x0)

println("Dokładna wartość pochodnej f'(1.0) = ", exact_derivative)
println("-"^120)
@printf("%-5s %-20s %-25s %-25s %-25s %-20s\n", "n", "h", "Przybliżona pochodna", "Błąd absolutny", "Błąd względny", "1.0 + h")
println("-"^120)

for n in 0:54
    h = Float64(2.0^(-n))

    # ef'(x0) ≈ (f(x0 + h) - f(x0)) / h
    approx_derivative = (f(x0 + h) - f(x0)) / h
    
    absolute_error = abs(exact_derivative - approx_derivative)
    
    relative_error = absolute_error / abs(exact_derivative)
    
    one_plus_h = Float64(1.0 + h)
    
    @printf("%-5d %-20.15e %-25.15f %-25.15e %-25.15e %-20.15f\n", n, h, approx_derivative, absolute_error, relative_error, one_plus_h)
end
