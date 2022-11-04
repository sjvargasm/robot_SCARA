# Robot SCARA
Diseño y programación de un Robot tipo SCARA. Proyecto originalmente planteado y ejecutado para la materia Proyecto de Sistemas Mecatrónicos II en la carrera Ingeniería en Electrónica con énfasis en Mecatrónica (IEK/MECA) de la Facultad Politécnica de la Universidad Nacional de Asunción (FPUNA).

El diseño físico del Robot SCARA se obtuvo desde [este](https://howtomechatronics.com/projects/scara-robot-how-to-build-your-own-arduino-based-robot/) proyecto en [HowToMechatronics](https://howtomechatronics.com/). La programación se encuentra fuertemente inspirada en dicho proyecto.

## Uso
// TODO: Explicación del uso

## Dependencias
### Arduino
- AccelStepper 1.61
- Servo 1.1.8

### Java
- jSerialComm 2.9.2

# Marco Teórico
## Cálculo de constantes de proporción
Las posiciones de los eslabones del robot deben ser "traducidas" a pasos de stepper para que estos puedan ubicarse correctamente.

### Ángulos a pasos
La interfaz gráfica devuelve el ángulo $\alpha$ que una articulación $J$ del robot debe tener con respecto al eje $y$. Esta posición está dada por el número $S$ de pasos que el stepper debe dar, que viene dado por:

$$
S = \frac{\alpha P}{\phi R} = \frac{P}{\phi R} \alpha = k \alpha
$$

Donde:
- $P$ es la proporción de reducción de los engranajes entre el motor stepper y el eslabón.
- $\phi$ es el ángulo de cada paso.
- $R$ es la resolución del paso, un factor que permite escalar $\phi$.

El producto $\alpha P$ indica el ángulo que debe tener el stepper para alcanzar dicha posición. Dividiendo ese ángulo entre $\phi$, se obtiene $S$. Considerando $\phi = 1.8\text{°}$ y $R = 1/4$ para todos los steppers de las articulaciones rotacionales, se tiene

$$
\begin{align}
    &P_{J_1} = 20:1 \rightarrow k_{J_1} = \frac{20 \times 4}{1.8} \approx 44.444444 \\
    &P_{J_2} = 16:1 \rightarrow k_{J_2} = \frac{16 \times 4}{1.8} \approx 35.555556 \\
    &P_{J_3} = 4:1 \rightarrow k_{J_3} = \frac{4 \times 4}{1.8} \approx 8.888889 \\ 
\end{align}
$$

### Distancia a pasos
La interfaz gráfica devuelve la distancia $L$ sobre el eje $z$ que debe posicionarse el efector final. Esta distancia viene dada por la posición del robot a lo largo de su eje vertical, el cual es un tornillo sinfín con un diámetro $D$ y un paso $p$.

$$
S = \frac{L}{p} \times \frac{360}{\phi}  = \frac{360}{\phi p} L = kL
$$

El cociente $L / p$ representa el número de revoluciones que realiza el stepper para llegar a la posición $L$, multiplicando eso por el número de pasos por revolución que da el stepper, $360 / \phi$, se obtiene $S$. Considerando $p = 2\text{mm}$, $\phi=1.8\text{°}$, se tiene que

$$
k = 100
$$

# Legales

## Licencias de uso y distribución
Cada carpeta contiene sus propias licencias, a menos que se indique lo contrario.

|  Carpeta  |    Licencia    |
|-----------|----------------|
| Software/ |   GNU GPLv3    |
| Hardware/ | CERN-OHL-S-2.1 |
| Diseños/  |  CC BY-SA 4.0  |

## Copyright
©2022 - Diego Noguera, Sebastián Vargas, Marissa Vazquez, Engelberto Unzain, Gerardo Cañete