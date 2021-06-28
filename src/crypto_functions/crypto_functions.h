#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rand.h>
#include <openssl/err.h>
#include <unistd.h>
#include <openssl/bio.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <openssl/x509.h>
#include <openssl/x509_vfy.h>

#include "./../error.h"
#include "./../macro.h"

using namespace std;


/* Function used to encrypt a plaintext with a symmetric cipher that ensures confidentiality and authenticity
 * param cipher -> symmetric cipher (AES_GCM)
 * param pt -> plaintext
 * param pt_len -> size of the plaintext
 * param key -> symmetric key
 * param iv -> initialization vector
 * param aad -> additional authenticated data
 * param aad_len -> size of the aad
 * param ct -> ciphertext that will be computed by the cipher
 * param tag_len -> size of the tag
 * param tag -> tag computed by the cipher
 * return -> ciphertext length or -10 in case of OpenSSL API errors
 */
int sym_auth_encr(const EVP_CIPHER* cipher, unsigned char* pt, int pt_len, unsigned char* key, unsigned char* iv, unsigned char* aad, int aad_len, unsigned char* ct, int tag_len, unsigned char* tag);


/* Function used to decrypt a ciphertext with a symmetric cipher that ensures confidentiality and authenticity
 * param cipher -> symmetric cipher (AES_GCM)
 * param ct -> ciphertext
 * param ct_len -> size of the ciphertext
 * param key -> symmetric key
 * param iv -> initialization vector
 * param aad -> additional authenticated data
 * param aad_len -> size of the aad
 * param pt -> plaintext that will be computed by the cipher
 * param tag_len -> size of the tag
 * param tag -> tag received with the ciphertext
 * return -> 1 in case of right decryption, -1 in case of tag mismatch and -10 in case of OpenSSL API errors
 */
int sym_auth_decr(const EVP_CIPHER* cipher, unsigned char *ct, int ct_len, unsigned char *key, unsigned char *iv, unsigned char* aad, int aad_len, unsigned char *pt, int tag_len, unsigned char* tag);


/* Function used to encrypt a plaintext with digital envelope
 * param cipher -> symmetric cipher (AES_CBC)
 * param public_key -> public key used to encrypt the symmetric key
 * param pt -> plaintext
 * param pt_len -> size of the plaintext
 * param encrypted_sym_key -> symmetric key generated from the digital envelope
 * paramencrypted_sym_key_len -> size of the symmetric key of the digital envelope
 * param iv -> initialization vector
 * param ct -> ciphertext that will be computed by the digital envelope
 * return -> ciphertext length or -10 in case of OpenSSL API errors
 */
int dig_env_encr(const EVP_CIPHER* cipher, EVP_PKEY* public_key, unsigned char* pt, int pt_len, unsigned char* encrypted_sym_key, int encrypted_sym_key_len, unsigned char* iv, unsigned char* ct);


/* Function used to decrypt a plaintext with digital envelope
 * param cipher -> symmetric cipher (AES_CBC)
 * param private_key -> private key used to decrypt the symmetric key
 * param ct -> ciphertext
 * param ct_len -> size of the ciphertext
 * param encrypted_sym_key -> symmetric key generated from the digital envelope
 * paramencrypted_sym_key_len -> size of the symmetric key of the digital envelope
 * param iv -> initialization vector
 * param pt -> plaintext that will be computed by the digital envelope
 * return -> plaintext length or -10 in case of OpenSSL API errors
 */
int dig_env_decr(const EVP_CIPHER* cipher, EVP_PKEY* private_key, unsigned char* ct, int ct_len, unsigned char* encrypted_sym_key, int encrypted_sym_key_len, unsigned char* iv, unsigned char* pt);


/* Function used to generate a signature for a given plaintext
 * param md -> algorithm for digital signature (e.g. SHA 256)
 * param private_key -> private key used to generate the signature
 * param pt -> plaintext
 * param pt_len -> size of the plaintext
 * param sign -> signature that will be generated by the algorithm
 * return -> signature length or -10 in case of OpenSSL API errors
 */
int dig_sign_sgn(const EVP_MD* md, EVP_PKEY* private_key, unsigned char* pt, int pt_len, unsigned char* sign);


/* Function used to verify a signature for a given plaintext
 * param md -> algorithm for digital signature (e.g. SHA 256)
 * param public_key -> public key used to verify the signature
 * param sign -> signature
 * param sign_size -> size of the signature
 * param pt -> plaintext
 * param pt_len -> size of the plaintext
 * return -> 0 for invalid signature, 1 for success or -10 in case of OpenSSL API errors
 */
int dig_sign_verif(const EVP_MD* md, EVP_PKEY* public_key, unsigned char* sign, int sign_size, unsigned char* pt, int pt_len);


/* Function used to verify a certificate
 * param CA_cert_filepath -> path of the CA's certificate
 * param CA_CRL_filepath -> path of the CA's CRL
 * param server_cert -> certificate to verify
 * return -> -1 for invalid certificate, 1 for success or -10 in case of OpenSSL API errors
 */
 int cert_verification(string CA_cert_filepath, string CA_CRL_filepath, X509* server_cert);


/* Function used to generate a certain amount of random bytes
 * param len -> number of bytes to generate
 * return -> buffer that contains random bytes or NULL in case of errors
 */
unsigned char* generate_random_bytes(int len);
