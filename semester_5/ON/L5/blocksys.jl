## Paweł Grzegory 282211

module blocksys


struct BlockMatrix
    n::Int          # Number of blocks
    l::Int          # Size of each block
    A_blocks::Vector{Matrix{Float64}}
    B_blocks::Vector{Matrix{Float64}}
    C_blocks::Vector{Matrix{Float64}}
end

# Helper to map global indices (i, j) to block indices and local coordinates
function global_to_local(i::Int, j::Int, l::Int)
    block_row = div(i - 1, l) + 1
    block_col = div(j - 1, l) + 1
    local_row = (i - 1) % l + 1
    local_col = (j - 1) % l + 1
    return block_row, block_col, local_row, local_col
end

# ==============================================================================
# I/O Functions
# ==============================================================================

function load_matrix(filepath::String)
    open(filepath, "r") do io
        line = split(readline(io))
        N = parse(Int, line[1])
        l = parse(Int, line[2])
        n = div(N, l)
        
        # Initialize blocks with zeros
        A_blocks = [zeros(Float64, l, l) for _ in 1:n]
        B_blocks = [zeros(Float64, l, l) for _ in 1:n]
        C_blocks = [zeros(Float64, l, l) for _ in 1:n] 

        while !eof(io)
            line_str = readline(io)
            if isempty(strip(line_str)) continue end
            parts = split(line_str)
            i = parse(Int, parts[1])
            j = parse(Int, parts[2])
            val = parse(Float64, parts[3])

            br, bc, lr, lc = global_to_local(i, j, l)

            if br == bc
                A_blocks[br][lr, lc] = val
            elseif br == bc + 1
                B_blocks[br][lr, lc] = val
            elseif br == bc - 1
                C_blocks[br][lr, lc] = val
            end
        end
        return BlockMatrix(n, l, A_blocks, B_blocks, C_blocks)
    end
end

function load_vector(filepath::String)
    open(filepath, "r") do io
        line = readline(io)
        N = parse(Int, strip(line))
        b = Vector{Float64}(undef, N)
        idx = 1
        while !eof(io) && idx <= N
            line_str = readline(io)
            if isempty(strip(line_str)) continue end
            b[idx] = parse(Float64, strip(line_str))
            idx += 1
        end
        return b
    end
end

function save_vector(filepath::String, x::Vector{Float64}, relative_error::Union{Float64, Nothing}=nothing)
    open(filepath, "w") do io
        if relative_error !== nothing
            println(io, relative_error)
        end
        for val in x
            println(io, val)
        end
    end
end

# Utility to multiply Matrix * Vector efficiently considering structure
function multiply_matrix_vector(M::BlockMatrix, x::Vector{Float64})
    N = M.n * M.l
    b = zeros(Float64, N)
    
    for k in 1:M.n
        # Indices for current block x range
        idx_curr = (k-1)*M.l + 1 : k*M.l
        
        # Contribution from Diagonal A_k
        b[idx_curr] .+= M.A_blocks[k] * x[idx_curr]
        
        # Contribution from Sub-diagonal B_k (if not first block)
        if k > 1
            idx_prev = (k-2)*M.l + 1 : (k-1)*M.l
            b[idx_curr] .+= M.B_blocks[k] * x[idx_prev]
        end
        
        # Contribution from Super-diagonal C_k (if not last block)
        if k < M.n
            idx_next = k*M.l + 1 : (k+1)*M.l
            b[idx_curr] .+= M.C_blocks[k] * x[idx_next]
        end
    end
    return b
end

# ==============================================================================
# 1. Gaussian Elimination Solvers (Without LU storage)
# ==============================================================================

