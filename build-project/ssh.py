import paramiko

ssh=paramiko.SSHClient()
ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())

ssh.connect('223.2.45.185', port = 22, username = "ubuntu", password = "hiwonder")

shell=ssh.invoke_shell()
shell.settimeout(1)

command=input()+"\n"
shell.send(command)
while True:
    try:
        recv=shell.recv(512).decode()
        if recv:
            print(recv)
        else:
            continue
    except:
        command=input()+"\n"
        shell.send(command)
ssh.close()
   
