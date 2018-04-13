/*
 * encoder.hpp
 *
 *  Created on: 09/06/2015
 *      Author: ALAN
 */

#ifndef ENCODER_HPP_
#define ENCODER_HPP_

#include <string>
#include <utils/utils_defs.hpp>
#include <utils/exception.hpp>

namespace utils {

namespace codec {


template <typename _Char_T>
class Encoder {
public:
	typedef _Char_T char_type;
	typedef size_t size_type;
	typedef std::basic_string<_Char_T> string_type;
	typedef unsigned char byte_type;
public:
	virtual ~Encoder() { }

	virtual size_type encode(const byte_type* source, size_type slength, char_type* dest, size_type dmax) {
		return 0;
	}

	virtual size_type decode(const char_type* source, size_type slength, byte_type* dest, size_type dmax) {
		return 0;
	}

};


template <typename _Char_T>
class HexEncoder :  public Encoder<_Char_T> {
public:
	typedef _Char_T char_type;
	typedef size_t size_type;
	typedef std::basic_string<_Char_T> string_type;
	typedef unsigned char byte_type;
private:
	bool _locase;
	int to_number(char_type ch) {
		if (ch >= 'a' && ch <= 'f')
			return ch - 'a' + 0x0a;
		if (ch >= 'A' && ch <= 'A')
			return ch - 'A' + 0x0a;
		if (ch >= '0' && ch <= '9')
			return ch - '0';
		throw invalid_argument_exception("Non hex character found.");
	}
public:

	HexEncoder() : _locase(false) {
	}

	HexEncoder(bool locase) : _locase(locase) {
	}

	virtual ~HexEncoder() { }

	virtual size_type encode(const byte_type* source, size_type slength, char_type* dest, size_type dmax) {
		const char* enc_tab = _locase ? "0123456789abcdef" : "0123456789ABCDEF";
		char_type* it = dest;
		char_type* max = dest + dmax;
		const byte_type* end = source + slength;
		for (; source != end; ++source) {
			unsigned char b = *source;
			*(it++) = static_cast<_Char_T>(enc_tab[(b >> 4) & 0x0f]);
			if (it >= max) break;
			*(it++) = static_cast<_Char_T>(enc_tab[b & 0x0f]);
			if (it >= max) break;
		}
		return it-dest;
	}

	virtual size_type decode(const char_type* source, size_type slength, byte_type* dest, size_type dmax) {
		const char_type* it = source;
		const char_type* end = source + slength;
		byte_type* cur = dest;
		byte_type* max = dest + dmax;
		unsigned char b = 0;
		for (unsigned int i = 0; it != end; ++it, ++i) {
			b = (b << 4) + to_number(*it);
			if ((i & 1) == 1) {
				if (cur < max) {
					*(cur++) = b;
					b = 0;
				} else
					return cur - dest;
			}
		}
		return cur - dest;
	}

};

template <typename _Char_T>
class Base32Encoder :  public Encoder<_Char_T> {
public:
	typedef _Char_T char_type;
	typedef size_t size_type;
	typedef std::basic_string<_Char_T> string_type;
	typedef unsigned char byte_type;
	MAKE_CONSTANT(char_type,PAD,char_type('='));
	MAKE_CONSTANT(int,MASK_B5,0x1f);
	MAKE_CONSTANT(int,MASK_B8,0xff);
private:
	bool _hex_alpha, _url_safe;
	const char* get_dec_tab_hex() const {
		static char _dec_tab_hex[] = {
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 63,
				 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, -1, -1, -1, -1, -1, -1,
				-1, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
				25, 26, 27, 28, 29, 30, 31, -1, -1, -1, -1};
		return _dec_tab_hex;
	}
	const char* get_dec_tab() const {
		static char _dec_tab[] = {
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 63,
				-1, -1, 26, 27, 28, 29, 30, 31, -1, -1, -1, -1, -1, -1, -1, -1,
				-1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
				15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25 };
		return _dec_tab;
	}
public:

	Base32Encoder() : _hex_alpha(false), _url_safe(false) {
	}

	Base32Encoder(bool hex_alpha, bool url_safe = false) : _hex_alpha(hex_alpha), _url_safe(url_safe) {
	}

	virtual ~Base32Encoder() { }

