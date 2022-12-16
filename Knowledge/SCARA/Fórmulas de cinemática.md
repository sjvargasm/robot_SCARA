![[zCinemática directa.png]]
# Definiciones preliminares
- $L_i$ es el segmento formado por un eslabón $i$.
- $J_i$ es el punto donde se ubica una articulación $i$. 
	- Específicamente para el robot SCARA, $J_1 = O = (0, 0)$ y $J_3 = (x, y)$ donde el efector final se ubica.
- $\theta_i$ es el menor de los ángulos que un eslabón $i$ forma con la prolongación del eslabón $i-1$.
	- Para el caso $\theta_1$, es el ángulo que forma el eslabón 1 con el eje $x$.

# Cinemática directa
Aplicando el algoritmo de Denavit-Hartenberg, se llega a la siguiente tabla

| |$\theta$|$d$|$a$|$\alpha$|
|:-:|:-:|:-:|:-:|:-:|
|0-1|0|$d$|0|0|
|1-2|$\theta_1$|0|$L_1$|0|
|2-3|$\theta_2$|0|$L_2$|0|
|3-4|$\theta_3$|0|0|0|
Aplicando las transformaciones correspondientes hallamos la matriz $T$ transformación del algoritmo DH
$$
T = \begin{bmatrix}
	cos(\theta_1 + \theta_2 + \theta_3) & -sin(\theta_1 + \theta_2 + \theta_3) & 0 & L_1 \cos{\theta_1} + L_2 \cos{(\theta_1 + \theta_2)} \\
	sin(\theta_1 + \theta_2 + \theta_3) & cos(\theta_1 + \theta_2 + \theta_3) & 0 & L_1 \sin{\theta_1} + L_2 \sin{(\theta_1 + \theta_2)} \\
	0 & 0 & 1 & d \\
	0 & 0 & 0 & 1
\end{bmatrix}
$$
Luego, para hallar la posición del efector final (EF) que es equivalente al origen del sistema de coordenadas del EF, se realiza
$$
	\begin{bmatrix}
		x & y & z & 1
	\end{bmatrix} ^T 
	= T \;
	\begin{bmatrix}
		0 & 0 & 0 & 1
	\end{bmatrix}^T
$$
Así se obtiene que la posición del EF es
$$
\begin{align}
	x &= L_1 \cos{\theta_1} + L_2 \cos{(\theta_1 + \theta_2)} \\
	y &= L_1 \sin{\theta_1} + L_2 \sin{(\theta_1 + \theta_2)} \\
	z &= d
\end{align}
$$
# Cinemática inversa
Aplicando el teorema de los cosenos sobre el triángulo $\triangle L_1 L_2 \overline{OJ_3}$, se obtiene
$$
\begin{align}
	\overline{OJ_3}^2 &= L_1^2 + L_2^2 - 2L_1L_2\cos{\left (\pi - \theta_2 \right )} \\
	x^2 + y^2 &= L_1^2 + L_2^2 + 2L_1L_2\cos{\theta_2}
\end{align}
$$
Luego, despejando para $\theta_2$,
$$
	\theta_2 = \arccos{\left ( \frac{x^2 + y^2 - L_1^2 - L_2^2}{2L_1L_2} \right)}
$$
Ahora, $\theta_1 = \arctan{(x/y)} - \alpha$, donde $\alpha$ es el ángulo opuesto a $L_2$ en $\triangle L_1 L_2 \overline{OJ_3}$, sin embargo, $\overline{OJ_3} \sin \alpha = L_2 \sin \theta_2$ y $\overline{OJ_3} \cos \alpha = L_1 + L_2 \cos \theta_2$. Por tanto,
$$
\theta_1 = \arctan{\frac{x}{y}} - \arctan{\frac{L_2 \sin{\theta_2}}{L_1 + L_2 \cos{\theta_2}}}
$$
