using Statistics


function delannoy(m::Int, n::Int)
    if m == 0 || n == 0
        return 1
    else
        return delannoy(m - 1, n) + delannoy(m - 1, n - 1) + delannoy(m, n - 1)
    end      
end

if length(ARGS) != 1
    println(stderr, "Usage: julia DelannoySeq.jl <N>")
    exit(1)
end

str = ARGS[1]

const n = parse(Int, str)
const m = n

const N_RUNS = 20
times = Float64[]

delannoy(m, n)
delannoy(m, n)
delannoy(m, n)

for i in 1:N_RUNS
    t = @elapsed result = delannoy(m, n)
    push!(times, t)
    println(result)
end

println("--- Times ---")
println(times)

println("\n--- Statistics ---")
println("Average:     $(mean(times)) s")