	virtual size_type encode(const byte_type* source, size_type slength, char_type* dest, size_type dmax) {
		if (slength == 0) return 0;
		const char* enc_tab = _hex_alpha ? "0123456789ABCDEFGHIJKLMNOPQRSTUV" : "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
		const byte_type* it = source;
		const byte_type* end = source + slength;
		char_type* cur = dest;
		size_type n = 0;
		long long l = 0;
		for (; it != end; ++it, n++) {
			int b = (int)*it;
			l = (l << 8LL) + (long long)b;
			if (n % 5 == 4) {
				if (n + 8 >= dmax)
					return cur - dest;
                *(cur++) = enc_tab[(int)(l >> 35LL) & MASK_B5];
                *(cur++) = enc_tab[(int)(l >> 30LL) & MASK_B5];
                *(cur++) = enc_tab[(int)(l >> 25LL) & MASK_B5];
                *(cur++) = enc_tab[(int)(l >> 20LL) & MASK_B5];
                *(cur++) = enc_tab[(int)(l >> 15LL) & MASK_B5];
                *(cur++) = enc_tab[(int)(l >> 10LL) & MASK_B5];
                *(cur++) = enc_tab[(int)(l >>  5LL) & MASK_B5];
                *(cur++) = enc_tab[(int)l & MASK_B5];
			}
		}
		if (n + 8 >= dmax)
			return cur - dest;
		--n;
        switch (n % 5) {
            case 0 :
                *(cur++) = enc_tab[(int)(l >> 3LL) & MASK_B5];
                *(cur++) = enc_tab[(int)(l << 2LL) & MASK_B5];
                if (!_url_safe) {
					*(cur++) = PAD;
					*(cur++) = PAD;
					*(cur++) = PAD;
					*(cur++) = PAD;
					*(cur++) = PAD;
					*(cur++) = PAD;
                }
                break;
            case 1 :
                *(cur++) = enc_tab[(int)(l >> 11LL) & MASK_B5];
                *(cur++) = enc_tab[(int)(l >>  6LL) & MASK_B5];
                *(cur++) = enc_tab[(int)(l >>  1LL) & MASK_B5];
                *(cur++) = enc_tab[(int)(l <<  4LL) & MASK_B5];
                if (!_url_safe) {
					*(cur++) = PAD;
					*(cur++) = PAD;
					*(cur++) = PAD;
					*(cur++) = PAD;
                }
                break;
            case 2 :
                *(cur++) = enc_tab[(int)(l >> 19LL) & MASK_B5];
                *(cur++) = enc_tab[(int)(l >> 14LL) & MASK_B5];
                *(cur++) = enc_tab[(int)(l >>  9LL) & MASK_B5];
                *(cur++) = enc_tab[(int)(l >>  4LL) & MASK_B5];
                *(cur++) = enc_tab[(int)(l <<  1LL) & MASK_B5];
                if (!_url_safe) {
					*(cur++) = PAD;
					*(cur++) = PAD;
					*(cur++) = PAD;
                }
                break;
            case 3 :
                *(cur++) = enc_tab[(int)(l >> 27LL) & MASK_B5];
                *(cur++) = enc_tab[(int)(l >> 22LL) & MASK_B5];
                *(cur++) = enc_tab[(int)(l >> 17LL) & MASK_B5];
                *(cur++) = enc_tab[(int)(l >> 12LL) & MASK_B5];
                *(cur++) = enc_tab[(int)(l >>  7LL) & MASK_B5];
                *(cur++) = enc_tab[(int)(l >>  2LL) & MASK_B5];
                *(cur++) = enc_tab[(int)(l <<  3LL) & MASK_B5];
                if (!_url_safe) {
                	*(cur++) = PAD;
                }
                break;
        }
		return cur - dest;
	}

	virtual size_type decode(const char_type* source, size_type slength, byte_type* dest, size_type dmax) {
		if (slength == 0) return 0;
		const char *dec_tab = _hex_alpha ? get_dec_tab_hex() : get_dec_tab();
		const char_type* it = source;
		const char_type* end = source + slength;
		byte_type* cur = dest;
		size_type n = 0;
		long long l = 0;
		for (; it!= end; ++it, ++n) {
			int b = (int)*it;
			if (b == PAD) break;
			if (b > 91 || b < 0)
				throw invalid_argument_exception("Source is not Base32");
			int v = (int) dec_tab[b];
			if (v == -1)
				throw invalid_argument_exception("Source is not Base32");
			l = (l << 5LL) + (long long) v;
			if ((n % 8) == 7) {
				if (n + 5 >= dmax)
					return cur - dest;
                *(cur++) = (byte_type) ((l >> 32LL) & MASK_B8);
                *(cur++) = (byte_type) ((l >> 24LL) & MASK_B8);
                *(cur++) = (byte_type) ((l >> 16LL) & MASK_B8);
                *(cur++) = (byte_type) ((l >> 8LL) & MASK_B8);
                *(cur++) = (byte_type) (l & MASK_B8);
			}
		}
		--n;
		 switch (n % 8) {
         case 1 :
        	 if (n >= dmax - 2) break;
             *(cur++) = (byte_type) ((l >> 2) & MASK_B8);
             break;
         case 2 :
        	 if (n >= dmax - 2) break;
             *(cur++) = (byte_type) ((l >> 7) & MASK_B8);
             break;
         case 3 :
        	 if (n >= dmax - 3) break;
             l = l >> 4;
             *(cur++) = (byte_type) ((l >> 8) & MASK_B8);
             *(cur++) = (byte_type) (l & MASK_B8);
             break;
         case 4 :
        	 if (n >= dmax - 4) break;
             l = l >> 1;
             *(cur++) = (byte_type) ((l >> 16) & MASK_B8);
             *(cur++) = (byte_type) ((l >> 8) & MASK_B8);
             *(cur++) = (byte_type) (l & MASK_B8);
             break;
         case 5 :
        	 if (n >= dmax - 4) break;
             l = l >> 6;
             *(cur++) = (byte_type) ((l >> 16) & MASK_B8);
             *(cur++) = (byte_type) ((l >> 8) & MASK_B8);
             *(cur++) = (byte_type) (l & MASK_B8);
             break;
         case 6 :
        	 if (n >= dmax - 5) break;
             l = l >> 3;
             *(cur++) = (byte_type) ((l >> 24) & MASK_B8);
             *(cur++) = (byte_type) ((l >> 16) & MASK_B8);
             *(cur++) = (byte_type) ((l >> 8) & MASK_B8);
             *(cur++) = (byte_type) (l & MASK_B8);
             break;
		 }
		return cur - dest;
	}

};

template <typename _Char_T>
class Base64Encoder :public Encoder<_Char_T> {
public:
	typedef _Char_T char_type;
	typedef size_t size_type;
	typedef std::basic_string<_Char_T> string_type;
	typedef unsigned char byte_type;
	MAKE_CONSTANT(char_type,PAD,char_type('='));
	MAKE_CONSTANT(int,MASK_B6,0x3f);
	MAKE_CONSTANT(int,MASK_B8,0xff);
private:
	bool _url_safe;
public:

