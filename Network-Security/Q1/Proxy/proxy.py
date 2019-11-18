from Crypto.PublicKey import RSA
from Crypto.Cipher import AES
import os
import sys
import socket
import threading
import ast

private_key_Prox = open("./private_key_Prox.pem","r").read()
private_key_Prox = RSA.importKey(private_key_Prox)
 
def pad(s):
	return s + (AES.block_size - len(s) % AES.block_size) * chr(AES.block_size - len(s) % AES.block_size)

def unpad(s):
	return s[:-ord(s[len(s)-1:])]

def server(cfd1, cfd2, aes1, aes2):

	while True:
		data = cfd1.recv(4096)
		data = unpad(aes1.decrypt(data))
		data = aes2.encrypt(pad(data))
		cfd2.send(data)

def main():
	sfd1 = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	servip1 = ('localhost', 6032)
	sfd1.bind(servip1)
	sfd1.listen(100)

	sfd2 = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	servip2 = ('localhost', 6033)
	sfd2.bind(servip2)
	sfd2.listen(100)

	cfd1, _ = sfd1.accept()
	cfd2, _ = sfd2.accept()
	
	sfd1.close()
	sfd2.close()

	key1 = cfd1.recv(4096).decode()
	key1 = private_key_Prox.decrypt(ast.literal_eval(key1))
	aes1 = AES.new(key1, AES.MODE_ECB)

	key2 = cfd2.recv(4096).decode()
	key2 = private_key_Prox.decrypt(ast.literal_eval(key2))
	aes2 = AES.new(key2, AES.MODE_ECB)

	tid1 = threading.Thread(target=server, args=(cfd1, cfd2, aes1, aes2))
	tid2 = threading.Thread(target=server, args=(cfd2, cfd1, aes2, aes1))
	tid1.start()
	tid2.start()
	tid1.join()
	tid2.join()
	cfd1.close()
	cfd2.close()

if __name__=="__main__":
	main()