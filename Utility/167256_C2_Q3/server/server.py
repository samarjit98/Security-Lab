from Crypto.PublicKey import RSA
from Crypto.Cipher import AES
import os
import sys
import socket
import threading
import ast

private_key_server = open("./private_key_server.pem","r").read()
private_key_server = RSA.importKey(private_key_server)	

public_key_client = open("../public_key_client.pem","r").read()
public_key_client = RSA.importKey(public_key_client)

def reader(cfd):
	while True:
		data = cfd.recv(4096)
		data = public_key_client.decrypt(ast.literal_eval(data))
		print(data)

def writer(cfd):
	while True:
		data = raw_input()
		data = private_key_server.encrypt(data, 4096)
		cfd.send(str(data))

def main():
	sfd = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	servip = ('localhost', 5007)
	sfd.bind(servip)
	sfd.listen(100)

	while True:
		cfd, _ = sfd.accept()
	
		sfd.close()
		tid1 = threading.Thread(target=reader, args=(cfd,))
		tid2 = threading.Thread(target=writer, args=(cfd,))
		tid1.start()
		tid2.start()
		tid1.join()
		tid2.join()
		cfd.close()

if __name__=="__main__":
	main()