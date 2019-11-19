from Crypto.PublicKey import RSA
from Crypto.Cipher import AES
import os
import sys
import socket
import threading
import ast

key = "This is the secret key 2"

public_key_A = open("../public_key_A.pem","r").read()
public_key_A = RSA.importKey(public_key_A)

private_key_B = open("./private_key_B.pem","r").read()
private_key_B = RSA.importKey(private_key_B)

public_key_Prox = open("../public_key_Prox.pem","r").read()
public_key_Prox = RSA.importKey(public_key_Prox)

aes = AES.new(key, AES.MODE_ECB)

def pad(s):
	return s + (AES.block_size - len(s) % AES.block_size) * chr(AES.block_size - len(s) % AES.block_size)

def unpad(s):
	return s[:-ord(s[len(s)-1:])]

def reader(cfd):
	while True:
		data = cfd.recv(4096)
		data = unpad(aes.decrypt(data))
		data = private_key_B.decrypt(ast.literal_eval(data))
		print(data)

def writer(cfd):
	data = public_key_Prox.encrypt(key, 4096)
	cfd.send(str(data))
	while True:
		data = raw_input()
		data = public_key_A.encrypt(str(data), 4096)
		data = aes.encrypt(pad(str(data)))
		cfd.send(data)

def main():
	sfd = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	servip = ('localhost', 6033)
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