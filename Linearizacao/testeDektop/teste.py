from math import pi
from math import sin

def senoL(num):
        if num >= 0 and num <= 0.5:
            return 0.958800*num + 0.000000
        elif num > 0.5 and num <= 1:
            return 0.724200*num + 0.117300
        elif num > 1 and num <= 1.5:
            return 0.312000*num + 0.529500
        elif num > 1.5 and num <= 2:
            return -0.176400*num + 1.262100
        elif num > 2 and num <= 2.5:
            return -0.621600*num + 2.152500
        elif num > 2.5 and num <= 4:
            return -0.903533*num + 2.857333
        elif num > 4 and num <= 4.5:
            return -0.441400*num + 1.008800
        elif num > 4.5 and num <= 5:
            return 0.037200*num - 1.144900
        elif num > 5 and num <= 5.5:
            return 0.506800*num - 3.492900
        elif num > 5.5 and num <= 6:
            return 0.852200*num - 5.392600
        else:
            return 100000

def cossenoL(num):
        if num >= 0 and num <= 0.5:
            return -0.244800*num + 1.000000
        elif num > 0.5 and num <= 1:
            return -0.674600*num + 1.214900
        elif num > 1 and num <= 2.5:
            return -0.894267X + 1.434567
        elif num > 2.5 and num <= 3:
            return -0.377800X + 0.143400
        elif num > 3 and num <= 3.5:
            return 0.107000X  - 1.311000
        elif num > 3.5 and num <= 4:
            return 0.565800X  - 2.916800
        elif num > 4 and num <= 5.5:
            return 0.908200X  - 4.286400
        elif num > 5.5 and num <= 6:
            return 10.493200X - 61.999000
        else:
            return 100000

    
while(1):
    num = float(input("Insira o valor: "))
    print("Seno: %f e SenoL: %f"%(sin(num), senoL(num)))
