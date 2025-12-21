#Paweł Grzegory 282211

function dot_forward(x::Vector{T}, y::Vector{T}) where T<:AbstractFloat
    S = zero(T)
    for i in 1:length(x)
        S += x[i] * y[i]
    end
    return S
end

function dot_backward(x::Vector{T}, y::Vector{T}) where T<:AbstractFloat
    S = zero(T)
    for i in length(x):-1:1
        S += x[i] * y[i]
    end
    return S
end

function dot_sorted(x::Vector{T}, y::Vector{T}; rev::Bool) where T<:AbstractFloat
    products = x .* y # Mnożenie elementów wektorów
    positives = filter(p -> p >= 0, products)
    negatives = filter(p -> p < 0, products)

    sort!(positives, rev=rev)
    sort!(negatives, rev=!rev)
    
    # Sumowanie w odpowiedniej kolejności
    sum_pos = isempty(positives) ? zero(T) : reduce(+, positives)
    sum_neg = isempty(negatives) ? zero(T) : reduce(+, negatives)

    return sum_pos + sum_neg
end

function main()
    x1_f64 = [2.718281828, -3.141592654, 1.414213562, 0.5772156649, 0.3010299957]
    x2_f64 = [2.718281828, -3.141592654, 1.414213562, 0.577215664, 0.301029995]
    y_f64 = [1486.2497, 878366.9879, -22.37492, 4773714.647, 0.000185049]

    x1_f32 = Float32.(x1_f64)
    x2_f32 = Float32.(x2_f64)
    y_f32 = Float32.(y_f64)

    exact_val = -1.00657107000000e-11

    println("--- Obliczenia dla Float32(x1_f32)  ---")
    res_a_32 = dot_forward(x1_f32, y_f32)
    println("(a) W przód:          ", res_a_32, "\t błąd absolutny: ", abs(res_a_32 - exact_val))
    res_b_32 = dot_backward(x1_f32, y_f32)
    println("(b) W tył:            ", res_b_32, "\t błąd absolutny: ", abs(res_b_32 - exact_val))
    res_c_32 = dot_sorted(x1_f32, y_f32, rev=true)
    println("(c) Od największego:  ", res_c_32, "\t błąd absolutny: ", abs(res_c_32 - exact_val))
    res_d_32 = dot_sorted(x1_f32, y_f32, rev=false)
    println("(d) Od najmniejszego: ", res_d_32, "\t błąd absolutny: ", abs(res_d_32 - exact_val))
    println()

    println("--- Obliczenia dla Float32(x2_f32)  ---")
    res_a_32 = dot_forward(x2_f32, y_f32)
    println("(a) W przód:          ", res_a_32, "\t błąd absolutny: ", abs(res_a_32 - exact_val))
    res_b_32 = dot_backward(x2_f32, y_f32)
    println("(b) W tył:            ", res_b_32, "\t błąd absolutny: ", abs(res_b_32 - exact_val))
    res_c_32 = dot_sorted(x2_f32, y_f32, rev=true)
    println("(c) Od największego:  ", res_c_32, "\t błąd absolutny: ", abs(res_c_32 - exact_val))
    res_d_32 = dot_sorted(x2_f32, y_f32, rev=false)
    println("(d) Od najmniejszego: ", res_d_32, "\t błąd absolutny: ", abs(res_d_32 - exact_val))
    println()

    println("--- Obliczenia dla Float64(x1_f64) ---")
    res_a_64 = dot_forward(x1_f64, y_f64)
    println("(a) W przód:          ", res_a_64, "\t błąd absolutny: ", abs(res_a_64 - exact_val))
    res_b_64 = dot_backward(x1_f64, y_f64)
    println("(b) W tył:            ", res_b_64, "\t błąd absolutny: ", abs(res_b_64 - exact_val))
    res_c_64 = dot_sorted(x1_f64, y_f64, rev=true)
    println("(c) Od największego:  ", res_c_64, "\t błąd absolutny: ", abs(res_c_64 - exact_val))
    res_d_64 = dot_sorted(x1_f64, y_f64, rev=false)
    println("(d) Od najmniejszego: ", res_d_64, "\t błąd absolutny: ", abs(res_d_64 - exact_val))
    println()

    println("--- Obliczenia dla Float64(x2_f64) ---")
    res_a_64 = dot_forward(x2_f64, y_f64)
    println("(a) W przód:          ", res_a_64, "\t błąd absolutny: ", abs(res_a_64 - exact_val))
    res_b_64 = dot_backward(x2_f64, y_f64)
    println("(b) W tył:            ", res_b_64, "\t błąd absolutny: ", abs(res_b_64 - exact_val))
    res_c_64 = dot_sorted(x2_f64, y_f64, rev=true)
    println("(c) Od największego:  ", res_c_64, "\t błąd absolutny: ", abs(res_c_64 - exact_val))
    res_d_64 = dot_sorted(x2_f64, y_f64, rev=false)
    println("(d) Od najmniejszego: ", res_d_64, "\t błąd absolutny: ", abs(res_d_64 - exact_val))
    println()

end

main()