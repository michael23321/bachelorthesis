using Base.Threads
using Statistics
using ThreadPinning 

pinthreads(:cores)

function delannoy_seq(m::Int, n::Int)
    if m == 0 || n == 0
        return 1
    else
        return delannoy_seq(m - 1, n) + delannoy_seq(m - 1, n - 1) + delannoy_seq(m, n - 1)
    end      
end


function delannoy(m::Int, n::Int)
    if m == 0 || n == 0
        return 1    
    elseif m < 9 || n < 9
        return delannoy_seq(m, n)
    end
    
    task1 = @spawn delannoy(m - 1, n)

    task2 = @spawn delannoy(m - 1, n - 1)

    task3 = @spawn delannoy(m, n - 1)

    return  fetch(task1) + fetch(task2) + fetch(task3)
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


