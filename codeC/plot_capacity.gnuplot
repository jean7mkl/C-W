set terminal pngcairo size 1280,720 enhanced font 'Verdana,12'
set output 'capacity_vs_lv_station.png'

set title "Capacity vs LV Station"
set xlabel "LV Station"
set ylabel "Capacity (MW)"
set grid
set datafile separator ";"

set logscale y
set style data linespoints
set xtics 1
set pointsize 1.5

# Filtrage des lignes à la volée avec awk (simplifié)
plot '< awk -F";" "NR > 1 && $4 ~ /^[0-9]+$/ && $7 > 0 {print $0}" output/output_lv_all.dat' using 4:7 with linespoints title 'Capacity'
