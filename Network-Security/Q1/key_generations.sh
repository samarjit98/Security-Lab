openssl genrsa -out ./Proxy/private_key_Prox.pem 1024
openssl rsa -in ./Proxy/private_key_Prox.pem -pubout -out public_key_Prox.pem

openssl genrsa -out ./User-A/private_key_A.pem 1024
openssl rsa -in ./User-A/private_key_A.pem -pubout -out public_key_A.pem

openssl genrsa -out ./User-B/private_key_B.pem 1024
openssl rsa -in ./User-B/private_key_B.pem -pubout -out public_key_B.pem