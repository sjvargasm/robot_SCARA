package org.svm.scaraKinematicsCalculator;

public class Calculator {
    public static double[] directKinematics(double[] jValue, double[] L) {
        double[] coord = new double[3];

        // Hexadecimal a Radián
        jValue[0] = jValue[0] * Math.PI / 180;
        jValue[1] = jValue[1] * Math.PI / 180;

        // x = L1 * cos(j1) + L2 * cos(j1 + j2)
        coord[0] = L[0] * Math.cos(jValue[0]) + L[1] * Math.cos(jValue[0] + jValue[1]);
        coord[1] = L[0] * Math.sin(jValue[1]) + L[1] * Math.sin(jValue[0] + jValue[1]);
        coord[2] = jValue[3];
        return coord;
    }


    // FIXME: What is happening here
    public static double[] inverseKinematics(double[] coord, double[] L) {
        double[] jValue = new double[4];

        jValue[1] = Math.acos((Math.pow(coord[0], 2) + Math.pow(coord[1], 2) - Math.pow(L[0], 2) - Math.pow(L[1], 2))
                / (2 * L[0] * L[1]));
        jValue[1] = jValue[1] * 180 / Math.PI;

        jValue[0] = Math.atan(coord[0] / coord[1])
                - Math.atan((L[1] * Math.sin(jValue[1])) / (L[0] + L[1] * Math.cos(jValue[1])));
        jValue[0] = jValue[0] * 180 / Math.PI;
        
        if (coord[0] >= 0 & coord[1] >= 0) {
            jValue[0] = 90 - jValue[0];
        }

        if (coord[0] < 0 && coord[1] > 0) {
            jValue[0] = 90 - jValue[0];
        }
        
        if (coord[0] < 0 && coord[1] < 0) {
            jValue[0] = 270 - jValue[0];
            jValue[2] = (-1) * (270 - jValue[0] - jValue[1]);
        }

        if (coord[0] > 0 && coord[1] < 0) {
            jValue[0] = -90 - jValue[0];
        }

        if(coord[0] < 0 && coord[1] == 0 ) {
            jValue[0] = 270 + jValue[0];
        }

        jValue[2] = (-1) * (90 + jValue[0] + jValue[1]);

        if (coord[0] < 0 && coord[1] < 0) {
            jValue[2] = 270 - jValue[0] - jValue[1];
        }

        if(Math.abs(jValue[2]) > 165) {
            jValue[2] = 180 + jValue[2];
        }

        jValue[3] = coord[2];
        return jValue;
    }
}
