/*
 * sha256.cpp
 *
 *  Created on: 11/06/2015
 *      Author: ALAN
 */

#include <utils/crypt/sha2.hpp>
#include <utils/funcs.hpp>

using namespace utils::crypt;

#define DBL_INT_ADD(a,b,c) if (a > 0xffffffff - (c)) ++b; a += c;
#define ROTLEFT(a,b) (((a) << (b)) | ((a) >> (32-(b))))
#define ROTRIGHT(a,b) (((a) >> (b)) | ((a) << (32-(b))))

#define CH(x,y,z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define EP0(x) (ROTRIGHT(x,2) ^ ROTRIGHT(x,13) ^ ROTRIGHT(x,22))
#define EP1(x) (ROTRIGHT(x,6) ^ ROTRIGHT(x,11) ^ ROTRIGHT(x,25))
#define SIG0(x) (ROTRIGHT(x,7) ^ ROTRIGHT(x,18) ^ ((x) >> 3))
#define SIG1(x) (ROTRIGHT(x,17) ^ ROTRIGHT(x,19) ^ ((x) >> 10))

t_dword sha256_tab[64] = {
   0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
   0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
   0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
   0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
   0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
   0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
   0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
   0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
};


SHA256Hasher::SHA256Hasher() {
	init();
}

SHA256Hasher::~SHA256Hasher() {
}

void SHA256Hasher::transform(byte_type* data) {
	t_dword a, b, c, d, e, f, g, h, i, j, t1, t2, m[64];

	for (i = 0, j = 0; i < 16; ++i, j += 4)
		m[i] = (data[j] << 24) | (data[j + 1] << 16) | (data[j + 2] << 8)
				| (data[j + 3]);
	for (; i < 64; ++i)
		m[i] = SIG1(m[i - 2]) + m[i - 7] + SIG0(m[i - 15]) + m[i - 16];

	a = state[0];
	b = state[1];
	c = state[2];
	d = state[3];
	e = state[4];
	f = state[5];
	g = state[6];
	h = state[7];

	for (i = 0; i < 64; ++i) {
		t1 = h + EP1(e) + CH(e, f, g) + sha256_tab[i] + m[i];
		t2 = EP0(a) + MAJ(a, b, c);
		h = g;
		g = f;
		f = e;
		e = d + t1;
		d = c;
		c = b;
		b = a;
		a = t1 + t2;
	}

	state[0] += a;
	state[1] += b;
	state[2] += c;
	state[3] += d;
	state[4] += e;
	state[5] += f;
	state[6] += g;
	state[7] += h;
}

void SHA256Hasher::update(const byte_type* src, size_type slength) {
	t_dword i;

	for (i = 0; i < slength; ++i) {
		data[datalen] = src[i];
		datalen++;
		if (datalen == 64) {
			transform(data);
			DBL_INT_ADD(bitlen[0], bitlen[1], 512);
			datalen = 0;
		}
	}
}

void SHA256Hasher::finalize(byte_type* dest) {
	t_dword i;

	i = datalen;

	if (datalen < 56) {
		data[i++] = 0x80;
		while (i < 56)
			data[i++] = 0x00;
	} else {
		data[i++] = 0x80;
		while (i < 64)
			data[i++] = 0x00;
		transform(data);
		memset(data, 0, 56);
	}

	DBL_INT_ADD(bitlen[0], bitlen[1], datalen * 8);
	data[63] = bitlen[0];
	data[62] = bitlen[0] >> 8;
	data[61] = bitlen[0] >> 16;
	data[60] = bitlen[0] >> 24;
	data[59] = bitlen[1];
	data[58] = bitlen[1] >> 8;
	data[57] = bitlen[1] >> 16;
	data[56] = bitlen[1] >> 24;
	transform(data);

	for (i = 0; i < 4; ++i) {
		dest[i] = (state[0] >> (24 - i * 8)) & 0x000000ff;
		dest[i + 4] = (state[1] >> (24 - i * 8)) & 0x000000ff;
		dest[i + 8] = (state[2] >> (24 - i * 8)) & 0x000000ff;
		dest[i + 12] = (state[3] >> (24 - i * 8)) & 0x000000ff;
		dest[i + 16] = (state[4] >> (24 - i * 8)) & 0x000000ff;
		dest[i + 20] = (state[5] >> (24 - i * 8)) & 0x000000ff;
		dest[i + 24] = (state[6] >> (24 - i * 8)) & 0x000000ff;
		dest[i + 28] = (state[7] >> (24 - i * 8)) & 0x000000ff;
	}
}

SHA256Hasher::size_type SHA256Hasher::digest_size() const {
	return 256/8;
}

void SHA256Hasher::init() {
	datalen = 0;
	bitlen[0] = 0;
	bitlen[1] = 0;
	state[0] = 0x6a09e667;
	state[1] = 0xbb67ae85;
	state[2] = 0x3c6ef372;
	state[3] = 0xa54ff53a;
	state[4] = 0x510e527f;
	state[5] = 0x9b05688c;
	state[6] = 0x1f83d9ab;
	state[7] = 0x5be0cd19;
}

