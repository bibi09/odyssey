v 20100214 2
C 40000 40000 0 0 0 title-B.sym
T 50000 40700 9 10 1 0 0 0 1
Mercury robot controller
T 50000 40100 9 10 1 0 0 0 1
1
T 51500 40100 9 10 1 0 0 0 1
1
T 53800 40400 9 10 1 0 0 0 1
1.0 - 18/12/2013
T 53900 40100 9 10 1 0 0 0 1
Adrien RICCIARDI
C 50400 44500 1 0 0 pic16F876-1.sym
{
T 53000 50600 5 10 1 1 0 0 1
refdes=U2
T 50700 50750 5 10 0 0 0 0 1
device=pic16F876
T 50700 51150 5 10 0 0 0 0 1
footprint=DIP28
}
C 49100 50000 1 0 0 resistor-1.sym
{
T 49400 50400 5 10 0 0 0 0 1
device=RESISTOR
T 49300 50300 5 10 1 1 0 0 1
refdes=R2
T 49600 50300 5 10 1 1 0 0 1
value=10K
}
C 48600 50200 1 0 0 5V-plus-1.sym
B 41700 42200 1500 800 3 0 0 0 -1 -1 0 -1 -1 -1 -1 -1
P 42500 41900 42500 42200 1 0 0
{
T 42500 41800 5 10 0 0 90 0 1
pintype=unknown
T 42745 42400 5 10 1 1 180 0 1
pinlabel=GND
T 42595 42150 5 10 1 1 180 6 1
pinnumber=1
T 42500 41800 5 10 0 0 90 0 1
pinseq=0
}
T 41700 43100 9 10 1 0 0 0 1
LM1085-5.0
T 43000 43100 9 10 1 0 0 0 1
U1
P 43500 42600 43200 42600 1 0 0
{
T 43500 42600 5 10 0 0 0 0 1
pintype=unknown
T 43145 42595 5 10 1 1 0 6 1
pinlabel=Vout
T 43295 42645 5 10 1 1 0 0 1
pinnumber=2
T 43500 42600 5 10 0 0 0 0 1
pinseq=0
}
P 41400 42600 41700 42600 1 0 0
{
T 41200 42600 5 10 0 0 0 0 1
pintype=unknown
T 41755 42595 5 10 1 1 0 0 1
pinlabel=Vin
T 41605 42645 5 10 1 1 0 6 1
pinnumber=3
T 41200 42600 5 10 0 0 0 0 1
pinseq=0
}
N 41400 42600 40400 42600 4
C 40700 42300 1 270 0 capacitor-2.sym
{
T 41400 42100 5 10 0 0 270 0 1
device=POLARIZED_CAPACITOR
T 40500 42000 5 10 1 1 0 0 1
refdes=C1
T 41600 42100 5 10 0 0 270 0 1
symversion=0.1
T 40500 41600 5 10 1 1 0 0 1
value=10u
}
C 42400 40500 1 0 0 gnd-1.sym
C 43700 42300 1 270 0 capacitor-2.sym
{
T 44400 42100 5 10 0 0 270 0 1
device=POLARIZED_CAPACITOR
T 43500 42000 5 10 1 1 0 0 1
refdes=C2
T 44600 42100 5 10 0 0 270 0 1
symversion=0.1
T 43500 41600 5 10 1 1 0 0 1
value=10u
T 43100 41400 5 10 1 1 0 0 1
description=tantalum
}
C 44600 42300 1 270 0 capacitor-2.sym
{
T 45300 42100 5 10 0 0 270 0 1
device=POLARIZED_CAPACITOR
T 44400 42000 5 10 1 1 0 0 1
refdes=C3
T 45500 42100 5 10 0 0 270 0 1
symversion=0.1
T 44400 41600 5 10 1 1 0 0 1
value=470u
}
N 42500 41900 42500 40800 4
N 40900 40800 43900 40800 4
N 43900 40800 43900 41400 4
N 40900 41400 40900 40800 4
N 42500 40800 45700 40800 4
N 44800 40800 44800 41400 4
N 40900 42300 40900 42600 4
N 44800 42600 44800 42300 4
N 43900 42300 43900 42600 4
C 45600 42600 1 270 0 resistor-1.sym
{
T 46000 42300 5 10 0 0 270 0 1
device=RESISTOR
T 45200 42200 5 10 1 1 0 0 1
refdes=R1
T 45500 42100 5 10 1 1 180 0 1
value=220
}
C 45600 41700 1 270 0 led-2.sym
{
T 45300 41100 5 10 1 1 0 0 1
refdes=D1
T 46200 41600 5 10 0 0 270 0 1
device=LED
}
N 43500 42600 46200 42600 4
C 46000 42600 1 0 0 5V-plus-1.sym
N 50000 50100 50400 50100 4
N 48800 50200 48800 50100 4
N 48800 50100 49100 50100 4
C 55000 50000 1 0 0 led-2.sym
{
T 55300 50400 5 10 1 1 180 0 1
refdes=D2
T 55100 50600 5 10 0 0 0 0 1
device=LED
}
C 53800 50000 1 0 0 resistor-1.sym
{
T 54100 50400 5 10 0 0 0 0 1
device=RESISTOR
T 54000 50300 5 10 1 1 0 0 1
refdes=R3
T 54300 50300 5 10 1 1 0 0 1
value=330
}
N 53800 50100 53500 50100 4
N 54700 50100 55000 50100 4
C 56100 49600 1 0 0 gnd-1.sym
N 55900 50100 56200 50100 4
N 56200 50100 56200 49900 4
C 48800 46200 1 90 0 crystal-1.sym
{
T 48300 46400 5 10 0 0 90 0 1
device=CRYSTAL
T 49200 46700 5 10 1 1 180 0 1
refdes=X1
T 48100 46400 5 10 0 0 90 0 1
symversion=0.1
T 48900 46400 5 10 1 1 0 0 1
value=3.6864MHz
}
C 47400 46700 1 0 0 capacitor-1.sym
{
T 47600 47400 5 10 0 0 0 0 1
device=CAPACITOR
T 47400 47100 5 10 1 1 0 0 1
refdes=C4
T 47600 47600 5 10 0 0 0 0 1
symversion=0.1
T 48000 47100 5 10 1 1 0 0 1
value=15p
}
C 47400 46000 1 0 0 capacitor-1.sym
{
T 47600 46700 5 10 0 0 0 0 1
device=CAPACITOR
T 47400 46400 5 10 1 1 0 0 1
refdes=C5
T 47600 46900 5 10 0 0 0 0 1
symversion=0.1
T 48000 46400 5 10 1 1 0 0 1
value=15p
}
N 48300 46200 49900 46200 4
N 49900 46200 49900 46500 4
N 50400 46500 49900 46500 4
C 47300 45500 1 0 0 gnd-1.sym
N 47400 46900 47400 45800 4
N 50400 46900 48300 46900 4
C 50000 47000 1 0 0 gnd-1.sym
N 50100 47300 50400 47300 4
C 48500 44900 1 180 0 dc_motor-1.sym
{
T 48100 43900 5 10 0 0 180 0 1
device=DC_MOTOR
T 48000 44300 5 10 1 1 180 0 1
refdes=M1
}
C 49800 44900 1 180 0 dc_motor-1.sym
{
T 49400 43900 5 10 0 0 180 0 1
device=DC_MOTOR
T 49300 44300 5 10 1 1 180 0 1
refdes=M2
}
N 49600 44900 49600 45300 4
N 49600 45300 50400 45300 4
N 50400 45700 48300 45700 4
N 48300 45700 48300 44900 4
N 48300 43900 49600 43900 4
C 48800 43600 1 0 0 gnd-1.sym
C 54100 46200 1 0 0 gnd-1.sym
N 54200 46500 53500 46500 4
C 54000 46900 1 0 0 5V-plus-1.sym
N 54200 46900 53500 46900 4
N 53500 46100 55200 46100 4
C 55200 45600 1 0 0 output-1.sym
{
T 55300 45900 5 10 0 0 0 0 1
device=OUTPUT
T 55600 45600 5 10 1 1 0 0 1
value=TX
}
C 56000 46200 1 180 0 input-1.sym
{
T 56000 45900 5 10 0 0 180 0 1
device=INPUT
T 55600 46000 5 10 1 1 0 0 1
value=RX
}
C 54700 45800 1 180 0 resistor-1.sym
{
T 54400 45400 5 10 0 0 180 0 1
device=RESISTOR
T 54100 45500 5 10 1 1 180 0 1
refdes=R4
T 54600 45500 5 10 1 1 180 0 1
value=1.8K
}
C 55100 44500 1 90 0 resistor-1.sym
{
T 54700 44800 5 10 0 0 90 0 1
device=RESISTOR
T 55500 45100 5 10 1 1 180 0 1
refdes=R5
T 55300 44700 5 10 1 1 0 0 1
value=3.3K
}
N 54700 45700 55200 45700 4
N 55000 45400 55000 45700 4
N 53800 45700 53500 45700 4
C 54900 44200 1 0 0 gnd-1.sym
C 45500 49300 1 0 0 resistor-1.sym
{
T 45800 49700 5 10 0 0 0 0 1
device=RESISTOR
T 45700 49600 5 10 1 1 0 0 1
refdes=R6
T 46000 49600 5 10 1 1 0 0 1
value=1K
}
C 47000 49100 1 270 0 resistor-1.sym
{
T 47400 48800 5 10 0 0 270 0 1
device=RESISTOR
T 46600 48700 5 10 1 1 0 0 1
refdes=R7
T 46900 48600 5 10 1 1 180 0 1
value=1K
}
C 47800 49100 1 270 0 resistor-1.sym
{
T 48200 48800 5 10 0 0 270 0 1
device=RESISTOR
T 47400 48700 5 10 1 1 0 0 1
refdes=R8
T 47700 48600 5 10 1 1 180 0 1
value=1K
}
N 45100 49400 45500 49400 4
N 46400 49400 47900 49400 4
N 47900 49100 47900 49700 4
N 47100 49100 47100 49400 4
N 47100 48200 47900 48200 4
C 47400 47900 1 0 0 gnd-1.sym
N 50400 49700 47900 49700 4
C 44900 49400 1 0 0 generic-power.sym
{
T 45100 49650 5 10 1 1 0 3 1
net=Vbat
}
C 40200 42600 1 0 0 generic-power.sym
{
T 40400 42850 5 10 1 1 0 3 1
net=Vbat
}