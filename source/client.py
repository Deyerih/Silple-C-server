import socket
import threading

HOST = '192.168.0.164'
PORT = 1488

def listen(sock):
    while True:
        try:
            data = sock.recv(1024)
            if not data:
                break
            print("", data.decode())
        except:
            break

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    print("Connected to server\nEnter 'exit' to end connection")

    threading.Thread(target=listen, args=(s,), daemon=True).start()

    while True:
        mess = input()
        if mess.lower() == "exit":
            break
        s.sendall(mess.encode())


