#include <stdio.h>
#include <string.h>

#define FIRSTBIT 0x8000000000000000
#define ENCRYPT_MODE 1
#define DECRYPT_MODE 2

int const PC1[56] = {
	57, 49, 41, 33, 25, 17,  9,
	 1, 58, 50, 42, 34, 26, 18,
	10,  2, 59, 51, 43, 35, 27,
	19, 11,  3, 60, 52, 44, 36,
	63, 55, 47, 39, 31, 23, 15,
	 7, 62, 54, 46, 38, 30, 22,
	14,  6, 61, 53, 45, 37, 29,
	21, 13,  5, 28, 20, 12,  4
};

int const Rotations[16] = {
	1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
};

int const PC2[48] = {
	14, 17, 11, 24,  1,  5,
	 3, 28, 15,  6, 21, 10,
	23, 19, 12,  4, 26,  8,
	16,  7, 27, 20, 13,  2,
	41, 52, 31, 37, 47, 55,
	30, 40, 51, 45, 33, 48,
	44, 49, 39, 56, 34, 53,
	46, 42, 50, 36, 29, 32
};

int const InitialPermutation[64] = {
	58, 50, 42, 34, 26, 18, 10,  2,
	60, 52, 44, 36, 28, 20, 12,  4,
	62, 54, 46, 38, 30, 22, 14,  6,
	64, 56, 48, 40, 32, 24, 16,  8,
	57, 49, 41, 33, 25, 17,  9,  1,
	59, 51, 43, 35, 27, 19, 11,  3,
	61, 53, 45, 37, 29, 21, 13,  5,
	63, 55, 47, 39, 31, 23, 15,  7
};

int const ExpansionPermutation[48] = {
	32,  1,  2,  3,  4,  5,  4,  5,
	 6,  7,  8,  9,  8,  9, 10, 11,
	12, 13, 12, 13, 14, 15, 16, 17,
	16, 17, 18, 19, 20, 21, 20, 21,
	22, 23, 24, 25, 24, 25, 26, 27,
	28, 29, 28, 29, 30, 31, 32,  1
};

int const S[8][4][16] = {
	{
		{14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7},
		{ 0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8},
		{ 4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0},
		{15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13},
	},
	{
		{15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10},
		{ 3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5},
		{ 0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15},
		{13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9},
	},
	{
		{10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8},
		{13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1},
		{13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7},
		{ 1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12},
	},
	{
		{ 7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15},
		{13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9},
		{10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4},
		{ 3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14},
	},
	{
		{ 2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9},
		{14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6},
		{ 4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14},
		{11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3},
	},
	{
		{12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11},
		{10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8},
		{ 9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6},
		{ 4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13},
	},
	{
		{ 4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1},
		{13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6},
		{ 1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2},
		{ 6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12},
	},
	{
		{13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7},
		{ 1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2},
		{ 7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8},
		{ 2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11},
	},
};

int const P[32] = {
	16,  7, 20, 21, 29, 12, 28, 17,
	 1, 15, 23, 26,  5, 18, 31, 10,
	 2,  8, 24, 14, 32, 27,  3,  9,
	19, 13, 30,  6, 22, 11,  4, 25
};

int const FinalPermutation[64] = {
	40,  8, 48, 16, 56, 24, 64, 32,
	39,  7, 47, 15, 55, 23, 63, 31,
	38,  6, 46, 14, 54, 22, 62, 30,
	37,  5, 45, 13, 53, 21, 61, 29,
	36,  4, 44, 12, 52, 20, 60, 28,
	35,  3, 43, 11, 51, 19, 59, 27,
	34,  2, 42, 10, 50, 18, 58, 26,
	33,  1, 41,  9, 49, 17, 57, 25
};

bool verify_parity_bits(unsigned long long &key) {
  bool parity_bit = false;
  for(int i=0; i<64; i++) {
    if (i%8 == 7) {
      if (parity_bit && ((FIRSTBIT>>i) & key) == 0) {
        return false;
      } else if (!parity_bit && ((FIRSTBIT>>i) & key) > 0) {
        return false;
      }
      parity_bit = false;
    } else {
      if (((FIRSTBIT>>i) & key) > 0) {
        parity_bit = !parity_bit;
      }
    }
  }
  return true;
}

