from Crypto.PublicKey import RSA
from Crypto.Cipher import AES
import os
import sys
import socket
import threading
import ast

public_key_server = open("../public_key_server.pem","r").read()
public_key_server = RSA.importKey(public_key_server)

private_key_client = open("./private_key_client.pem","r").read()
private_key_client = RSA.importKey(private_key_client)

def reader(cfd):
	while True:
		data = cfd.recv(4096)
		data = public_key_server.decrypt(ast.literal_eval(data))
		print(data)

def writer(cfd):
	while True:
		data = raw_input()
		data = private_key_client.encrypt(data, 4096)
		cfd.send(str(data))

def main():
	sfd = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	servip = ('localhost', 5007)
	sfd.connect(servip)

	tid1 = threading.Thread(target=reader, args=(sfd,))
	tid2 = threading.Thread(target=writer, args=(sfd,))
	tid1.start()
	tid2.start()
	tid1.join()
	tid2.join()

	sfd.close()

if __name__=="__main__":
	main()