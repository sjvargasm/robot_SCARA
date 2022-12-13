# Constantes de proporción
Cada [[Calculos de constantes de proporción|constante]] calculada, se almacena en un arreglo `float factor[]` en la sección de configuraciones del código del Firmware del Arduino.
```cpp
const float factor[nSteppers] = {44.444444, 35.555556, 8.8888889, 100}
```
Luego, es utilizado en el movimiento de los steppers durante la ejecución de `loop()`.
```cpp
for (xStepper = 0; xStepper < nSteppers; xStepper++)
{
	stepper_position[xStepper] = data[xStepper + 2] * factor[xStepper];
	stepper[xStepper].moveTo(stepper_position[xStepper]);
}
runSteppers(stepper_position);
```
# Fórmulas de cinemática
Los cálculos de [[Fórmulas de cinemática#Cinemática directa|cinemática directa]] y [[Fórmulas de cinemática#Cinemática inversa|cinemática inversa]] se realizan en la clase `Calculator` de la GUI, con el propósito de reducir el uso de memoria y tiempo de procesamiento del Arduino.
```java
// Código abreviado
public class Calculator {
	public static double[] directKinematics(double[] jValue, double[] L) {
		double[] coord = new double[3];
		coord[0]=L[0]*Math.cos(jValue[0])+L[1]*Math.cos(jValue[0]+jValue[1]);
		coord[1]=L[0]*Math.sin(jValue[0])+L[1]*Math.sin(jValue[0]+jValue[1]);
		coord[2]=jValue[3];
		return coord;
	}
	public static double[] inverseKinematics(double[] coord, double[] L) {
		double[] jValue = new double[4];
		double x=coord[0], y=coord[1], z=coord[2], L1=L[0], L2=L[1];
		
		// theta2 = arccos((x²+y²-L1²-L2²)/(2*L1*L2))
		jValue[1]=Math.acos((Math.pow(x,2)+Math.pow(y,2)-Math.pow(L1,2)-
		Math.pow(L2,2))/(2*L1*L2));
		
		// theta1 = arctan(x/y)-arctan(L2*sin(theta2)/(L1+L2*cos(theta2)))
		jValue[0]=Math.atan(x/y)-Math.atan(L2*Math.sin(jValue[1])/
		(L1+L2*Math.cos(jValue[1])));
		
		// theta3
		jValue[2]=(-1)*(90+jValue[0]+jValue[1]);
		
		// Z
		jValue[3]=z
		return jValue;
	}
}
```
Debido a las implementaciones de [`Math.acos()`](https://docs.oracle.com/javase/8/docs/api/java/lang/Math.html#acos-double-)  y [`Math.atan()`](https://docs.oracle.com/javase/8/docs/api/java/lang/Math.html#atan-double-) en Java, luego de cada cálculo, se debe realizar un ajuste de cuadrantes.