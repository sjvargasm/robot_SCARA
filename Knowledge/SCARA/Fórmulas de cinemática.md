![[zCinemática directa.png]]
# Definiciones preliminares
- $L_i$ es el segmento formado por un eslabón $i$.
- $J_i$ es el punto donde se ubica una articulación $i$. 
	- Específicamente para el robot SCARA, $J_1 = O = (0, 0)$ y $J_3 = (x, y)$ donde el efector final se ubica.
- $\theta_i$ es el menor de los ángulos que un eslabón $i$ forma con la prolongación del eslabón $i-1$.
	- Para el caso $\theta_1$, es el ángulo que forma el eslabón 1 con el eje $x$.
- Transormación de rotación en un ángulo $\alpha$
$$
	\rot{\alpha} = \begin{bmatrix}
	\cos\alpha & -\sin\alpha & 0 \\
	\sin\alpha & \cos\alpha & 0 \\
	0 & 0 & 1
	\end{bmatrix}
$$
- Transformación de traslación a un punto $(x,y)$
$$
	\trans{x}{y} = \begin{bmatrix}
	1 & 0 & x \\
	0 & 1 & y \\
	0 & 0 & 1
	\end{bmatrix}
$$

# Cinemática directa
Siguiendo un procedimiento similar al algoritmo de Denavit-Hartenberg,
$$
	\begin{bmatrix}
	x & y & 1
	\end{bmatrix}^T = \rot{\theta_1} \trans{L_1}{0} \rot{\theta_2} \trans{L_2}{0}\begin{bmatrix} 0 & 0 & 1 \end{bmatrix}^T
$$
se obtiene
$$
	\newcommand{rot}[1]{\operatorname{Rot}({#1})}
	\newcommand{trans}[2]{\operatorname{T}({#1}, {#2})}
	\begin{align}
	x &= L_1 \cos{\theta_1} + L_2 \cos{(\theta_1 + \theta_2)} \\
	y &= L_1 \sin{\theta_1} + L_2 \sin{(\theta_1 + \theta_2)}
	\end{align}
$$
# Cinemática inversa
Aplicando el teorema de los cosenos sobre el t
riángulo $\triangle L_1 L_2 \overline{OJ_3}$, se obtiene
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
