set terminal postscript eps enhanced color "Times-New-Roman" 18
set grid
set output "traffic-jam-1500-tl1-tl3-real.eps"
set xlabel "time (s)"
set ylabel "traffic light color"
set key left
set title "injection period 1.5 s"

plot [70:160] [-15:15] "tl3" title "tl3 behavior" with lines lt 1 lc 1 lw 2, "tl1" title "tl1 behavior" with lines  lt 1 lc 3 lw 2
