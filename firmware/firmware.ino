#include <AccelStepper.h>
#include <Servo.h>
#include <math.h>
#include "definitions.h"

const byte
    // Número de steppers
    nSteppers = 4,

    // Pines finales de carrera
    limitSwtich[nSteppers] = {11, 10, 9, A3},

    // Pines de paso
    stepper_Step[nSteppers] = {2, 3, 4, 12},

    // Pines de dirección
    stepper_Dir[nSteppers] = {5, 6, 7, 13},

    // Pin del gripper
    gripperPin = A0;

const int
    // Valores de velocidad máxima
    stepper_maxSpeedVal[nSteppers] = {200, 600, 600, 600},

    // Valores de aceleración
    stepper_accelVal[nSteppers] = {2000, 2000, 2000, 2000},

    // Posiciones máximas de las articulaciones
    stepper_maxPosition[nSteppers] = {17000, -1662, -5420, -3955},

    // Posiciones iniciales de las articulaciones
    stepper_startingPosition[nSteppers] = {10000, 0, 0, 0},

    // Velocidades de homing de steppers
    stepper_homingSpeed[nSteppers] = {1500, -1100, -1300, -1200},

    // Valor máximo de apertura de gripper
    gripper_maxPosition = 180,

    // PWM mínimo para gripper
    gripper_min = 600,

    // PWM máximo para gripper
    gripper_max = 2500;

// Factores de conversión de ángulos a pasos de stepper
const float factor[nSteppers] = {44.444444, 35.555555, 10, 100};

// Tipos de finales de carrera
const boolean limitSwitchType[nSteppers] = {NO, NO, NO, NO};

// Array de objetos steppers
AccelStepper *stepper = new AccelStepper[nSteppers];

// Objeto gripper
Servo gripper;

int
    // Posicion del stepper
    stepper_position[nSteppers],

    // Índice de stepper
    xStepper,

    // Tabla de poses
    pose[5][100],

    // Contador de poses
    positionsCounter = 0,

    // Datos procesados del puerto serial
    data[10];
/**
 * data[0] = Estado del botón GUARDAR
 * data[1] = Estado del boton EJECUTAR
 * data[2] = Ángulo de la articulación 1 (theta1)
 * data[3] = Ángulo de la articulación 2 (tehta2)
 * data[4] = Ángulo de la articulación 3 (phi)
 * data[5] = Posición Z
 * data[6] = Apertura del Gripper
 * data[7] = Velocidad
 * data[8] = Aceleración
 * data[9] = '\0'
 */

void setup()
{
  Serial.begin(115200);

  // Inicializar cada stepper
  for (xStepper = 0; xStepper < nSteppers; xStepper++)
  {
    // Fin de carrera asociado
    pinMode(limitSwtich[xStepper],
            limitSwitchType[xStepper] == NO ? INPUT_PULLUP : INPUT);

    // Inicialización objeto stepper
    stepper[xStepper] = AccelStepper(AccelStepper::DRIVER,
                                     stepper_Step[xStepper],
                                     stepper_Dir[xStepper]);
    stepper[xStepper].setMaxSpeed(stepper_maxSpeedVal[xStepper]);
    stepper[xStepper].setAcceleration(stepper_accelVal[xStepper]);
  }

  // Anclar servo del gripper y abrir gripper
  gripper.attach(gripperPin, gripper_min, gripper_max);
  gripper.write(gripper_maxPosition);
  homing();
}

void loop()
{
  // Al inicial un ciclo, leer puerto Serial, si hay algún dato nuevo
  if (Serial.available())
    readSerial();

  // Verificar estado de guardado
  // Si data[0] == 0, nada
  if (data[SAVE] == 1) // Si 1, guardar
  {
    for (xStepper = 0; xStepper < nSteppers; xStepper++)
      pose[xStepper][positionsCounter] = data[xStepper + 2] *
                                         factor[xStepper];
    pose[GRIPPER][positionsCounter] = data[DGRIPPER];
    positionsCounter++;
  }
  else if (data[SAVE] == 2) // Si 2, borrar
  {
    for (xStepper = 0; xStepper < nSteppers; xStepper++)
      memset(pose[xStepper], 0, sizeof(pose[xStepper]));
    memset(pose[GRIPPER], 0, sizeof(pose[GRIPPER]));
    positionsCounter = 0;
  }

  // Verificar estado de ejecución
  while (data[RUN] == 1)
  {
    setSpeedAccel();

    // Ejecutar poses guardadas
    for (int nPose = 0; nPose < positionsCounter; nPose++)
    {
      if (data[RUN] == 0)
        break;

      // 1. Mover steppers
      for (xStepper = 0; xStepper < nSteppers; xStepper++)
      {
        stepper_position[xStepper] = pose[xStepper][nPose];
        stepper[xStepper].moveTo(stepper_position[xStepper]);
      }
      runSteppers(stepper_position);

      // 2. Abrir gripper (Solo si su valor cambió)
      if (nPose == 0)
        gripper.write(pose[GRIPPER][nPose]);
      else if (pose[GRIPPER][nPose] != pose[GRIPPER][nPose - 1])
      {
        gripper.write(pose[GRIPPER][nPose]);
        delay(800);
      }

      // 3. Verificar cambios en velocidad, aceleración o ejecución
      if (Serial.available())
      {
        readSerial();
        if (data[RUN] == 0)
          break;
        setSpeedAccel();
      }
    }
  }

  // Si el estado de ejecución == 0. Mover a la posición especificada desde
  // Serial
  for (xStepper = 0; xStepper < nSteppers; xStepper++)
  {
    stepper_position[xStepper] = data[xStepper + 2] * factor[xStepper];
    stepper[xStepper].moveTo(stepper_position[xStepper]);
  }
  setSpeedAccel();
  runSteppers(stepper_position);
  delay(100);
  gripper.write(data[DGRIPPER]);
  delay(300);
}

void homing()
{
  for (xStepper = 0; xStepper < nSteppers; xStepper++)
  {
    while (digitalRead(limitSwtich[xStepper] == HIGH))
    {
      stepper[xStepper].setSpeed(stepper_homingSpeed[xStepper]);
      stepper[xStepper].runSpeed();
      stepper[xStepper].setCurrentPosition(stepper_maxPosition[xStepper]);
    }
    delay(20);
    stepper[xStepper].moveTo(stepper_startingPosition[xStepper]);
    while (stepper[xStepper].currentPosition() !=
           stepper_startingPosition[xStepper])
      stepper[xStepper].run();
  }
}

void readSerial()
{
  String content = Serial.readString();
  for (int j = 0; j < sizeof(data) / sizeof(data[0]); j++)
  {
    int commaIndex = content.indexOf(",");
    data[j] = atol(content.substring(0, commaIndex).c_str());
    content = content.substring(commaIndex + 1);
  }
}

void setSpeedAccel()
{
  for (xStepper = 0; xStepper < nSteppers; xStepper++)
  {
    stepper[xStepper].setSpeed(data[SPEED]);
    stepper[xStepper].setAcceleration(data[ACCEL]);
  }
}

void runSteppers(int *target)
{
  boolean runningFlag = true;
  while (runningFlag)
  {
    runningFlag = false;
    for (xStepper = 0; xStepper < nSteppers; xStepper++)
    {
      if (stepper[xStepper].currentPosition() != target[xStepper])
      {
        stepper[xStepper].run();
        runningFlag = true;
      }
    }
  }
}