# (a) Without pivoting
function solve_gauss(M_orig::BlockMatrix, b_orig::Vector{Float64})
    M = BlockMatrix(M_orig.n, M_orig.l, deepcopy(M_orig.A_blocks), deepcopy(M_orig.B_blocks), deepcopy(M_orig.C_blocks))
    b = copy(b_orig)
    n = M.n
    l = M.l
    N = n * l

    # Elimination Phase
    for k in 1:n-1
        # Eliminate Block B_{k+1} using Block A_k
        
        # For each column 'p' in block k
        for p in 1:l
            pivot_val = M.A_blocks[k][p, p]
            
            # Eliminate rows below pivot within A_k 
            for r in p+1:l
                factor = M.A_blocks[k][r, p] / pivot_val
                # A_k row op
                M.A_blocks[k][r, p:l] .-= factor * M.A_blocks[k][p, p:l]
                # C_k row op (needed for back sub)
                M.C_blocks[k][r, :] .-= factor * M.C_blocks[k][p, :]
                # b update
                b[(k-1)*l + r] -= factor * b[(k-1)*l + p]
            end

            # Eliminate rows in B_{k+1} (The block below)
            for r in 1:l
                if abs(M.B_blocks[k+1][r, p]) > 1e-14 
                    factor = M.B_blocks[k+1][r, p] / pivot_val
                    # B_{k+1} row op (elimination)
                    M.B_blocks[k+1][r, p:l] .-= factor * M.A_blocks[k][p, p:l]
                    M.A_blocks[k+1][r, :] .-= factor * M.C_blocks[k][p, :]
                    
                    b[k*l + r] -= factor * b[(k-1)*l + p]
                end
            end
        end
    end
    
    # Final "cleanup" of last block A_n internal elimination
    k = n
    for p in 1:l-1
        pivot_val = M.A_blocks[k][p, p]
        for r in p+1:l
            factor = M.A_blocks[k][r, p] / pivot_val
            M.A_blocks[k][r, p:l] .-= factor * M.A_blocks[k][p, p:l]
            b[(k-1)*l + r] -= factor * b[(k-1)*l + p]
        end
    end

    # Back Substitution
    x = zeros(Float64, N)
    
    # Handle last block separately
    x[N] = b[N] / M.A_blocks[n][l, l]
    for i in N-1:-1:(n-1)*l+1
        local_row = (i - 1) % l + 1
        sum_val = 0.0
        # Contribution from A_n upper triangle
        for j in local_row+1:l
            sum_val += M.A_blocks[n][local_row, j] * x[(n-1)*l + j]
        end
        x[i] = (b[i] - sum_val) / M.A_blocks[n][local_row, local_row]
    end

    # Handle remaining blocks
    for k in n-1:-1:1
        for i in k*l:-1:(k-1)*l+1
            local_row = (i - 1) % l + 1
            sum_val = 0.0
            
            # Contribution from A_k upper triangle
            for j in local_row+1:l
                sum_val += M.A_blocks[k][local_row, j] * x[(k-1)*l + j]
            end
            
            # Contribution from C_k (which interacts with x_{k+1})
            for j in 1:l
                 sum_val += M.C_blocks[k][local_row, j] * x[k*l + j]
            end
            
            x[i] = (b[i] - sum_val) / M.A_blocks[k][local_row, local_row]
        end
    end
    
    return x
end