void generate_keys(unsigned long long key, unsigned long long keys[]) {
  unsigned long long left = 0;
  unsigned long long right = 0;
  int shift = 0;

  for(int i=0; i<28; i++) {
    if ((FIRSTBIT>>(PC1[i]-1)) & key) {
      left |= FIRSTBIT>>(i);
    }
  }

  for(int i=0; i<28; i++) {
    if ((FIRSTBIT>>(PC1[i+28]-1)) & key) {
      right |= FIRSTBIT>>(i);
    }
  }
	// printf("left: %llX\n", left);
	// printf("right: %llX\n", right);
  for(int i=0; i<16; i++) {
    shift = Rotations[i];
    for(int j=0; j<shift; j++) {
      if(FIRSTBIT & left) {
        left = left | (FIRSTBIT>>28);
      }
      left = left << 1;

      if(FIRSTBIT & right) {
        right = right | (FIRSTBIT>>28);
      }
      right = right << 1;
    }
		// printf("left: %llX\n", left);
		// printf("right: %llX\n", right);
    key = left;
    for(int j=0; j<28; j++) {
      if((FIRSTBIT>>j) & right) {
        key |= FIRSTBIT>>(j+28);
      }
    }
		// printf(":key %llX\n", key);
    keys[i] = 0;
    for(int j=0; j<48; j++) {
      if ((FIRSTBIT>>(PC2[j]-1)) & key) {
        keys[i] |= FIRSTBIT>>(j);
      }
    }
		// printf(":PC2 %llX\n\n", keys[i]);
  }

}

void permutation(unsigned long long &data, const int Permutation[]) {
  unsigned long long tmp = 0;
  for(int i=0; i<64; i++) {
    if ((FIRSTBIT>>(Permutation[i]-1)) & data) {
      tmp |= FIRSTBIT>>(i);
    }
  }
  data = tmp;
}

unsigned long long feistel(unsigned long long right, unsigned long long key) {
  unsigned long long tmp = 0;
  for(int i=0; i<48; i++) {
    if ((FIRSTBIT>>(ExpansionPermutation[i]-1)) & right) {
      tmp |= FIRSTBIT>>i;
    }
  }
  right = tmp;
  // printf("E: %llX\n", right);
  // printf("K: %llX\n", key);
  tmp = 0;
  tmp = right ^ key;
  // printf("X: %llX\n", tmp);
  int m; // m -> row
  int n; // n -> column
  unsigned long long result = 0;
  for(int i=0; i<8; i++) {
    m = 0;
    n = 0;
    if ((FIRSTBIT>>(6*i))  & tmp) {
      m += 2;
    }
    if ((FIRSTBIT>>(6*i+1)) & tmp) {
      n += 8;
    }
    if ((FIRSTBIT>>(6*i+2)) & tmp) {
      n += 4;
    }
    if ((FIRSTBIT>>(6*i+3)) & tmp) {
      n += 2;
    }
    if ((FIRSTBIT>>(6*i+4)) & tmp) {
      n += 1;
    }
    if ((FIRSTBIT>>(6*i+5)) & tmp) {
      m += 1;
    }
    // printf("%d\n", S[i][m][n]);
    result |= S[i][m][n] << ((7-i)*4);
  }
  result <<= 32;
  // printf("S: %llX\n", result);
  tmp = 0;
  for(int i=0; i<32; i++) {
    if ((FIRSTBIT>>(P[i]-1)) & result) {
      tmp |= FIRSTBIT>>i;
      // printf("%d ", i);
    }
  }
  // printf("\nP: %llX\n", tmp);
  return tmp;
}

unsigned long long encrypt(unsigned long long data, unsigned long long keys[]) {
  unsigned long long left = 0;
  unsigned long long right = 0;

  for(int i=0; i<32; i++) {
    if ((FIRSTBIT>>i) & data) {
      left |= FIRSTBIT>>(i);
    }
  }

  for(int i=0; i<32; i++) {
    if ((FIRSTBIT>>(i+32)) & data) {
      right |= FIRSTBIT>>(i);
    }
  }

  unsigned long long tmp = 0;
  for(int i=0; i<16; i++) {
		// printf("left: %llX\n", left);printf("right: %llX\n", right);
		tmp = left ^ feistel(right, keys[i]);
    left = right;
    right = tmp;

    // printf("xor: %llX\n\n", tmp);
  }

  tmp = right;
  for(int j=0; j<32; j++) {
    if((FIRSTBIT>>j) & left) {
			tmp |= FIRSTBIT>>(j+32);
		}
  }
	// printf("R.L: %llX\n", tmp);
  return tmp;
}

