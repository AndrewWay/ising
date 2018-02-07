#!/usr/bin/env gnuplot

reset
#set terminal wxt enhanced
set terminal png
set title "Varying lattice sizes"

#Energies
set key bottom right
set output "lattice_size_energies.png"
set ylabel "Energy per spin, <E>/N"
set xlabel "Temperature"
plot 'lat64.dat' u 1:2 w lp t "64", \
'lat32.dat' u 1:2 w lp t "32", \
'lat16.dat' u 1:2 w lp t "16", \
'lat08.dat' u 1:2 w lp t "8", \
'lat04.dat' u 1:2 w lp t "4", \
'lat02.dat' u 1:2 w lp t "2"

#Magnetization
set key top right
set output "lattice_size_magnetization.png"
set ylabel "Magnetization per spin, <E>^2/N"
set xlabel "Temperature"
plot 'lat64.dat' u 1:6 w lp t "64", \
'lat32.dat' u 1:5 w lp t "32", \
'lat16.dat' u 1:5 w lp t "16", \
'lat08.dat' u 1:5 w lp t "8", \
'lat04.dat' u 1:5 w lp t "4", \
'lat02.dat' u 1:5 w lp t "2"
