using FileIO
using Images
using Base.Threads
using Statistics
using ThreadPinning 

pinthreads(:cores)

const X = 1920
const Y = 1080
const MAX_ITER = 10000

function calc_mandelbrot()
    image = Array{Float32}(undef, Y, X)
    @threads :greedy for i in 1:Y
        cy = (i - 1) / Y * 2.0 - 1.0
        for j in 1:X            
            x = 0.0
            y = 0.0
            cx = (j - 1) / X * 3.5 - 2.5   
            iteration = 0
            while x*x + y*y <= 4.0 && iteration < MAX_ITER
                x_tmp = x*x - y*y + cx
                y = 2*x*y + cy
                x = x_tmp
                iteration += 1
            end

            image[i, j] = iteration / MAX_ITER
        end
    end

    return image    
end

const N_RUNS = 20
times = Float64[]

calc_mandelbrot()
calc_mandelbrot()
calc_mandelbrot()

for i in 1:N_RUNS
    t = @elapsed calc_mandelbrot()
    push!(times, t)
end

img = calc_mandelbrot()

println("--- Times ---")
println(times)

println("\n--- Statistics ---")
println("Average:     $(mean(times)) s")

save("mandelbrot.png", colorview(Gray, img))
println("Saved Mandelbrot image to mandelbrot.png")