# TODO
num = int(input("Enter the card number: "))
check = False
card = ""
counter = 1
sum2 = 0
sumother = 0
sumall = 0

i = num
while i > 0:
    if counter % 2 == 0:
        prod = (i % 10) * 2
        j = prod
        while j > 0:
            sum2 += j % 10
            j //= 10
    else:
        sumother += i % 10
    i //= 10
    counter += 1

sumall = sum2 + sumother

if sumall % 10 == 0:
    check = True

if check:
    if (int)(num // pow(10, 13)) == 34 or (int)(num // pow(10, 13)) == 37:
        card = "AMEX"
    elif (
        (int)(num // pow(10, 14)) == 51
        or (int)(num // pow(10, 14)) == 52
        or (int)(num // pow(10, 14)) == 53
        or (int)(num / pow(10, 14)) == 54
        or (int)(num / pow(10, 14)) == 55
    ):
        card = "MASTERCARD"
    elif (int)(num // pow(10, 12)) == 4 or (int)(num // pow(10, 15)) == 4:
        card = "VISA"
    else:
        card = "INVALID"
else:
    card = "INVALID"

print(f"{card}")
