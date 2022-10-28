n=0
while n<=0:
    n=int(input("inserisci un numero"))
a = b = 1
f = 1
for i in range(2,n):
    f = a + b
    
    b = a
    a = f
print(f)
