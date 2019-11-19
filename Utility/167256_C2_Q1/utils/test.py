from Crypto.PublicKey import RSA

public_key_string = open("public_key.pem","r").read()
public_key = RSA.importKey(public_key_string)

private_key_string = open("private_key.pem","r").read()
private_key = RSA.importKey(private_key_string)

message = "The quick brown fox jumps over the lazy dog."

#Encrypt with public key
encrypted = public_key.encrypt(message, 32)

#Decrypt with private key
decrypted = private_key.decrypt(encrypted)

print(decrypted)