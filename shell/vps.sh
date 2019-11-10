#apt-get install python-pip python-gevent python-m2crypto
#pip install --upgrade setuptools
#pip install shadowsocks
#mkdir /etc/shadowsocks

echo '{
	"server":"",
    "server_port":8888,
    "password":"",
    "timeout":600,
    "method":"aes-256-cfb",
    "fast_open": false
}' > ./temp

sed -i "s/libcrypto.EVP_CIPHER_CTX_cleanup.argtypes/libcrypto.EVP_CIPHER_CTX_reset.argtypes/g" /usr/local/lib/python2.7/dist-packages/shadowsocks/crypto/openssl.py

ssserver -c /etc/shadowsocks/config.json -d start
