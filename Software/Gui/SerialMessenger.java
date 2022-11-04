import com.fazecast.jSerialComm.SerialPort;

public class SerialMessenger {
    private static String datos;

    public static void send(int[] dato, String portDesciptor, boolean testMode) {
        datos = "";
        for (int i = 0; i < dato.length; i++) {
            datos += dato[i];
            if (i < dato.length - 1) {
                datos += ",";
            }
        }

        datos += "\n";

        if (!testMode) {
            try {
                byte[] b = datos.getBytes();
                SerialPort comPort = SerialPort.getCommPort(portDesciptor);
                comPort.setBaudRate(115200);
                comPort.setComPortTimeouts(SerialPort.TIMEOUT_WRITE_BLOCKING, 100, 0);
                comPort.clearDTR();
                comPort.openPort();
                comPort.writeBytes(b, b.length);
                comPort.closePort();
            } catch (Exception e) {
                System.out.println(e.getMessage());
                System.out.println(e.getStackTrace());
            }
        } else {
            System.out.println(datos);
        }
    }
}
