using Statistics
using Profile

const ITERATIONS = 700000000

function monte_carlo_pi(iterations)
    points_in_circle = 0
    for _ in 1:iterations
        x = rand()
        y = rand()

        if x * x + y * y <= 1.0
            points_in_circle += 1
        end
    end

    return 4 *  (points_in_circle / iterations)
end

const N_RUNS = 20
times = Float64[]

monte_carlo_pi(ITERATIONS)
monte_carlo_pi(ITERATIONS)
monte_carlo_pi(ITERATIONS)

for i in 1:N_RUNS
    t = @elapsed pi = monte_carlo_pi(ITERATIONS)
    push!(times, t)
    println(pi)
end

println("--- Times ---")
println(times)

println("\n--- Statistics ---")
println("Average:     $(mean(times)) s")