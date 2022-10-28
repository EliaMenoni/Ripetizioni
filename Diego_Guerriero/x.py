x = 11
while x > 10 or x < 0:
    x=int(input("inserisci un numero positivo minore o uguale a 10: "))
y = -1
while y < 0:
    y=int(input("inserisci un numero positivo: "))

n=1
i=1
while x**i<y:
    n=x**i
    i=i+1
print(n)