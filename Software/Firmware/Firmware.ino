#include <AccelStepper.h>
#include <Servo.h>
#include <math.h>
#include "Definitions.h"

// Definiciones de pines
const byte
    nSteppers = 4,
    limitSwtich[nSteppers] = {11, 10, 9, A3},
    stepper_Step[nSteppers] = {2, 3, 4, 12},
    stepper_Dir[nSteppers] = {5, 6, 7, 13},
    gripperPin = A0;

// Configuraciones
const int
    stepper_maxSpeedVal[nSteppers] = {200, 600, 600, 600},
    stepper_accelVal[nSteppers] = {2000, 2000, 2000, 2000},
    stepper_maxPosition[nSteppers] = {2000, -6500, 0, 17000},
    stepper_startingPosition[nSteppers] = {0, 0, 0, 10000},
    stepper_homingSpeed[nSteppers] = {-2000, -2000, -2000, 2000},

    gripper_maxPosition = 180,
    gripper_min = 600,
    gripper_max = 2500;

const float factor[nSteppers] = {44.444444, 35.555556, 8.888889, 100};
const boolean limitSwitchType[nSteppers] = {NO, NO, NO, NO};

AccelStepper *stepper = new AccelStepper[nSteppers];
Servo gripper;
long targetPosition[nSteppers];
int
    xStepper,
    pose[5][100],
    positionsCounter = 0,
    data[10] = {0, 0, 0, 0, 0, 100, 180, 2000, 2000};
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
    pinMode(limitSwtich[xStepper], limitSwitchType[xStepper] == NO ? INPUT_PULLUP : INPUT);

    // Inicialización objeto stepper
    stepper[xStepper] = AccelStepper(AccelStepper::DRIVER, stepper_Step[xStepper], stepper_Dir[xStepper]);
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
  {
    readSerial();

    // Verificar estado de guardado
    // Si data[0] == 0, nada
    if (data[SAVE] == 1) // Si 1, guardar
    {
      for (xStepper = 0; xStepper < nSteppers; xStepper++)
        pose[xStepper][positionsCounter] = data[xStepper + 2] * factor[xStepper];
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
        targetPosition[xStepper] = pose[xStepper][nPose];
        stepper[xStepper].moveTo(targetPosition[xStepper]);
      }
      runSteppers(targetPosition);

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

  // // Si el estado de ejecución == 0. Mover a la posición especificada desde
  // // Serial
  // for (xStepper = 0; xStepper < nSteppers; xStepper++)
  // {
  //   targetPosition[xStepper] = data[xStepper + 2] * factor[xStepper];
  //   stepper[xStepper].moveTo(targetPosition[xStepper]);
  // }
  // setSpeedAccel();
  // runSteppers(targetPosition);

  targetPosition[0] = data[2] * factor[0];
  targetPosition[1] = data[3] * factor[1];
  targetPosition[2] = data[4] * factor[2];
  targetPosition[3] = data[5] * factor[3];

  stepper[0].setSpeed(data[7]);
  stepper[1].setSpeed(data[7]);
  stepper[2].setSpeed(data[7]);
  stepper[3].setSpeed(data[7]);

  stepper[0].setAcceleration(data[8]);
  stepper[1].setAcceleration(data[8]);
  stepper[2].setAcceleration(data[8]);
  stepper[3].setAcceleration(data[8]);

  stepper[0].moveTo(targetPosition[0]);
  stepper[1].moveTo(targetPosition[1]);
  stepper[2].moveTo(targetPosition[2]);
  stepper[3].moveTo(targetPosition[3]);

  while (stepper[0].currentPosition() != targetPosition[0] ||
         stepper[1].currentPosition() != targetPosition[1] ||
         stepper[2].currentPosition() != targetPosition[2] ||
         stepper[3].currentPosition() != targetPosition[3])
  {
    stepper[0].run();
    stepper[1].run();
    stepper[2].run();
    stepper[3].run();
  }

  delay(100);
  gripper.write(data[DGRIPPER]);
  delay(300);
}

/**
 * Sitúa al robot en su posición incial (Home)
 */
void homing()
{
  for (xStepper = 0; xStepper < nSteppers; xStepper++)
  {
    while (digitalRead(limitSwtich[xStepper]) == HIGH)
    {
      stepper[xStepper].setSpeed(stepper_homingSpeed[xStepper]);
      stepper[xStepper].runSpeed();
      stepper[xStepper].setCurrentPosition(stepper_maxPosition[xStepper]);
    }
    delay(20);
    stepper[xStepper].moveTo(stepper_startingPosition[xStepper]);
    while (stepper[xStepper].currentPosition() != stepper_startingPosition[xStepper])
      stepper[xStepper].run();
  }
}

/**
 * Lee los valores del puerto Serial
 */
void readSerial()
{
  String content = Serial.readString();
  for (int j = 0; j < sizeof(data) / sizeof(data[0]); j++)
  {
    int commaIndex = content.indexOf(",");
    data[j] = atoi(content.substring(0, commaIndex).c_str());
    content = content.substring(commaIndex + 1);
  }
}

/**
 * Configura la velocidad y aceleración de las articulaciones del robot
 */
void setSpeedAccel()
{
  for (xStepper = 0; xStepper < nSteppers; xStepper++)
  {
    stepper[xStepper].setSpeed(data[SPEED]);
    stepper[xStepper].setAcceleration(data[ACCEL]);
  }
}

/**
 * Mueve los steppers hasta la posición indicada
 * @param targetPosition Array de posiciones en pasos de los steppers
 */
void runSteppers(long *targetPosition)
{
  boolean runningFlag = true;
  while (runningFlag)
  {
    runningFlag = false;
    for (xStepper = 0; xStepper < nSteppers; xStepper++)
    {
      if (stepper[xStepper].currentPosition() != targetPosition[xStepper])
      {
        runningFlag = stepper[xStepper].run();
      }
    }
  }
}