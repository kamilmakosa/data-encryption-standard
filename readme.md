# Data Encryption Standard

The Data Encryption Standard is a symmetric-key algorithm for the encryption of digital data

### Compilation

```sh
gcc des.cpp -o des
```

### Running

Encryption

```sh
./des -m "<<plaintext>>" -k "<<64-bit key in hex format>>"
```

Decryption

```sh
./des -c "<<ciphertext>>" -k "<<64-bit key in hex format>>"
```