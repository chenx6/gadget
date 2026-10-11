__int64 __fastcall sub_2370(__int64 a1, __int64 *a2)
{
  __int64 v4; // rcx
  __int64 v5; // rax
  _BYTE *v6; // rdi
  _BYTE *v7; // rdx
  _BYTE *v8; // rbp
  unsigned __int64 v9; // rsi
  _BYTE *v10; // rbx
  __int64 v11; // r8
  __int64 v13; // rax
  __int64 v14; // rdx
  __int64 v15; // rcx
  __int64 v16; // rsi
  __int64 v17; // r8
  __int64 v18; // r11
  _BYTE *i; // r9
  int v20; // eax
  unsigned __int8 v21; // al
  _BYTE *v22; // rbx

  v4 = *a2;
  v5 = a2[2];
  v6 = (*a2 + v5);
  v7 = (v5 + 1);
  v8 = (*a2 + v5 + 1);
  if ( *v6 != 34 )
    goto LABEL_3;
  v9 = a2[1];
  v10 = v8;
  v11 = 0;
  if ( v7 >= v9 )
    goto LABEL_3;
  while ( *v10 != 34 )
  {
    if ( *v10 == 92 )
    {
      if ( &(++v10)[-v4] >= v9 )
        goto LABEL_3;
      ++v11;
    }
    v7 = &(++v10)[-v4];
    if ( &v10[-v4] >= v9 )
      goto LABEL_3;
  }
  if ( v7 >= v9 || (v13 = (a2[4])(v10 - v6 - v11 + 1), v18 = v13, v13 == 0) )
  {
LABEL_3:
    if ( v8 != nullptr )
      goto LABEL_4;
    return 0;
  }
  if ( v8 >= v10 )
  {
    v14 = v13;
LABEL_28:
    *v14 = 0;
    v22 = &v10[-*a2];
    *(a1 + 32) = v18;
    *(a1 + 24) = 16;
    a2[2] = (v22 + 1);
    return 1;
  }
  for ( i = v13; ; i = v14 )
  {
    LOBYTE(v20) = *v8;
    if ( *v8 != 92 )
    {
      ++v8;
      v14 = (i + 1);
      goto LABEL_17;
    }
    v20 = v8[1];
    if ( v20 > 0x5Cu )
      break;
    if ( v20 <= 0x21u )
      goto LABEL_23;
    v15 = 0x400000000002001LL;
    v14 = (v20 - 34);
    if ( !_bittest64(&v15, (v20 - 34)) )
      goto LABEL_23;
    v14 = (i + 1);
    v15 = 2;
    v8 += 2;
LABEL_17:
    *i = v20;
    if ( v8 >= v10 )
      goto LABEL_28;
  }
  v21 = v20 - 98;
  if ( v21 <= 0x13u )
    __asm { jmp     rax; switch jump }
LABEL_23:
  (a2[5])(v18, v16, v14, v15, v17, i);
LABEL_4:
  a2[2] = &v8[-*a2];
  return 0;
}
char *__fastcall sub_2700(__int64 *a1, unsigned __int64 a2)
{
  char *v3; // rdi
  unsigned __int64 v4; // rax
  __int64 v5; // r8
  unsigned __int64 v6; // rdx
  __int64 v7; // r12
  __int64 (__fastcall *v8)(char *, __int64); // rax
  __int64 v9; // rbp
  __int64 v10; // rax
  void *v12; // rax

  if ( a1 == nullptr )
    return nullptr;
  v3 = *a1;
  if ( v3 != nullptr )
  {
    v4 = a1[1];
    if ( (v4 == 0 || a1[2] < v4) && a2 < 0x80000000 )
    {
      v5 = a1[2];
      v6 = v5 + a2 + 1;
      if ( v4 >= v6 )
        return &v3[v5];
      if ( *(a1 + 8) == 0 )
      {
        if ( v6 > 0x3FFFFFFF )
        {
          v7 = 0x7FFFFFFF;
          if ( v6 >= 0x80000000 )
            return nullptr;
        }
        else
        {
          v7 = 2 * v6;
        }
        v8 = a1[7];
        if ( v8 != nullptr )
        {
          v9 = v8(v3, v7);
          if ( v9 != 0 )
          {
LABEL_11:
            v10 = a1[2];
            a1[1] = v7;
            *a1 = v9;
            return (v9 + v10);
          }
        }
        else
        {
          v12 = (a1[5])(v7);
          v9 = v12;
          if ( v12 != nullptr )
          {
            memcpy(v12, *a1, a1[2] + 1);
            (a1[6])(*a1);
            goto LABEL_11;
          }
        }
        (a1[6])(*a1);
        a1[1] = 0;
        *a1 = 0;
        return nullptr;
      }
    }
  }
  return nullptr;
}
__int64 __fastcall sub_2830(unsigned __int8 *src, __int64 *a2)
{
  unsigned __int8 *v2; // rbx
  unsigned __int8 v3; // al
  unsigned __int8 *v4; // rdx
  __int64 v5; // r13
  __int64 v6; // rdx
  size_t v7; // r15
  __int64 v8; // rbp
  char *v9; // rax
  char *v10; // r14
  void *v11; // rdi
  __int64 v12; // r9
  unsigned __int8 *v13; // r12
  unsigned __int8 v14; // al
  unsigned __int8 *v15; // rdi
  int v16; // r8d
  char *v18; // rax
  char *v19; // rax
  __int64 v20; // rbp
  __int64 v21; // [rsp+0h] [rbp-40h]
  __int64 v22; // [rsp+0h] [rbp-40h]

  if ( a2 == nullptr )
    return 0;
  v2 = src;
  if ( src == nullptr )
  {
    v18 = sub_2700(a2, 3u);
    if ( v18 != nullptr )
    {
      strcpy(v18, "\"\"");
      return 1;
    }
    return 0;
  }
  v3 = *src;
  v4 = src;
  v5 = 0;
  if ( *src == 0 )
  {
    v19 = sub_2700(a2, 3u);
    v10 = v19;
    if ( v19 != nullptr )
    {
      v20 = 2;
      v12 = 1;
      v11 = v19 + 1;
      v7 = 0;
      goto LABEL_41;
    }
    return 0;
  }
  do
  {
    while ( 1 )
    {
      if ( v3 == 34 )
        goto LABEL_12;
      if ( v3 <= 0x22u )
        break;
      if ( v3 == 92 )
        goto LABEL_12;
LABEL_8:
      v3 = *++v4;
      if ( v3 == 0 )
        goto LABEL_13;
    }
    if ( v3 <= 0xAu )
    {
      if ( v3 > 7u )
        goto LABEL_12;
LABEL_7:
      v5 += 5;
      goto LABEL_8;
    }
    if ( (v3 - 12) > 1u )
    {
      if ( v3 > 0x1Fu )
        goto LABEL_8;
      goto LABEL_7;
    }
LABEL_12:
    v3 = *++v4;
    ++v5;
  }
  while ( v3 != 0 );
LABEL_13:
  v6 = v4 - src;
  v7 = v6 + v5;
  v8 = v6;
  v9 = sub_2700(a2, v6 + v5 + 3);
  v10 = v9;
  if ( v9 == nullptr )
    return 0;
  v11 = v9 + 1;
  v12 = v7 + 1;
  v13 = (v9 + 1);
  if ( v5 != 0 )
  {
    *v9 = 34;
    v14 = *v2;
    if ( *v2 == 0 )
    {
LABEL_28:
      *&v10[v12] = 34;
      return 1;
    }
    while ( v14 == 34 || v14 <= 0x1Fu || v14 == 92 )
    {
      *v13 = 92;
      v16 = *v2;
      v15 = v13 + 1;
      if ( v16 <= 0x22u )
      {
        switch ( v16 )
        {
          case 8:
            v13[1] = 98;
            goto LABEL_19;
          case 9:
            v13[1] = 116;
            goto LABEL_19;
          case 10:
            v13[1] = 110;
            goto LABEL_19;
          case 12:
            v13[1] = 102;
            goto LABEL_19;
          case 13:
            LOBYTE(v16) = 114;
            goto LABEL_32;
          case 34:
            goto LABEL_32;
          default:
            goto LABEL_27;
        }
      }
      if ( v16 == 92 )
      {
LABEL_32:
        v13[1] = v16;
LABEL_19:
        v14 = *++v2;
        v13 = v15 + 1;
        if ( v14 == 0 )
          goto LABEL_28;
        continue;
      }
LABEL_27:
      ++v2;
      v21 = v12;
      __sprintf_chk(v15, 1, -1, "u%04x", v16);
      v14 = *v2;
      v12 = v21;
      v13 += 6;
      if ( *v2 == 0 )
        goto LABEL_28;
    }
    *v13 = v14;
    v15 = v13;
    goto LABEL_19;
  }
  v20 = v8 + 2;
LABEL_41:
  *v10 = 34;
  v22 = v12;
  memcpy(v11, v2, v7);
  v10[v22] = 34;
  v10[v20] = 0;
  return 1;
}
__int64 __fastcall sub_2BE0(__int64 a1, __m128i *a2)
{
  unsigned int v3; // eax
  const char *v5; // rdi
  unsigned __int64 v6; // r12
  char *v7; // rdi
  int v8; // r14d
  _QWORD *v10; // rbp
  char *v11; // rax
  __m128i v12; // xmm0
  char *v13; // rax
  unsigned __int64 v14; // rsi
  __int64 v15; // r12
  char *v16; // rax
  _BYTE *v17; // rdx
  __int64 v18; // r12
  double v19; // xmm0_8
  char v20; // r13
  int v21; // r8d
  char *v22; // rax
  __int64 i; // rdx
  char v24; // cl
  char *v25; // rax
  char *v26; // rax
  char *v27; // rax
  unsigned __int64 v28; // rsi
  __int64 v29; // r12
  __int64 **v30; // rbp
  char *v31; // rax
  __int32 v32; // edx
  __int64 v33; // r12
  unsigned __int64 v34; // r12
  char *v35; // rax
  __int64 v36; // r12
  __int64 v37; // r12
  char *v38; // rax
  char *v39; // rdx
  char *v40; // rax
  unsigned __int64 v41; // rcx
  unsigned __int64 j; // rdx
  unsigned __int64 v43; // rsi
  char *v44; // rax
  char *v45; // rdx
  char *v46; // rdi
  double v47; // [rsp+8h] [rbp-70h]
  double v48; // [rsp+18h] [rbp-60h] BYREF
  char v49[40]; // [rsp+20h] [rbp-58h] BYREF
  unsigned __int64 v50; // [rsp+48h] [rbp-30h]

  v50 = __readfsqword(0x28u);
  if ( a1 == 0 || a2 == nullptr )
    return 0;
  v3 = *(a1 + 24);
  if ( v3 <= 0x20 )
  {
    if ( *(a1 + 24) != 0 )
    {
      switch ( *(a1 + 24) )
      {
        case 1:
          v27 = sub_2700(a2->m128i_i64, 6u);
          if ( v27 == nullptr )
            return 0;
          strcpy(v27, "false");
          return 1;
        case 2:
          v26 = sub_2700(a2->m128i_i64, 5u);
          if ( v26 == nullptr )
            return 0;
          strcpy(v26, "true");
          return 1;
        case 4:
          v25 = sub_2700(a2->m128i_i64, 5u);
          if ( v25 == nullptr )
            return 0;
          strcpy(v25, "null");
          return 1;
        case 8:
          v19 = *(a1 + 48);
          memset(v49, 0, 26);
          v47 = v19;
          v20 = *localeconv()->decimal_point;
          v48 = 0.0;
          if ( __builtin_isnan(v47) || !__builtin_isfinite(v47) )
          {
            strcpy(v49, "null");
            v8 = 4;
          }
          else
          {
            v21 = *(a1 + 40);
            if ( v47 == v21 )
            {
              v8 = __sprintf_chk(v49, 1, 26, "%d", v21);
            }
            else
            {
              v8 = __sprintf_chk(v49, 1, 26, "%1.15g", v47);
              if ( __isoc99_sscanf(v49, "%lg", &v48) != 1
                || fmax(fabs(v48), fabs(v47)) * 2.220446049250313e-16 < fabs(v48 - v47) )
              {
                v8 = __sprintf_chk(v49, 1, 26, "%1.17g", v47);
              }
              if ( v8 > 0x19 )
                return 0;
            }
          }
          v22 = sub_2700(a2->m128i_i64, v8 + 1LL);
          if ( v22 == nullptr )
            return 0;
          if ( v8 != 0 )
          {
            for ( i = 0; i != v8; v22[i++] = v24 )
            {
              v24 = v49[i];
              if ( v20 == v24 )
                v24 = 46;
            }
            v22 += v8;
          }
          *v22 = 0;
          a2[1].m128i_i64[0] += v8;
          return 1;
        case 0x10:
          return sub_2830(*(a1 + 32), a2->m128i_i64);
        case 0x20:
          v10 = *(a1 + 16);
          v11 = sub_2700(a2->m128i_i64, 1u);
          if ( v11 == nullptr )
            return 0;
          *v11 = 91;
          v12.m128i_i64[0] = 1;
          v12.m128i_i64[1] = 1;
          a2[1] = _mm_add_epi64(v12, a2[1]);
          if ( v10 == nullptr )
            goto LABEL_18;
          break;
        default:
          return 0;
      }
      while ( sub_2BE0(v10, a2) != 0 )
      {
        if ( a2->m128i_i64[0] != 0 )
        {
          v18 = a2[1].m128i_i64[0];
          a2[1].m128i_i64[0] = strlen((v18 + a2->m128i_i64[0])) + v18;
        }
        if ( *v10 != 0 )
        {
          if ( a2[2].m128i_i32[1] != 0 )
          {
            v14 = 3;
            v15 = 2;
          }
          else
          {
            v14 = 2;
            v15 = 1;
          }
          v16 = sub_2700(a2->m128i_i64, v14);
          if ( v16 == nullptr )
            return 0;
          *v16 = 44;
          v17 = v16 + 1;
          if ( a2[2].m128i_i32[1] != 0 )
          {
            v16[1] = 32;
            v17 = v16 + 2;
          }
          *v17 = 0;
          v10 = *v10;
          a2[1].m128i_i64[0] += v15;
          if ( v10 != nullptr )
            continue;
        }
LABEL_18:
        v13 = sub_2700(a2->m128i_i64, 2u);
        if ( v13 == nullptr )
          return 0;
        *v13 = 93;
        --a2[1].m128i_i64[1];
        return 1;
      }
    }
    return 0;
  }
  if ( v3 == 64 )
  {
    if ( a2[2].m128i_i32[1] != 0 )
    {
      v28 = 3;
      v29 = 2;
    }
    else
    {
      v28 = 2;
      v29 = 1;
    }
    v30 = *(a1 + 16);
    v31 = sub_2700(a2->m128i_i64, v28);
    if ( v31 != nullptr )
    {
      *v31 = 123;
      v32 = a2[2].m128i_i32[1];
      ++a2[1].m128i_i64[1];
      if ( v32 != 0 )
      {
        v31[1] = 10;
        v32 = a2[2].m128i_i32[1];
      }
      for ( a2[1].m128i_i64[0] += v29; v30 != nullptr; v32 = a2[2].m128i_i32[1] )
      {
        if ( v32 != 0 )
        {
          v40 = sub_2700(a2->m128i_i64, a2[1].m128i_u64[1]);
          if ( v40 == nullptr )
            return 0;
          v41 = a2[1].m128i_u64[1];
          if ( v41 != 0 )
          {
            for ( j = 0; j < v41; ++j )
            {
              v40[j] = 9;
              v41 = a2[1].m128i_u64[1];
            }
          }
          a2[1].m128i_i64[0] += v41;
        }
        if ( sub_2830(v30[7], a2->m128i_i64) == 0 )
          return 0;
        if ( a2->m128i_i64[0] != 0 )
        {
          v33 = a2[1].m128i_i64[0];
          a2[1].m128i_i64[0] = strlen((v33 + a2->m128i_i64[0])) + v33;
        }
        v34 = 1;
        if ( a2[2].m128i_i32[1] != 0 )
          v34 = 2;
        v35 = sub_2700(a2->m128i_i64, v34);
        if ( v35 == nullptr )
          return 0;
        *v35 = 58;
        if ( a2[2].m128i_i32[1] != 0 )
          v35[1] = 9;
        a2[1].m128i_i64[0] += v34;
        if ( sub_2BE0(v30, a2) == 0 )
          return 0;
        if ( a2->m128i_i64[0] != 0 )
        {
          v36 = a2[1].m128i_i64[0];
          a2[1].m128i_i64[0] = strlen((v36 + a2->m128i_i64[0])) + v36;
        }
        v37 = (a2[2].m128i_i32[1] != 0) - ((*v30 == nullptr) - 1LL);
        v38 = sub_2700(a2->m128i_i64, v37 + 1);
        if ( v38 == nullptr )
          return 0;
        v39 = v38;
        if ( *v30 != nullptr )
        {
          *v38 = 44;
          v39 = v38 + 1;
        }
        if ( a2[2].m128i_i32[1] != 0 )
          *v39++ = 10;
        *v39 = 0;
        v30 = *v30;
        a2[1].m128i_i64[0] += v37;
      }
      v43 = 2;
      if ( a2[2].m128i_i32[1] != 0 )
        v43 = a2[1].m128i_i64[1] + 1;
      v44 = sub_2700(a2->m128i_i64, v43);
      if ( v44 != nullptr )
      {
        if ( a2[2].m128i_i32[1] != 0 && a2[1].m128i_i64[1] != 1 )
        {
          v45 = v44 + 1;
          do
          {
            *(v45 - 1) = 9;
            v46 = v45++;
          }
          while ( v45 - v44 - 1 < (a2[1].m128i_i64[1] - 1) );
          v44 = v46;
        }
        *v44 = 125;
        --a2[1].m128i_i64[1];
        return 1;
      }
    }
    return 0;
  }
  if ( v3 != 128 )
    return 0;
  v5 = *(a1 + 32);
  if ( v5 == nullptr )
    return 0;
  v6 = strlen(v5) + 1;
  v7 = sub_2700(a2->m128i_i64, v6);
  if ( v7 == nullptr )
    return 0;
  memcpy(v7, *(a1 + 32), v6);
  return 1;
}
__int64 __fastcall sub_3410(__int64 a1, void *a2, __int64 a3, int a4)
{
  int v6; // eax
  unsigned int v7; // r13d
  void *v8; // rdi
  __int64 v9; // rax
  _QWORD *v10; // rdx
  void *v12; // [rsp+0h] [rbp-20h]

  if ( a1 == 0 || a2 == nullptr )
    return 0;
  if ( a3 != 0 && a1 != a3 )
  {
    if ( a4 != 0 )
    {
      v6 = *(a3 + 24);
      v7 = v6 | 0x200;
      goto LABEL_7;
    }
    a2 = sub_2B90(a2);
    if ( a2 != nullptr )
    {
      v6 = *(a3 + 24);
      v7 = v6 & 0xFFFFFDFF;
LABEL_7:
      if ( (v6 & 0x200) == 0 )
      {
        v8 = *(a3 + 56);
        if ( v8 != nullptr )
        {
          v12 = a2;
          off_9018(v8);
          a2 = v12;
        }
      }
      v9 = *(a1 + 16);
      *(a3 + 56) = a2;
      *(a3 + 24) = v7;
      if ( v9 != 0 )
      {
        v10 = *(v9 + 8);
        if ( v10 != nullptr )
        {
          *v10 = a3;
          *(a3 + 8) = v10;
          *(v9 + 8) = a3;
        }
      }
      else
      {
        *(a1 + 16) = a3;
        *(a3 + 8) = a3;
        *a3 = 0;
      }
      return 1;
    }
  }
  return 0;
}
void *(**__fastcall cJSON_InitHooks(__int64 a1))(size_t size)
{
  void *(**result)(size_t); // rax
  void (*v2)(void *); // rdx

  if ( a1 == 0 )
  {
    off_9020 = &realloc;
    *&off_9010 = _mm_loadh_ps(&free_ptr);
    return &realloc;
  }
  result = *a1;
  if ( *a1 == 0 )
  {
    v2 = *(a1 + 8);
    off_9010 = &malloc;
    result = &malloc;
    if ( v2 != nullptr )
    {
      off_9018 = v2;
      off_9020 = nullptr;
      goto LABEL_8;
    }
LABEL_10:
    off_9020 = nullptr;
    off_9018 = &free;
    if ( result == &malloc )
      goto LABEL_9;
    return result;
  }
  v2 = *(a1 + 8);
  off_9010 = *a1;
  if ( v2 == nullptr )
    goto LABEL_10;
  off_9018 = v2;
  off_9020 = nullptr;
  if ( result != &malloc )
    return result;
LABEL_8:
  if ( v2 == &free )
  {
LABEL_9:
    off_9020 = &realloc;
    return &realloc;
  }
  return result;
}
void __fastcall cJSON_Delete(_QWORD *a1, __int64 a2)
{
  _QWORD *v2; // rbp
  _QWORD *v3; // rbx
  unsigned int v4; // eax
  int v5; // edx
  __int64 v6; // rdi
  void *v7; // rdi
  void *v8; // rdi

  if ( a1 != nullptr )
  {
    v2 = a1;
    do
    {
      v3 = v2;
      v2 = *v2;
      v4 = *(v3 + 6);
      BYTE1(v5) = BYTE1(v4);
      if ( (v4 & 0x100) == 0 )
      {
        v6 = v3[2];
        if ( v6 == 0 || (cJSON_Delete(v6, a2, v4), v4 = *(v3 + 6), BYTE1(v5) = BYTE1(v4), (v4 & 0x100) == 0) )
        {
          v7 = v3[4];
          if ( v7 != nullptr )
          {
            off_9018(v7);
            v3[4] = 0;
            v5 = *(v3 + 6);
          }
          else
          {
            BYTE1(v5) = BYTE1(v4);
          }
        }
      }
      if ( (v5 & 0x200) == 0 )
      {
        v8 = v3[7];
        if ( v8 != nullptr )
        {
          off_9018(v8);
          v3[7] = 0;
        }
      }
      off_9018(v3);
    }
    while ( v2 != nullptr );
  }
}
__int64 __fastcall sub_36F0(__int64 a1, __int64 a2)
{
  __int64 v2; // r13
  unsigned __int64 v4; // r12
  unsigned __int64 v5; // r15
  unsigned __int64 v7; // r14
  int v8; // eax
  const char *v9; // rdi
  int v10; // eax
  _BYTE *v11; // rdx
  char v12; // al
  unsigned __int64 v14; // rsi
  __int64 v15; // rsi
  unsigned __int64 v16; // r14
  __int64 v17; // r12
  __int64 v18; // r13
  unsigned __int64 v19; // rax
  __int64 *v20; // rsi
  __int64 *v21; // rdi
  __int64 v22; // rax
  unsigned __int64 v23; // rax
  __int64 v24; // rsi
  _QWORD *v25; // r14
  __int64 v26; // rax
  struct lconv *v27; // rax
  __int64 v28; // rsi
  unsigned __int64 v29; // rcx
  unsigned __int64 v30; // rdx
  unsigned __int64 v31; // rdx
  int v32; // r14d
  size_t v33; // r12
  unsigned __int64 v34; // rdi
  __int64 v35; // rcx
  char v36; // r13
  char *v37; // rax
  const char *v38; // r15
  char *v39; // rax
  const char *v40; // r12
  double v41; // xmm0_8
  void (__fastcall *v42)(const char *, double); // rcx
  int v43; // edx
  signed __int64 v44; // rax
  unsigned __int64 v45; // rsi
  __int64 v46; // rsi
  __int64 *v47; // r12
  __int64 v48; // rsi
  __int64 *v49; // r14
  __int64 v50; // rax
  const char *v51; // rax
  char *endptr[9]; // [rsp+10h] [rbp-48h] BYREF

  endptr[1] = __readfsqword(0x28u);
  if ( a2 == 0 )
    return 0;
  v2 = *a2;
  if ( *a2 == 0 )
    return 0;
  v4 = *(a2 + 16);
  v5 = *(a2 + 8);
  v7 = v4 + 4;
  if ( v5 >= v4 + 4 )
  {
    v8 = strncmp((v2 + v4), "null", 4u);
    v9 = (v2 + v4);
    if ( v8 == 0 )
    {
      *(a1 + 24) = 4;
      goto LABEL_18;
    }
    if ( v5 < v4 + 5 || (v10 = strncmp(v9, "false", 5u), v9 = (v2 + v4), v10 != 0) )
    {
      if ( strncmp(v9, "true", 4u) != 0 )
        goto LABEL_8;
      *(a1 + 24) = 2;
      *(a1 + 40) = 1;
LABEL_18:
      *(a2 + 16) = v7;
      return 1;
    }
    v7 = v4 + 5;
LABEL_17:
    *(a1 + 24) = 1;
    goto LABEL_18;
  }
  v7 = v4 + 5;
  if ( v5 >= v4 + 5 && strncmp((v2 + v4), "false", 5u) == 0 )
    goto LABEL_17;
LABEL_8:
  if ( v4 >= v5 )
    return 0;
  v11 = (v2 + v4);
  v12 = *(v2 + v4);
  if ( v12 == 34 )
    return sub_2370(a1, a2);
  if ( (v12 - 48) > 9u && v12 != 45 )
  {
    if ( v12 == 91 )
    {
      v45 = *(a2 + 24);
      if ( v45 > 0x3E7 )
        return 0;
      *(a2 + 24) = v45 + 1;
      if ( *v11 != 91 )
        return 0;
      *(a2 + 16) = v4 + 1;
      sub_2690(a2);
      v16 = *(a2 + 16);
      if ( v16 < v5 )
      {
        if ( *(v2 + v16) != 93 )
        {
          v47 = nullptr;
          v18 = 0;
          *(a2 + 16) = v16 - 1;
          while ( 1 )
          {
            v49 = v47;
            v50 = (*(a2 + 32))(64);
            v47 = v50;
            if ( v50 == 0 )
              goto LABEL_36;
            *v50 = 0;
            *(v50 + 16) = 0;
            *(v50 + 32) = 0;
            *(v50 + 48) = 0;
            if ( v18 != 0 )
            {
              *v49 = v50;
              *(v50 + 8) = v49;
            }
            else
            {
              v18 = v50;
            }
            ++*(a2 + 16);
            sub_2690(a2);
            if ( sub_36F0(v47, v48) == 0 )
              goto LABEL_37;
            sub_2690(a2);
            v16 = *(a2 + 16);
            if ( v16 >= *(a2 + 8) )
              goto LABEL_37;
            if ( *(*a2 + v16) != 44 )
            {
              if ( *(*a2 + v16) != 93 )
                goto LABEL_37;
              --*(a2 + 24);
              *(v18 + 8) = v47;
              goto LABEL_79;
            }
          }
        }
        *(a2 + 24) = v46;
        v18 = 0;
LABEL_79:
        *(a1 + 24) = 32;
        v7 = v16 + 1;
        *(a1 + 16) = v18;
        goto LABEL_18;
      }
    }
    else
    {
      if ( v12 != 123 )
        return 0;
      v14 = *(a2 + 24);
      if ( v14 > 0x3E7 )
        return 0;
      *(a2 + 24) = v14 + 1;
      if ( *v11 != 123 )
        return 0;
      *(a2 + 16) = v4 + 1;
      sub_2690(a2);
      v16 = *(a2 + 16);
      if ( v16 < v5 )
      {
        if ( *(v2 + v16) != 125 )
        {
          v17 = 0;
          v18 = 0;
          *(a2 + 16) = v16 - 1;
          while ( 1 )
          {
            v25 = v17;
            v26 = (*(a2 + 32))(64);
            v17 = v26;
            if ( v26 == 0 )
              break;
            *v26 = 0;
            *(v26 + 16) = 0;
            *(v26 + 32) = 0;
            *(v26 + 48) = 0;
            if ( v18 != 0 )
            {
              *v25 = v26;
              *(v26 + 8) = v25;
            }
            else
            {
              v18 = v26;
            }
            v19 = *(a2 + 16) + 1LL;
            if ( v19 >= *(a2 + 8) )
              goto LABEL_37;
            *(a2 + 16) = v19;
            sub_2690(a2);
            if ( sub_2370(v17, v20) == 0 )
              goto LABEL_37;
            sub_2690(a2);
            v22 = *(v17 + 32);
            *(v17 + 32) = 0;
            *(v17 + 56) = v22;
            v23 = *(a2 + 16);
            if ( v23 >= *(a2 + 8) )
              goto LABEL_37;
            if ( *(*a2 + v23) != 58 )
              goto LABEL_37;
            *(a2 + 16) = v23 + 1;
            sub_2690(v21);
            if ( sub_36F0(v17, v24) == 0 )
              goto LABEL_37;
            sub_2690(a2);
            v16 = *(a2 + 16);
            if ( v16 >= *(a2 + 8) )
              goto LABEL_37;
            if ( *(*a2 + v16) != 44 )
            {
              if ( *(*a2 + v16) != 125 )
                goto LABEL_37;
              --*(a2 + 24);
              *(v18 + 8) = v17;
              goto LABEL_83;
            }
          }
LABEL_36:
          if ( v18 != 0 )
LABEL_37:
            cJSON_Delete(v18);
          return 0;
        }
        *(a2 + 24) = v15;
        v18 = 0;
LABEL_83:
        *(a1 + 24) = 64;
        v7 = v16 + 1;
        *(a1 + 16) = v18;
        goto LABEL_18;
      }
    }
    *(a2 + 16) = v16 - 1;
    return 0;
  }
  endptr[0] = nullptr;
  v27 = localeconv();
  v28 = *a2;
  if ( *a2 == 0 )
    return 0;
  v29 = *(a2 + 16);
  v30 = *(a2 + 8);
  if ( v29 >= v30 )
  {
    v51 = (*(a2 + 32))(1);
    v38 = v51;
    if ( v51 == nullptr )
      return 0;
    *v51 = 0;
  }
  else
  {
    v31 = v30 - v29;
    v32 = 0;
    v33 = 0;
    v34 = v28 + v29;
    do
    {
      v35 = *(v34 + v33) - 43;
      if ( (*(v34 + v33) - 43) > 0x3Au )
        break;
      v28 = 1LL << (*(v34 + v33) - 43);
      if ( (v28 & 0x400000004007FE5LL) == 0 )
      {
        if ( *(v34 + v33) != 46 )
          break;
        v32 = 1;
      }
      ++v33;
    }
    while ( v31 != v33 );
    v36 = *v27->decimal_point;
    v37 = (*(a2 + 32))(v33 + 1, v28, v31, v35, 0x400000004007FE5LL, 1);
    v38 = v37;
    if ( v37 == nullptr )
      return 0;
    v39 = memcpy(v37, (*a2 + *(a2 + 16)), v33);
    v38[v33] = 0;
    if ( v32 != 0 && v33 != 0 )
    {
      v40 = &v38[v33];
      do
      {
        if ( *v39 == 46 )
          *v39 = v36;
        ++v39;
      }
      while ( v39 != v40 );
    }
  }
  v41 = strtod(v38, endptr);
  v42 = *(a2 + 40);
  if ( endptr[0] == v38 )
  {
    v42(v38, v41);
    return 0;
  }
  v43 = 0x7FFFFFFF;
  if ( v41 < 2147483647.0 )
  {
    v43 = 0x80000000;
    if ( v41 > -2147483648.0 )
      v43 = v41;
  }
  v44 = endptr[0] - v38;
  *(a1 + 40) = v43;
  *(a1 + 24) = 8;
  *(a1 + 48) = v41;
  *(a2 + 16) += v44;
  (v42)(v38);
  return 1;
}
void *__fastcall cJSON_PrintBuffered(__int64 a1, int a2, __int32 a3)
{
  void *v3; // rbx
  int v6; // edx
  void *result; // rax
  void *ptr[2]; // [rsp+0h] [rbp-68h] BYREF
  __int128 v9; // [rsp+10h] [rbp-58h]
  __m128i v10[2]; // [rsp+20h] [rbp-48h] BYREF
  unsigned __int64 v11; // [rsp+48h] [rbp-20h]

  v11 = __readfsqword(0x28u);
  *ptr = 0;
  v9 = 0;
  memset(v10, 0, sizeof(v10));
  if ( a2 >= 0 )
  {
    v3 = a2;
    ptr[0] = off_9010(a2);
    if ( ptr[0] != nullptr )
    {
      ptr[1] = v3;
      *&v9 = 0;
      v10[0].m128i_i32[0] = 0;
      v10[0].m128i_i32[1] = a3;
      v10[1].m128i_i64[1] = off_9020;
      *(v10 + 8) = *&off_9010;
      v6 = sub_2BE0(a1, ptr);
      result = ptr[0];
      if ( v6 != 0 )
        return result;
      off_9018(ptr[0]);
    }
  }
  return nullptr;
}
__int64 __fastcall cJSON_AddItemReferenceToArray(__m128i *a1, __m128i *a2)
{
  __m128i *v4; // rax
  __m128i v5; // xmm0
  __int64 v6; // rdx
  __m128i **v7; // rcx

  if ( a2 == nullptr )
    return 0;
  if ( a1 == nullptr )
    return 0;
  v4 = off_9010(0x40u);
  if ( v4 == nullptr )
    return 0;
  v4->m128i_i128 = 0;
  v4[1].m128i_i128 = 0;
  v4[3].m128i_i128 = 0;
  v4[2].m128i_i128 = 0;
  *v4 = *a2;
  v4[1] = a2[1];
  v4[2] = a2[2];
  v5 = a2[3];
  v4[1].m128i_i32[2] |= 0x100u;
  v4[3] = v5;
  v4[3].m128i_i64[1] = 0;
  v4->m128i_i128 = 0;
  if ( a1 == v4 )
    return 0;
  v6 = a1[1].m128i_i64[0];
  if ( v6 != 0 )
  {
    v7 = *(v6 + 8);
    if ( v7 != nullptr )
    {
      *v7 = v4;
      v4->m128i_i64[1] = v7;
      *(v6 + 8) = v4;
    }
    return 1;
  }
  else
  {
    a1[1].m128i_i64[0] = v4;
    v4->m128i_i64[1] = v4;
    v4->m128i_i64[0] = 0;
    return 1;
  }
}
__int64 __fastcall cJSON_AddItemReferenceToObject(__int64 a1, void *a2, __m128i *a3)
{
  __m128i *v6; // rax
  __int64 v7; // rdx
  __m128i v8; // xmm0

  if ( a1 == 0 )
    return 0;
  if ( a2 == nullptr )
    return 0;
  if ( a3 != nullptr )
  {
    v6 = off_9010(0x40u);
    v7 = v6;
    if ( v6 != nullptr )
    {
      v6->m128i_i128 = 0;
      v6[1].m128i_i128 = 0;
      v6[3].m128i_i128 = 0;
      v6[2].m128i_i128 = 0;
      *v6 = *a3;
      v6[1] = a3[1];
      v6[2] = a3[2];
      v8 = a3[3];
      v6[1].m128i_i32[2] |= 0x100u;
      v6[3] = v8;
      v6[3].m128i_i64[1] = 0;
      v6->m128i_i128 = 0;
    }
  }
  else
  {
    v7 = 0;
  }
  return sub_3410(a1, a2, v7, 0);
}
__int64 __fastcall cJSON_InsertItemInArray(__int64 a1, __int64 a2, __int64 a3)
{
  const double *v3; // rcx
  const double *v4; // rax
  __m128 v5; // xmm0
  _QWORD *v7; // rax

  if ( a2 < 0 )
    return 0;
  if ( a3 == 0 )
    return 0;
  a2 = a2;
  if ( a1 == 0 )
    return 0;
  v3 = *(a1 + 16);
  if ( a2 != 0 )
  {
    if ( v3 != nullptr )
    {
      v4 = *(a1 + 16);
      while ( 1 )
      {
        v4 = *v4;
        --a2;
        if ( v4 == nullptr )
          break;
        if ( a2 == 0 )
        {
          if ( v3 == v4 )
            goto LABEL_14;
          v5 = _mm_loadh_ps(v4 + 1);
          if ( *(v4 + 1) != 0 )
          {
            *a3 = v5;
            *(v4 + 1) = a3;
            **(a3 + 8) = a3;
            return 1;
          }
          return 0;
        }
      }
      if ( a3 == a1 )
        return 0;
      v7 = *(v3 + 1);
      if ( v7 != nullptr )
      {
        *v7 = a3;
        *(a3 + 8) = v7;
        *(v3 + 1) = a3;
      }
      return 1;
    }
  }
  else if ( v3 != nullptr )
  {
LABEL_14:
    *a3 = _mm_loadh_ps(v3 + 1);
    *(v3 + 1) = a3;
    *(a1 + 16) = a3;
    return 1;
  }
  if ( a3 != a1 )
  {
    *(a1 + 16) = a3;
    *(a3 + 8) = a3;
    *a3 = 0;
    return 1;
  }
  return 0;
}
__int64 __fastcall cJSON_ReplaceItemViaPointer(__int64 a1, __m128i *a2, __m128i *a3)
{
  __m128i *v4; // rcx
  __m128i v6; // xmm0
  __int64 v7; // rsi
  __m128i **v8; // rax
  __m128i *v10; // rcx

  if ( a1 == 0 )
    return 0;
  v4 = *(a1 + 16);
  if ( v4 == nullptr || a3 == nullptr || a2 == nullptr )
    return 0;
  if ( a3 != a2 )
  {
    v6 = *a2;
    v7 = a2->m128i_i64[0];
    *a3 = v6;
    if ( v7 != 0 )
    {
      *(v7 + 8) = a3;
      if ( v4 != a2 )
      {
        v8 = a3->m128i_i64[1];
        if ( v8 == nullptr )
        {
LABEL_10:
          a2->m128i_i128 = 0;
          cJSON_Delete(a2);
          return 1;
        }
LABEL_9:
        *v8 = a3;
        if ( a3->m128i_i64[0] != 0 )
          goto LABEL_10;
LABEL_17:
        v4->m128i_i64[1] = a3;
        goto LABEL_10;
      }
    }
    else if ( v4 != a2 )
    {
      v8 = a3->m128i_i64[1];
      if ( v8 == nullptr )
        goto LABEL_17;
      goto LABEL_9;
    }
    v10 = a3;
    if ( a2->m128i_i64[1] != a2 )
      v10 = a3->m128i_i64[1];
    a3->m128i_i64[1] = v10;
    *(a1 + 16) = a3;
    goto LABEL_10;
  }
  return 1;
}
__int64 __fastcall cJSON_CreateIntArray(int *a1, int a2)
{
  __int64 Array; // r12
  __int64 v5; // r13
  __int64 Number; // rax
  _QWORD *v7; // r14
  __int64 v8; // rbx
  __int64 v9; // rax
  __int64 v10; // rax

  if ( a2 < 0 )
    return 0;
  if ( a1 == nullptr )
    return 0;
  Array = cJSON_CreateArray();
  if ( Array == 0 )
    return 0;
  v5 = a2;
  if ( a2 != 0 )
  {
    Number = cJSON_CreateNumber(*a1);
    v7 = Number;
    if ( Number == 0 )
    {
LABEL_11:
      cJSON_Delete(Array);
      return 0;
    }
    *(Array + 16) = Number;
    v8 = 1;
    if ( v5 != 1 )
    {
      do
      {
        v9 = cJSON_CreateNumber(a1[v8]);
        if ( v9 == 0 )
          goto LABEL_11;
        ++v8;
        *v7 = v9;
        *(v9 + 8) = v7;
        v7 = v9;
      }
      while ( v5 != v8 );
    }
  }
  else
  {
    v7 = nullptr;
  }
  v10 = *(Array + 16);
  if ( v10 != 0 )
  {
    *(v10 + 8) = v7;
    return Array;
  }
  return Array;
}
__int64 __fastcall cJSON_CreateFloatArray(float *a1, int a2)
{
  __int64 Array; // r12
  __int64 v5; // r13
  __int64 Number; // rax
  _QWORD *v7; // r14
  __int64 v8; // rbx
  __int64 v9; // rax
  __int64 v10; // rax

  if ( a2 < 0 )
    return 0;
  if ( a1 == nullptr )
    return 0;
  Array = cJSON_CreateArray();
  if ( Array == 0 )
    return 0;
  v5 = a2;
  if ( a2 != 0 )
  {
    Number = cJSON_CreateNumber(*a1);
    v7 = Number;
    if ( Number == 0 )
    {
LABEL_11:
      cJSON_Delete(Array);
      return 0;
    }
    *(Array + 16) = Number;
    v8 = 1;
    if ( v5 != 1 )
    {
      do
      {
        v9 = cJSON_CreateNumber(a1[v8]);
        if ( v9 == 0 )
          goto LABEL_11;
        ++v8;
        *v7 = v9;
        *(v9 + 8) = v7;
        v7 = v9;
      }
      while ( v5 != v8 );
    }
  }
  else
  {
    v7 = nullptr;
  }
  v10 = *(Array + 16);
  if ( v10 != 0 )
  {
    *(v10 + 8) = v7;
    return Array;
  }
  return Array;
}
__int64 __fastcall cJSON_CreateDoubleArray(double *a1, int a2)
{
  __int64 Array; // r12
  __int64 v5; // r13
  __int64 Number; // rax
  _QWORD *v7; // rbp
  __int64 v8; // rbx
  __int64 v9; // rax
  __int64 v10; // rax

  if ( a2 < 0 )
    return 0;
  if ( a1 == nullptr )
    return 0;
  Array = cJSON_CreateArray();
  if ( Array == 0 )
    return 0;
  v5 = a2;
  if ( a2 != 0 )
  {
    Number = cJSON_CreateNumber(*a1);
    v7 = Number;
    if ( Number == 0 )
    {
LABEL_11:
      cJSON_Delete(Array);
      return 0;
    }
    *(Array + 16) = Number;
    v8 = 1;
    if ( v5 != 1 )
    {
      do
      {
        v9 = cJSON_CreateNumber(a1[v8]);
        if ( v9 == 0 )
          goto LABEL_11;
        ++v8;
        *v7 = v9;
        *(v9 + 8) = v7;
        v7 = v9;
      }
      while ( v5 != v8 );
    }
  }
  else
  {
    v7 = nullptr;
  }
  v10 = *(Array + 16);
  if ( v10 != 0 )
  {
    *(v10 + 8) = v7;
    return Array;
  }
  return Array;
}
__int64 __fastcall cJSON_CreateStringArray(void **a1, int a2)
{
  __int64 Array; // r12
  __int64 v5; // r13
  __int64 String; // rax
  _QWORD *v7; // rbp
  __int64 v8; // rbx
  __int64 v9; // rax
  __int64 v10; // rax

  if ( a2 < 0 )
    return 0;
  if ( a1 == nullptr )
    return 0;
  Array = cJSON_CreateArray();
  if ( Array == 0 )
    return 0;
  v5 = a2;
  if ( a2 != 0 )
  {
    String = cJSON_CreateString(*a1);
    v7 = String;
    if ( String == 0 )
    {
LABEL_11:
      cJSON_Delete(Array);
      return 0;
    }
    *(Array + 16) = String;
    v8 = 1;
    if ( v5 != 1 )
    {
      do
      {
        v9 = cJSON_CreateString(a1[v8]);
        if ( v9 == 0 )
          goto LABEL_11;
        ++v8;
        *v7 = v9;
        *(v9 + 8) = v7;
        v7 = v9;
      }
      while ( v5 != v8 );
    }
  }
  else
  {
    v7 = nullptr;
  }
  v10 = *(Array + 16);
  if ( v10 != 0 )
  {
    *(v10 + 8) = v7;
    return Array;
  }
  return Array;
}
void __fastcall cJSON_Minify(_BYTE *a1)
{
  char v1; // dl
  _BYTE *v2; // r10
  _BYTE *v3; // rcx
  char v4; // al
  _BYTE *v5; // rsi
  _BYTE *v6; // r9
  _BYTE *v7; // r8
  bool v8; // zf
  _BYTE *v9; // rax
  char v10; // cl
  char v11; // al
  _BYTE *v12; // rdx
  char v13; // cl

  if ( a1 != nullptr )
  {
    v1 = *a1;
    if ( *a1 != 0 )
    {
      v2 = a1;
      while ( 1 )
      {
        switch ( v1 )
        {
          case 9:
          case 10:
          case 13:
          case 32:
            goto LABEL_6;
          case 34:
            *v2 = 34;
            v4 = a1[1];
            v5 = a1 + 1;
            v3 = v2 + 1;
            if ( v4 == 0 )
              goto LABEL_9;
            while ( 1 )
            {
              v6 = v5 + 1;
              v7 = v3 + 1;
              *v3 = v4;
              v1 = v5[1];
              a1 = v5 + 1;
              v2 = v3 + 1;
              if ( v4 == 34 )
                goto LABEL_7;
              v8 = v4 == 92;
              v4 = v5[1];
              if ( v8 && v1 == 34 )
              {
                v3[1] = 34;
                v6 = v5 + 2;
                v4 = v5[2];
                v7 = v3 + 2;
              }
              v5 = v6;
              v3 = v7;
              if ( v4 == 0 )
                goto LABEL_9;
            }
          case 47:
            v1 = a1[1];
            if ( v1 == 47 )
            {
              v1 = a1[2];
              v9 = a1 + 2;
              do
              {
                if ( v1 == 0 )
                  goto LABEL_8;
                v10 = v1;
                v1 = *++v9;
              }
              while ( v10 != 10 );
              a1 = v9;
            }
            else if ( v1 == 42 )
            {
              v11 = a1[2];
              v12 = a1 + 2;
              if ( v11 == 0 )
              {
LABEL_8:
                v3 = v2;
                goto LABEL_9;
              }
              while ( 1 )
              {
                v13 = v11;
                v11 = v12[1];
                if ( v13 == 42 && v11 == 47 )
                  break;
                ++v12;
                if ( v11 == 0 )
                  goto LABEL_8;
              }
              a1 = v12 + 2;
              v1 = v12[2];
            }
            else
            {
              ++a1;
            }
LABEL_7:
            if ( v1 == 0 )
              goto LABEL_8;
            break;
          default:
            *v2++ = v1;
LABEL_6:
            v1 = *++a1;
            goto LABEL_7;
        }
      }
    }
    v3 = a1;
LABEL_9:
    *v3 = 0;
  }
}
__int64 __fastcall cJSON_Compare(__int64 a1, __int64 a2, unsigned int a3)
{
  int v4; // eax
  unsigned int v6; // ebp
  int v8; // edx
  __int64 v9; // rcx
  __int64 v10; // rax
  const char *v12; // rdi
  const char *v13; // rsi
  _QWORD *v14; // rbx
  _QWORD *v15; // r12
  __int64 *i; // r14
  _QWORD *v17; // rsi
  __int64 *v18; // r12
  _QWORD *v19; // rsi
  unsigned int v20; // eax

  if ( a1 == 0 )
    return 0;
  if ( a2 == 0 )
    return 0;
  v4 = *(a1 + 24);
  v6 = (v4 ^ *(a2 + 24));
  if ( v4 != *(a2 + 24) )
    return 0;
  v8 = v4;
  if ( v4 > 0x40u )
  {
    if ( v4 == 128 )
    {
      if ( a1 != a2 )
      {
LABEL_15:
        v12 = *(a1 + 32);
        if ( v12 != nullptr )
        {
          v13 = *(a2 + 32);
          if ( v13 != nullptr )
            return strcmp(v12, v13) == 0;
        }
        return v6;
      }
      return 1;
    }
    return v6;
  }
  if ( (v4 & 0xFC) != 0 )
  {
    v9 = 0x1000000010001011LL;
    v10 = v4 - 4;
    if ( _bittest64(&v9, (v8 - 4)) )
    {
      if ( a1 == a2 )
        return 1;
      if ( v8 <= 32 )
      {
        switch ( v10 )
        {
          case 0LL:
            return 1;
          case 4LL:
            return fmax(fabs(*(a1 + 48)), fabs(*(a2 + 48))) * 2.220446049250313e-16 >= fabs(*(a1 + 48) - *(a2 + 48));
          case 12LL:
            goto LABEL_15;
          case 28LL:
            v14 = *(a1 + 16);
            v15 = *(a2 + 16);
            if ( v14 == nullptr )
              return v14 == v15;
            break;
          default:
            return v6;
        }
        do
        {
          if ( v15 == nullptr )
            break;
          v6 = cJSON_Compare(v14, v15, a3);
          if ( v6 == 0 )
            return v6;
          v14 = *v14;
          v15 = *v15;
        }
        while ( v14 != nullptr );
        return v14 == v15;
      }
      if ( v8 == 64 )
      {
        for ( i = *(a1 + 16); i != nullptr; i = *i )
        {
          v17 = sub_2AD0(a2, i[7], a3);
          if ( v17 == nullptr )
            return v6;
          if ( cJSON_Compare(i, v17, a3) == 0 )
            return 0;
        }
        v18 = *(a2 + 16);
        if ( v18 != nullptr )
        {
          while ( 1 )
          {
            v19 = sub_2AD0(a1, v18[7], a3);
            if ( v19 == nullptr )
              break;
            v20 = cJSON_Compare(v18, v19, a3);
            if ( v20 == 0 )
              return 0;
            v18 = *v18;
            if ( v18 == nullptr )
              return v20;
          }
          return v6;
        }
        return 1;
      }
    }
    return v6;
  }
  return v4 - 1 <= 1;
}
