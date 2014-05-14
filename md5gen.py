import hashlib
with open('blacklist.md5', 'w') as f:
	for n in range(0,99999):
		m = hashlib.md5()
		m.update(str(n))
		f.write(m.hexdigest())
		f.write('\n')

with open('samples.md5', 'w') as f:
	for n in range(100000,199999):
		m = hashlib.md5()
		m.update(str(n))
		f.write(m.hexdigest())
		f.write('\n')
