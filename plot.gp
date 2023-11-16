set terminal pngcairo enhanced font 'Verdana,10'
set output 'wave.png'

set title "Wave Equation Solution"
set xlabel "Position"
set ylabel "Time"
set zlabel "Displacement"
set grid

splot 'wave.dat' using 1:2:3 with lines
