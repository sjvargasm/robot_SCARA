import javafx.event.Event;
import javafx.fxml.FXML;
import javafx.scene.control.Button;
import javafx.scene.control.Control;
import javafx.scene.control.Slider;
import javafx.scene.control.TextField;
import javafx.scene.input.KeyCode;
import javafx.scene.input.KeyEvent;

class SliderControl {
    public Slider slider;
    public TextField valueTextField, stepTextField;
    public Button stepUpButton, stepDownButton;

    SliderControl(Slider slider, TextField value, TextField step, Button stepUp, Button stepDown) {
        this.slider = slider;
        this.valueTextField = value;
        this.stepTextField = step;
        this.stepUpButton = stepUp;
        this.stepDownButton = stepDown;
        valueTextField.setText(Math.round(slider.getValue()) + "");
    }

    public void syncTextAndValue(Class<?> whoChanged) {
        String className = whoChanged.getName();
        if (className.contains("TextField")) {
            this.slider.setValue(Double.valueOf(this.valueTextField.getText()));
        } else if (className.contains("Slider")) {
            this.valueTextField.setText(String.valueOf(this.slider.getValue()));
        }
    }
}

public class Controlador {
    @FXML
    public Slider j1Slider, j2Slider, j3Slider, zSlider, gripperSlider, velSlider, accelSlider;

    @FXML
    public TextField j1Value, j2Value, j3Value, zValue, gripperValue, velValue, accelValue;

    @FXML
    public TextField j1Step, j2Step, j3Step, zStep, gripperStep, velStep, accelStep;

    @FXML
    public Button j1Up, j2Up, j3Up, zUp, gripperUp, velUp, accelUp;

    @FXML
    public Button j1Down, j2Down, j3Down, zDown, gripperDown, velDown, accelDown;

    @FXML
    public Button saveButton, startButton, eraseButton, moveButton, setPortButton;

    @FXML
    public TextField xCoord, yCoord, zCoord, portTextField;

    private double[] dimensiones = { 228, 136.5 };
    private SliderControl[] sliders = new SliderControl[7];
    private static int saveStatus = 0, executionStatus = 0;
    private String portDescriptor;

    @FXML
    public void initialize() {
        sliders[0] = new SliderControl(j1Slider, j1Value, j1Step, j1Up, j1Down);
        sliders[1] = new SliderControl(j2Slider, j2Value, j2Step, j2Up, j2Down);
        sliders[2] = new SliderControl(j3Slider, j3Value, j3Step, j3Up, j3Down);
        sliders[3] = new SliderControl(zSlider, zValue, zStep, zUp, zDown);
        sliders[4] = new SliderControl(gripperSlider, gripperValue, gripperStep, gripperUp, gripperDown);
        sliders[5] = new SliderControl(velSlider, velValue, velStep, velUp, velDown);
        sliders[6] = new SliderControl(accelSlider, accelValue, accelStep, accelUp, accelDown);
        setPort();
        updateCoords();
        sendData(0, 0);
    }

    @FXML
    public void sendData(Event event) {
        sendData(0, 0);
    }

    @FXML
    public void automaticControl(Event event) {
        String sourceId = ((Button) event.getSource()).getId();
        switch (sourceId) {
            case "moveButton":
                saveStatus = 0;
                executionStatus = 0;
                updateSliders();
                break;
            case "saveButton":
                saveStatus = 1;
                executionStatus = 0;
                break;
            case "startButton":
                saveStatus = 0;
                switch (executionStatus) {
                    case 0:
                    case 2:
                        executionStatus = 1;
                        startButton.setText("Parar");
                        break;
                    case 1:
                        executionStatus = 0;
                        startButton.setText("Ejecutar");
                        break;
                }
                break;
            case "eraseButton":
                saveStatus = 0;
                executionStatus = 2;
                startButton.setText("Ejecutar");
                break;
        }
        sendData(saveStatus, executionStatus);
    }