	Base64Encoder() : _url_safe(false) {
	}

	Base64Encoder(bool url_safe) : _url_safe(url_safe) {
	}

	virtual ~Base64Encoder() { }

	virtual size_type encode(const byte_type* source, size_type slength, char_type* dest, size_type dmax) {
		if (slength == 0) return 0;
		const char* enc_tab = _url_safe ? "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_" : "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
		const byte_type* it = source;
		const byte_type* end = source + slength;
		char_type* cur = dest;
		size_type n = 0;
		long long l = 0;
		for (; it != end; ++it, n++) {
			int b = (int)*it;
			l = (l << 8LL) + (long long)b;
			if (n % 3 == 2) {
				if (n + 4 >= dmax)
					return cur - dest;
                *(cur++) = enc_tab[(int)(l >> 18LL) & MASK_B6];
                *(cur++) = enc_tab[(int)(l >> 12LL) & MASK_B6];
                *(cur++) = enc_tab[(int)(l >>  6LL) & MASK_B6];
                *(cur++) = enc_tab[(int)l & MASK_B6];
			}
		}
		--n;
		if (n + 4 >= dmax)
			return cur - dest;
        switch (n % 3) {
            case 0 :
                *(cur++) = enc_tab[(l >> 2LL) & MASK_B6];
                *(cur++) = enc_tab[(l << 4LL) & MASK_B6];
                if (!_url_safe) {
                    *(cur++) = PAD;
                    *(cur++) = PAD;
                }
                break;

            case 1 :
                *(cur++) = enc_tab[(l >> 10LL) & MASK_B6];
                *(cur++) = enc_tab[(l >> 4LL) & MASK_B6];
                *(cur++) = enc_tab[(l << 2LL) & MASK_B6];
                if (!_url_safe) {
                    *(cur++) = PAD;
                }
                break;
        }
		return cur - dest;
	}

	virtual size_type decode(const char_type* source, size_type slength, byte_type* dest, size_type dmax) {
		if (slength == 0) return 0;
		static char dec_tab[] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
				62, -1, 62, -1, 63, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1,
				-1, -1, -1, -1, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
				12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1,
				-1, -1, 63, -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37,
				38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51 };
		const char_type* it = source;
		const char_type* end = source + slength;
		byte_type* cur = dest;
		size_type n = 0;
		long long l = 0;
		for (; it!= end; ++it, ++n) {
			int b = (int)*it;
			if (b == PAD) break;
			if (b < 0 || b >= 123)
				throw invalid_argument_exception("Source is not Base64");
			int v = (int) dec_tab[b];
			if (v == -1)
				throw invalid_argument_exception("Source is not Base64");
			l = (l << 6LL) + (long long) v;
			if ((n % 4) == 3) {
				if (n + 3 >= dmax)
					return cur - dest;
                *(cur++) = (byte_type) ((l >> 16LL) & MASK_B8);
                *(cur++) = (byte_type) ((l >> 8LL) & MASK_B8);
                *(cur++) = (byte_type) (l & MASK_B8);
			}
		}
		--n;
        switch (n % 4) {
            case 1:
				if (n + 1 >= dmax)
					return cur - dest;
                l = l >> 4;
                *(cur++) = (byte_type) ((l) & MASK_B8);
                break;
            case 2:
				if (n + 2 >= dmax)
					return cur - dest;
                l = l >> 2;
                *(cur++) = (byte_type) ((l >> 8) & MASK_B8);
                *(cur++) = (byte_type) ((l) & MASK_B8);
                break;
        }
		return cur - dest;
	}

};


} // codec

} // utils

#endif /* ENCODER_HPP_ */
