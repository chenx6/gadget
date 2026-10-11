__int64 __fastcall pb_decode_varint32_eof(__int64 a1, int *a2, _BYTE *a3)
{
  __int64 v4; // rax
  unsigned int v7; // r12d
  int v8; // r15d
  __int64 v9; // rax
  int v11; // ebp
  int v12; // edx
  char v13; // si
  const char *v14; // rax
  const char *v15; // rdx
  const char *v16; // rax
  const char *v17; // rax
  unsigned __int8 v18; // [rsp+7h] [rbp-41h] BYREF
  unsigned __int64 v19; // [rsp+8h] [rbp-40h]

  v4 = *(a1 + 16);
  v19 = __readfsqword(0x28u);
  if ( v4 != 0 )
  {
    v7 = (*a1)(a1, &v18, 1);
    v4 = *(a1 + 16);
    if ( v7 != 0 )
    {
      v8 = v18;
      v9 = v4 - 1;
      *(a1 + 16) = v9;
      if ( (v8 & 0x80u) == 0 )
      {
LABEL_4:
        *a2 = v8;
        return v7;
      }
      v8 &= 0x7Fu;
      v11 = 7;
LABEL_7:
      if ( v9 == 0 )
      {
LABEL_27:
        v16 = *(a1 + 24);
        if ( v16 == nullptr )
          v16 = "end-of-stream";
        v7 = 0;
        *(a1 + 24) = v16;
        return v7;
      }
      while ( 1 )
      {
        if ( (*a1)(a1, &v18, 1) == 0 )
        {
          v17 = *(a1 + 24);
          if ( v17 == nullptr )
            v17 = "io error";
          v7 = 0;
          *(a1 + 24) = v17;
          return v7;
        }
        v12 = v18;
        v9 = *(a1 + 16) - 1LL;
        *(a1 + 16) = v9;
        if ( v11 <= 0x1Fu )
        {
          if ( v11 != 28 )
          {
            v8 |= (v12 & 0x7F) << v11;
LABEL_25:
            v11 += 7;
            if ( (v12 & 0x80u) == 0 )
              goto LABEL_4;
            goto LABEL_26;
          }
          if ( (v12 & 0x70) != 0 && (~v12 & 0x78) != 0 )
            goto LABEL_14;
          v11 += 7;
          v8 |= v12 << 28;
          if ( (v12 & 0x80u) == 0 )
            goto LABEL_4;
        }
        else
        {
          v13 = v12 & 0x7F;
          if ( v11 <= 0x3Eu )
          {
            if ( v13 != 0 )
            {
              if ( v12 == 0xFF && v8 < 0 )
              {
                v11 += 7;
                goto LABEL_7;
              }
LABEL_14:
              v14 = *(a1 + 24);
              if ( v14 == nullptr )
                v14 = "varint overflow";
              *(a1 + 24) = v14;
              return 0;
            }
            goto LABEL_25;
          }
          if ( v13 != 0 )
          {
            if ( v8 < 0 && v12 == 1 && v11 == 63 )
              goto LABEL_4;
            goto LABEL_14;
          }
          if ( v11 != 63 )
            goto LABEL_14;
          v11 += 7;
          if ( (v12 & 0x80u) == 0 )
            goto LABEL_4;
        }
LABEL_26:
        if ( v9 == 0 )
          goto LABEL_27;
      }
    }
    v15 = *(a1 + 24);
    if ( v15 == nullptr )
      v15 = "io error";
  }
  else
  {
    v15 = *(a1 + 24);
    if ( v15 == nullptr )
      v15 = "end-of-stream";
  }
  *(a1 + 24) = v15;
  if ( a3 == nullptr || v4 != 0 )
  {
    return 0;
  }
  else
  {
    *a3 = 1;
    return 0;
  }
}
unsigned __int64 __fastcall pb_release_single_field(__int64 a1)
{
  unsigned __int8 v1; // r12
  char v3; // r13
  char v4; // r15
  void ***v6; // rdx
  void **v7; // rbp
  _QWORD *v8; // rax
  __int64 v9; // rbp
  unsigned __int16 v10; // r14
  unsigned __int16 *v11; // rax
  __int64 v12; // rdx
  void **v13; // r12
  void *v14; // rdi
  _BYTE v15[72]; // [rsp+0h] [rbp-88h] BYREF
  unsigned __int64 v16; // [rsp+48h] [rbp-40h]

  v1 = *(a1 + 30);
  v16 = __readfsqword(0x28u);
  v3 = v1 & 0x30;
  if ( (v1 & 0x30) != 0x30 || **(a1 + 48) == *(a1 + 24) )
  {
    v4 = v1 & 0xC0;
    if ( (v1 & 0xF) == 0xA )
    {
      v8 = *(a1 + 40);
      v9 = *v8;
      if ( *v8 != 0 )
      {
        if ( pb_field_iter_begin_extension(v15, *v8) != 0 )
          goto LABEL_15;
        while ( 1 )
        {
          v9 = *(v9 + 16);
          if ( v9 == 0 )
            break;
          if ( pb_field_iter_begin_extension(v15, v9) != 0 )
LABEL_15:
            pb_release_single_field(v15);
        }
      }
LABEL_4:
      if ( v4 != 0x80 )
        return v16 - __readfsqword(0x28u);
      v6 = *(a1 + 32);
      if ( v3 != 32 )
      {
        v7 = *v6;
LABEL_8:
        free(v7);
        **(a1 + 32) = 0;
        return v16 - __readfsqword(0x28u);
      }
      goto LABEL_26;
    }
    if ( (v1 & 0xE) != 8 )
      goto LABEL_4;
    if ( v4 == 64 )
      return v16 - __readfsqword(0x28u);
    v6 = *(a1 + 32);
    if ( v4 == 0x80 )
    {
      v7 = *v6;
      *(a1 + 40) = *v6;
      if ( v3 != 32 )
      {
        if ( v7 == nullptr )
          goto LABEL_8;
        v6 = v7;
        v10 = 1;
        goto LABEL_20;
      }
      if ( v7 == nullptr )
      {
LABEL_26:
        v11 = *(a1 + 48);
        v7 = *v6;
        if ( (v1 & 0xE) == 6 )
        {
          v12 = *v11;
          if ( v12 == 0 )
            goto LABEL_27;
          v13 = &v7[v12];
          do
          {
            v14 = *v7++;
            free(v14);
            *(v7 - 1) = nullptr;
          }
          while ( v13 != v7 );
          v11 = *(a1 + 48);
          v6 = *(a1 + 32);
LABEL_35:
          v7 = *v6;
        }
LABEL_27:
        *v11 = 0;
        goto LABEL_8;
      }
      v11 = *(a1 + 48);
      v10 = *v11;
      if ( *v11 == 0 )
        goto LABEL_35;
    }
    else
    {
      *(a1 + 40) = v6;
      if ( v3 != 32 )
      {
        v10 = 1;
        if ( v6 == nullptr )
          return v16 - __readfsqword(0x28u);
        goto LABEL_20;
      }
      v10 = **(a1 + 48);
      if ( v1 > 0x3Fu )
      {
        if ( v6 == nullptr )
          return v16 - __readfsqword(0x28u);
      }
      else
      {
        if ( v6 == nullptr )
          return v16 - __readfsqword(0x28u);
        if ( v10 > *(a1 + 28) )
          v10 = *(a1 + 28);
      }
      if ( v10 == 0 )
        return v16 - __readfsqword(0x28u);
    }
    v6 = *(a1 + 40);
    do
    {
LABEL_20:
      if ( v6 != nullptr )
      {
        if ( pb_field_iter_begin(v15, *(a1 + 56)) != 0 )
        {
          do
            pb_release_single_field(v15);
          while ( pb_field_iter_next(v15) != 0 );
        }
        v6 = *(a1 + 40);
      }
      v6 = (v6 + *(a1 + 26));
      *(a1 + 40) = v6;
      --v10;
    }
    while ( v10 != 0 );
    goto LABEL_4;
  }
  return v16 - __readfsqword(0x28u);
}
__int64 __fastcall pb_read(__int64 a1, __int64 a2, unsigned __int64 a3)
{
  unsigned __int64 v4; // rbp
  unsigned __int64 v5; // rax
  const char *v7; // rax
  unsigned __int64 v8; // rax
  unsigned __int64 v9; // rax
  _QWORD v10[9]; // [rsp+0h] [rbp-48h] BYREF

  v10[3] = __readfsqword(0x28u);
  if ( a3 == 0 )
    return 1;
  v4 = a3;
  if ( a2 != 0 || *a1 == buf_read )
  {
    if ( *(a1 + 16) < a3 )
    {
LABEL_9:
      v7 = *(a1 + 24);
      if ( v7 == nullptr )
        v7 = "end-of-stream";
      goto LABEL_11;
    }
    if ( (*a1)(a1, a2, a3) == 0 )
    {
LABEL_12:
      v7 = *(a1 + 24);
      if ( v7 == nullptr )
        v7 = "io error";
LABEL_11:
      *(a1 + 24) = v7;
      return 0;
    }
    v5 = 0;
    if ( *(a1 + 16) >= v4 )
      v5 = *(a1 + 16) - v4;
    *(a1 + 16) = v5;
    return 1;
  }
  if ( a3 > 0x10 )
  {
    v8 = *(a1 + 16);
    while ( v8 > 0xF )
    {
      if ( (*a1)(a1, v10, 16) == 0 )
        goto LABEL_12;
      v9 = *(a1 + 16);
      if ( v9 < 0x10 )
        v9 = 16;
      v4 -= 16LL;
      v8 = v9 - 16;
      *(a1 + 16) = v8;
      if ( v4 <= 0x10 )
        return pb_read(a1, v10);
    }
    goto LABEL_9;
  }
  return pb_read(a1, v10);
}
__int64 __fastcall pb_decode_varint(__int64 a1, _QWORD *a2)
{
  __int64 v2; // r14
  char v3; // r13
  __int64 v5; // rdx
  __int64 result; // rax
  __int64 v8; // rsi
  __int64 v9; // rdx
  char v10; // cl
  const char *v11; // rax
  unsigned __int8 v12; // [rsp+7h] [rbp-31h] BYREF
  unsigned __int64 v13; // [rsp+8h] [rbp-30h]

  v2 = 0;
  v3 = 0;
  v5 = *(a1 + 16);
  v13 = __readfsqword(0x28u);
  if ( v5 != 0 )
  {
    while ( 1 )
    {
      result = (*a1)(a1, &v12, 1);
      if ( result == 0 )
      {
        v11 = *(a1 + 24);
        if ( v11 == nullptr )
          v11 = "io error";
        goto LABEL_10;
      }
      v8 = v12;
      v9 = *(a1 + 16) - 1LL;
      *(a1 + 16) = v9;
      if ( v3 == 63 )
        break;
      v10 = v3;
      v3 += 7;
      v2 |= (v8 & 0x7F) << v10;
      if ( (v8 & 0x80u) == 0LL )
        goto LABEL_5;
      if ( v9 == 0 )
        goto LABEL_8;
    }
    v2 |= v8 << 63;
    if ( v8 <= 1u )
    {
LABEL_5:
      *a2 = v2;
      return result;
    }
    v11 = *(a1 + 24);
    if ( v11 == nullptr )
      v11 = "varint overflow";
  }
  else
  {
LABEL_8:
    v11 = *(a1 + 24);
    if ( v11 == nullptr )
      v11 = "end-of-stream";
  }
LABEL_10:
  *(a1 + 24) = v11;
  return 0;
}
__int64 __fastcall pb_skip_field(__int64 a1, unsigned int a2)
{
  __int64 result; // rax
  unsigned __int64 v4; // rdx
  const char *v5; // rax
  __int64 v6; // rdx
  __int64 v7; // rdx
  bool v8; // sf
  unsigned int v9; // [rsp+4h] [rbp-24h] BYREF
  unsigned __int64 v10; // [rsp+8h] [rbp-20h]

  v10 = __readfsqword(0x28u);
  if ( a2 != 2 )
  {
    if ( a2 <= 2 )
    {
      if ( a2 != 0 )
        return pb_read(a1, 0, 8u);
      v6 = *(a1 + 16);
      while ( 1 )
      {
        if ( v6 == 0 )
          goto LABEL_19;
        result = (*a1)(a1, &v9, 1);
        if ( result == 0 )
          break;
        v7 = *(a1 + 16);
        if ( v7 == 0 )
        {
          if ( (v9 & 0x80u) == 0 )
            return result;
LABEL_19:
          v5 = *(a1 + 24);
          if ( v5 == nullptr )
            v5 = "end-of-stream";
          goto LABEL_10;
        }
        v6 = v7 - 1;
        v8 = (v9 & 0x80u) != 0;
        *(a1 + 16) = v6;
        if ( !v8 )
          return result;
      }
      v5 = *(a1 + 24);
      if ( v5 == nullptr )
        v5 = "io error";
      goto LABEL_10;
    }
    if ( a2 != 5 )
    {
      v5 = *(a1 + 24);
      if ( v5 == nullptr )
        v5 = "invalid wire_type";
LABEL_10:
      *(a1 + 24) = v5;
      return 0;
    }
    v4 = 4;
    return pb_read(a1, 0, v4);
  }
  result = pb_decode_varint32_eof(a1, &v9, nullptr);
  if ( result != 0 )
  {
    v4 = v9;
    return pb_read(a1, 0, v4);
  }
  return result;
}
__int64 __fastcall pb_decode_inner(__m128i *a1, __int64 a2, __int64 a3, char a4)
{
  unsigned int v6; // r15d
  unsigned int v7; // r13d
  unsigned int v8; // r12d
  __int64 v9; // rax
  __int64 v10; // rbp
  __int64 v11; // r14
  char v12; // al
  __int64 (__fastcall *v13)(__m128i *, __int64, _QWORD, _QWORD); // rax
  __int64 result; // rax
  const char *v15; // rax
  const char *v16; // rax
  const char *v17; // rax
  unsigned __int16 v18; // dx
  const char *v19; // rax
  const char *v20; // rax
  int v21; // eax
  __int64 v22; // rsi
  __int64 v23; // [rsp+20h] [rbp-E8h]
  __int16 v24; // [rsp+28h] [rbp-E0h]
  __int16 v25; // [rsp+2Ah] [rbp-DEh]
  char v26; // [rsp+2Ch] [rbp-DCh]
  char v27; // [rsp+3Dh] [rbp-CBh] BYREF
  __int16 v28; // [rsp+3Eh] [rbp-CAh] BYREF
  __int64 v29[2]; // [rsp+40h] [rbp-C8h] BYREF
  __int16 v30; // [rsp+50h] [rbp-B8h]
  unsigned __int16 v31; // [rsp+54h] [rbp-B4h]
  unsigned __int16 v32; // [rsp+58h] [rbp-B0h]
  __int16 v33; // [rsp+5Ch] [rbp-ACh] BYREF
  char v34; // [rsp+5Eh] [rbp-AAh]
  __int64 *v35; // [rsp+68h] [rbp-A0h]
  __int16 *v36; // [rsp+70h] [rbp-98h]
  unsigned int v37; // [rsp+80h] [rbp-88h] BYREF
  __int64 v38; // [rsp+88h] [rbp-80h]
  unsigned __int16 v39; // [rsp+98h] [rbp-70h]
  _QWORD v40[9]; // [rsp+C0h] [rbp-48h]

  v40[1] = __readfsqword(0x28u);
  v28 = 0;
  v40[0] = 0;
  if ( pb_field_iter_begin(v29, a2) != 0 && (a4 & 1) == 0 && pb_message_set_to_defaults(v29) == 0 )
  {
    v17 = a1[1].m128i_i64[1];
    if ( v17 == nullptr )
      v17 = "failed to set defaults";
    a1[1].m128i_i64[1] = v17;
    return 0;
  }
  if ( a1[1].m128i_i64[0] == 0 )
    goto LABEL_60;
  v26 = a4;
  v6 = 0;
  v25 = 0;
  v24 = -1;
  v23 = 0;
  do
  {
    v27 = 0;
    if ( pb_decode_varint32_eof(a1, &v37, &v27) == 0 )
    {
      if ( v27 == 0 )
        return 0;
      break;
    }
    v7 = v37 & 7;
    v8 = v37 >> 3;
    if ( v37 >> 3 == 0 )
    {
      if ( (v26 & 4) == 0 )
      {
        v20 = a1[1].m128i_i64[1];
        if ( v20 == nullptr )
          v20 = "zero tag";
        a1[1].m128i_i64[1] = v20;
        return 0;
      }
      break;
    }
    if ( pb_field_iter_find(v29) != 0 && (v34 & 0xF) != 0xA )
    {
      if ( (v34 & 0x30) == 0x20 )
      {
        if ( v36 == &v33 )
        {
          if ( v24 != v30 )
          {
            if ( v28 != v25 && v24 != -1 )
            {
LABEL_37:
              v15 = a1[1].m128i_i64[1];
              if ( v15 == nullptr )
                v15 = "wrong size for fixed count field";
              a1[1].m128i_i64[1] = v15;
              return 0;
            }
            v28 = 0;
            v25 = v33;
          }
          v24 = v30;
          v36 = &v28;
        }
      }
      else if ( (v34 & 0x30) == 0 && v31 <= 0x3Fu )
      {
        *(v40 + (v31 >> 5)) |= 1 << v31;
      }
      if ( decode_field(a1, v7, v29) == 0 )
        return 0;
      goto LABEL_14;
    }
    if ( v6 != 0 )
    {
      if ( v8 < v6 )
        goto LABEL_31;
      v10 = a1[1].m128i_i64[0];
      if ( v23 == 0 )
        goto LABEL_31;
    }
    else
    {
      if ( pb_field_iter_find_extension(v29) != 0 )
      {
        v23 = *v35;
        if ( *v35 == 0 )
          goto LABEL_70;
        v6 = v32;
        if ( v8 < v32 )
        {
          if ( pb_skip_field(a1, v7) == 0 )
            return 0;
          goto LABEL_14;
        }
      }
      else if ( v23 == 0 )
      {
LABEL_70:
        v6 = 0xFFFFFFFF;
        if ( pb_skip_field(a1, v7) == 0 )
          return 0;
        goto LABEL_14;
      }
      v10 = a1[1].m128i_i64[0];
    }
    v11 = v23;
    while ( 1 )
    {
      v13 = **v11;
      if ( v13 != nullptr )
      {
        v12 = v13(a1, v11, v8, v7);
        goto LABEL_22;
      }
      if ( pb_field_iter_begin_extension(&v37, v11) == 0 )
      {
        v16 = a1[1].m128i_i64[1];
        if ( v16 == nullptr )
          v16 = "invalid extension";
        a1[1].m128i_i64[1] = v16;
        return 0;
      }
      if ( v8 == v39 && v38 != 0 )
      {
        *(v11 + 24) = 1;
        v12 = decode_field(a1, v7, &v37);
LABEL_22:
        if ( v12 == 0 )
          return 0;
      }
      v11 = *(v11 + 16);
      v9 = a1[1].m128i_i64[0];
      if ( v11 == 0 )
        break;
      if ( v10 != v9 )
        goto LABEL_15;
    }
    if ( v10 != v9 )
      continue;
LABEL_31:
    if ( pb_skip_field(a1, v7) == 0 )
      return 0;
LABEL_14:
    v9 = a1[1].m128i_i64[0];
LABEL_15:
    ;
  }
  while ( v9 != 0 );
  if ( v24 != -1 && v28 != v25 )
    goto LABEL_37;
LABEL_60:
  v18 = *(v29[0] + 34);
  result = 1;
  if ( v18 == 0 )
    return result;
  if ( v18 > 0x40u )
    v18 = 64;
  if ( v18 >> 5 == 0 )
  {
    v21 = 0;
LABEL_77:
    v22 = v21;
    result = 1;
    if ( *(v40 + v22) != 0xFFFFFFFF >> (32 - (v18 & 0x1F)) )
      goto LABEL_67;
    return result;
  }
  if ( LODWORD(v40[0]) != -1 )
  {
LABEL_67:
    v19 = a1[1].m128i_i64[1];
    if ( v19 == nullptr )
      v19 = "missing required field";
    a1[1].m128i_i64[1] = v19;
    return 0;
  }
  if ( v18 >> 5 != 2 )
  {
    if ( (v18 & 0x1F) == 0 )
      return result;
    v21 = 1;
    goto LABEL_77;
  }
  if ( HIDWORD(v40[0]) != -1 )
    goto LABEL_67;
  return result;
}
__int64 __fastcall decode_basic_field(__int64 a1, int a2, __int64 a3)
{
  unsigned int v3; // r12d
  char v4; // al
  char v6; // al
  __int16 v8; // dx
  unsigned __int64 v9; // rax
  __int64 v10; // rdx
  const char *v11; // rax
  const char *v12; // rax
  _DWORD *v14; // rbp
  unsigned __int64 v15; // rax
  _QWORD *v16; // rbp
  unsigned __int64 v17; // rax
  unsigned __int32 v18; // r13d
  unsigned __int64 v19; // rdx
  _WORD *v20; // rax
  __int64 v21; // rax
  __int64 v22; // r13
  __int64 v23; // r12
  __int64 v24; // rsi
  size_t v25; // rdx
  const char *v26; // rax
  bool *v27; // rbp
  unsigned int v28; // eax
  __int16 v29; // dx
  __int64 v30; // rdx
  __int64 v31; // rcx
  __int64 v32; // rax
  __int64 v33; // rsi
  unsigned __int8 v34; // al
  _BOOL8 v35; // rcx
  const char *v36; // rax
  const char *v37; // rax
  __int64 v38; // rdx
  __int64 (__fastcall *v39)(__m128i *, __int64, __int64); // rcx
  unsigned int v40; // eax
  const char *v41; // rax
  const char *v42; // rax
  const char *v43; // rax
  const char *v44; // rax
  const char *v45; // rax
  unsigned int v46; // [rsp+Ch] [rbp-5Ch] BYREF
  __m128i v47; // [rsp+10h] [rbp-58h] BYREF
  __m128i v48; // [rsp+20h] [rbp-48h]
  _QWORD v49[7]; // [rsp+30h] [rbp-38h] BYREF

  v4 = *(a3 + 30);
  v49[1] = __readfsqword(0x28u);
  v6 = v4 & 0xF;
  switch ( v6 )
  {
    case 0:
      if ( a2 != 0 && a2 != 255 )
        goto LABEL_16;
      v27 = *(a3 + 40);
      v3 = pb_decode_varint32_eof(a1, &v47, 0);
      if ( v3 != 0 )
        *v27 = v47.m128i_i32[0] != 0;
      return v3;
    case 1:
    case 2:
    case 3:
      LOBYTE(v3) = a2 != 255 && a2 != 0;
      if ( v3 != 0 )
        goto LABEL_16;
      if ( v6 == 2 )
      {
        v28 = pb_decode_varint(a1, &v47);
        if ( v28 == 0 )
          return v3;
        v29 = *(a3 + 26);
        if ( v29 == 8 )
        {
          **(a3 + 40) = v47.m128i_i64[0];
        }
        else
        {
          switch ( v29 )
          {
            case 1:
              v30 = v47.m128i_i64[0];
              **(a3 + 40) = v47.m128i_i8[0];
              v31 = v30;
              break;
            case 2:
              v30 = v47.m128i_i64[0];
              **(a3 + 40) = v47.m128i_i16[0];
              v31 = v30;
              break;
            case 4:
              v30 = v47.m128i_i64[0];
              **(a3 + 40) = v47.m128i_i32[0];
              v31 = v30;
              break;
            default:
              goto LABEL_116;
          }
          if ( v31 != v30 )
            goto LABEL_12;
        }
        return v28;
      }
      if ( v6 != 3 )
      {
        if ( pb_decode_varint(a1, &v47) == 0 )
          return v3;
        v8 = *(a3 + 26);
        v9 = v47.m128i_i32[0];
        if ( v8 != 8 )
        {
LABEL_7:
          switch ( v8 )
          {
            case 4:
              **(a3 + 40) = v9;
              v10 = v9;
              break;
            case 2:
              **(a3 + 40) = v9;
              v10 = v9;
              break;
            case 1:
              **(a3 + 40) = v9;
              v10 = v9;
              break;
            default:
LABEL_116:
              v41 = *(a1 + 24);
              if ( v41 == nullptr )
                v41 = "invalid data_size";
              *(a1 + 24) = v41;
              return v3;
          }
          if ( v10 != v9 )
          {
LABEL_12:
            v11 = *(a1 + 24);
            if ( v11 == nullptr )
              v11 = "integer too large";
            *(a1 + 24) = v11;
            return v3;
          }
          return 1;
        }
        v9 = v47.m128i_i64[0];
LABEL_94:
        **(a3 + 40) = v9;
        return 1;
      }
      if ( pb_decode_varint(a1, &v47) != 0 )
      {
        v9 = (v47.m128i_i64[0] >> 1) ^ -(v47.m128i_i8[0] & 1);
        v8 = *(a3 + 26);
        if ( v8 != 8 )
          goto LABEL_7;
        goto LABEL_94;
      }
      return v3;
    case 4:
      if ( a2 != 255 && a2 != 5 )
        goto LABEL_16;
      if ( *(a1 + 16) <= 3u )
        goto LABEL_67;
      v14 = *(a3 + 40);
      v3 = (*a1)(a1, v49, 4);
      if ( v3 == 0 )
        goto LABEL_86;
      v15 = *(a1 + 16);
      if ( v15 < 4 )
        v15 = 4;
      *(a1 + 16) = v15 - 4;
      *v14 = v49[0];
      return v3;
    case 5:
      if ( a2 != 255 && a2 != 1 )
        goto LABEL_16;
      if ( *(a1 + 16) <= 7u )
      {
LABEL_67:
        v12 = *(a1 + 24);
        if ( v12 == nullptr )
          v12 = "end-of-stream";
        goto LABEL_18;
      }
      v16 = *(a3 + 40);
      v3 = (*a1)(a1, v49, 8);
      if ( v3 == 0 )
      {
LABEL_86:
        v12 = *(a1 + 24);
        if ( v12 == nullptr )
          v12 = "io error";
        goto LABEL_18;
      }
      v17 = *(a1 + 16);
      if ( v17 < 8 )
        v17 = 8;
      *(a1 + 16) = v17 - 8;
      *v16 = v49[0];
      return v3;
    case 6:
      if ( a2 != 2 )
        goto LABEL_16;
      v3 = pb_decode_varint32_eof(a1, &v47, 0);
      if ( v3 == 0 )
        return 0;
      v18 = v47.m128i_i32[0];
      if ( v47.m128i_i32[0] > 0xFFFFu )
        goto LABEL_89;
      v19 = v47.m128i_u32[0] + 2LL;
      if ( (*(a3 + 30) & 0xC0) == 0x80 )
      {
        if ( *(a1 + 16) < v47.m128i_u32[0] )
          goto LABEL_99;
        if ( allocate_field(a1, *(a3 + 40), v19, 1) == 0 )
          return 0;
        v20 = **(a3 + 40);
      }
      else
      {
        if ( *(a3 + 26) < v19 )
          goto LABEL_89;
        v20 = *(a3 + 40);
      }
      *v20 = v18;
      if ( v18 == 0 )
        return v3;
      if ( *(a1 + 16) < v18 )
        goto LABEL_99;
      if ( (*a1)(a1, v20 + 1, v18) != 0 )
      {
        v21 = 0;
        if ( *(a1 + 16) >= v18 )
          v21 = *(a1 + 16) - v18;
        *(a1 + 16) = v21;
        return v3;
      }
      v45 = *(a1 + 24);
      if ( v45 == nullptr )
        v45 = "io error";
      *(a1 + 24) = v45;
      return 0;
    case 7:
      if ( a2 != 2 )
        goto LABEL_16;
      v22 = *(a3 + 40);
      if ( pb_decode_varint32_eof(a1, &v47, 0) == 0 )
        return 0;
      if ( v47.m128i_i32[0] == -1 )
      {
        v42 = *(a1 + 24);
        if ( v42 == nullptr )
          v42 = "size too large";
        *(a1 + 24) = v42;
      }
      else if ( (*(a3 + 30) & 0xC0) == 0x80 )
      {
        v23 = v47.m128i_u32[0];
        if ( *(a1 + 16) < v47.m128i_u32[0] )
        {
LABEL_99:
          v37 = *(a1 + 24);
          if ( v37 == nullptr )
            v37 = "end-of-stream";
          *(a1 + 24) = v37;
        }
        else if ( allocate_field(a1, *(a3 + 40), (v47.m128i_i32[0] + 1), 1) != 0 )
        {
          v22 = **(a3 + 40);
          goto LABEL_50;
        }
      }
      else
      {
        v23 = v47.m128i_u32[0];
        if ( *(a3 + 26) >= (v47.m128i_i32[0] + 1) )
        {
LABEL_50:
          *(v22 + v23) = 0;
          v24 = v22;
          return pb_read(a1, v24);
        }
        v44 = *(a1 + 24);
        if ( v44 == nullptr )
          v44 = "string overflow";
        *(a1 + 24) = v44;
      }
      return 0;
    case 8:
    case 9:
      if ( a2 != 2 )
        goto LABEL_16;
      if ( pb_decode_varint32_eof(a1, &v46, 0) == 0 )
        return 0;
      v32 = v46;
      v47 = *a1;
      v48 = *(a1 + 16);
      if ( v48.m128i_i64[0] < v46 )
      {
        v36 = *(a1 + 24);
        if ( v36 == nullptr )
          v36 = "parent stream too short";
        *(a1 + 24) = v36;
        return 0;
      }
      v33 = *(a3 + 56);
      *(a1 + 16) -= v46;
      v48.m128i_i64[0] = v32;
      if ( v33 == 0 )
      {
        v43 = *(a1 + 24);
        if ( v43 == nullptr )
          v43 = "invalid field descriptor";
        *(a1 + 24) = v43;
        return 0;
      }
      v34 = *(a3 + 30);
      if ( (v34 & 0xF) != 9 )
        goto LABEL_82;
      v38 = *(a3 + 48);
      if ( v38 == 0 )
        goto LABEL_82;
      v39 = *(v38 - 16);
      if ( v39 == nullptr )
        goto LABEL_82;
      v40 = v39(&v47, a3, v38 - 8);
      v3 = v40;
      if ( v48.m128i_i64[0] != 0 )
      {
        if ( v40 == 0 )
          goto LABEL_106;
        v34 = *(a3 + 30);
        v33 = *(a3 + 56);
LABEL_82:
        v35 = false;
        if ( v34 <= 0x3Fu )
          v35 = (v34 & 0x30) != 32;
        v3 = pb_decode_inner(&v47, v33, *(a3 + 40), v35);
        if ( v48.m128i_i64[0] != 0 )
        {
LABEL_106:
          if ( pb_read(&v47, 0) == 0 )
            return 0;
        }
      }
      *(a1 + 8) = v47.m128i_i64[1];
      *(a1 + 24) = v48.m128i_i64[1];
      return v3;
    case 11:
      if ( a2 != 2 )
      {
LABEL_16:
        v12 = *(a1 + 24);
        if ( v12 == nullptr )
          v12 = "wrong wire type";
LABEL_18:
        *(a1 + 24) = v12;
        return 0;
      }
      v3 = pb_decode_varint32_eof(a1, &v47, 0);
      if ( v3 == 0 )
        return 0;
      if ( v47.m128i_i32[0] > 0xFFFFu )
      {
LABEL_89:
        v12 = *(a1 + 24);
        if ( v12 == nullptr )
          v12 = "bytes overflow";
        goto LABEL_18;
      }
      v25 = *(a3 + 26);
      if ( v47.m128i_i32[0] == 0 )
      {
        memset(*(a3 + 40), 0, v25);
        return v3;
      }
      if ( v47.m128i_i32[0] != v25 )
      {
        v26 = *(a1 + 24);
        if ( v26 == nullptr )
          v26 = "incorrect fixed length bytes size";
        *(a1 + 24) = v26;
        return 0;
      }
      v24 = *(a3 + 40);
      return pb_read(a1, v24);
    default:
      v12 = *(a1 + 24);
      if ( v12 == nullptr )
        v12 = "invalid field type";
      goto LABEL_18;
  }
}
__int64 __fastcall decode_field(__m128i *a1, unsigned int a2, __int64 a3)
{
  char v5; // al
  unsigned __int8 v7; // cl
  char v8; // dl
  unsigned __int8 v9; // dl
  unsigned __int16 *v10; // r13
  unsigned __int16 v11; // cx
  const char *v12; // rax
  unsigned int v13; // ebp
  _WORD *v15; // rdx
  __int16 v16; // cx
  char v17; // si
  const char *v18; // rax
  __int64 v19; // rbp
  _BYTE *v20; // rsi
  __int64 v21; // r14
  const char *v22; // rax
  char v23; // al
  void **v24; // rsi
  _QWORD *v25; // rdi
  char v26; // al
  unsigned __int64 v27; // r14
  unsigned __int16 *v28; // r15
  unsigned __int64 v29; // r13
  unsigned __int16 v30; // ax
  _BYTE *v31; // rax
  const char *v32; // rax
  __int64 v33; // rsi
  const char *v34; // rax
  unsigned __int8 v35; // dl
  const char *v36; // rax
  char v37; // al
  unsigned __int64 v38; // rax
  unsigned __int16 v39; // dx
  unsigned __int64 v40; // rcx
  unsigned __int64 v41; // rax
  unsigned __int64 v42; // rdx
  _QWORD *v43; // rax
  int v44; // ecx
  __int64 v45; // rdi
  bool v46; // zf
  _QWORD *v47; // rdi
  char v48; // al
  const char *v49; // rax
  __int64 v50; // rax
  unsigned __int16 v51; // ax
  unsigned int v52; // eax
  __int64 v53; // rdx
  const char *v54; // rax
  unsigned int v55; // [rsp+Ch] [rbp-9Ch] BYREF
  __m128i v56; // [rsp+10h] [rbp-98h] BYREF
  __m128i v57; // [rsp+20h] [rbp-88h]
  __m128i v58; // [rsp+30h] [rbp-78h]
  __m128i v59; // [rsp+40h] [rbp-68h]
  _BYTE v60[10]; // [rsp+5Eh] [rbp-4Ah] BYREF
  unsigned __int64 v61; // [rsp+68h] [rbp-40h]

  v5 = *(a3 + 30);
  v61 = __readfsqword(0x28u);
  v7 = v5 & 0x30;
  if ( (v5 & 0x30) == 0x30 )
  {
    v15 = *(a3 + 48);
    v16 = *(a3 + 24);
    v56 = *a3;
    v57 = *(a3 + 16);
    v58 = *(a3 + 32);
    v59 = *(a3 + 48);
    if ( *v15 == 0 || *v15 == v16 )
    {
      v17 = v5 & 0xC0;
      if ( (v5 & 0xC0) == 0x40 )
        goto LABEL_26;
      if ( v17 != 0x80 )
      {
        if ( v17 == 0 )
          goto LABEL_71;
        goto LABEL_19;
      }
    }
    else
    {
      if ( pb_field_iter_find(&v56) == 0 )
      {
        v32 = a1[1].m128i_i64[1];
        if ( v32 == nullptr )
          v32 = "invalid union tag";
        a1[1].m128i_i64[1] = v32;
        return 0;
      }
      pb_release_single_field(&v56);
      v5 = *(a3 + 30);
      v8 = v5 & 0xC0;
      if ( (v5 & 0xC0) != 0x80 )
      {
        if ( v8 != 64 )
        {
LABEL_4:
          if ( v8 != 0 )
            goto LABEL_19;
          v9 = v5 & 0x30;
          if ( (v5 & 0x30) == 0x10 )
          {
            v31 = *(a3 + 48);
            if ( v31 != nullptr )
              *v31 = 1;
          }
          else if ( v9 > 0x10u )
          {
            if ( v9 == 32 )
            {
              v10 = *(a3 + 48);
              v11 = *v10;
              *(a3 + 40) = *(a3 + 32) + *v10 * *(a3 + 26);
              if ( a2 == 2 && (v5 & 0xFu) <= 5 )
              {
                v13 = pb_decode_varint32_eof(a1, &v55, nullptr);
                if ( v13 != 0 )
                {
                  v50 = v55;
                  v56 = *a1;
                  v57 = a1[1];
                  if ( v57.m128i_i64[0] >= v55 )
                  {
                    a1[1].m128i_i64[0] -= v55;
                    v57.m128i_i64[0] = v50;
                    if ( v50 != 0 )
                    {
                      v51 = *v10;
                      while ( v51 < *(a3 + 28) )
                      {
                        v52 = decode_basic_field(&v56, 255, a3);
                        v53 = v57.m128i_i64[0];
                        if ( v52 == 0 )
                          goto LABEL_129;
                        v51 = *v10 + 1;
                        *v10 = v51;
                        *(a3 + 40) += *(a3 + 26);
                        if ( v53 == 0 )
                          goto LABEL_96;
                      }
                      v53 = v57.m128i_i64[0];
                      v52 = v13;
LABEL_129:
                      if ( v53 == 0 )
                      {
                        v13 = v52;
                        goto LABEL_96;
                      }
                      goto LABEL_10;
                    }
LABEL_96:
                    a1->m128i_i64[1] = v56.m128i_i64[1];
                    a1[1].m128i_i64[1] = v57.m128i_i64[1];
                    return v13;
                  }
                  goto LABEL_116;
                }
                return 0;
              }
              *v10 = v11 + 1;
              if ( v11 >= *(a3 + 28) )
              {
LABEL_10:
                v12 = a1[1].m128i_i64[1];
                if ( v12 == nullptr )
                  v12 = "array overflow";
                a1[1].m128i_i64[1] = v12;
                return 0;
              }
              return decode_basic_field(a1, a2, a3);
            }
            if ( v9 == 48 )
            {
              v15 = *(a3 + 48);
              v16 = *(a3 + 24);
LABEL_71:
              if ( (v5 & 0xE) == 8 && *v15 != v16 )
              {
                memset(*(a3 + 40), 0, *(a3 + 26));
                v33 = *(a3 + 56);
                if ( (*(v33 + 16) != 0 || *(v33 + 24) != 0 || **(v33 + 8) != 0)
                  && pb_field_iter_begin(&v56, v33) != 0
                  && pb_message_set_to_defaults(v56.m128i_i64) == 0 )
                {
                  v54 = a1[1].m128i_i64[1];
                  if ( v54 == nullptr )
                    v54 = "failed to set defaults";
                  a1[1].m128i_i64[1] = v54;
                  return 0;
                }
                v15 = *(a3 + 48);
                v16 = *(a3 + 24);
              }
              *v15 = v16;
              return decode_basic_field(a1, a2, a3);
            }
LABEL_19:
            v18 = a1[1].m128i_i64[1];
            if ( v18 == nullptr )
              v18 = "invalid field type";
LABEL_21:
            a1[1].m128i_i64[1] = v18;
            return 0;
          }
          return decode_basic_field(a1, a2, a3);
        }
        goto LABEL_26;
      }
      **(a3 + 32) = 0;
      v35 = v5 & 0x30;
      *(a3 + 40) = 0;
      if ( (v5 & 0x30) == 0x20 )
        goto LABEL_58;
      if ( v35 <= 0x20u )
        goto LABEL_40;
      if ( v35 != 48 )
        goto LABEL_19;
    }
    if ( (v5 & 0xE) != 8 )
      goto LABEL_86;
    goto LABEL_84;
  }
  v8 = v5 & 0xC0;
  if ( (v5 & 0xC0) != 0x40 )
  {
    if ( v8 != 0x80 )
      goto LABEL_4;
    if ( v7 != 32 )
    {
      if ( v7 > 0x20u )
        goto LABEL_19;
LABEL_40:
      if ( (v5 & 0xE) != 8 )
      {
        v23 = *(a3 + 30);
        v24 = *(a3 + 32);
        goto LABEL_42;
      }
LABEL_84:
      v24 = *(a3 + 32);
      if ( *v24 == nullptr )
      {
        v23 = *(a3 + 30);
        if ( (~v23 & 0x30) == 0 )
        {
LABEL_87:
          **(a3 + 48) = *(a3 + 24);
LABEL_42:
          if ( (v23 & 0xE) == 6 )
          {
            *(a3 + 40) = v24;
            return decode_basic_field(a1, a2, a3);
          }
        }
        if ( allocate_field(a1, v24, *(a3 + 26), 1) == 0 )
          return 0;
        v25 = **(a3 + 32);
        v26 = *(a3 + 30) & 0xE;
        *(a3 + 40) = v25;
        if ( v26 == 6 )
        {
LABEL_45:
          *v25 = 0;
          return decode_basic_field(a1, a2, a3);
        }
LABEL_62:
        if ( v26 == 8 )
          memset(v25, 0, *(a3 + 26));
        return decode_basic_field(a1, a2, a3);
      }
      pb_release_single_field(a3);
LABEL_86:
      v23 = *(a3 + 30);
      v24 = *(a3 + 32);
      if ( (~v23 & 0x30) != 0 )
        goto LABEL_42;
      goto LABEL_87;
    }
LABEL_58:
    v28 = *(a3 + 48);
    v29 = *v28;
    if ( a2 == 2 && (v5 & 0xFu) <= 5 )
    {
      v13 = pb_decode_varint32_eof(a1, &v55, nullptr);
      if ( v13 != 0 )
      {
        v38 = v55;
        v56 = *a1;
        v57 = a1[1];
        if ( v57.m128i_i64[0] >= v55 )
        {
          a1[1].m128i_i64[0] -= v55;
          v57.m128i_i64[0] = v38;
          if ( v38 != 0 )
          {
            v39 = *v28;
            while ( v39 != 0xFFFF )
            {
              if ( v29 < v39 + 1 )
              {
                v40 = 0xFFFF - v29;
                v41 = (v38 - 1) / *(a3 + 26) + 1;
                v42 = v29 + v41;
                ++v29;
                if ( v41 < v40 )
                  v29 = v42;
                if ( allocate_field(&v56, *(a3 + 32), *(a3 + 26), v29) == 0 )
                {
LABEL_136:
                  v38 = v57.m128i_i64[0];
                  goto LABEL_137;
                }
                v39 = *v28;
              }
              v43 = *(a3 + 32);
              v44 = *(a3 + 26);
              v45 = v39 * v44;
              v46 = *v43 + v45 == 0;
              v47 = (*v43 + v45);
              *(a3 + 40) = v47;
              if ( v46 )
                goto LABEL_136;
              v48 = *(a3 + 30) & 0xE;
              if ( v48 == 6 )
              {
                *v47 = 0;
              }
              else if ( v48 == 8 )
              {
                memset(v47, 0, v44);
              }
              v13 = decode_basic_field(&v56, 255, a3);
              v38 = v57.m128i_i64[0];
              if ( v13 == 0 )
                goto LABEL_137;
              v39 = *v28 + 1;
              *v28 = v39;
              if ( v38 == 0 )
                goto LABEL_96;
            }
            a1[1].m128i_i64[1] = "too many array entries";
            v38 = v57.m128i_i64[0];
LABEL_137:
            if ( v38 == 0 )
            {
              v13 = 0;
              goto LABEL_96;
            }
            if ( pb_read(&v56, 0, v38) != 0 )
            {
              v13 = 0;
              goto LABEL_96;
            }
            return 0;
          }
          goto LABEL_96;
        }
LABEL_116:
        v49 = a1[1].m128i_i64[1];
        if ( v49 == nullptr )
          v49 = "parent stream too short";
        a1[1].m128i_i64[1] = v49;
      }
      return 0;
    }
    if ( v29 == 0xFFFF )
    {
      v36 = a1[1].m128i_i64[1];
      if ( v36 == nullptr )
        v36 = "too many array entries";
      a1[1].m128i_i64[1] = v36;
      return 0;
    }
    if ( allocate_field(a1, *(a3 + 32), *(a3 + 26), v29 + 1) == 0 )
      return 0;
    v30 = *v28 + 1;
    v25 = (**(a3 + 32) + *v28 * *(a3 + 26));
    *(a3 + 40) = v25;
    *v28 = v30;
    v26 = *(a3 + 30) & 0xE;
    if ( v26 == 6 )
      goto LABEL_45;
    goto LABEL_62;
  }
LABEL_26:
  if ( *(*a3 + 24LL) != 0 )
  {
    if ( a2 == 2 )
    {
      if ( pb_decode_varint32_eof(a1, &v55, nullptr) != 0 )
      {
        v27 = v55;
        v56 = *a1;
        v57 = a1[1];
        if ( v57.m128i_i64[0] >= v55 )
        {
          a1[1].m128i_i64[0] -= v55;
          for ( v57.m128i_i64[0] = v27; ; v27 = v57.m128i_i64[0] )
          {
            v13 = (*(*a3 + 24LL))(&v56, 0, a3);
            if ( v13 == 0 )
            {
              v18 = a1[1].m128i_i64[1];
              if ( v18 == nullptr )
              {
                v18 = v57.m128i_i64[1];
                if ( v57.m128i_i64[1] == 0 )
                  v18 = "callback failed";
              }
              goto LABEL_21;
            }
            if ( v57.m128i_i64[0] == 0 )
              goto LABEL_96;
            if ( v57.m128i_i64[0] >= v27 )
              break;
          }
          if ( pb_read(&v56, 0, v57.m128i_u64[0]) == 0 )
            return 0;
          goto LABEL_96;
        }
        goto LABEL_116;
      }
    }
    else
    {
      if ( a2 == 1 )
      {
        v19 = 8;
        v37 = pb_read(a1, v60, 8u);
      }
      else
      {
        if ( a2 != 5 )
        {
          if ( a2 != 0 )
          {
            v34 = a1[1].m128i_i64[1];
            if ( v34 == nullptr )
              v34 = "invalid wire_type";
            a1[1].m128i_i64[1] = v34;
          }
          else
          {
            v19 = 0;
            while ( 1 )
            {
              v20 = &v60[v19];
              v21 = v19++;
              if ( pb_read(a1, v20, 1u) == 0 )
                break;
              if ( v60[v21] >= 0 )
                goto LABEL_93;
              if ( v19 == 10 )
              {
                v22 = a1[1].m128i_i64[1];
                if ( v22 == nullptr )
                  v22 = "varint overflow";
                a1[1].m128i_i64[1] = v22;
                return 0;
              }
            }
          }
          return 0;
        }
        v19 = 4;
        v37 = pb_read(a1, v60, 4u);
      }
      if ( v37 != 0 )
      {
LABEL_93:
        v57.m128i_i128 = v19;
        v56.m128i_i64[1] = v60;
        v56.m128i_i64[0] = buf_read;
        return (*(*a3 + 24LL))(&v56, 0, a3);
      }
    }
    return 0;
  }
  return pb_skip_field(a1, a2);
}
__int64 __fastcall pb_message_set_to_defaults(__int64 *a1)
{
  __int64 v1; // rax
  __int64 v3; // rax
  unsigned int v4; // r14d
  unsigned int v5; // r13d
  unsigned __int8 v6; // al
  __int64 v7; // rbx
  _BYTE *v9; // rdx
  __int64 v10; // rsi
  _BYTE *v11; // rax
  int v12; // r14d
  unsigned int v13; // r13d
  char v14; // [rsp+Fh] [rbp-99h] BYREF
  __int64 (__fastcall *v15)(__int64, void *, size_t); // [rsp+10h] [rbp-98h] BYREF
  __int64 v16; // [rsp+18h] [rbp-90h]
  __int64 v17; // [rsp+20h] [rbp-88h]
  __int64 v18; // [rsp+28h] [rbp-80h]
  int v19[18]; // [rsp+30h] [rbp-78h] BYREF
  unsigned __int64 v20; // [rsp+78h] [rbp-30h]

  v1 = *a1;
  v20 = __readfsqword(0x28u);
  v15 = nullptr;
  v16 = 0;
  v3 = *(v1 + 16);
  v17 = 0;
  v18 = 0;
  if ( v3 != 0 )
  {
    v16 = v3;
    v14 = 0;
    v15 = buf_read;
    v17 = -1;
    if ( pb_decode_varint32_eof(&v15, v19, &v14) != 0 )
    {
      v4 = v19[0] & 7;
      v5 = v19[0] >> 3;
      goto LABEL_11;
    }
    return 0;
  }
  v4 = 0;
  v5 = 0;
LABEL_11:
  while ( 1 )
  {
    v6 = *(a1 + 30);
    if ( (v6 & 0xF) == 0xA )
      break;
    if ( v6 > 0x3Fu )
    {
      if ( (v6 & 0xC0) != 0x80 )
        goto LABEL_8;
      *a1[4] = 0;
      if ( (v6 & 0x20) == 0 )
        goto LABEL_8;
    }
    else
    {
      if ( (v6 & 0x30) == 0x10 )
      {
        v9 = a1[6];
        if ( v9 != nullptr )
          *v9 = 0;
LABEL_23:
        if ( (v6 & 0xE) == 8 && (*((v10 = a1[7]) + 16) != 0 || *(v10 + 24) != 0 || **(v10 + 8) != 0) )
        {
          if ( pb_field_iter_begin(v19, v10) != 0 && pb_message_set_to_defaults(v19) == 0 )
            return 0;
        }
        else
        {
          memset(a1[5], 0, *(a1 + 13));
        }
        goto LABEL_8;
      }
      if ( (v6 & 0x20) == 0 )
        goto LABEL_23;
    }
    *a1[6] = 0;
LABEL_8:
    if ( v5 != 0 && *(a1 + 12) == v5 )
    {
      if ( decode_field(&v15, v4, a1) == 0 )
        return 0;
      v14 = 0;
      if ( pb_decode_varint32_eof(&v15, v19, &v14) == 0 )
        return 0;
      v11 = a1[6];
      v12 = v19[0];
      if ( v11 != nullptr )
        *v11 = 0;
      v13 = v12;
      v4 = v12 & 7;
      v5 = v13 >> 3;
      if ( pb_field_iter_next(a1) == 0 )
        return 1;
    }
    else if ( pb_field_iter_next(a1) == 0 )
    {
      return 1;
    }
  }
  v7 = *a1[5];
  if ( v7 == 0 )
    goto LABEL_8;
  while ( 1 )
  {
    if ( pb_field_iter_begin_extension(v19, v7) != 0 )
    {
      *(v7 + 24) = 0;
      if ( pb_message_set_to_defaults(v19) == 0 )
        return 0;
    }
    v7 = *(v7 + 16);
    if ( v7 == 0 )
      goto LABEL_8;
  }
}
__int64 __fastcall pb_decode_ex(__m128i *a1, __int64 a2, __int64 a3, char a4)
{
  unsigned int v8; // ebp
  __int64 v10; // rax
  const char *v11; // rax
  unsigned int v12; // [rsp+Ch] [rbp-7Ch] BYREF
  __m128i v13; // [rsp+10h] [rbp-78h] BYREF
  __m128i v14; // [rsp+20h] [rbp-68h]
  unsigned __int64 v15; // [rsp+58h] [rbp-30h]

  v15 = __readfsqword(0x28u);
  if ( (a4 & 2) != 0 )
  {
    if ( pb_decode_varint32_eof(a1, &v12, nullptr) == 0 )
      return 0;
    v10 = v12;
    v13 = *a1;
    v14 = a1[1];
    if ( v14.m128i_i64[0] < v12 )
    {
      v11 = a1[1].m128i_i64[1];
      if ( v11 == nullptr )
        v11 = "parent stream too short";
      v8 = 0;
      a1[1].m128i_i64[1] = v11;
      return v8;
    }
    a1[1].m128i_i64[0] -= v12;
    v14.m128i_i64[0] = v10;
    v8 = pb_decode_inner(&v13, a2, a3, a4);
    if ( v14.m128i_i64[0] != 0 && pb_read(&v13, 0, v14.m128i_u64[0]) == 0 )
    {
LABEL_4:
      if ( a3 != 0 && pb_field_iter_begin(&v13, a2) != 0 )
      {
        do
          pb_release_single_field(&v13);
        while ( pb_field_iter_next(&v13) != 0 );
      }
      return 0;
    }
    a1->m128i_i64[1] = v13.m128i_i64[1];
    a1[1].m128i_i64[1] = v14.m128i_i64[1];
  }
  else
  {
    v8 = pb_decode_inner(a1, a2, a3, a4);
  }
  if ( v8 == 0 )
    goto LABEL_4;
  return v8;
}