# (b) With partial pivoting
function solve_gauss_pivot(M_orig::BlockMatrix, b_orig::Vector{Float64})
    M = BlockMatrix(M_orig.n, M_orig.l, deepcopy(M_orig.A_blocks), deepcopy(M_orig.B_blocks), deepcopy(M_orig.C_blocks))
    b = copy(b_orig)
    n = M.n
    l = M.l
    

    for k in 1:n
        
        start_col = (k-1)*l + 1
        end_col = k*l
        
        for p in 1:l
            curr_global_col = start_col + p - 1
            curr_global_row = curr_global_col 
            
            # --- Finding Pivot ---
            max_val = 0.0
            pivot_row_idx = -1 # Local index relative to current search window
            pivot_block_idx = -1 # 0 for A_k, 1 for B_{k+1}
            
            # Search in A_k (from p to l)
            for r in p:l
                val = abs(M.A_blocks[k][r, p])
                if val > max_val
                    max_val = val
                    pivot_row_idx = r
                    pivot_block_idx = 0 
                end
            end
            
            # Search in B_{k+1} (from 1 to l), only if not last block
            if k < n
                for r in 1:l
                     val = abs(M.B_blocks[k+1][r, p])
                     if val > max_val
                         max_val = val
                         pivot_row_idx = r
                         pivot_block_idx = 1
                     end
                end
            end
            
            if max_val < 1e-14
                error("Matrix is singular")
            end
            
            # --- Swapping ---
            if pivot_block_idx == 0 && pivot_row_idx != p
                # Swap within A_k
                row1, row2 = p, pivot_row_idx
                M.A_blocks[k][row1, :], M.A_blocks[k][row2, :] = M.A_blocks[k][row2, :], M.A_blocks[k][row1, :]
                M.C_blocks[k][row1, :], M.C_blocks[k][row2, :] = M.C_blocks[k][row2, :], M.C_blocks[k][row1, :]
                b[curr_global_row], b[(k-1)*l + row2] = b[(k-1)*l + row2], b[curr_global_row]
                
            elseif pivot_block_idx == 1
                # Swap Row p of A_k with Row pivot_row_idx of B_{k+1}
                
                # Indices
                r_A = p
                r_B = pivot_row_idx
                
                # Swap A_k[p, :] with B_{k+1}[r, :]
                tmpA = M.A_blocks[k][r_A, :]
                M.A_blocks[k][r_A, :] = M.B_blocks[k+1][r_B, :]
                M.B_blocks[k+1][r_B, :] = tmpA
                
                # Swap C_k[p, :] with A_{k+1}[r, :] (The logical extension of the rows)
                tmpC = M.C_blocks[k][r_A, :]
                M.C_blocks[k][r_A, :] = M.A_blocks[k+1][r_B, :]
                M.A_blocks[k+1][r_B, :] = tmpC
                
                # Swap RHS
                idx1 = (k-1)*l + r_A
                idx2 = k*l + r_B
                b[idx1], b[idx2] = b[idx2], b[idx1]
            end
            
            # --- Elimination ---
            pivot_val = M.A_blocks[k][p, p]
            
            # Eliminate A_k below p
            for r in p+1:l
                factor = M.A_blocks[k][r, p] / pivot_val
                M.A_blocks[k][r, p:l] .-= factor * M.A_blocks[k][p, p:l]
                M.C_blocks[k][r, :] .-= factor * M.C_blocks[k][p, :]
                b[(k-1)*l + r] -= factor * b[(k-1)*l + p]
            end
            
            # Eliminate B_{k+1} (if k < n)
            if k < n
                for r in 1:l
                    if abs(M.B_blocks[k+1][r, p]) > 1e-15
                        factor = M.B_blocks[k+1][r, p] / pivot_val
                        M.B_blocks[k+1][r, p:l] .-= factor * M.A_blocks[k][p, p:l]
                        M.A_blocks[k+1][r, :] .-= factor * M.C_blocks[k][p, :]
                        b[k*l + r] -= factor * b[(k-1)*l + p]
                    end
                end
            end
        end
    end

    # Reuse back substitution from non-pivot version (matrices are now upper triangular in block sense)
    N = n * l
    x = zeros(Float64, N)
    
    # Last block
    for i in N:-1:(n-1)*l+1
        lr = (i-1)%l + 1
        sum_v = 0.0
        for j in lr+1:l
             sum_v += M.A_blocks[n][lr, j] * x[(n-1)*l + j]
        end
        x[i] = (b[i] - sum_v) / M.A_blocks[n][lr, lr]
    end

    # Other blocks
    for k in n-1:-1:1
        for i in k*l:-1:(k-1)*l+1
            lr = (i-1)%l + 1
            sum_v = 0.0
            for j in lr+1:l
                sum_v += M.A_blocks[k][lr, j] * x[(k-1)*l + j]
            end
            for j in 1:l
                sum_v += M.C_blocks[k][lr, j] * x[k*l + j]
            end
            x[i] = (b[i] - sum_v) / M.A_blocks[k][lr, lr]
        end
    end
    
    return x
end

# ==============================================================================
# 2. LU Decomposition
# ==============================================================================

