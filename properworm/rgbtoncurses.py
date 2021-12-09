def f(x):
    return round((x/255)*1000)

rgb = input("rgb: #")

r = f(int(rgb[0:2], 16))
g = f(int(rgb[2:4], 16))
b = f(int(rgb[4:6], 16))

print ("%3d, %3d, %3d" % (r, g, b))

