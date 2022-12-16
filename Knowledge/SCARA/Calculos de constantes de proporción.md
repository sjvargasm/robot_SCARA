# Ángulos a pasos
La interfaz gráfica devuelve el ángulo $\alpha$ que una articulación $J$ del robot debe tener con respecto al eje $x$. Esta posición está dada por el número $S$ de pasos que el stepper debe dar, que viene dado por:

$$
	S = \frac{\alpha P}{\phi R} = \frac{P}{\phi R} \alpha = k \alpha
$$

Donde:
- $P$ es la proporción de reducción de los engranajes entre el motor stepper y el eslabón.
- $\phi$ es el ángulo de cada paso.
- $R$ es la resolución del paso, un factor que permite escalar $\phi$ consiguiendo microstepping.

El producto $\alpha P$ indica el ángulo que debe tener el stepper para alcanzar dicha posición. Dividiendo ese ángulo entre $\phi$, se obtiene $S$. Considerando $\phi = 1.8\text{°}$ y $R = 1$ para todos los steppers de las articulaciones rotacionales, se tiene

$$
\begin{align}
    &P_{J_1} = 20:1 \rightarrow k_{J_1} = \frac{20 \times 4}{1.8} \approx 44,444444 \\
    &P_{J_2} = 16:1 \rightarrow k_{J_2} = \frac{16 \times 4}{1.8} \approx 35,555556 \\
    &P_{J_3} = 4:1 \rightarrow k_{J_3} = \frac{4 \times 4}{1.8} \approx 8,888889
 \\ 
\end{align}
$$

# Distancia a pasos
La interfaz gráfica devuelve la distancia $L$ sobre el eje $z$ que debe posicionarse el efector final. Esta distancia viene dada por la posición del robot a lo largo de su eje vertical, el cual es un tornillo sinfín con un diámetro $D$ y un paso $p$.

$$
S = \frac{L}{p} \times \frac{360}{\phi}  = \frac{360}{\phi p} L = kL
$$

El cociente $L / p$ representa el número de revoluciones que realiza el stepper para llegar a la posición $L$, multiplicando eso por el número de pasos por revolución que da el stepper, $360 / \phi$, se obtiene $S$. Considerando $p = 2\text{mm}$, $\phi=1.8\text{°}$, se tiene que

$$
k = 100
$$
