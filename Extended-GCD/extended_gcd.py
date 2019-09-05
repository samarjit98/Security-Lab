def extended_gcd(a, b):
	if(a==0):
		return b, 0, 1	
	else:
		gcd, x, y = extended_gcd(b%a, a)
		return gcd, y-(b//a)*x, x

a, b = input().split(" ")
a, b = int(a), int(b)
gcd, x, y = extended_gcd(a, b)
print(gcd, x, y)