struct LUResult
    M::BlockMatrix # Holds both L (multipliers) and U
    p::Vector{Int} # Permutation vector
    pivot::Bool    # Flag if pivoting was used
end

# In-place LU without pivoting
function compute_lu(M_in::BlockMatrix)
    M = BlockMatrix(M_in.n, M_in.l, deepcopy(M_in.A_blocks), deepcopy(M_in.B_blocks), deepcopy(M_in.C_blocks))
    n = M.n
    l = M.l

    for k in 1:n-1
        for p in 1:l
            pivot_val = M.A_blocks[k][p, p]
            
            # Within A_k
            for r in p+1:l
                factor = M.A_blocks[k][r, p] / pivot_val
                M.A_blocks[k][r, p] = factor # Store L
                M.A_blocks[k][r, p+1:l] .-= factor * M.A_blocks[k][p, p+1:l]
                M.C_blocks[k][r, :] .-= factor * M.C_blocks[k][p, :]
            end
            
            # Within B_{k+1}
            for r in 1:l
                if abs(M.B_blocks[k+1][r, p]) > 1e-15
                    factor = M.B_blocks[k+1][r, p] / pivot_val
                    M.B_blocks[k+1][r, p] = factor # Store L
                    M.B_blocks[k+1][r, p+1:l] .-= factor * M.A_blocks[k][p, p+1:l]
                    M.A_blocks[k+1][r, :] .-= factor * M.C_blocks[k][p, :]
                end
            end
        end
    end
    
    # Last block
    k = n
    for p in 1:l-1
        pivot_val = M.A_blocks[k][p, p]
        for r in p+1:l
            factor = M.A_blocks[k][r, p] / pivot_val
            M.A_blocks[k][r, p] = factor # Store L
            M.A_blocks[k][r, p+1:l] .-= factor * M.A_blocks[k][p, p+1:l]
        end
    end

    return LUResult(M, Int[], false)
end

# In-place LU with pivoting
function compute_lu_pivot(M_in::BlockMatrix)
    M = BlockMatrix(M_in.n, M_in.l, deepcopy(M_in.A_blocks), deepcopy(M_in.B_blocks), deepcopy(M_in.C_blocks))
    n = M.n
    l = M.l
    
    p_vec = collect(1:n*l) # Permutation vector

    for k in 1:n
        
        for p in 1:l
            # Pivot search
            max_val = 0.0
            p_idx = -1
            block_type = -1 
            
            # 1. Search in A_k
            for r in p:l
                if abs(M.A_blocks[k][r, p]) > max_val
                    max_val = abs(M.A_blocks[k][r, p])
                    p_idx = r
                    block_type = 0
                end
            end
            
            # 2. Search in B_{k+1} (if not the last block)
            if k < n
                for r in 1:l
                    if abs(M.B_blocks[k+1][r, p]) > max_val
                        max_val = abs(M.B_blocks[k+1][r, p])
                        p_idx = r
                        block_type = 1
                    end
                end
            end
            
            if max_val < 1e-14 
                error("Singular matrix") 
            end

            # --- Swapping ---
            if block_type == 0 && p_idx != p
                 # Swap rows within A_k
                 M.A_blocks[k][p, :], M.A_blocks[k][p_idx, :] = M.A_blocks[k][p_idx, :], M.A_blocks[k][p, :]
                 # Swap rows within C_k (same height as A_k)
                 M.C_blocks[k][p, :], M.C_blocks[k][p_idx, :] = M.C_blocks[k][p_idx, :], M.C_blocks[k][p, :]
                 
                 if k > 1
                     M.B_blocks[k][p, :], M.B_blocks[k][p_idx, :] = M.B_blocks[k][p_idx, :], M.B_blocks[k][p, :]
                 end

                 idx1 = (k-1)*l + p
                 idx2 = (k-1)*l + p_idx
                 p_vec[idx1], p_vec[idx2] = p_vec[idx2], p_vec[idx1]

            elseif block_type == 1

                 
                 # Swap A_k[p, :] <-> B_{k+1}[p_idx, :]
                 tmpA = M.A_blocks[k][p, :]
                 M.A_blocks[k][p, :] = M.B_blocks[k+1][p_idx, :]
                 M.B_blocks[k+1][p_idx, :] = tmpA
                 
                 # Swap C_k[p, :] <-> A_{k+1}[p_idx, :] (logical extension of rows)
                 tmpC = M.C_blocks[k][p, :]
                 M.C_blocks[k][p, :] = M.A_blocks[k+1][p_idx, :]
                 M.A_blocks[k+1][p_idx, :] = tmpC
                 
                 # Update permutation vector
                 idx1 = (k-1)*l + p
                 idx2 = k*l + p_idx
                 p_vec[idx1], p_vec[idx2] = p_vec[idx2], p_vec[idx1]
            end
            
            # --- Elimination (Gaussian Elimination) ---
            pivot_val = M.A_blocks[k][p, p]
            
            # Elimination in A_k (below diagonal)
            for r in p+1:l
                factor = M.A_blocks[k][r, p] / pivot_val
                if abs(factor) <= 1e-15
                    factor = 0.0
                end
                M.A_blocks[k][r, p] = factor # Store multiplier L
                
                if abs(factor) > 1e-15
                    M.A_blocks[k][r, p+1:l] .-= factor * M.A_blocks[k][p, p+1:l]
                    M.C_blocks[k][r, :] .-= factor * M.C_blocks[k][p, :]
                end
            end
            
            # Elimination in B_{k+1} (block below)
            if k < n
                for r in 1:l
                    factor = M.B_blocks[k+1][r, p] / pivot_val
                    if abs(factor) <= 1e-15
                        factor = 0.0
                    end
                    M.B_blocks[k+1][r, p] = factor # Store multiplier L
                    
                    if abs(factor) > 1e-15
                        M.B_blocks[k+1][r, p+1:l] .-= factor * M.A_blocks[k][p, p+1:l]
                        M.A_blocks[k+1][r, :] .-= factor * M.C_blocks[k][p, :]
                    end
                end
            end
        end
    end
    
    return LUResult(M, p_vec, true)
