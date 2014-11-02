set terminal png
set output "momentum changes"
plot "mom0.01lr0.01n30" using 1 with lines title "momentum 0.01", \
	"mom0.05lr0.01n30" using 1 with lines title "momentum 0.05", \
	"mom0.1lr0.01n30" using 1 with lines title "momentum 0.1", \
	"mom0.2lr0.01n30" using 1 with lines title "momentum 0.2"