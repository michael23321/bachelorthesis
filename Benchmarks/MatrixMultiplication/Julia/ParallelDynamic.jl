using Base.Threads
using Statistics
using Random
using ThreadPinning 

pinthreads(:cores)

const R1 = 1000
const C1 = 1000
const R2 = 1000
const C2 = 1000

function matrix_multiplication(At::Matrix, B::Matrix, C::Matrix)
    @threads :dynamic for j in 1:C2 
        for i in 1:R1
            s = zero(Float64)
            for k in 1:R2
                s += At[k, i] * B[k, j]
            end
            C[i, j] = s
        end
    end
    return C 
end

Random.seed!(1234)
A = rand(Float64, R1, C1)
B = rand(Float64, R2, C2)
C = zeros(Float64, R1, C2)

At = copy(A')

const N_RUNS = 50
times = Float64[]

matrix_multiplication(At, B, C)
matrix_multiplication(At, B, C)
matrix_multiplication(At, B, C)

for i in 1:N_RUNS
    t = @elapsed matrix_multiplication(At, B, C)
    push!(times, t)
end

println("--- Times ---")
println(times)

println("\n--- Statistics ---")
println("Average:     $(mean(times)) s")