#Paweł Grzegory 282211

module InterpolacjaNewtona

using Plots
export ilorazyRoznicowe, warNewton, naturalna

function ilorazyRoznicowe(x::Vector{Float64}, f::Vector{Float64})
    if length(x) != length(f)
        error("Wektory x i f muszą mieć tę samą długość")
    end

    n = length(x) - 1
    fx = copy(f)

    for j in 1:n
        for i in n+1:-1:j+1
            fx[i] = (fx[i] - fx[i-1]) / (x[i] - x[i-j])
        end
    end

    return fx
end

function warNewton(x::Vector{Float64}, fx::Vector{Float64}, t::Float64)
    n = length(fx) - 1
    if n < 0
        return 0.0
    end
    
    y = fx[n+1]
    for i in n:-1:1
        y = fx[i] + (t - x[i]) * y
    end
    
    return y
end

function naturalna(x::Vector{Float64}, fx::Vector{Float64})
    if length(x) != length(fx)
        error("Wektory `x` i `fx` muszą mieć tę samą długość")
    end

    n = length(fx)
    a = copy(fx)

    for i in (n-1):-1:1
        for j in i:(n-1)
            a[j] = a[j] - a[j+1] * x[i]
        end
    end

    return a
end

function rysujNnfx(f, a::Float64, b::Float64, n::Int; wezly::Symbol = :rownoodlegle, filename::String = "interpolacja_n")
    if n < 0
        error("Stopień wielomianu n nie może być ujemny.")
    end

    nodes_count = n + 1
    x_nodes = Vector{Float64}(undef, nodes_count)

    if wezly == :rownoodlegle
        h = (b - a) / n
        for k in 0:n
            x_nodes[k+1] = a + k * h
        end
    elseif wezly == :czebyszew
        for k in 1:nodes_count
            cos_val = cos(pi * (2.0 * k - 1.0) / (2.0 * nodes_count))
            x_nodes[k] = (a + b) / 2.0 + (b - a) / 2.0 * cos_val
        end
        sort!(x_nodes)
    else
        error("Nieznany typ węzłów: $wezly. Użyj :rownoodlegle lub :czebyszew.")
    end

    y_nodes = f.(x_nodes)
    fx = ilorazyRoznicowe(x_nodes, y_nodes)

    plot_points = 1000
    x_plot = range(a, stop=b, length=plot_points)
    y_f = f.(x_plot)
    y_p = [warNewton(x_nodes, fx, t) for t in x_plot]

    plot(x_plot, y_f, label="Funkcja $(string(f))", legend=:topleft, color=:blue)
    plot!(x_plot, y_p, label="Wielomian P(x) (n=$n)", color=:red, linestyle=:dash)
    scatter!(x_nodes, y_nodes, label="Węzły interpolacji", color=:black)
    title!("Interpolacja Newtona (węzły: $wezly)")
    xlabel!("x")
    ylabel!("y")
    
    #savefig("plots/$(filename)_n=$(n)_węzły_$(wezly).png")
    
    display(current())
end


end 
