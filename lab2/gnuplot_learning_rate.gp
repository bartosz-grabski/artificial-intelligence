set terminal png
set output "learning rate changes"
plot "mom0.01lr0.1n30" using 1 with lines title "learning rate 0.1", \
	"mom0.01lr0.2n30" using 1:(0.0074) with yerrorbars title "learning rate 0.2", \
	"mom0.01lr0.3n30" using 1 with lines title "learning rate 0.3", \