end

# ==============================================================================
# 3. Solve using LU
# ==============================================================================

function solve_lu_system(lu::LUResult, b_in::Vector{Float64})
    M = lu.M
    n = M.n
    l = M.l
    N = n * l
    
    # Apply permutation to b
    b = copy(b_in)
    if lu.pivot
        b_perm = zeros(Float64, N)
        for i in 1:N
            b_perm[i] = b[lu.p[i]]
        end
        b = b_perm
    end

    # Forward Substitution (Ly = b)
    
    y = copy(b)
    
    for k in 1:n
        # Process A_k lower triangle
        for i in 1:l
            # Pivot row is (k-1)l + i.
            
            # Within A_k (rows below i)
            for r in i+1:l
                y[(k-1)*l + r] -= M.A_blocks[k][r, i] * y[(k-1)*l + i]
            end
            
            # Within B_{k+1} (all rows, since B is below A)
            if k < n
                for r in 1:l
                     y[k*l + r] -= M.B_blocks[k+1][r, i] * y[(k-1)*l + i]
                end
            end
        end
    end

    # Backward Substitution (Ux = y)
    x = copy(y)
    
    # Last block
    for i in N:-1:(n-1)*l+1
        lr = (i-1)%l + 1
        sum_val = 0.0
        for j in lr+1:l
             sum_val += M.A_blocks[n][lr, j] * x[(n-1)*l + j]
        end
        x[i] = (x[i] - sum_val) / M.A_blocks[n][lr, lr]
    end
    
    # Rest blocks
    for k in n-1:-1:1
        for i in k*l:-1:(k-1)*l+1
            lr = (i-1)%l + 1
            sum_val = 0.0
            for j in lr+1:l
                sum_val += M.A_blocks[k][lr, j] * x[(k-1)*l + j]
            end
            for j in 1:l
                sum_val += M.C_blocks[k][lr, j] * x[k*l + j]
            end
            x[i] = (x[i] - sum_val) / M.A_blocks[k][lr, lr]
        end
    end
    
    return x
end

end # module blocksys