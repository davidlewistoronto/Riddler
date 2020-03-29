import itertools
import math
import numpy as np
import sympy as sp

#thanks Jerome Kelleher
def partition(n):
	a = [0 for i in range(n + 1)]
	k = 1
	y = n - 1
	while k != 0:
		x = a[k - 1] + 1
		k -= 1
		while 2 * x <= y:
			a[k] = x
			y -= x
			k += 1
		l = k + 1
		while x <= y:
			a[k] = x
			a[l] = y
			yield a[:k + 2]
			x += 1
			y -= 1
		a[k] = x + y
		y = x + y - 1
		yield a[:k + 1]

def smartPerm(startState, endState):
	permutes = []
	toCheck = []
	endStateLen = len(endState)
	for permute in itertools.permutations(range(len(startState)), endStateLen):
		y = [[permute[i], endState[i]] for i in range(endStateLen)]
		y = sorted(y, key=lambda z:z[0])
		if y not in toCheck:
			toCheck.append(y)
			permutes.append(permute)
	return permutes


for N in range(2,15):
	states = [i for i in partition(N)]

	MM = []
	print("\tGenerating MM...")
	for startState in states:
		MMrow = []
		for endState in states:
			endStateLen = len(endState)
			if endStateLen > len(startState):
				MMrow.append(0)
				continue
			
			tally = 0

			for permute in smartPerm(startState, endState):
				permuteTally = 1
				for char in range(len(permute)):
					permuteTally *= startState[permute[char]] ** endState[char]
				tally += permuteTally

			orderings = math.factorial(N) / np.prod([math.factorial(i) for i in endState])
			tally *= orderings

			MMrow.append(int(round(tally)))
		MM.append(MMrow)

	numStates = len(states)

	print("\tMatrix calculations...")
	Q = [i[:-1] for i in MM[:-1]] 
	Q = sp.Matrix(Q) / (N**N)

	Finv = sp.eye(numStates - 1) - Q

	F = Finv**-1

	t = sum(F.row(0))

	print(f'N = {N}:  {t} = {sp.N(t, 5)}\n')