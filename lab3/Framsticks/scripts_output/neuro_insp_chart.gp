#this is a sample script that draws output (neuro_insp_log.txt) produced by neuro_inspection.script using gnuplot
set terminal png
set output "neuro_insp_log.png"
set title "Neural signals in the Pendulum"
plot 'neuro_insp_log.txt' using 1:2 w lines title "#3 (from gyro 1)",'' using 1:3 w lines title "#5 (from gyro 2)",'' using 1:4 w lines title "#0,1 (aggregator,muscle)"
