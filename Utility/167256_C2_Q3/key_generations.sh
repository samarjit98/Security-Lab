openssl genrsa -out public_key_client.pem 1024
openssl rsa -in public_key_client.pem -pubout -out ./client/private_key_client.pem

openssl genrsa -out public_key_server.pem 1024
openssl rsa -in public_key_server.pem -pubout -out ./server/private_key_server.pem