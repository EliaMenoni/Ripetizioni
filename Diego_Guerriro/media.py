n=int(input("inserisci un numero"))
s=0
a=0
while n!=0:
    if n>0:
        a=a+1
        s=s+n
    n=int(input("inserisci un numero"))
print(s/a)