import subprocess

loop = 50
for i in range(loop):
    cmd = f'cargo run --release --bin tester ..\\a.exe < .\\in\\{i:04}.txt > ..\\out\\{i:04}_latest.txt'
    print(f'{i:04}')
    # print(cmd)
    # returnCode = subprocess.Popen(cmd, shell=True)