    // Actualizar dinámicamente el valor del campo luego de mover el slider
    @FXML
    public void updateTextFieldValue(Event event) {
        SliderControl sliderController = getController((Slider) event.getSource());
        sliderController.syncTextAndValue(event.getSource().getClass());
        if (!sliderController.slider.getId().contains("gripper") && !sliderController.slider.getId().contains("vel")
                && !sliderController.slider.getId().contains("acc"))
            updateCoords();
    }

    // Actualizar el valor del slider luego de ingresar un valor por el campo
    @FXML
    public void updateSliderValue(KeyEvent event) {
        if (event.getCode() == KeyCode.ENTER) {
            SliderControl sliderController = getController((TextField) event.getSource());
            sliderController.syncTextAndValue(event.getSource().getClass());
            if (!sliderController.slider.getId().contains("gripper") && !sliderController.slider.getId().contains("vel")
                    && !sliderController.slider.getId().contains("acc"))
                updateCoords();
            sendData(0, 0);
        }
    }

    // Mover el slider por paso y actualizar el campo de valor
    @FXML
    public void stepSliderValue(Event event) {
        Button source = (Button) event.getSource();
        SliderControl sliderController = getController(source);
        if (source.getId().contains("Up")) {
            sliderController.slider.setValue(
                    sliderController.slider.getValue() + Double.valueOf(sliderController.stepTextField.getText()));
        } else if (source.getId().contains("Down")) {
            sliderController.slider.setValue(
                    sliderController.slider.getValue() - Double.valueOf(sliderController.stepTextField.getText()));
        }
        sliderController.syncTextAndValue(sliderController.slider.getClass());
        if (!sliderController.slider.getId().contains("gripper") && !sliderController.slider.getId().contains("vel")
                && !sliderController.slider.getId().contains("acc"))
            updateCoords();
        sendData(0, 0);
    }

    /**
     * 
     */
    @FXML
    public void setPort() {
        portDescriptor = portTextField.getText();
        sendData(0, 0);
    }

    @FXML
    public void setPortOnKey(KeyEvent event) {
        if(event.getCode() == KeyCode.ENTER) {
            setPort();
        }
    }

    private void sendData(int save, int execution) {
        double[] datos = {
                save,
                execution,
                Math.round(j1Slider.getValue()),
                Math.round(j2Slider.getValue()),
                Math.round(j3Slider.getValue()),
                Math.round(zSlider.getValue()),
                Math.round(gripperSlider.getValue()),
                Math.round(velSlider.getValue()),
                Math.round(accelSlider.getValue())
        };
        SerialMessenger.send(datos, portDescriptor, false);
    }

    private SliderControl getController(Control controller) {
        SliderControl sliderReturn = new SliderControl(new Slider(), new TextField(),
                new TextField(), new Button(),
                new Button());
        String sliderId, valueId, stepUpId, stepDownId, controllerId = controller.getId();

        for (SliderControl slider : sliders) {
            sliderId = slider.slider.getId();
            valueId = slider.valueTextField.getId();
            stepUpId = slider.stepUpButton.getId();
            stepDownId = slider.stepDownButton.getId();
            if (controllerId == sliderId ||
                    controllerId == valueId ||
                    controllerId == stepUpId ||
                    controllerId == stepDownId)
                sliderReturn = slider;
        }
        return sliderReturn;
    }

    private void updateSliders() {
        double[] joints = Calculator.inverseKinematics(
                new double[] {
                        Double.valueOf(xCoord.getText()),
                        Double.valueOf(yCoord.getText()),
                        Double.valueOf(zCoord.getText()) },
                dimensiones);
        for (int i = 0; i < 4; i++) {
            sliders[i].slider.setValue(joints[i]);
            sliders[i].syncTextAndValue(sliders[i].getClass());
        }
    }

    private void updateCoords() {
        double[] coords = Calculator.directKinematics(
                new double[] {
                        sliders[0].slider.getValue(),
                        sliders[1].slider.getValue(),
                        sliders[2].slider.getValue(),
                        sliders[3].slider.getValue()
                },
                dimensiones);
        xCoord.setText(String.valueOf(coords[0]));
        yCoord.setText(String.valueOf(coords[1]));
        zCoord.setText(String.valueOf(coords[2]));
    }
}
