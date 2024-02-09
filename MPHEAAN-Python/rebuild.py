import os
import sys

sudoPassword = '4afroot' # change to your password

def rebuild_lib():
	os.system('cd HEAAN/lib && make all && cd ../..')

def rebuild_py():
	command = 'rm -r ./build'
	os.system('echo %s|sudo -S %s' % (sudoPassword, command))
	command = 'python3.10 setup.py install'
	os.system('echo %s|sudo -S %s' % (sudoPassword, command))

if __name__ == '__main__':
	if '1' in sys.argv:
		rebuild_lib()
	elif '2' in sys.argv:
		rebuild_lib()
		rebuild_py()
	else:
		rebuild_py()