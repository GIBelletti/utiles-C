def pos_menor(arr,n,base = 0):
	if n == 2:
		if arr[0] > arr[1]:
			return base + 1
		return base
	if n <= 1:
		return base
	medio = (int)(n / 2)
	primero = arr[base]
	ultimo = arr[-1]
	if primero > ultimo:
		if arr[medio] < primero:
			return pos_menor(arr[:medio],medio,base)
		return pos_menor(arr[medio:],medio,medio)
	return 0

def main():
	arrs = [[7,0,3,5],[0,3,5,7],[3,5,7,0]]
	for arr in arrs:
		n = len(arr)
		print(pos_menor(arr,n))
main()
