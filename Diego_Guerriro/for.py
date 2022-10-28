a=int(input("inserisci un numero"))
b=int(input("inserisci un numero"))
if b>a:
    for i in range(a,b+1):
        if i%3==0:
            print(i)
        else:
            pass
else:
    for i in range(b,a+1):
        if i%3==0:
            print(i)
        else:
            pass