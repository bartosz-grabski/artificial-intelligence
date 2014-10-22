rem A sample script which processes all *.log files in this directory
rem and produces charts for each of them. Requires gnuplot and awk.

for %%i in (*.log) do (
awk -f getLOGlines.awk <%%i >%%i.dat
echo set terminal png >chart.gp
echo set output "%%i.png">>chart.gp
awk -f getTITLEline.awk <%%i >>chart.gp
echo set y2tics >>chart.gp
echo set ytics nomirror >>chart.gp
echo plot "%%i.dat" using 1:5 with lines  title "best","%%i.dat" using 1:4 with lines title "avg","%%i.dat" using 1:3 with lines title "worst","%%i.dat" using 1:2 axis x1y2 with lines title "# unique gen"  >>chart.gp

wgnuplot chart.gp
)
