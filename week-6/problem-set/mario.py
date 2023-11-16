# TODO
n = 0
while True:
    try:
        n = int(input("Height: "))
    except ValueError:
        continue
    if n >= 1 and n <= 8:
        break
for i in range(n):
    for j in range(n - i - 1):
        print(end=" ")
    for j in range(i + 1):
        print("#", end="")
    print(end="  ")
    for j in range(i + 1):
        print("#", end="")
    print()
