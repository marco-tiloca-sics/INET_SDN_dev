set terminal postscript eps enhanced color "Times-New-Roman" 18
set grid
set output "traffic-jam-500-tl3-real-report.eps"
set xlabel "time (s)"
set ylabel "traffic light color"
set key left
set title "injection period 0.5 s"

plot [70:160] [-15:15] "tl3" title "tl3 real" with lines lt 1 lc 1 lw 2, "cn_tl3" title "tl3 report" with lines  lt 1 lc 3 lw 2
