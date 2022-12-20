public class Calculator {
    /**
     * Calcula la cinemática directa del robot SCARA.
     * @param jValue Arreglo con valores de las articulaciones
     * @param L Arreglo con las longitudes de cada eslabón
     * @return Un arreglo con los valores cartesianos de la posición del efector final
     */
    public static double[] directKinematics(double[] jValue, double[] L) {
        double[] coord = new double[3];

        // Hexadecimal a Radián
        jValue[0] = jValue[0] * Math.PI / 180;
        jValue[1] = jValue[1] * Math.PI / 180;

        // x = L1 * cos(j1) + L2 * cos(j1 + j2)
        coord[0] = L[0] * Math.cos(jValue[0]) + L[1] * Math.cos(jValue[0] + jValue[1]);
        coord[1] = L[0] * Math.sin(jValue[0]) + L[1] * Math.sin(jValue[0] + jValue[1]);
        coord[2] = jValue[3];
        return coord;
    }

    /**
     * Calcula la cinemática inversa del robot SCARA
     * @param coord Arreglo con los valores cartesianos del efector final
     * @param L Arreglo con las longitudes de los eslabones del robot
     * @return Un arreglo con los valores de los ángulos de los eslabones
     */
    public static double[] inverseKinematics(double[] coord, double[] L) {
        double[] jValue = new double[4];
        double x = coord[0], y = coord[1], z = coord[2], L1 = L[0], L2 = L[1];

        jValue[1] = Math.acos((Math.pow(x, 2) + Math.pow(y, 2) - Math.pow(L1, 2) - Math.pow(L2, 2)) / (2 * L1 * L2));

        if (x < 0 && y < 0) {
            jValue[1] = (-1) * jValue[1];
        }

        // theta1 = arctan(x / y) - arctan(L2 * sen(theta2) / (L1 + L2 * cos(theta2)));
        jValue[0] = Math.atan(x / y) - Math.atan(L2 * Math.sin(jValue[1]) / (L1 + L2 * Math.cos(jValue[1])));

        jValue[1] = (-1) * jValue[1] * 180 / Math.PI;
        jValue[0] = jValue[0] * 180 / Math.PI;

        if (x >= 0 & y >= 0) {
            jValue[0] = 90 - jValue[0];
        }

        if (x < 0 && y > 0) {
            jValue[0] = 90 - jValue[0];
        }

        if (x < 0 && y < 0) {
            jValue[0] = 270 - jValue[0];
            jValue[2] = (-1) * (270 - jValue[0] - jValue[1]);
        }

        if (x > 0 && y < 0) {
            jValue[0] = -90 - jValue[0];
        }

        if (x < 0 && y == 0) {
            jValue[0] = 270 + jValue[0];
        }

        jValue[2] = (-1) * (90 + jValue[0] + jValue[1]);

        if (x < 0 && y < 0) {
            jValue[2] = 270 - jValue[0] - jValue[1];
        }

        if (Math.abs(jValue[2]) > 165) {
            jValue[2] = 180 + jValue[2];
        }

        jValue[3] = z;
        return jValue;
    }
}
