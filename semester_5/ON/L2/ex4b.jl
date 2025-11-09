#Paweł Grzegory 282211

using Polynomials
using Printf

function p_func(x::Real)
    return (x - 1.0) * (x - 2.0) * (x - 3.0) * (x - 4.0) * (x - 5.0) *
           (x - 6.0) * (x - 7.0) * (x - 8.0) * (x - 9.0) * (x - 10.0) *
           (x - 11.0) * (x - 12.0) * (x - 13.0) * (x - 14.0) * (x - 15.0) *
           (x - 16.0) * (x - 17.0) * (x - 18.0) * (x - 19.0) * (x - 20.0)
end

coefficients = Array{Float64}(reverse([1, -210.0 - 2^(-23), 20615.0,-1256850.0,
      53327946.0,-1672280820.0, 40171771630.0, -756111184500.0,          
      11310276995381.0, -135585182899530.0,
      1307535010540395.0,     -10142299865511450.0,
      63030812099294896.0,     -311333643161390640.0,
      1206647803780373360.0,     -3599979517947607200.0,
      8037811822645051776.0,      -12870931245150988800.0,
      13803759753640704000.0,      -8752948036761600000.0,
      2432902008176640000.0]))

P = Polynomial(coefficients)
zeros_complex = roots(P)
zeros = real(zeros_complex)
sorted_zeros = sort(zeros)

println("Obliczone pierwiastki wielomianu:")
for r in sorted_zeros
    println(r)
end

println("\nWeryfikacja obliczonych pierwiastków zk, dla k = 1..20:")
println("=====================================================================================")
@printf "%-3s | %-22s | %-15s | %-15s | %-15s\n" "k" "zk" "|P(zk)|" "|p(zk)|" "|zk - k|"
println("-------------------------------------------------------------------------------------")

for k in 1:20
    zk = sorted_zeros[k]
    # Wartość wielomianu z postaci rozwiniętej
    P_zk_val = P(zk)
    # Wartość wielomianu z postaci iloczynowej
    p_zk_val = p_func(zk)
    # Różnica między obliczonym pierwiastkiem a liczbą całkowitą
    diff = abs(zk - k)
    
    @printf "%-3d | %-22.18f | %-15.2e | %-15.2e | %-15.2e\n" k zk abs(P_zk_val) abs(p_zk_val) diff
end
