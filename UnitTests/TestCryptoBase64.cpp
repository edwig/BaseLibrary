/////////////////////////////////////////////////////////////////////////////////
//
// SourceFile: TestCryptoBase64.cpp
//
// BaseLibrary: Indispensable general objects and functions
//              UNIT TESTS for the BaseLibrary
// 
// Copyright (c) 2014-2022 ir. W.E. Huisman
// All rights reserved
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
#include "pch.h"
#include "CppUnitTest.h"
// BaseLibrary headers
#include <Base64.h>
#include <Crypto.h>
#include <ConvertWideString.h>
// General MS-Windows headers
#include <Wincrypt.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

// Test values synchronized with: https://dencode.com/hash
// UNICODE is UTF16-LE (Motorola / MAC OS)

#ifdef UNICODE
XString expected1(L"61c2b4221f6392fc57b286e72482e65fabfb4a5f"); // SHA1
XString expected2(L"db65b0c7d41a286f53556898ababca67");         // MD2
XString expected3(L"a29a06a79d4f1bc307b879baf502a8b7");         // MD4
XString expected4(L"553a711fd0cc756ad8028368192364a7");         // MD5
XString expected5(L"cabab95aae664d2875e28c47e7ecc37d702ff01b576ee6811d00a7a9916d116d"); // SHA-256
XString expected6(L"19088d9c2b65976364dfd3446c6779858b3ca2ddd3478c1134ca3946e1921e85f0003b7607cad0a56560c39880209723"); // SHA-384
XString expected7(L"7d288871f20bae4dc24ba40b9e26fee12ae0bde49e9a26797e5393d9167b1948e04d4b2a291c913e081bd16a3da0507169fee64d6fbccd996c1874d27c377ce9");  // SHA-512
XString expected8(L"GA5MilPV9rF22LeakCmAW6S0bNOEbrYFhUwlFvDgSZtWkR06KosiDQFZd7IBLXIdBcb5Loe3AuJUkHHkP3pfMIYcHeUG8UVFZKT6tkzeTZ3SBT+TH/gaJcGdL4Qfx0lLPQpIblJsdU99B4jfSgZHomySw4wSmY1v8iRIZWSISSA=");
XString expected9(L"7P0o2xb/zacd2cn7QRN5YwdUpiNs+96R0pVQ6v/jyHDjTqW7Npe/fU4uNu6hfz78gDHJft+YXgEcan2VmqRauNa3/dE+6C3EtIQoQ05ZhMROXVDe8Zy8+WGFYB9Na9FR6+dRN6gmwE8iCpHJ0Zlg3UHvv1a91khs");
#else
XString expected1("5c1d73d9d1ccb9c36fcfc8ecc5795cbeefbbe4c6"); // SHA1
XString expected2("6f250196cc6bb16cbf461cfc2f88916b");         // MD2
XString expected3("5d639330a7eaf6296253d131ed4594c8");         // MD4
XString expected4("39fcc4f943bdd74e252af223d18cd79b");         // MD5
XString expected5("f45bb2165916242588a63785accac37c08086c115a2fc74d0ad6ed12a27a6dd8"); // SHA-256
XString expected6("8348a11cfc2e111bf38cbe6cc4b488e4fea0b5a2159fedf3047a0ca0d7741e581a31200cefe493b8ff14543502bfb0bd"); // SHA-384
XString expected7("bedfd93c0bdaa6bbfe2f3bb26a40acb9b6b6ee3fe08201e60de2460922266eabefad4723074f310058e7992bf3a0864b1d03d21eb0d94c9a6006dc7a55866f0c");  // SHA-512
XString expected8("iCby8h4MjErrm3rj4EwKsKVb5/Gt+EyNrDwE2ZG7pAqTgvzZwAMZ+D0045WhM2+5NqVYBrdGQmr+Xhn/ufq5Kw==");
XString expected9("F8jAdeNxzKzNzPN70RUAtlpuOuyraqlNTDmUImGPMsfdqZgVY7rvIqWjFxtHQRNi6+NprOHcpsuyIQDX");
#endif

// Take a hash from this string
XString message(_T("This is a somewhat longer string with an intentional message"));

