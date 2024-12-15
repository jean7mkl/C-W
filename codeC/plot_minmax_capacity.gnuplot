# plot_minmax_capacity.gnuplot
set terminal pngcairo size 1280,720 enhanced font 'Verdana,12'
set output 'minmax_capacity.png'

set title "Min and Max Capacities"
set xlabel "Index"
set ylabel "Capacity (MW)"
set grid
set datafile separator ";"

plot 'output/lv_all_minmax.csv' using 0:7 with linespoints title 'Capacity'
