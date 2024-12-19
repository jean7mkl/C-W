 reset
    set terminal pngcairo enhanced font "Arial,12" size 1200,1000
    set output 'graphs/lv_all_minmax.png'
    set datafile separator ":"
    datafile = 'output/lv_all_minmax.csv'
    set title 'lv all minmax : Production des stations et consommation des consommateurs'
    set xlabel 'ID Stations'
    set ylabel 'Capacité (kWh)'
    set xtics rotate by -90
    set yrange [0:*]
    set format y "%.0f"
    set key outside
    set style data histograms
    set style histogram rowstacked
    set style fill solid 1.0 border -1
    set boxwidth 0.75
    plot datafile every ::1 using 2 lc rgb "#4CBB17" ti "Capacité", \
         '' every ::1 using (($3 >= 0) ? $3 : 0):xtic(1) lc rgb "#0000FF" ti "Consommation totale", \
         '' every ::1 using (($4 > 0) ? $4 : 0) lc rgb "#FF0000" ti "Consommation en trop"