namespace BaseLibraryUnitTests
{
TEST_CLASS(CryptoBase64)
{
private:
#ifdef UNICODE
  // On input:  Convert Big-Endian (Blefuscu) to Little-Endian (Lilliput)
  // On output: Convert Little-Endian (Lilliput) to Big-Endian (Blefuscu)
  // MS-Windows = Intel architecture = Little-Endian (LE)
  // Mac-OS     = Motorola architecture = Big-Endian (BE)
  void BlefuscuToLaputa(XString& p_gulliver)
  {
    _TUCHAR ch;
    for(int ind = 0;ind < p_gulliver.GetLength();++ind)
    {
      ch = p_gulliver.GetAt(ind);
      int ch1 = ch & 0xFF;
      int ch2 = (ch >> 8);

      ch = (_TUCHAR) ((ch1 << 8) | ch2);
      p_gulliver.SetAt(ind,ch);
    }
  }
#endif

  XString CreateNonce()
  {
    // Init random generator
    clock_t now = clock();
    srand((unsigned int) now);

    XString nonce;
    for(int index = 0; index < 20; ++index)
    {
      long key = (rand() % 255) + 1;
      nonce += (_TUCHAR) key;
    }
    return nonce;
  }

public:

  TEST_METHOD(TestHashSHA1)
  {
    Crypto  crypt;
    XString buffer(message);

#ifdef UNICODE
    // Test against UTF16-LE !!
    BlefuscuToLaputa(buffer);
#endif
    XString hash = crypt.Digest(buffer,buffer.GetLength() * sizeof(TCHAR),CALG_SHA1);
    Logger::WriteMessage(_T("DIGEST Method  : CALG_SHA1"));
    Logger::WriteMessage(_T("DIGEST Expected: ") + expected1);
    Logger::WriteMessage(_T("DIGEST hashval : ") + hash);

    Assert::AreEqual(hash.GetString(),expected1.GetString());
  }

  TEST_METHOD(TestHashMD2)
  {
    Crypto  crypt;
    XString buffer(message);

#ifdef UNICODE
    // Test against UTF16-LE !!
    BlefuscuToLaputa(buffer);
#endif

    XString hash = crypt.Digest(buffer,buffer.GetLength() * sizeof(TCHAR),CALG_MD2);
    Logger::WriteMessage(_T("DIGEST Method  : CALG_MD2"));
    Logger::WriteMessage(_T("DIGEST Expected: ") + expected2);
    Logger::WriteMessage(_T("DIGEST hashval : ") + hash);

    Assert::AreEqual(hash.GetString(),expected2.GetString());
  }

  TEST_METHOD(TestHashMD4)
  {
    Crypto  crypt;
    XString buffer(message);

#ifdef UNICODE
    // Test against UTF16-LE !!
    BlefuscuToLaputa(buffer);
#endif
    crypt.SetHashMethod(CALG_MD4);
    XString hash = crypt.Digest(buffer,buffer.GetLength() * sizeof(TCHAR));
    Logger::WriteMessage(_T("DIGEST Method  : CALG_MD4"));
    Logger::WriteMessage(_T("DIGEST Expected: ") + expected3);
    Logger::WriteMessage(_T("DIGEST hashval : ") + hash);

    Assert::AreEqual(hash.GetString(),expected3.GetString());
  }

  TEST_METHOD(TestHashMD5)
  {
    Crypto  crypt;
    XString buffer(message);

#ifdef UNICODE
    // Test against UTF16-LE !!
    BlefuscuToLaputa(buffer);
#endif
    crypt.SetHashMethod(CALG_MD5);
    XString hash = crypt.Digest(buffer,buffer.GetLength() * sizeof(TCHAR));
    Logger::WriteMessage(_T("DIGEST Method  : CALG_MD5"));
    Logger::WriteMessage(_T("DIGEST Expected: ") + expected4);
    Logger::WriteMessage(_T("DIGEST hashval : ") + hash);

    Assert::AreEqual(hash.GetString(),expected4.GetString());
  }

  TEST_METHOD(TestHashSHA256)
  {
    Crypto  crypt;
    XString buffer(message);

#ifdef UNICODE
    // Test against UTF16-LE !!
    BlefuscuToLaputa(buffer);
#endif
    crypt.SetHashMethod(CALG_SHA_256);
    XString hash = crypt.Digest(buffer,buffer.GetLength() * sizeof(TCHAR));
    Logger::WriteMessage(_T("DIGEST Method  : CALG_SHA_256"));
    Logger::WriteMessage(_T("DIGEST Expected: ") + expected5);
    Logger::WriteMessage(_T("DIGEST hashval : ") + hash);

    Assert::AreEqual(hash.GetString(),expected5.GetString());
  }

  TEST_METHOD(TestHashSHA384)
  {
    Crypto  crypt;
    XString buffer(message);

#ifdef UNICODE
    // Test against UTF16-LE !!
    BlefuscuToLaputa(buffer);
#endif
    crypt.SetHashMethod(CALG_SHA_384);
    XString hash = crypt.Digest(buffer,buffer.GetLength() * sizeof(TCHAR));
    Logger::WriteMessage(_T("DIGEST Method  : CALG_SHA_384"));
    Logger::WriteMessage(_T("DIGEST Expected: ") + expected6);
    Logger::WriteMessage(_T("DIGEST hashval : ") + hash);

    Assert::AreEqual(hash.GetString(),expected6.GetString());
  }

  TEST_METHOD(TestHashSHA512)
  {
    Crypto  crypt;
    XString buffer(message);

#ifdef UNICODE
    // Test against UTF16-LE !!
    BlefuscuToLaputa(buffer);
#endif
    crypt.SetHashMethod(CALG_SHA_512);
    XString hash = crypt.Digest(buffer,buffer.GetLength() * sizeof(TCHAR));
    Logger::WriteMessage(_T("DIGEST Method  : CALG_SHA_512"));
    Logger::WriteMessage(_T("DIGEST Expected: ") + expected7);
    Logger::WriteMessage(_T("DIGEST hashval : ") + hash);

    Assert::AreEqual(hash.GetString(),expected7.GetString());
  }

  TEST_METHOD(TestNonce)
  {
    Crypto  crypt1;
    XString nonce = CreateNonce();
    XString created(_T("2022-04-25T10:23:55"));
    XString password(_T("P@$$w03d4m3")); // Password for me

    Logger::WriteMessage(_T("Nonce for SHA1"));
    Logger::WriteMessage(_T("Nonce timestamp: ") + created);
    Logger::WriteMessage(_T("Nonce password : ") + password);

    // Password text = Base64(SHA1(nonce + created + password))
    XString combined = nonce + created + password;
    XString total1   = crypt1.Digest(combined.GetString(),combined.GetLength(),CALG_SHA1);
    Crypto  crypt2;
    XString total2   = crypt2.Digest(combined.GetString(),combined.GetLength(),CALG_SHA1);

    Assert::AreEqual(total1.GetString(),total2.GetString());
  }

  TEST_METHOD(TokenProfile)
  {
    Base64 base;
    Crypto crypt;

    Logger::WriteMessage("Testing TokenProfile for SOAP Services");

    XString nonce64(_T("ZmJEiZTMUu7IqTVuvZPy7g=="));
    XString date(_T("2022-04-25T11:32:22.745Z"));
    XString password2(_T("MijnWachtwoord$$"));
    XString total3;

    //#ifdef UNICODE
    AutoCSTR cnonce64(nonce64);
    AutoCSTR cdate(date);
    AutoCSTR cpasswd(password2);
    char noncebuf[100];
    base.Decrypt((BYTE*) cnonce64.cstr(),cnonce64.size(),(BYTE*) noncebuf,100);
    char cbuffer[1024];
    sprintf_s(cbuffer,1024,"%s%s%s",noncebuf,cdate.cstr(),cpasswd.cstr());
    total3 = crypt.Digest(cbuffer,strlen(cbuffer),CALG_SHA1);
    Base64 hexbase(CRYPT_STRING_HEXRAW,0);
    XString binary = hexbase.Decrypt(total3);
    total3 = base.Encrypt(binary);
    // #else
    //   nonce = base.Decrypt(nonce64);
    //   combined = nonce + date + password2;
    //   total3 = crypt.Digest(combined.GetString(), combined.GetLength(),CALG_SHA1);
    //   Base64 hexbase(CRYPT_STRING_HEXRAW,0);
    //   XString binary = hexbase.Decrypt(total3);
    //   total3 = base.Encrypt(binary);
    // #endif

    XString expected10(_T("cvVPyMJr/9W6plAwmuG2wl27akU="));

    Assert::AreEqual(total3.GetString(),expected10.GetString());
  }

  TEST_METHOD(TestStringEncryption)
  {
    Crypto  crypt;
    XString result;
    XString password(_T("P@$$w03d4m3")); // Password for me
    XString buffer(_T("This is a somewhat longer string with an intentional message"));

    // This is what we test
    Logger::WriteMessage(_T("TESTING ENCRYPTION PROVIDERS AND METHODS OF MS-Cryptographic-providers"));
    Logger::WriteMessage(_T("======================================================================"));
    Logger::WriteMessage(_T("Loading provider: MS Cryptographic provider in AES-256 mode."));
    Logger::WriteMessage(_T("DIGEST Password : ") + password);
    Logger::WriteMessage(_T("DIGEST Buffer   : ") + buffer);
    Logger::WriteMessage(_T("Provider        : PROV_RSA_AES"));
    Logger::WriteMessage(_T("Password-hash   : CALG_SHA_256"));
    Logger::WriteMessage(_T("Encryption      : CALG_AES_256"));

    result = crypt.Encryption(buffer,password);

    Logger::WriteMessage(_T("Expected        : ") + expected8);
    Logger::WriteMessage(_T("ENCRYPTION      : ") + result);

    result = crypt.Decryption(result,password);

    Logger::WriteMessage(_T("DECRYPTION     : ") + result);

    Assert::AreEqual(result.GetString(),buffer.GetString());
  }

  TEST_METHOD(TestFastStringEncryption)
  {
    Crypto  crypt;
    XString result;
    XString password(_T("P@$$w03d4m3")); // Password for me
    XString buffer(_T("This is a somewhat longer string with an intentional message"));

    // This is what we test
    Logger::WriteMessage(_T("TESTING ENCRYPTION PROVIDERS AND METHODS OF MS-Cryptographic-providers"));
    Logger::WriteMessage(_T("======================================================================"));
    Logger::WriteMessage(_T("Loading provider: MS Cryptographic provider in fast RC4 mode."));
    Logger::WriteMessage(_T("DIGEST Password : ") + password);
    Logger::WriteMessage(_T("DIGEST Buffer   : ") + buffer);

    Logger::WriteMessage(_T("Provider        : BCrypt"));
    Logger::WriteMessage(_T("Password-hash   : RC4"));
    Logger::WriteMessage(_T("Encryption      : RC4"));

    result = crypt.FastEncryption(buffer,password);

    Logger::WriteMessage(_T("Expected        : ") + expected9);
    Logger::WriteMessage(_T("ENCRYPTION      : ") + result);

    result = crypt.FastDecryption(result,password);

    Logger::WriteMessage(_T("DECRYPTION      : ") + result);

    Assert::AreEqual(result.GetString(),buffer.GetString());
  }

  TEST_METHOD(TestLongerStrings)
  {
    XString result;
    XString password(_T("V3ryL0ngP@$$w03dL0ng3rTh@n16Ch@r$")); // 38 chars long!
    int     errors = 0;

    Logger::WriteMessage(_T("TESTING THE MICROSOFT ENCYRPTION PROVIDER FOR ALL SORTS OF STRING"));
    Logger::WriteMessage(_T("================================================================="));


    for(int length = 1; length <= 3000; ++length)
    {
      // Construct original string
      XString original;
      for(int index = 0; index < length; ++index)
      {
        original += (TCHAR) (_T('A') + index % 26);
      }

      Crypto crypting;
      XString encrypted = crypting.Encryption(original,password);
      XString decrypted = crypting.Decryption(encrypted,password);

      if(original != decrypted)
      {
        ++errors;
        Logger::WriteMessage(_T("Error: ") + crypting.GetError());
      }
    }
    Assert::AreEqual(0,errors);
  }
};
}
