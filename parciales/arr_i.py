def existe_i(arr,n,base = 0):
	if n <= 1:
		return None
	if arr[0] > base:
		return -1
	medio = (int)(n / 2)
	if arr[medio] > medio:
		val = existe_i(arr[:medio],medio)
		if not val == None:
			return val
	elif arr[medio] == medio:
		return medio
	elif arr[medio] < medio:
		val = existe_i(arr[medio:],medio,medio)
		if not val == None:
			return val
	return -1

def menu():
	arr = [-2,0,2,21]
	n = len(arr)
	print(existe_i(arr,n))
menu()
