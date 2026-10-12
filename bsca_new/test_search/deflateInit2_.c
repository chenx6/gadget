__int64 __fastcall deflateInit2_(_QWORD *a1, unsigned int a2, int a3, int a4, unsigned int a5, unsigned int a6)
{
  int v6; // r12d
  unsigned int v7; // eax
  unsigned int v8; // r14d
  int v9; // r15d
  __int64 v11; // rdi
  __int64 v13; // rax
  __int64 v14; // rbx
  __int64 v15; // rsi
  unsigned int v16; // edx
  __int128 v17; // xmm0
  __int64 v18; // rdi
  __int64 v19; // rax
  __int64 v20; // rsi
  __int64 v21; // rdi
  __int64 v22; // rax
  __int64 v23; // rsi
  __int64 v24; // rdi
  __int64 v25; // rax
  __int64 v26; // rdi
  __int64 v27; // rsi
  __int64 v28; // rax
  __int64 v29; // rcx

  if ( a1 == nullptr )
    return 4294967294LL;
  v6 = a4;
  a1[6] = 0;
  if ( a1[8] == 0 )
  {
    a1[10] = 0;
    a1[8] = sub_6310;
  }
  if ( a1[9] == 0 )
    a1[9] = &sub_6320;
  v7 = 6;
  if ( a2 != -1 )
    v7 = a2;
  v8 = v7;
  if ( a4 < 0 )
  {
    if ( a4 < -15 )
      return 4294967294LL;
    v6 = -a4;
    v9 = 0;
  }
  else
  {
    v9 = 1;
    if ( a4 > 15 )
    {
      v6 = a4 - 16;
      v9 = 2;
    }
  }
  if ( a5 - 1 > 8 || a3 != 8 || v6 <= 7 || v6 > 15 || v7 > 9 || a6 > 4 || v6 == 8 && v9 != 1 )
    return 4294967294LL;
  v11 = a1[10];
  if ( v6 == 8 )
    v6 = 9;
  v13 = (a1[8])(a1: v11, a2: 1, a3: 5952);
  v14 = v13;
  if ( v13 != 0 )
  {
    a1[7] = v13;
    v15 = (1 << v6);
    *v13 = a1;
    v16 = 1 << (a5 + 7);
    *(v13 + 80) = v15;
    *(v13 + 8) = 42;
    *(v13 + 48) = v9;
    *&v17 = _mm_unpacklo_epi32(_mm_cvtsi32_si128(v16), _mm_cvtsi32_si128(a5 + 7)).m128i_u64[0];
    *(v13 + 56) = 0;
    *(v13 + 84) = v6;
    *(v13 + 88) = v15 - 1;
    v18 = a1[10];
    *(&v17 + 1) = _mm_unpacklo_epi32(_mm_cvtsi32_si128(v16 - 1), _mm_cvtsi32_si128(a5 / 3 + 3)).m128i_u64[0];
    *(v13 + 132) = v17;
    v19 = (a1[8])(a1: v18, a2: v15, a3: 2);
    v20 = *(v14 + 80);
    v21 = a1[10];
    *(v14 + 96) = v19;
    v22 = (a1[8])(a1: v21, a2: v20, a3: 2);
    v23 = *(v14 + 132);
    v24 = a1[10];
    *(v14 + 112) = v22;
    v25 = (a1[8])(a1: v24, a2: v23, a3: 2);
    v26 = a1[10];
    v27 = (1 << (a5 + 6));
    *(v14 + 120) = v25;
    *(v14 + 5896) = v27;
    *(v14 + 5944) = 0;
    v28 = (a1[8])(a1: v26, a2: v27, a3: 4);
    v29 = *(v14 + 5896);
    *(v14 + 16) = v28;
    *(v14 + 24) = 4 * v29;
    if ( *(v14 + 96) != 0 && *(v14 + 112) != 0 && *(v14 + 120) != 0 && v28 != 0 )
    {
      *(v14 + 196) = v8;
      *(v14 + 5888) = v29 + v28;
      *(v14 + 5904) = 3 * v29 - 3;
      *(v14 + 72) = 8;
      *(v14 + 200) = a6;
      return deflateReset(a1);
    }
    *(v14 + 8) = 666;
    a1[6] = "insufficient memory";
    deflateEnd(a1);
  }
  return 4294967292LL;
}