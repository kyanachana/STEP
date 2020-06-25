def foo(b):
    print('b: %#08x' %id(b))
    b.append(2)
    print('b: %#08x' %id(b))
    b = b + [3]
    print('b: %#08x' %id(b))
    b.append(4)
    print('b: %#08x' %id(b))
    print('b:', b)
    print('b0: %#08x' %id(b[0]))
    print('b1: %#08x' %id(b[1]))
    print('b2: %#08x' %id(b[2]))
    print('b3: %#08x' %id(b[3]))
a = [1]

foo(a)
a.append(5)
print('a:', a)
print('a: %#08x' %id(a))


print('a0: %#08x' %id(a[0]))
print('a1: %#08x' %id(a[1]))
print('a2: %#08x' %id(a[2]))