import os

# os.system('cd LHH/lib')
# os.system('cd LHH/lib && make all')
# os.system('cd ../..')

sudoPassword = '4afroot'
command = 'rm -r ./build'
os.system('echo %s|sudo -S %s' % (sudoPassword, command))
command = 'python3.10 setup.py install'
os.system('echo %s|sudo -S %s' % (sudoPassword, command))