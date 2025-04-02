import subprocess

commands = [
	"scp mm.c root@8.130.147.152:/root/csapp/malloc"
 ]

for command in commands:
    process = subprocess.run(command, shell=True, text=True, capture_output=True)

