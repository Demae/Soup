// Fun Fact: #pragma once is supported by literally every compiler despite not being part of any standard
#ifndef _SOUP_H
#define _SOUP_H

#if !defined(SOUP_CEXPORT) || SOUP_CODE_INSPECTOR
	#ifdef _WIN32
		#define SOUP_EXPORT __declspec(dllexport)
	#else
		#define SOUP_EXPORT
	#endif

	#define SOUP_CEXPORT extern "C" SOUP_EXPORT

	typedef void Bigint;
	typedef void Canvas;
	typedef void cbResult;
	typedef void KeyGenId;
	typedef void Mixed;
	typedef void QrCode;
	typedef void RsaKeypair;
	typedef void Totp;

	typedef void stdexception;
	typedef void stdstring;
#endif

typedef void (*void_func_t)();

// [global namespace]
SOUP_CEXPORT void beginScope();
SOUP_CEXPORT void endScope();
SOUP_CEXPORT void broadenScope(void* inst);
SOUP_CEXPORT void endLifetime(void* inst);
SOUP_CEXPORT const char* tryCatch(void_func_t f);
// base32
SOUP_CEXPORT const char* base32_encode(const stdstring* x, bool pad);
SOUP_CEXPORT stdstring* base32_decode(const char* x);
// base40
SOUP_CEXPORT const char* base40_encode(const stdstring* x);
SOUP_CEXPORT stdstring* base40_decode(const char* x);
// base64
SOUP_CEXPORT const char* base64_encode(const stdstring* x);
// Bigint
SOUP_CEXPORT const char* Bigint_toString(const Bigint* x);
// Canvas
SOUP_CEXPORT unsigned int Canvas_getWidth(const Canvas* x);
SOUP_CEXPORT unsigned int Canvas_getHeight(const Canvas* x);
SOUP_CEXPORT void Canvas_resizeNearestNeighbour(Canvas* x, unsigned int desired_width, unsigned int desired_height);
SOUP_CEXPORT const char* Canvas_toSvg(const Canvas* x, unsigned int scale);
SOUP_CEXPORT stdstring* Canvas_toNewPngString(const Canvas* x);
// cbResult
SOUP_CEXPORT const char* cbResult_getResponse(const cbResult* x);
SOUP_CEXPORT bool cbResult_isDelete(const cbResult* x);
SOUP_CEXPORT int cbResult_getDeleteNum(const cbResult* x);
// Chatbot
SOUP_CEXPORT cbResult* Chatbot_process(const char* text);
// Hotp
SOUP_CEXPORT stdstring* Hotp_generateSecret(size_t bytes);
// InquiryLang
SOUP_CEXPORT Mixed* InquiryLang_execute(const char* x);
SOUP_CEXPORT const char* InquiryLang_formatResultLine(const Mixed* x);
// KeyGenId
SOUP_CEXPORT KeyGenId* KeyGenId_newFromSeedsExport(unsigned int bits, const stdstring* str);
SOUP_CEXPORT KeyGenId* KeyGenId_generate(unsigned int bits);
SOUP_CEXPORT stdstring* KeyGenId_toSeedsExport(const KeyGenId* x);
SOUP_CEXPORT RsaKeypair* KeyGenId_getKeypair(const KeyGenId* x);
// Mixed
SOUP_CEXPORT bool Mixed_isCanvas(const Mixed* x);
SOUP_CEXPORT Canvas* Mixed_getCanvas(const Mixed* x);
// QrCode
SOUP_CEXPORT QrCode* QrCode_newFromText(const char* x);
SOUP_CEXPORT Canvas* QrCode_toNewCanvas(const QrCode* x, unsigned int border, bool black_bg);
// RsaKeypair
SOUP_CEXPORT const Bigint* RsaKeypair_getN(const RsaKeypair* x);
SOUP_CEXPORT const Bigint* RsaKeypair_getP(const RsaKeypair* x);
SOUP_CEXPORT const Bigint* RsaKeypair_getQ(const RsaKeypair* x);
// Totp
SOUP_CEXPORT Totp* Totp_new(const stdstring* secret);
SOUP_CEXPORT const char* Totp_getQrCodeUri(const Totp* x, const char* label, const char* issuer);
SOUP_CEXPORT int Totp_getValue(const Totp* x);
// exception
SOUP_CEXPORT const char* exception_what(const stdexception* x);

#endif // #ifndef _SOUP_H
