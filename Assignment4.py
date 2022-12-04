
f = open('Assignment4.txt', 'r')
lines = f.readlines()
overlap = 0
contained = 0
for line in lines:
    [a,b] = line.split(',')
    [n1,n2] = a.split('-')
    [m1,m2] = b.split('-')

    first = set(range(int(n1), int(n2)+1))
    second = set(range(int(m1), int(m2)+1))
    combination = first | second

    if combination == first or combination == second:
        contained+=1

    if len(combination) < len(first) + len(second):
        overlap+=1

print(contained)
print(overlap)
    