// ---------------------------------------------------------------------------------------------------
// TODO SHA384

static t_qword SHA512_K[80] = {
		0x428a2f98d728ae22LL, 0x7137449123ef65cdLL, 0xb5c0fbcfec4d3b2fLL, 0xe9b5dba58189dbbcLL,
		0x3956c25bf348b538LL, 0x59f111f1b605d019LL, 0x923f82a4af194f9bLL, 0xab1c5ed5da6d8118LL,
		0xd807aa98a3030242LL, 0x12835b0145706fbeLL, 0x243185be4ee4b28cLL, 0x550c7dc3d5ffb4e2LL,
		0x72be5d74f27b896fLL, 0x80deb1fe3b1696b1LL, 0x9bdc06a725c71235LL, 0xc19bf174cf692694LL,
		0xe49b69c19ef14ad2LL, 0xefbe4786384f25e3LL, 0x0fc19dc68b8cd5b5LL, 0x240ca1cc77ac9c65LL,
		0x2de92c6f592b0275LL, 0x4a7484aa6ea6e483LL, 0x5cb0a9dcbd41fbd4LL, 0x76f988da831153b5LL,
		0x983e5152ee66dfabLL, 0xa831c66d2db43210LL, 0xb00327c898fb213fLL, 0xbf597fc7beef0ee4LL,
		0xc6e00bf33da88fc2LL, 0xd5a79147930aa725LL, 0x06ca6351e003826fLL, 0x142929670a0e6e70LL,
		0x27b70a8546d22ffcLL, 0x2e1b21385c26c926LL, 0x4d2c6dfc5ac42aedLL, 0x53380d139d95b3dfLL,
		0x650a73548baf63deLL, 0x766a0abb3c77b2a8LL, 0x81c2c92e47edaee6LL, 0x92722c851482353bLL,
		0xa2bfe8a14cf10364LL, 0xa81a664bbc423001LL, 0xc24b8b70d0f89791LL, 0xc76c51a30654be30LL,
		0xd192e819d6ef5218LL, 0xd69906245565a910LL, 0xf40e35855771202aLL, 0x106aa07032bbd1b8LL,
		0x19a4c116b8d2d0c8LL, 0x1e376c085141ab53LL, 0x2748774cdf8eeb99LL, 0x34b0bcb5e19b48a8LL,
		0x391c0cb3c5c95a63LL, 0x4ed8aa4ae3418acbLL, 0x5b9cca4f7763e373LL, 0x682e6ff3d6b2b8a3LL,
		0x748f82ee5defb2fcLL, 0x78a5636f43172f60LL, 0x84c87814a1f0ab72LL, 0x8cc702081a6439ecLL,
		0x90befffa23631e28LL, 0xa4506cebde82bde9LL, 0xbef9a3f7b2c67915LL, 0xc67178f2e372532bLL,
		0xca273eceea26619cLL, 0xd186b8c721c0c207LL, 0xeada7dd6cde0eb1eLL, 0xf57d4f7fee6ed178LL,
		0x06f067aa72176fbaLL, 0x0a637dc5a2c898a6LL, 0x113f9804bef90daeLL, 0x1b710b35131c471bLL,
		0x28db77f523047d84LL, 0x32caab7b40c72493LL, 0x3c9ebe0a15c9bebcLL, 0x431d67c49c100d4cLL,
		0x4cc5d4becb3e42b6LL, 0x597f299cfc657e2aLL, 0x5fcb6fab3ad6faecLL, 0x6c44198c4a475817LL
};

#define SHA384_LENGTH 48

SHA384Hasher::SHA384Hasher() {
	init();
}

SHA384Hasher::~SHA384Hasher() {
}

#define GETBE(d,n) (((t_qword)d[n] << 56LL) | \
		((t_qword)d[n+1] << 48LL) | \
		((t_qword)d[n+2] << 40LL) | \
		((t_qword)d[n+3] << 32LL) | \
		((t_qword)d[n+4] << 24LL) | \
		((t_qword)d[n+5] << 16LL) | \
		((t_qword)d[n+6] << 8LL) | \
		((t_qword)d[n+7]))

#define SETBE(d,s,n) d[n]   = (byte_type) (s >> 56LL); \
					 d[n+1] = (byte_type) (s >> 48LL); \
					 d[n+2] = (byte_type) (s >> 40LL); \
					 d[n+3] = (byte_type) (s >> 32LL); \
					 d[n+4] = (byte_type) (s >> 24LL); \
					 d[n+5] = (byte_type) (s >> 16LL); \
					 d[n+6] = (byte_type) (s >>  8LL); \
					 d[n+7] = (byte_type) (s);

#define ROTLEFT64(a,b) (((a) << (b)) | ((a) >> (64-(b))))
#define ROTRIGHT64(a,b) (((a) >> (b)) | ((a) << (64-(b))))

