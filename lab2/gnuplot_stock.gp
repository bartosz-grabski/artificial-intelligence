set terminal png
set output "predictions"
set xrange [0:270]
plot "stock_to_plot.txt" using 1 with lines title "Actual stock", \
	"stock_lr20" using 1:2 with lines title "learning rate 0.2", \
	"stock_lr30" using 1:2  with lines title "learning rate 0.3", \
	"stock_mom0.01" using 1:2 with lines title "momentum 0.01", \
	"stock_mom0.05" using 1:2 with lines title "momentum 0.05", \