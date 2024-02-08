### Exercise 3

For the following numbers of processing units p and non-parallelisable fraction f, predict the parallel speedup and efficiency according to (a) Amdahl’s law, (b) the GustafsonBarsis law.

Amdahl's law: $S = \frac{1}{f+\frac{1-f}{p}}$  

Gustafson Barsis Law: $S = p + f(1-p)$

Efficiency: $E = \frac{S}{p}$

i)
$p=2$ and $f=0.1$
a)
$S = \frac{1}{0.1+\frac{1-0.1}{2}} = \frac{20}{3} _{(6.7)}$
$E = \frac{\frac{20}{3}}{2} = \frac{10}{3} _{(3.3)}$
b)
$S = 2 + 0.1(1-2) = 1.9$ 
$E = \frac{1.9}{2} = 0.95$

ii)
$p=17$ and $f=0.2$
a)
$S = \frac{1}{0.2+\frac{1-0.2}{17}} = \frac{85}{21} _{(4.05)}$
$E = \frac{\frac{85}{21}}{17} = \frac{5}{21} _{(0.24)}$
b)
$S = 17 + 0.2(1-17) = 13.8$ 
$E = \frac{13.8}{17} = 0.81$

iii)
$p=\infty$ and $f=0.05$
a)
$S = \frac{1}{0.05+\frac{1-0.05}{\infty}} = 20$
$E = \frac{20}{\infty} = 0$
b)
$S = \infty + 0.05(1-\infty) = 0.95\infty$ 
$E = \frac{ 0.95\infty}{\infty} = 0.95$











