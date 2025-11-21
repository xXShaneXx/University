#Paweł Grzegory 282211

module functions
export bisection_method, newton_method

    function bisection_method(f, a::Float64, b::Float64, delta::Float64, epsilon::Float64)
        if f(a) * f(b) >= 0
            r = (a + b) / 2
            v = f(r)
            it = 0
            err = 1
            return (r, v, it, err)
        end

        it = 0
        while abs((b - a)) > delta
            c = (a + b) / 2
            it += 1
            if abs(f(c)) < epsilon
                r = c
                v = f(c)
                err = 0
                return (r, v, it, err)
            elseif f(c) * f(a) < 0
                b = c
            else
                a = c
            end
        end

        r = (a + b) / 2
        v = f(r)
        err = 0
        return (r, v, it, err)
    end

    function newton_method(f, df, x0::Float64, delta::Float64, epsilon::Float64, maxit::Int)
        it = 0
        r = x0
        v = f(r)
        if abs(v) < epsilon
            return (r, v, it, 0)
        end
        for it in 1:maxit
            # Derivative close to zero
            if abs(df(r)) < 1e-10
                return (r, v, it - 1, 2)
            end
            # new x{n} = x{n-1} - f(r)/f'(r)
            x1 = r - v / df(r)
            v = f(x1)
            if abs(x1 - r) < delta || abs(v) < epsilon
                return (x1, v, it, 0)
            end
            r = x1
        end
        return (r, v, maxit, 1)
    end
end