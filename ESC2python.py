import os

def CursorSave():
    print("\0337", end="", flush=True)

def CursorRestore():
    print("\0338", end="", flush=True)

def CursorDown(y):
    print("\x1B[" + str(y) + "B", end="", flush=True)

def CursorRight(x):
    print("\x1B[" + str(x) + "C", end="", flush=True)

def CursorLeft(x):
    print("\x1B[" + str(x) + "D", end="", flush=True)

def CursorUp(y):
    print("\x1B[" + str(y) + "A", end="", flush=True)

def CursorMoveX(x):
    if x > 0:
        CursorRight(x)
    elif x < 0:
        CursorLeft(x * -1)

def CursorMoveY(y):
    if y > 0:
        CursorUp(y)
    elif y < 0:
        CursorDown(y * -1)

def CursorMoveXY(x, y):
    CursorMoveX(x)
    CursorMoveY(y)

def CLS():
    os.system('cls' if os.name == 'nt' else 'clear')    

def TxtBold(set):
    if set:
        print("\x1B[1m", end="", flush=True)
    else:
        print("\x1B[22m", end="", flush=True)

def ResetForeGround():
    print("\x1B[39m", end="", flush=True)

def ResetBackGround():
    print("\x1B[49m", end="", flush=True)

def ResetForeBack():
    ResetForeGround()
    ResetBackGround()

def SetForeGround(c):
    if not ((c > 29 and c < 38) or (c > 89 and c < 98)):
        ResetForeGround()
    else:
        print("\x1B[" + str(c) + "m", end="", flush=True)

def SetBackGround(c):
    if not ((c > 39 and c < 48) or (c > 99 and c < 108)):
        ResetBackGround()
    else:
        print("\x1B[" + str(c) + "m", end="", flush=True)

def SetForeBack(fc, bc):
    SetForeGround(fc)
    SetBackGround(bc)
