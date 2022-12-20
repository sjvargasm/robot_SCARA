from roboticstoolbox import DHRobot, PrismaticDH, RevoluteDH
from numpy import pi

scaling = 1000 # Necesario para poder observar los ejes.
L1 = 228/scaling
L2 = 136.5/scaling

# Posición articular del robot
j1 = 150 # mm
j2 = 30 # deg
j3 = 60 # deg
j4 = 60 # deg

SCARA = DHRobot(
    [
        PrismaticDH(0, 0, 0, qlim=[0, 150/scaling]),
        RevoluteDH(0, L1, 0, qlim=[-90*pi/180, 266*pi/180]),
        RevoluteDH(0, L2, 0, qlim=[-150*pi/180, 150*pi/180]),
        RevoluteDH(0, 0, 0, qlim=[-162*pi/180, 162*pi/180])
    ],
    name = "Robot SCARA",
    manufacturer = "Grupo 2"
)

print(SCARA)
SCARA.plot([j1/scaling, j2*pi/180, j3*pi/180, j4*pi/180], block=True)

exit()