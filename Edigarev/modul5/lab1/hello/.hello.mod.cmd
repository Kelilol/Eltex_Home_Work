savedcmd_/home/vboxuser/workspace/modul5/lab1/hello/hello.mod := printf '%s\n'   hello.o | awk '!x[$$0]++ { print("/home/vboxuser/workspace/modul5/lab1/hello/"$$0) }' > /home/vboxuser/workspace/modul5/lab1/hello/hello.mod