#define SHA512_EP0(x)  (ROTRIGHT64(x,28) ^ ROTRIGHT64(x,34) ^ ROTRIGHT64(x,39))
#define SHA512_EP1(x)  (ROTRIGHT64(x,14) ^ ROTRIGHT64(x,18) ^ ROTRIGHT64(x,41))
#define SHA512_SIG0(x) (ROTRIGHT64(x, 1) ^ ROTRIGHT64(x, 8) ^ ((x) >> 7))
#define SHA512_SIG1(x) (ROTRIGHT64(x,19) ^ ROTRIGHT64(x,61) ^ ((x) >> 6))

void SHA384Hasher::process(const byte_type* data) {

	t_qword W[80], t1, t2;
	int i = 0;
	t_qword a = _h[0],
			b = _h[1],
			c = _h[2],
			d = _h[3],
			e = _h[4],
			f = _h[5],
			g = _h[6],
			h = _h[7];

	for (; i < 16; ++i)
		W[i] = GETBE(data,i*8);

	for (; i < 80; ++i)
		W[i] = SHA512_SIG1(W[i- 2]) + W[i-7] +
			   SHA512_SIG0(W[i-15]) + W[i-16];

	for (i = 0; i < 80; ++i) {
		t1 = h + SHA512_EP1(e) + CH(e,f,g) + SHA512_K[i] + W[i];
		t2 = SHA512_EP0(a) + MAJ(a,b,c);
		h = g;
		g = f;
		f = e;
		e = d + t1;
		d = c;
		c = b;
		b = a;
		a = t1 + t2;
	}

	_h[0] += a;
	_h[1] += b;
	_h[2] += c;
	_h[3] += d;
	_h[4] += e;
	_h[5] += f;
	_h[6] += g;
	_h[7] += h;

}

void SHA384Hasher::update(const byte_type* src, size_type slength) {
	size_t fill;
	unsigned int left;

	if (slength == 0)
		return;

	left = (unsigned int) (_total[0] & 0x7F);
	fill = 128 - left;

	_total[0] += (t_qword) slength;

	if (_total[0] < (t_qword) slength)
		_total[1]++;

	if (left && slength >= fill) {
		memcpy(_buffer + left, src, fill);
		process(_buffer);
		src += fill;
		slength -= fill;
		left = 0;
	}

	while (slength >= 128) {
		process(src);
		src += 128;
		slength -= 128;
	}

	if (slength > 0)
		memcpy(_buffer + left, src, slength);

}

static unsigned char SHA512_PADDING[128] = {
 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

void SHA384Hasher::finalize(byte_type* dest) {
	size_t last, padn;
	t_qword high, low;
	byte_type msg[16];

	high = (_total[0] >> 61) | (_total[1] << 3);
	low = (_total[0] << 3);

	SETBE(msg, high, 0);
	SETBE(msg, low, 8);

	last = (size_t) (_total[0] & 0x7F);
	padn = (last < 112) ? (112 - last) : (240 - last);

	update(SHA512_PADDING, padn);
	update(msg, 16);

	_copy_output(dest);

}

SHA384Hasher::size_type SHA384Hasher::digest_size() const {
	return 384/8;
}

void SHA384Hasher::_copy_output(byte_type* dest) {
    SETBE(dest, _h[0],  0);
    SETBE(dest, _h[1],  8);
    SETBE(dest, _h[2], 16);
    SETBE(dest, _h[3], 24);
    SETBE(dest, _h[4], 32);
    SETBE(dest, _h[5], 40);
}

void SHA384Hasher::init() {
	_h[0]=0xcbbb9d5dc1059ed8LL;
	_h[1]=0x629a292a367cd507LL;
	_h[2]=0x9159015a3070dd17LL;
	_h[3]=0x152fecd8f70e5939LL;
	_h[4]=0x67332667ffc00b31LL;
	_h[5]=0x8eb44a8768581511LL;
	_h[6]=0xdb0c2e0d64f98fa7LL;
	_h[7]=0x47b5481dbefa4fa4LL;
	memset((void*)_buffer,0,128);
	_total[0] = 0;
	_total[1] = 0;
}


void SHA512Hasher::_copy_output(byte_type* dest) {
    SETBE(dest, _h[0],  0);
    SETBE(dest, _h[1],  8);
    SETBE(dest, _h[2], 16);
    SETBE(dest, _h[3], 24);
    SETBE(dest, _h[4], 32);
    SETBE(dest, _h[5], 40);
    SETBE(dest, _h[6], 48);
    SETBE(dest, _h[7], 56);
}

SHA512Hasher::SHA512Hasher() {
	init();
}

SHA512Hasher::~SHA512Hasher() {
}

void SHA512Hasher::init() {
	_h[0] = 0x6A09E667F3BCC908LL;
	_h[1] = 0xBB67AE8584CAA73BLL;
	_h[2] = 0x3C6EF372FE94F82BLL;
	_h[3] = 0xA54FF53A5F1D36F1LL;
	_h[4] = 0x510E527FADE682D1LL;
	_h[5] = 0x9B05688C2B3E6C1FLL;
	_h[6] = 0x1F83D9ABFB41BD6BLL;
	_h[7] = 0x5BE0CD19137E2179LL;
	memset((void*)_buffer,0,128);
	_total[0] = 0;
	_total[1] = 0;
}

SHA512Hasher::size_type SHA512Hasher::digest_size() const {
	return 512/8;
}

