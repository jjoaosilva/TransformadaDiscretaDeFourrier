##
##
##y1 = ax1 + b -> b = y1 - ax1    : b = y1 - ((y2 - b)/x2)*x1                                                ==> '''b = y1 - ax1'''
##y2 = ax2 + b -> a = (y2 - b)/x2 : a = (y2 - y1 - ax1)/x2 -->  ax2 + ax1 = y2 - y1 --> a(x2 + x1) = y2 - y1 ==> '''a = (y2 - y1)/(x2 + x1)'''

while(True):
    x1 = 0
    y1 = 0
    x2 = 0
    y2 = 0
    a  = 0
    b  = 0
    x1 = float(input("Insira o valor X1: "))
    y1 = float(input("Insira o valor Y1: "))
    x2 = float(input("Insira o valor X2: "))
    y2 = float(input("Insira o valor Y2: "))
    a = (y2 - y1)/(x2 - x1)
    b = y1 - a*x1
    print("Y = %fX + %f" %(a, b))
    
