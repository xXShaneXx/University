function calculateFloatCount(start::Float64, intervalEnd::Float64)
    if start >= intervalEnd
        return 0
    end
    return reinterpret(UInt64, intervalEnd) - reinterpret(UInt64, start)
end

function print_info(start::Float64, intervalEnd::Float64)
    count = calculateFloatCount(start, intervalEnd)
    println("\nIlosc liczb Float64 w [", start, ", ", intervalEnd, "): ", count, ", log2: ", log2(count))

    println("Reprezentacja ", start, ":            ", bitstring(start))
    println("Reprezentacja nextfloat(", start, "): ", bitstring(nextfloat(start)))
    println("Krok na poczatku: ", log2(nextfloat(start) - start))

    println("Reprezentacja prevfloat(", intervalEnd, "): ", bitstring(prevfloat(intervalEnd)))
    println("Reprezentacja ", intervalEnd, ":            ", bitstring(intervalEnd))
    println("Krok na koncu:   ", log2(intervalEnd - prevfloat(intervalEnd)))
end

print_info(1.0, 2.0)
print_info(0.5, 1.0)
print_info(2.0, 4.0)