unsigned long long decrypt(unsigned long long data, unsigned long long keys[]) {
  unsigned long long left = 0;
  unsigned long long right = 0;

  for(int i=0; i<32; i++) {
    if ((FIRSTBIT>>i) & data) {
      left |= FIRSTBIT>>(i);
    }
  }

  for(int i=0; i<32; i++) {
    if ((FIRSTBIT>>(i+32)) & data) {
      right |= FIRSTBIT>>(i);
    }
  }

  unsigned long long tmp = 0;
  for(int i=15; i>=0; i--) {
		// printf("left: %llX\n", left);printf("right: %llX\n", right);
		tmp = left ^ feistel(right, keys[i]);
    left = right;
    right = tmp;

    // printf("xor: %llX\n\n", tmp);
  }

  tmp = right;
  for(int j=0; j<32; j++) {
    if((FIRSTBIT>>j) & left) {
			tmp |= FIRSTBIT>>(j+32);
		}
  }
	// printf("R.L: %llX\n", tmp);
  return tmp;
}

int main(int argc, char ** argv) {
	char * message;
	unsigned long long key = 0;
	unsigned int mode = 0; // 1 -> encrypt, 2 -> decrypt

  for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-c") == 0) {
			message = argv[i+1];
			mode = DECRYPT_MODE;
		}
		if (strcmp(argv[i], "-k") == 0) {
			sscanf(argv[i+1], "%llX", &key);
		}
    if (strcmp(argv[i], "-m") == 0) {
      message = argv[i+1];
			mode = ENCRYPT_MODE;
    }
  }

	if (mode == 0) {
		printf("Invalid message or cryptogram\n");
	}

	if (key == 0 || !verify_parity_bits(key)) {
		printf("Invalid key\n");
		return 0;
	}

	unsigned int len = strlen(message);
	unsigned int size;
	if (mode == ENCRYPT_MODE) {
		size = len/8;
		if (len%8 > 0 ) {
			size ++;
		}
	} else {
		size = len/16;
	}

	unsigned long long data[size] = {0};
	unsigned int i;
	unsigned int index;
	if (mode == ENCRYPT_MODE) {
		for (i=0; i<len; i++) {
			index = i/8;
			data[index] <<= 8;
			data[index] |= message[i];
		}
		data[size-1] <<= (7-((i-1)%8))*8;
	}
	if (mode == DECRYPT_MODE) {
		for (i=0; i<size; i++) {
			char block[16];
			for(int j=0; j<16; j++) {
				block[j] = message[i*16+j];
			}
			sscanf(block, "%llX", &data[i]);
		}
	}

  // key = 0x133457799BBCDFF1;
  unsigned long long keys[16];
  generate_keys(key, keys);

	char c[9];
	c[8] = '\0';
	unsigned long long x;
	// for (i=0; i<size; i++) {
	// 	permutation(data[i], InitialPermutation);
	// 	data[i] = encrypt(data[i], keys);
	// 	permutation(data[i], FinalPermutation);
	// 	printf("%llX", data[i]);
	// }
	// printf("\n");
	// printf("\n");
// 	for (i=0; i<size; i++) {
// 		permutation(data[i], InitialPermutation);
// 		data[i] = decrypt(data[i], keys);
// 		permutation(data[i], FinalPermutation);
// 		printf("%llX\n", data[i]);
// 		printf("\n%s\n", c);
// 		for (int j=0; j<8; j++) {
// 			unsigned long long x = ((0xFF00000000000000>>(8*j)) & data[i])>>(56-8*j);
// 			printf("%llX ", x);
// 			c[j] = x;
// 		}
// 		printf("\n%s\n", c);
// 	}
// 	printf("\n");
// return 0;
	if (mode == ENCRYPT_MODE) {
		for(i=0; i<size; i++) {
			permutation(data[i], InitialPermutation);
			data[i] = encrypt(data[i], keys);
			permutation(data[i], FinalPermutation);
			printf("%llX", data[i]);
		}
	}
	if (mode == DECRYPT_MODE) {
		for(i=0; i<size; i++) {
			permutation(data[i], InitialPermutation);
			data[i] = decrypt(data[i], keys);
			permutation(data[i], FinalPermutation);
			// printf("%llX\n", data[i]);
			for (int j=0; j<8; j++) {
				x = ((0xFF00000000000000>>(8*j)) & data[i])>>(56-8*j);
				// printf("%llX ", x);
				c[j] = x;
			}
			printf("%s", c);
		}
	}

	printf("\n");

  return 0;
}
