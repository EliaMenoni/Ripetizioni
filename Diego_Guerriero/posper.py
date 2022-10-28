n=0
while n<=0:
    n=int(input("inserisci un numero"))
p=0
for i in range (0,n):
    x=int(input("inserisci un numero"))
    if x>=0:
        p=p+1
print(p)
print(str(100/n*(n-p))+"%")

