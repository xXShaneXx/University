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
    products = x .* y
    positives = filter(p -> p >= 0, products)
    negatives = filter(p -> p < 0, products)

    # Dla sortowania od największego do najmniejszego (rev=true):
    # - dodatnie sortujemy malejąco
    # - ujemne sortujemy rosnąco (od najbardziej ujemnej)
    # Dla sortowania od najmniejszego do największego (rev=false):
    # - dodatnie sortujemy rosnąco
    # - ujemne sortujemy malejąco (do najmniej ujemnej)
    sort!(positives, rev=rev)
    sort!(negatives, rev=!rev)
    
    # Sumowanie w odpowiedniej kolejności
    # `sum` w Julii domyślnie używa algorytmu kompensacyjnego,
    # który jest dokładniejszy. Aby symulować prostą pętlę for,
    # używamy `reduce(+, ...)`.
    sum_pos = isempty(positives) ? zero(T) : reduce(+, positives)
    sum_neg = isempty(negatives) ? zero(T) : reduce(+, negatives)

    return sum_pos + sum_neg
end

function main()
    x_f64 = [2.718281828, -3.141592654, 1.414213562, 0.5772156649, 0.3010299957]
    y_f64 = [1486.2497, 878366.9879, -22.37492, 4773714.647, 0.000185049]

    x_f32 = Float32.(x_f64)
    y_f32 = Float32.(y_f64)

    println("--- Obliczenia dla Float32 ---")
    println("(a) W przód:          ", dot_forward(x_f32, y_f32))
    println("(b) W tył:            ", dot_backward(x_f32, y_f32))
    println("(c) Od największego:  ", dot_sorted(x_f32, y_f32, rev=true))
    println("(d) Od najmniejszego: ", dot_sorted(x_f32, y_f32, rev=false))
    println()

    println("--- Obliczenia dla Float64 ---")
    println("(a) W przód:          ", dot_forward(x_f64, y_f64))
    println("(b) W tył:            ", dot_backward(x_f64, y_f64))
    println("(c) Od największego:  ", dot_sorted(x_f64, y_f64, rev=true))
    println("(d) Od najmniejszego: ", dot_sorted(x_f64, y_f64, rev=false))
    println()
end

main()