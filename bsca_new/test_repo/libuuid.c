__int64 __fastcall sub_2530(_DWORD *a1, _DWORD *a2, _WORD *a3, int *a4)
{
  __int64 addr; // rbx
  int v9; // eax
  __int64 v10; // rbx
  int v11; // ebx
  int v12; // eax
  __int64 v13; // rbx
  __m128i *v14; // rax
  __int64 v15; // rbx
  bool v16; // zf
  __int64 v17; // rbx
  __int16 v18; // ax
  __m128i *v19; // rax
  unsigned __int64 v20; // rax
  __time_t tv_sec; // rbx
  __int64 v22; // rax
  __suseconds_t tv_usec; // rcx
  __m128i *v24; // rax
  __m128i v25; // xmm0
  __int64 v26; // rsi
  int v27; // edx
  __int64 v28; // rbx
  int v29; // ebp
  __int64 v30; // rax
  __int64 v31; // rsi
  __int64 v32; // rax
  __int64 v33; // rbp
  __m128i *v35; // rax
  int v36; // edx
  __int64 v37; // rax
  __int16 v38; // cx
  __int16 v39; // dx
  __int16 v40; // dx
  int v41; // edx
  __m128 v42; // xmm0
  unsigned int v43; // [rsp+8h] [rbp-80h]
  __suseconds_t v44; // [rsp+10h] [rbp-78h]
  __suseconds_t v45; // [rsp+10h] [rbp-78h]
  __int16 v46; // [rsp+20h] [rbp-68h] BYREF
  int v47; // [rsp+24h] [rbp-64h] BYREF
  __int64 v48; // [rsp+28h] [rbp-60h] BYREF
  struct timeval tv; // [rsp+30h] [rbp-58h] BYREF
  unsigned __int64 v50; // [rsp+48h] [rbp-40h]

  v50 = __readfsqword(0x28u);
  addr = __tls_get_addr(&stru_8FC8);
  v9 = *(addr + 40);
  if ( v9 == -2 )
  {
    v9 = sub_24C0("/var/lib/libuuid/clock.txt", (addr + 56));
    *(addr + 40) = v9;
  }
  if ( v9 < 0 )
  {
LABEL_9:
    v14 = __tls_get_addr(&stru_8FC8);
    v43 = 0xFFFFFFFF;
    if ( _mm_or_si128(v14[4], _mm_srli_si128(v14[4], 8)).m128i_u64[0] != 0 )
      goto LABEL_18;
  }
  else
  {
    v10 = __tls_get_addr(&stru_8FC8);
    rewind(*(v10 + 56));
    v11 = *(v10 + 40);
    while ( flock(v11, 2) < 0 )
    {
      v12 = *__errno_location();
      if ( v12 != 11 && v12 != 4 )
      {
        v13 = __tls_get_addr(&stru_8FC8);
        fclose(*(v13 + 56));
        close(*(v13 + 40));
        *(v13 + 40) = -1;
        goto LABEL_9;
      }
    }
    if ( v11 < 0 )
    {
      v35 = __tls_get_addr(&stru_8FC8);
      v43 = 0;
      v20 = _mm_or_si128(v35[4], _mm_srli_si128(v35[4], 8)).m128i_u64[0];
LABEL_17:
      if ( v20 != 0 )
        goto LABEL_18;
      goto LABEL_10;
    }
    v17 = __tls_get_addr(&stru_8FC8);
    if ( __isoc23_fscanf(*(v17 + 56), "clock: %04x tv: %lu %lu adj: %d\n", &v46, &v48, &tv, &v47) == 4 )
    {
      v41 = v47;
      v18 = v46 & 0x3FFF;
      v42 = _mm_loadh_ps(&tv.tv_sec);
      *(v17 + 48) = v46 & 0x3FFF;
      *(v17 + 80) = v41;
      *(v17 + 64) = v42;
    }
    else
    {
      v18 = *(v17 + 48);
    }
    if ( v18 != 0 )
    {
      v19 = __tls_get_addr(&stru_8FC8);
      v43 = 0;
      v20 = _mm_or_si128(v19[4], _mm_srli_si128(v19[4], 8)).m128i_u64[0];
      goto LABEL_17;
    }
    v43 = 0;
    *(__tls_get_addr(&stru_8FC8) + 64) = 0;
  }
  do
  {
LABEL_10:
    v15 = __tls_get_addr(&stru_8FC8);
    sub_41C0((v15 + 48), 2u);
    v16 = (*(v15 + 48) & 0x3FFF) == 0;
    *(v15 + 48) &= 0x3FFFu;
  }
  while ( v16 );
  gettimeofday((v15 + 64), nullptr);
  --*(v15 + 64);
  do
  {
LABEL_18:
    gettimeofday(&tv, nullptr);
    tv_sec = tv.tv_sec;
    v22 = __tls_get_addr(&stru_8FC8);
    tv_usec = tv.tv_usec;
    if ( tv_sec < *(v22 + 64) )
      goto LABEL_33;
    if ( tv_sec != *(v22 + 64) )
      goto LABEL_20;
    if ( *(v22 + 72) > tv.tv_usec )
    {
LABEL_33:
      v45 = tv.tv_usec;
      v37 = __tls_get_addr(&stru_8FC8);
      v38 = *(v37 + 48);
      *(v37 + 64) = tv;
      *(v37 + 80) = 0;
      v39 = v38 + 1;
      tv_usec = v45;
      v40 = v39 & 0x3FFF;
      if ( v40 == 0 )
        v40 = 1;
      *(v37 + 48) = v40;
      goto LABEL_21;
    }
    if ( *(v22 + 72) != tv.tv_usec )
    {
LABEL_20:
      v44 = tv.tv_usec;
      v24 = __tls_get_addr(&stru_8FC8);
      v25 = tv;
      tv_usec = v44;
      v24[5].m128i_i32[0] = 0;
      v24[4] = v25;
LABEL_21:
      v26 = 0;
      v27 = 0;
      goto LABEL_22;
    }
    v36 = *(v22 + 80);
  }
  while ( v36 > 9 );
  v27 = v36 + 1;
  *(v22 + 80) = v27;
  v26 = v27;
LABEL_22:
  v28 = v26 + 10 * tv_usec + 10000000 * tv_sec + 122192928000000000LL;
  if ( a4 != nullptr && *a4 > 1 )
  {
    v29 = v27 + *a4 - 1;
    v30 = __tls_get_addr(&stru_8FC8);
    v31 = *(v30 + 72) + v29 / 10;
    *(v30 + 80) = v29 % 10;
    *(v30 + 64) += v31 / 1000000;
    *(v30 + 72) = v31 % 1000000;
  }
  v32 = __tls_get_addr(&stru_8FC8);
  v33 = v32;
  if ( *(v32 + 40) >= 0 )
  {
    rewind(*(v32 + 56));
    __fprintf_chk(
      *(v33 + 56),
      1,
      "clock: %04x tv: %016ld %08ld adj: %08d                   \n",
      *(v33 + 48),
      *(v33 + 64),
      *(v33 + 72),
      *(v33 + 80));
    fflush(*(v33 + 56));
    rewind(*(v33 + 56));
    flock(*(v33 + 40), 8);
  }
  *a1 = HIDWORD(v28);
  *a2 = v28;
  *a3 = *(__tls_get_addr(&stru_8FC8) + 48);
  return v43;
}
__int64 sub_2AC0()
{
  int v0; // eax
  int v1; // ebx
  int v2; // r12d
  int v3; // r13d
  int v5; // [rsp+0h] [rbp-478h] BYREF
  const char *v6; // [rsp+8h] [rbp-470h]
  char dest[18]; // [rsp+10h] [rbp-468h] BYREF
  int v8; // [rsp+22h] [rbp-456h]
  __int16 v9; // [rsp+26h] [rbp-452h]
  _QWORD v10[135]; // [rsp+40h] [rbp-438h] BYREF

  v10[129] = __readfsqword(0x28u);
  v0 = socket(2, 2, 0);
  if ( v0 < 0 )
    return 0xFFFFFFFFLL;
  v1 = v0;
  v6 = v10;
  memset(v10, 0, 0x400u);
  v5 = 1024;
  if ( ioctl(v0, 0x8912u, &v5) < 0 )
  {
    close(v1);
    return 0xFFFFFFFFLL;
  }
  v2 = v5;
  if ( v5 <= 0 )
  {
LABEL_11:
    close(v1);
    return 0;
  }
  else
  {
    v3 = 0;
    while ( 1 )
    {
      strncpy(dest, &v6[v3], 0x10u);
      if ( ioctl(v1, 0x8927u, dest) >= 0 && (HIBYTE(v9) | (v9 | HIBYTE(v8) | BYTE2(v8) | BYTE1(v8) | v8)) != 0 )
        break;
      v3 += 40;
      if ( v2 <= v3 )
        goto LABEL_11;
    }
    dword_9038 = v8;
    word_903C = v9;
    close(v1);
    return 1;
  }
}
__int64 __fastcall _uuid_generate_time_cont(__int64 a1, _DWORD *a2, unsigned int a3)
{
  __int64 v4; // r12
  int v6; // r14d
  unsigned __int64 v7; // rcx
  __int64 v8; // rdx
  __int64 v9; // r12
  unsigned __int64 v10; // r14
  unsigned int v11; // r12d
  unsigned __int64 v12; // rax
  __int16 v13; // dx
  _DWORD *v14; // rcx
  int v16; // edi
  int v17; // eax
  int v18; // eax
  int v19; // [rsp+Ch] [rbp-FCh]
  int v20; // [rsp+10h] [rbp-F8h]
  __time_t v21; // [rsp+18h] [rbp-F0h]
  struct timeval tv; // [rsp+20h] [rbp-E8h] BYREF
  struct stat buf; // [rsp+30h] [rbp-D8h] BYREF
  unsigned __int64 v24; // [rsp+C8h] [rbp-40h]

  v4 = a3;
  v24 = __readfsqword(0x28u);
  if ( dword_9040 == 0 )
  {
    if ( sub_2AC0() != 1 )
    {
      sub_41C0(&dword_9038, 6u);
      LOBYTE(dword_9038) = dword_9038 | 1;
    }
    dword_9040 = 1;
  }
  if ( v4 == 0 )
  {
    v14 = a2;
LABEL_11:
    v11 = sub_2530(&tv, &buf, &buf.st_ino, v14);
    LODWORD(v12) = tv.tv_sec;
    v13 = LOWORD(buf.st_ino) | 0x8000;
    goto LABEL_12;
  }
  v6 = fd;
  v19 = *a2;
  if ( fd == -1 )
    goto LABEL_22;
  gettimeofday(&tv, nullptr);
  v7 = 10000000 * tv.tv_sec + 10 * tv.tv_usec;
  v8 = qword_9028;
  if ( v6 != -2 )
    goto LABEL_5;
  v21 = 10000000 * tv.tv_sec + 10 * tv.tv_usec;
  v17 = sub_24C0("/var/lib/libuuid/clock-cont.txt");
  fd = v17;
  if ( v17 == -1 )
  {
LABEL_22:
    v14 = a2;
    LOWORD(buf.st_ino) = 0;
    goto LABEL_11;
  }
  v20 = v17;
  v18 = fstat(v17, &buf);
  v16 = v20;
  if ( v18 != 0 )
    goto LABEL_17;
  v7 = v21;
  if ( buf.st_size != 0 )
  {
    rewind(stream);
    if ( __isoc23_fscanf(stream, "cont: %lu\n", &qword_9028) != 1 )
    {
LABEL_16:
      v16 = fd;
LABEL_17:
      if ( v16 >= 0 )
        close(v16);
      if ( stream != nullptr )
        fclose(stream);
      fd = -1;
      stream = nullptr;
      goto LABEL_22;
    }
    v8 = qword_9028;
    v7 = v21;
  }
  else
  {
    qword_9028 = v21;
    v8 = v21;
  }
  qword_9020 = v8;
LABEL_5:
  v9 = 10000000 * v4;
  if ( v9 + v8 < v7 )
  {
    v8 = v7 - v9;
    qword_9028 = v7 - v9;
  }
  v10 = v8 + v19;
  if ( v10 >= v7 + 10 )
    goto LABEL_22;
  if ( v10 >= qword_9020 )
  {
    rewind(stream);
    if ( __fprintf_chk(stream, 1, "cont: %020lu                   \n", v10 + 100000000) > 29 && fflush(stream) == 0 )
    {
      qword_9020 = v10 + 100000000;
      v8 = qword_9028;
      goto LABEL_9;
    }
    goto LABEL_16;
  }
LABEL_9:
  qword_9028 = v10;
  v11 = 0;
  LODWORD(buf.st_dev) = v8 + 327237632;
  v12 = (v8 + 122192928000000000LL) >> 32;
  v13 = 0x8000;
LABEL_12:
  WORD2(buf.st_dev) = v12;
  LOWORD(buf.st_ino) = v13;
  HIWORD(buf.st_dev) = WORD1(v12) & 0xFFF | 0x1000;
  *(&buf.st_ino + 2) = dword_9038;
  HIWORD(buf.st_ino) = word_903C;
  sub_3A00(&buf, a1);
  return v11;
}
__int64 __fastcall uuid_generate_time(__m128i *a1)
{
  __int64 v2; // rbx
  __int64 v3; // r12
  int v4; // eax
  int v5; // ebx
  int v6; // r12d
  int *v7; // rax
  __int32 *v8; // r12
  int v9; // eax
  int v10; // ebx
  __int32 v11; // eax
  __int64 v12; // r14
  int v13; // r15d
  size_t v14; // r12
  int *v15; // r13
  ssize_t v16; // rax
  int v17; // eax
  time_t v18; // rax
  int v19; // r13d
  bool v20; // zf
  _DWORD *v21; // rbx
  __int64 v23; // rax
  __m128i *p_buf; // r15
  __int64 v25; // r13
  int v26; // r14d
  size_t v27; // r12
  ssize_t v28; // rax
  int v29; // eax
  __int64 v30; // r12
  int v31; // eax
  int v32; // eax
  int v33; // [rsp+1Ch] [rbp-10Ch] BYREF
  struct timespec requested_time; // [rsp+20h] [rbp-108h] BYREF
  struct sockaddr addr; // [rsp+30h] [rbp-F8h] BYREF
  __m128i buf; // [rsp+A0h] [rbp-88h] BYREF
  int v37; // [rsp+B0h] [rbp-78h]
  unsigned __int64 v38; // [rsp+E8h] [rbp-40h]

  v38 = __readfsqword(0x28u);
  if ( dword_9018 == 0 )
  {
    sub_6640(0, 0, sub_2A80);
    dword_9018 = 1;
  }
  v2 = __tls_get_addr(&stru_8FC8);
  v3 = v2;
  if ( *v2 > 0 )
  {
    v18 = time(nullptr);
    v19 = *v2;
    if ( *(v2 + 32) + 1LL < v18 )
    {
      v5 = *(v2 + 4);
      v4 = *(v3 + 4) - v19;
      *(v3 + 8) = v4;
      if ( v5 != v4 )
        goto LABEL_5;
      goto LABEL_22;
    }
    if ( v19 > 0 )
    {
      v20 = (*(v2 + 12))++ == -1;
      if ( v20 )
      {
        v20 = (*(v2 + 16))++ == 0xFFFF;
        if ( v20 )
          ++*(v2 + 18);
      }
      v21 = __tls_get_addr(&stru_8FC8);
      *v21 = v19 - 1;
      sub_3A00(v21 + 3, a1);
      if ( *v21 == 0 )
        v21[2] = v21[1];
      return 0;
    }
  }
  v4 = *(v2 + 8);
  v5 = *(v2 + 4);
  if ( v5 != v4 )
  {
LABEL_5:
    v6 = v5 / 2;
    if ( v5 / 2 > v4 && v5 > 64 )
    {
      v5 /= 2;
      *(__tls_get_addr(&stru_8FC8) + 4) = v6;
    }
    goto LABEL_8;
  }
LABEL_22:
  if ( v5 <= 0x3FFFF )
  {
    v5 *= 2;
    *(__tls_get_addr(&stru_8FC8) + 4) = v5;
  }
LABEL_8:
  v7 = __tls_get_addr(&stru_8FC8);
  v33 = 0;
  *v7 = v5;
  v8 = v7;
  v9 = socket(1, 1, 0);
  v10 = v9;
  if ( v9 >= 0 )
  {
    addr.sa_family = 1;
    strcpy(addr.sa_data, "/run/uuidd/request");
    if ( connect(v9, &addr, 0x6Eu) >= 0 )
    {
      v11 = *v8;
      buf.m128i_i8[0] = 4;
      *(buf.m128i_i32 + 1) = v11;
      if ( write(v10, &buf, 5u) > 0 )
      {
        v33 = 0;
        v12 = 0;
        v13 = 0;
        v14 = 4;
        v15 = &v33;
        while ( 1 )
        {
          while ( 1 )
          {
            v16 = read(v10, v15, v14);
            if ( v16 < 0 )
              break;
            if ( v16 == 0 )
              goto LABEL_34;
            v15 = (v15 + v16);
            v12 += v16;
            v14 -= v16;
            if ( v14 == 0 )
              goto LABEL_34;
            v13 = 0;
          }
          v17 = *__errno_location();
          if ( v17 != 11 && v17 != 4 )
            break;
          if ( v13 == 5 )
            break;
          ++v13;
          requested_time = xmmword_7120;
          nanosleep(&requested_time, nullptr);
        }
        if ( v12 == 0 )
          goto LABEL_32;
LABEL_34:
        if ( v12 >= 0 && v33 == 20 )
        {
          p_buf = &buf;
          v25 = 0;
          v26 = 0;
          buf.m128i_i128 = 0;
          v27 = 20;
          v37 = 0;
          while ( 1 )
          {
            while ( 1 )
            {
              v28 = read(v10, p_buf, v27);
              if ( v28 < 0 )
                break;
              if ( v28 == 0 )
                goto LABEL_46;
              p_buf = (p_buf + v28);
              v25 += v28;
              v27 -= v28;
              if ( v27 == 0 )
                goto LABEL_46;
              v26 = 0;
            }
            v29 = *__errno_location();
            if ( v29 != 11 && v29 != 4 )
              break;
            if ( v26 == 5 )
              break;
            ++v26;
            requested_time = xmmword_7120;
            nanosleep(&requested_time, nullptr);
          }
          if ( v25 == 0 )
          {
            v32 = *__tls_get_addr(&stru_8FC8);
            *a1 = buf;
            v37 = v32;
            close(v10);
            goto LABEL_33;
          }
LABEL_46:
          v30 = __tls_get_addr(&stru_8FC8);
          v31 = *v30;
          *a1 = buf;
          v37 = v31;
          close(v10);
          if ( v25 != 20 )
            goto LABEL_33;
          *(v30 + 32) = time(nullptr);
          sub_3C20(a1, v30 + 12);
          --*v30;
          return 0;
        }
      }
    }
LABEL_32:
    close(v10);
  }
LABEL_33:
  v23 = __tls_get_addr(&stru_8FC8);
  *v23 = 0;
  *(v23 + 32) = 0;
  *(v23 + 4) = 64;
  *(v23 + 16) = 0;
  return __uuid_generate_time(a1, nullptr);
}
unsigned __int64 __fastcall uuid_generate_time_v6(__int64 a1)
{
  _BYTE v3[2]; // [rsp+Eh] [rbp-1Ah] BYREF
  unsigned int v4; // [rsp+10h] [rbp-18h] BYREF
  unsigned int v5; // [rsp+14h] [rbp-14h] BYREF
  unsigned __int64 v6; // [rsp+18h] [rbp-10h]

  v6 = __readfsqword(0x28u);
  sub_2530(&v4, &v5, v3, 0);
  *a1 = (v4 >> 20)
      | (((v4 >> 12)
        | (((v4 >> 4)
          | ((((16 * v4) | (v5 >> 28)) | (((v5 >> 20) | (((v5 >> 12) | ((BYTE1(v5) | (v5 << 8)) << 8)) << 8)) << 8)) << 8)) << 8)) << 8);
  sub_41C0((a1 + 8), 8u);
  *(a1 + 6) = *(a1 + 6) & 0xF | 0x60;
  *(a1 + 8) = *(a1 + 8) & 0x3F | 0x80;
  return v6 - __readfsqword(0x28u);
}
__int64 __fastcall uuid_parse_range(__int64 a1, __int64 a2, __int64 a3)
{
  __int64 v3; // rbp
  int *v5; // rax
  int *v6; // rbp
  int v7; // eax
  int v8; // esi
  __int16 v10; // ax
  int v11; // ecx
  __int16 v12; // ax
  int v13; // edx
  __int16 *v14; // rbx
  int *v15; // r14
  __int16 v16; // ax
  unsigned int v18; // [rsp+Ch] [rbp-5Ch]
  int v19; // [rsp+10h] [rbp-58h] BYREF
  __int16 v20; // [rsp+14h] [rbp-54h]
  _WORD v21[7]; // [rsp+16h] [rbp-52h] BYREF
  __int16 v22; // [rsp+24h] [rbp-44h] BYREF
  char v23; // [rsp+26h] [rbp-42h]
  unsigned __int64 v24; // [rsp+28h] [rbp-40h]

  v24 = __readfsqword(0x28u);
  if ( a2 - a1 == 36 )
  {
    v3 = 0;
    while ( v3 != 36 )
    {
      if ( ((*__ctype_b_loc())[*(a1 + v3)] & 0x1000) == 0 )
        return 0xFFFFFFFFLL;
      while ( ((1LL << ++v3) & 0x842100) != 0 )
      {
        if ( *(a1 + v3) != 45 )
          return 0xFFFFFFFFLL;
      }
    }
    v5 = __errno_location();
    *v5 = 0;
    v6 = v5;
    v7 = __isoc23_strtoul(a1, 0, 16);
    v8 = *v6;
    v19 = v7;
    if ( v8 == 0 )
    {
      v10 = __isoc23_strtoul(a1 + 9, 0, 16);
      v11 = *v6;
      v20 = v10;
      if ( v11 == 0 )
      {
        v12 = __isoc23_strtoul(a1 + 14, 0, 16);
        v13 = *v6;
        v21[0] = v12;
        if ( v13 == 0 )
        {
          v21[1] = __isoc23_strtoul(a1 + 19, 0, 16);
          if ( *v6 == 0 )
          {
            v23 = 0;
            v14 = (a1 + 24);
            v15 = &v19;
            while ( 1 )
            {
              v16 = *v14;
              *v6 = 0;
              ++v14;
              v22 = v16;
              *(v15 + 10) = __isoc23_strtoul(&v22, 0, 16);
              if ( *v6 != 0 )
                break;
              v15 = (v15 + 1);
              if ( v15 == v21 )
              {
                v18 = *v6;
                sub_3A00(&v19, a3);
                return v18;
              }
            }
          }
        }
      }
    }
  }
  return 0xFFFFFFFFLL;
}
unsigned __int64 __fastcall uuid_time64(__int64 a1, unsigned __int64 *a2)
{
  __int16 v3; // ax
  unsigned __int64 v4; // rcx
  unsigned __int64 v5; // rsi
  unsigned __int64 v6; // rdx
  unsigned int v8; // [rsp+0h] [rbp-28h] BYREF
  unsigned __int16 v9; // [rsp+4h] [rbp-24h]
  unsigned __int16 v10; // [rsp+6h] [rbp-22h]
  unsigned __int64 v11; // [rsp+18h] [rbp-10h]

  v11 = __readfsqword(0x28u);
  sub_3C20(a1, &v8);
  v3 = v10 >> 12;
  if ( v10 >> 12 == 6 )
  {
    v4 = ((((v8 << 16) | v9) << 12) | v10 & 0xFFF) - 122192928000000000LL;
    v5 = v4 / 0x989680;
    goto LABEL_5;
  }
  if ( v3 == 7 )
  {
    v5 = ((v8 << 16) | v9) / 0x3E8;
    v6 = 1000 * (((v8 << 16) | v9) % 0x3E8);
  }
  else
  {
    if ( v3 == 1 )
    {
      v4 = (v8 | ((v9 | (v10 << 16) & 0xFFF0000u) << 32)) - 122192928000000000LL;
      v5 = v4 / 0x989680;
LABEL_5:
      v6 = (v4 - 10000000 * v5) / 0xA;
      goto LABEL_6;
    }
    v6 = -1;
    v5 = -1;
  }
LABEL_6:
  if ( a2 != nullptr )
  {
    *a2 = v5;
    a2[1] = v6;
  }
  return v5;
}
unsigned __int64 sub_4060()
{
  __pid_t v0; // ebx
  __uid_t v1; // eax
  _WORD *v2; // r12
  __int16 v3; // ax
  __time_t v4; // rdx
  int v5; // ebx
  int v6; // ebp
  struct timeval v8; // [rsp+0h] [rbp-38h] BYREF
  unsigned __int64 v9; // [rsp+18h] [rbp-20h]

  v9 = __readfsqword(0x28u);
  gettimeofday(&v8, nullptr);
  v0 = getpid();
  v1 = getuid();
  srandom((v0 << 16) ^ LODWORD(v8.tv_sec) ^ v1 ^ LODWORD(v8.tv_usec));
  LOWORD(v0) = getpid();
  v2 = (__tls_get_addr(&stru_8FC8) + 84);
  *v2 = LOWORD(v8.tv_sec) ^ v0;
  v3 = getppid();
  v4 = (v8.tv_sec ^ v8.tv_usec) >> 16;
  v2[1] = LOWORD(v8.tv_usec) ^ v3;
  v2[2] = v4;
  gettimeofday(&v8, nullptr);
  v5 = (LOBYTE(v8.tv_sec) ^ LOBYTE(v8.tv_usec)) & 0x1F;
  if ( ((LOBYTE(v8.tv_sec) ^ LOBYTE(v8.tv_usec)) & 0x1F) != 0 )
  {
    v6 = v5 - 1;
    if ( ((LOBYTE(v8.tv_sec) ^ LOBYTE(v8.tv_usec)) & 1) == 0 || (random(), --v5, v6 != 0) )
    {
      do
      {
        random();
        random();
        v5 -= 2;
      }
      while ( v5 != 0 );
    }
  }
  return v9 - __readfsqword(0x28u);
}
_BOOL8 __fastcall sub_41C0(char *buf, size_t nbytes)
{
  int *v4; // rax
  int *v5; // r13
  size_t v6; // rbx
  char *v7; // r12
  int i; // r14d
  int v9; // eax
  int v10; // eax
  _BOOL4 v11; // r12d
  char *v12; // r15
  char *v13; // rbx
  __int64 v14; // rax
  unsigned __int16 v15; // bx
  __int64 v16; // rax
  int v18; // r14d
  int v19; // r13d
  ssize_t v20; // rax
  __int64 addr; // rax
  unsigned __int16 v22; // bx
  int v23; // edi
  struct timespec v24; // [rsp+0h] [rbp-58h] BYREF
  unsigned __int16 xsubi[3]; // [rsp+12h] [rbp-46h] BYREF
  unsigned __int64 v26; // [rsp+18h] [rbp-40h]

  v26 = __readfsqword(0x28u);
  v4 = __errno_location();
  v5 = v4;
  if ( nbytes == 0 )
  {
    if ( *v4 == 38 )
    {
      v23 = sub_4170();
      if ( v23 >= 0 )
        close(v23);
    }
    sub_4060();
    v11 = false;
    addr = __tls_get_addr(&stru_8FC8);
    v22 = *(addr + 88);
    *xsubi = *(addr + 84);
    xsubi[2] = v22;
    *(addr + 88) = syscall(186) ^ v22;
    goto LABEL_15;
  }
  v6 = nbytes;
  v7 = buf;
LABEL_4:
  for ( i = 0; ; ++i )
  {
    *v5 = 0;
    v9 = getrandom(v7, v6, 1);
    if ( v9 > 0 )
    {
      *v5 = 0;
      v7 += v9;
      v6 -= v9;
      if ( v6 != 0 )
        goto LABEL_4;
      goto LABEL_10;
    }
    v10 = *v5;
    if ( i == 8 || v10 != 11 )
      break;
    v24 = xmmword_7140;
    nanosleep(&v24, nullptr);
  }
  if ( v10 != 38 || (v18 = 0, (v19 = sub_4170()) < 0) )
  {
LABEL_10:
    v11 = v6 != 0;
    sub_4060();
    goto LABEL_11;
  }
  while ( 1 )
  {
    v20 = read(v19, v7, v6);
    if ( v20 > 0 )
      break;
    if ( v18 == 9 )
    {
      v11 = true;
      close(v19);
      sub_4060();
      goto LABEL_11;
    }
LABEL_21:
    ++v18;
    v24 = xmmword_7140;
    nanosleep(&v24, nullptr);
  }
  while ( 1 )
  {
    v7 += v20;
    v6 -= v20;
    if ( v6 == 0 )
      break;
    v20 = read(v19, v7, v6);
    if ( v20 <= 0 )
    {
      v18 = 0;
      goto LABEL_21;
    }
  }
  v11 = false;
  close(v19);
  sub_4060();
LABEL_11:
  v12 = &buf[nbytes];
  v13 = buf;
  do
    *v13++ ^= random() >> 7;
  while ( v13 != v12 );
  v14 = __tls_get_addr(&stru_8FC8);
  v15 = *(v14 + 88);
  *xsubi = *(v14 + 84);
  xsubi[2] = v15;
  *(v14 + 88) = syscall(186) ^ v15;
  do
    *buf++ ^= jrand48(xsubi) >> 7;
  while ( buf != v12 );
LABEL_15:
  v16 = __tls_get_addr(&stru_8FC8);
  *(v16 + 84) = *xsubi;
  return v11;
}
__int64 __fastcall sub_4470(__m128i *a1, _DWORD *a2)
{
  _DWORD *v3; // rax
  __int32 v4; // r15d
  int v5; // r14d
  __int32 v6; // edi
  int v7; // edx
  int v8; // ecx
  int v9; // ebp
  int v10; // esi
  int v11; // edi
  int v12; // edx
  int v13; // r13d
  int v14; // ecx
  int v15; // r12d
  int v16; // esi
  int v17; // edi
  int v18; // ebx
  int v19; // edx
  int v20; // ecx
  int v21; // esi
  int v22; // r9d
  int v23; // edi
  int v24; // edx
  int v25; // r10d
  int v26; // ecx
  int v27; // esi
  int v28; // edx
  int v29; // ecx
  int v30; // esi
  int v31; // edx
  int v32; // ecx
  int v33; // esi
  int v34; // edx
  int v35; // ecx
  int v36; // edi
  int v37; // edx
  int v38; // esi
  int v39; // edi
  int v40; // ecx
  int v41; // edx
  int v42; // esi
  int v43; // ecx
  int v44; // edi
  int v45; // edx
  int v46; // esi
  int v47; // ecx
  int v48; // edi
  int v49; // esi
  int v50; // edx
  int v51; // ecx
  int v52; // edi
  int v53; // edx
  int v54; // ecx
  int v55; // esi
  int v56; // edx
  int v57; // ecx
  int v58; // esi
  int v59; // edx
  int v60; // ecx
  int v61; // esi
  int v62; // edi
  int v63; // edx
  __m128i v64; // xmm1
  int v65; // ecx
  __int64 result; // rax
  int v67; // [rsp+0h] [rbp-5Ch]
  int v68; // [rsp+4h] [rbp-58h]
  int v69; // [rsp+8h] [rbp-54h]
  int v70; // [rsp+Ch] [rbp-50h]
  int v71; // [rsp+10h] [rbp-4Ch]
  int v72; // [rsp+14h] [rbp-48h]
  int v73; // [rsp+18h] [rbp-44h]
  int v74; // [rsp+1Ch] [rbp-40h]
  int v75; // [rsp+20h] [rbp-3Ch]

  v3 = a2;
  v4 = a1->m128i_i32[1];
  v5 = *a2;
  v67 = a2[1];
  v6 = v4
     + __builtin_rotateleft32(
         *a2 + a1->m128i_i32[0] - 680876936 + (a1->m128i_i32[3] ^ v4 & (a1->m128i_i32[3] ^ a1->m128i_i32[2])),
         7);
  v74 = a2[3];
  v68 = a2[2];
  v7 = v6
     + __builtin_rotateleft32(
         v67 + a1->m128i_i32[3] - 389564586 + (a1->m128i_i32[2] ^ v6 & (v4 ^ a1->m128i_i32[2])),
         12);
  v8 = v7 + __builtin_rotateright32(v68 + a1->m128i_i32[2] + 606105819 + (v4 ^ v7 & (v6 ^ v4)), 15);
  v69 = a2[4];
  v9 = a2[5];
  v10 = v8 + __builtin_rotateright32(v74 + v4 - 1044525330 + (v6 ^ v8 & (v7 ^ v6)), 10);
  v11 = v10 + __builtin_rotateleft32(v69 + v6 - 176418897 + (v7 ^ v10 & (v8 ^ v7)), 7);
  v70 = v3[6];
  v12 = v11 + __builtin_rotateleft32(v9 + v7 + 1200080426 + (v8 ^ v11 & (v10 ^ v8)), 12);
  v13 = v3[7];
  v72 = v3[9];
  v14 = v12 + __builtin_rotateright32(v70 + v8 - 1473231341 + (v10 ^ v12 & (v11 ^ v10)), 15);
  v71 = v3[8];
  v15 = v3[14];
  v16 = v14 + __builtin_rotateright32(v13 + v10 - 45705983 + (v11 ^ v14 & (v12 ^ v11)), 10);
  v17 = v16 + __builtin_rotateleft32(v71 + v11 + 1770035416 + (v12 ^ v16 & (v14 ^ v12)), 7);
  v73 = v3[10];
  v18 = v3[12];
  v19 = v17 + __builtin_rotateleft32(v72 + v12 - 1958414417 + (v14 ^ v17 & (v16 ^ v14)), 12);
  v20 = v19 + __builtin_rotateright32(v73 + v14 - 42063 + (v16 ^ v19 & (v17 ^ v16)), 15);
  v75 = v3[11];
  v21 = v20 + __builtin_rotateright32(v75 + v16 - 1990404162 + (v17 ^ v20 & (v19 ^ v17)), 10);
  v22 = v3[13];
  v23 = v21 + __builtin_rotateleft32(v18 + v17 + 1804603682 + (v19 ^ v21 & (v20 ^ v19)), 7);
  v24 = v23 + __builtin_rotateleft32(v22 + v19 - 40341101 + (v20 ^ v23 & (v21 ^ v20)), 12);
  v25 = v3[15];
  v26 = v24 + __builtin_rotateright32(v15 + v20 - 1502002290 + (v21 ^ v24 & (v23 ^ v21)), 15);
  v27 = v26 + __builtin_rotateright32(v25 + v21 + 1236535329 + (v23 ^ v26 & (v24 ^ v23)), 10);
  LODWORD(v3) = v27 + __builtin_rotateleft32(v67 + v23 - 165796510 + (v26 ^ v24 & (v27 ^ v26)), 5);
  v28 = v3 + __builtin_rotateleft32(v70 + v24 - 1069501632 + (v27 ^ v26 & (v3 ^ v27)), 9);
  v29 = v28 + __builtin_rotateleft32(v75 + v26 + 643717713 + (v3 ^ v27 & (v28 ^ v3)), 14);
  v30 = v29 + __builtin_rotateright32(v5 + v27 - 373897302 + (v28 ^ v3 & (v29 ^ v28)), 12);
  LODWORD(v3) = v30 + __builtin_rotateleft32(v9 + v3 - 701558691 + (v29 ^ v28 & (v30 ^ v29)), 5);
  v31 = v3 + __builtin_rotateleft32(v73 + v28 + 38016083 + (v30 ^ v29 & (v3 ^ v30)), 9);
  v32 = v31 + __builtin_rotateleft32(v25 + v29 - 660478335 + (v3 ^ v30 & (v31 ^ v3)), 14);
  v33 = v32 + __builtin_rotateright32(v69 + v30 - 405537848 + (v31 ^ v3 & (v32 ^ v31)), 12);
  LODWORD(v3) = v33 + __builtin_rotateleft32(v72 + v3 + 568446438 + (v32 ^ v31 & (v33 ^ v32)), 5);
  v34 = v3 + __builtin_rotateleft32(v15 + v31 - 1019803690 + (v33 ^ v32 & (v3 ^ v33)), 9);
  v35 = v34 + __builtin_rotateleft32(v74 + v32 - 187363961 + (v3 ^ v33 & (v34 ^ v3)), 14);
  v36 = v35 + __builtin_rotateright32(v71 + v33 + 1163531501 + (v34 ^ v3 & (v35 ^ v34)), 12);
  LODWORD(v3) = v36 + __builtin_rotateleft32(v22 + v3 - 1444681467 + (v35 ^ v34 & (v36 ^ v35)), 5);
  v37 = v3 + __builtin_rotateleft32(v68 + v34 - 51403784 + (v36 ^ v35 & (v3 ^ v36)), 9);
  v38 = v37 + __builtin_rotateleft32(v13 + v35 + 1735328473 + (v3 ^ v36 & (v37 ^ v3)), 14);
  v39 = v38 + __builtin_rotateright32(v18 + v36 - 1926607734 + (v37 ^ v3 & (v38 ^ v37)), 12);
  v40 = v39 + __builtin_rotateleft32(v9 + v3 - 378558 + (v39 ^ v38 ^ v37), 4);
  v41 = v40 + __builtin_rotateleft32(v71 + v37 - 2022574463 + (v40 ^ v39 ^ v38), 11);
  v42 = v41 + __builtin_rotateleft32(v75 + v38 + 1839030562 + (v41 ^ v40 ^ v39), 16);
  LODWORD(v3) = v42 + __builtin_rotateright32(v15 + v39 - 35309556 + (v42 ^ v41 ^ v40), 9);
  v43 = v3 + __builtin_rotateleft32(v67 + v40 - 1530992060 + (v3 ^ v42 ^ v41), 4);
  v44 = v43 + __builtin_rotateleft32(v69 + v41 + 1272893353 + (v43 ^ v3 ^ v42), 11);
  v45 = v44 + __builtin_rotateleft32(v13 + v42 - 155497632 + (v44 ^ v43 ^ v3), 16);
  v46 = v45 + __builtin_rotateright32(v73 + v3 - 1094730640 + (v45 ^ v44 ^ v43), 9);
  v47 = v46 + __builtin_rotateleft32(v22 + v43 + 681279174 + (v46 ^ v45 ^ v44), 4);
  LODWORD(v3) = v47 + __builtin_rotateleft32(v5 + v44 - 358537222 + (v47 ^ v46 ^ v45), 11);
  v48 = v3 + __builtin_rotateleft32(v74 + v45 - 722521979 + (v3 ^ v47 ^ v46), 16);
  v49 = v48 + __builtin_rotateright32(v70 + v46 + 76029189 + (v48 ^ v3 ^ v47), 9);
  v50 = v49 + __builtin_rotateleft32(v72 + v47 - 640364487 + (v49 ^ v48 ^ v3), 4);
  v51 = v50 + __builtin_rotateleft32(v18 + v3 - 421815835 + (v50 ^ v49 ^ v48), 11);
  v52 = v51 + __builtin_rotateleft32(v25 + v48 + 530742520 + (v51 ^ v50 ^ v49), 16);
  LODWORD(v3) = v52 + __builtin_rotateright32(v68 + v49 - 995338651 + (v52 ^ v51 ^ v50), 9);
  v53 = v3 + __builtin_rotateleft32(v5 + v50 - 198630844 + (v52 ^ (v3 | ~v51)), 6);
  v54 = v53 + __builtin_rotateleft32(v13 + v51 + 1126891415 + (v3 ^ (v53 | ~v52)), 10);
  v55 = v54 + __builtin_rotateleft32(v15 + v52 - 1416354905 + (v53 ^ (v54 | ~v3)), 15);
  LODWORD(v3) = v55 + __builtin_rotateright32(v9 + v3 - 57434055 + (v54 ^ (v55 | ~v53)), 11);
  v56 = v3 + __builtin_rotateleft32(v18 + v53 + 1700485571 + (v55 ^ (v3 | ~v54)), 6);
  v57 = v56 + __builtin_rotateleft32(v74 + v54 - 1894986606 + (v3 ^ (v56 | ~v55)), 10);
  v58 = v57 + __builtin_rotateleft32(v73 + v55 - 1051523 + (v56 ^ (v57 | ~v3)), 15);
  LODWORD(v3) = v58 + __builtin_rotateright32(v67 + v3 - 2054922799 + (v57 ^ (v58 | ~v56)), 11);
  v59 = v3 + __builtin_rotateleft32(v71 + v56 + 1873313359 + (v58 ^ (v3 | ~v57)), 6);
  v60 = v59 + __builtin_rotateleft32(v25 + v57 - 30611744 + (v3 ^ (v59 | ~v58)), 10);
  v61 = v60 + __builtin_rotateleft32(v70 + v58 - 1560198380 + (v59 ^ (v60 | ~v3)), 15);
  v62 = v61 + __builtin_rotateright32(v22 + v3 + 1309151649 + (v60 ^ (v61 | ~v59)), 11);
  LODWORD(v3) = v62 + __builtin_rotateleft32(v69 + v59 - 145523070 + (v61 ^ (v62 | ~v60)), 6);
  v63 = v3 + __builtin_rotateleft32(v75 + v60 - 1120210379 + (v62 ^ (v3 | ~v61)), 10);
  v64 = _mm_cvtsi32_si128(a1->m128i_i32[0] + v3);
  v65 = v63 + __builtin_rotateleft32(v68 + v61 + 718787259 + (v3 ^ (v63 | ~v62)), 15);
  result = (v4 + v65 + __builtin_rotateright32(v72 + v62 - 343485551 + (v63 ^ (v65 | ~v3)), 11));
  v64.m128i_i64[0] = _mm_unpacklo_epi32(v64, _mm_cvtsi32_si128(result)).m128i_u64[0];
  v64.m128i_i64[1] = _mm_unpacklo_epi32(
                       _mm_cvtsi32_si128(a1->m128i_i32[2] + v65),
                       _mm_cvtsi32_si128(a1->m128i_i32[3] + v63)).m128i_u64[0];
  *a1 = v64;
  return result;
}
__int64 __fastcall sub_4B60(__m128i *a1, __m128i *a2, unsigned int a3)
{
  __m128i *v3; // r15
  __m128i *v4; // r14
  __int64 v5; // r12
  int v6; // edx
  unsigned __int32 v8; // eax
  __int32 v9; // ecx
  unsigned int v10; // edx
  __int64 result; // rax
  unsigned int v12; // edx
  _QWORD *v13; // rcx
  __int64 v14; // r13
  __m128i *v15; // rbx
  __m128i v16; // xmm0
  unsigned __int64 v17; // rbp
  unsigned __int64 v18; // rbp
  __int64 v19; // rdx
  __int64 v20; // rcx
  signed __int64 v21; // r14
  unsigned int v22; // r12d
  unsigned int v23; // r12d
  __int64 v24; // rdx
  unsigned __int64 v25; // rsi
  char *v26; // rcx
  char *v27; // r8
  unsigned int v28; // edx
  unsigned int v29; // edx
  unsigned int v30; // ecx
  __int64 v31; // rdi
  unsigned __int64 v32; // rdx
  char *v33; // rcx
  char *v34; // r14
  unsigned int v35; // ecx
  __int64 v36; // rsi
  __int64 v37; // [rsp+8h] [rbp-40h]

  v3 = (a1 + 24);
  v4 = a2;
  v5 = a3;
  v6 = 8 * a3;
  v8 = a1[1].m128i_u32[0];
  v9 = a1[1].m128i_i32[1];
  a1[1].m128i_i32[0] = v8 + v6;
  v10 = v9 - (!__CFADD__(v8, v6) - 1) + (v5 >> 29);
  result = (v8 >> 3) & 0x3F;
  a1[1].m128i_i32[1] = v10;
  if ( result == 0 )
  {
LABEL_9:
    if ( v5 > 0x3F )
    {
      v15 = v4;
      v37 = (v5 - 64) >> 6;
      do
      {
        v16 = *v15;
        v15 += 4;
        *v3 = v16;
        v3[1] = v15[-3];
        v3[2] = v15[-2];
        v3[3] = v15[-1];
        sub_4470(a1, v3);
      }
      while ( v15 != &v4[4 * ((v5 - 64) >> 6) + 4] );
      v5 &= 0x3Fu;
      result = v37 << 6;
      v4 += 4 * v37 + 4;
    }
    if ( v5 < 8 )
    {
      if ( (v5 & 4) != 0 )
      {
        a1[1].m128i_i32[2] = v4->m128i_i32[0];
        *(&v3->m128i_i32[-1] + v5) = *(&v4->m128i_i32[-1] + v5);
      }
      else if ( v5 != 0 )
      {
        a1[1].m128i_i8[8] = v4->m128i_i8[0];
        if ( (v5 & 2) != 0 )
          *(&v3->m128i_i16[-1] + v5) = *(&v4->m128i_i16[-1] + v5);
      }
    }
    else
    {
      v17 = &a1[2];
      *(v17 - 8) = v4->m128i_i64[0];
      v18 = v17 & 0xFFFFFFFFFFFFFFF8LL;
      v19 = v5;
      result = v3->m128i_i64 - v18;
      v20 = *(&v4->m128i_i64[-1] + v5);
      v21 = v4 - (v3 - v18);
      v22 = (v3 - v18 + v5) & 0xFFFFFFF8;
      *(&v3->m128i_i64[-1] + v19) = v20;
      if ( v22 >= 8 )
      {
        v23 = v22 & 0xFFFFFFF8;
        LODWORD(result) = 0;
        do
        {
          v24 = result;
          result = (result + 8);
          *(v18 + v24) = *(v21 + v24);
        }
        while ( result < v23 );
      }
    }
    return result;
  }
  v3 = (a1 + 24);
  v12 = 64 - result;
  v13 = (&a1[1].m128i_i64[1] + result);
  if ( v5 >= 64 - result )
  {
    v14 = v12;
    if ( v12 >= 8 )
    {
      *v13 = a2->m128i_i64[0];
      *(v13 + v12 - 8) = *(&a2->m128i_i64[-1] + v12);
      v25 = (v13 + 1) & 0xFFFFFFFFFFFFFFF8LL;
      v26 = v13 - v25;
      v27 = (v4 - v26);
      v28 = (v26 + v12) & 0xFFFFFFF8;
      if ( v28 >= 8 )
      {
        v29 = v28 & 0xFFFFFFF8;
        v30 = 0;
        do
        {
          v31 = v30;
          v30 += 8;
          *(v25 + v31) = *&v27[v31];
        }
        while ( v30 < v29 );
      }
    }
    else if ( (v12 & 4) != 0 )
    {
      *v13 = a2->m128i_i32[0];
      *(v13 + v12 - 4) = *(&a2->m128i_i32[-1] + v12);
    }
    else if ( v12 != 0 )
    {
      *v13 = a2->m128i_i8[0];
      if ( (v12 & 2) != 0 )
        *(v13 + v12 - 2) = *(&a2->m128i_i16[-1] + v12);
    }
    v5 = (result + v5 - 64);
    v4 = (v4 + v14);
    result = sub_4470(a1, v3);
    goto LABEL_9;
  }
  if ( v5 >= 8 )
  {
    *v13 = a2->m128i_i64[0];
    *(v13 + v5 - 8) = *(&a2->m128i_i64[-1] + v5);
    v32 = (v13 + 1) & 0xFFFFFFFFFFFFFFF8LL;
    v33 = v13 - v32;
    v34 = (a2 - v33);
    result = (v5 + v33) & 0xFFFFFFF8;
    if ( result >= 8 )
    {
      result = (v5 + v33) & 0xFFFFFFF8;
      v35 = 0;
      do
      {
        v36 = v35;
        v35 += 8;
        *(v32 + v36) = *&v34[v36];
      }
      while ( v35 < result );
    }
  }
  else if ( (v5 & 4) != 0 )
  {
    *v13 = a2->m128i_i32[0];
    result = *(&a2->m128i_u32[-1] + v5);
    *(v13 + v5 - 4) = result;
  }
  else if ( v5 != 0 )
  {
    result = a2->m128i_u8[0];
    *v13 = result;
    if ( (v5 & 2) != 0 )
    {
      result = *(&a2->m128i_u16[-1] + v5);
      *(v13 + v5 - 2) = result;
    }
  }
  return result;
}
__int64 __fastcall sub_4E30(__m128i *a1, __m128i *a2)
{
  __int32 *v4; // rsi
  unsigned int v5; // eax
  unsigned int v6; // ecx
  char *v7; // rdx
  char *v8; // rdi
  unsigned int i; // eax
  __int64 v10; // rdx
  __int64 v12; // rcx
  unsigned __int64 v13; // rdx
  unsigned int v14; // ecx
  unsigned int v15; // ecx
  unsigned int v16; // eax
  __int64 v17; // rdi
  __int32 *v18; // [rsp+8h] [rbp-20h]

  v4 = &a2[1].m128i_i32[2];
  v5 = (*(v4 - 2) >> 3) & 0x3F;
  v6 = 63 - v5;
  v7 = v4 + v5;
  *v7 = 0x80;
  v8 = v7 + 1;
  if ( 63 - v5 > 7 )
  {
    v12 = 55 - v5;
    if ( v12 >= 8 )
    {
      *(v7 + 1) = 0;
      *&v8[v12 - 8] = 0;
      v13 = (v7 + 9) & 0xFFFFFFFFFFFFFFF8LL;
      v14 = (v8 - v13 + v12) & 0xFFFFFFF8;
      if ( v14 >= 8 )
      {
        v15 = v14 & 0xFFFFFFF8;
        v16 = 0;
        do
        {
          v17 = v16;
          v16 += 8;
          *(v13 + v17) = 0;
        }
        while ( v16 < v15 );
      }
    }
    else if ( ((55 - v5) & 4) != 0 )
    {
      *(v7 + 1) = 0;
      *&v8[v12 - 4] = 0;
    }
    else if ( v12 != 0 )
    {
      v7[1] = 0;
      if ( ((55 - v5) & 2) != 0 )
        *&v8[v12 - 2] = 0;
    }
  }
  else
  {
    if ( v6 != 0 )
    {
      for ( i = 0; i < v6; ++i )
      {
        v10 = i;
        v8[v10] = 0;
      }
    }
    v18 = v4;
    sub_4470(a2, v4);
    v4 = v18;
    *(&a2[1] + 8) = 0;
    *(v18 + 6) = 0;
    *(v18 + 1) = 0;
    *(v18 + 2) = 0;
  }
  a2[5].m128i_i64[0] = a2[1].m128i_i64[0];
  sub_4470(a2, v4);
  *a1 = *a2;
  a2->m128i_i64[0] = 0;
  a2[5].m128i_i64[0] = 0;
  memset((&a2->m128i_u64[1] & 0xFFFFFFFFFFFFFFF8LL), 0, 8LL * ((a2 - ((a2 + 8) & 0xFFFFFFF8) + 88) >> 3));
  return 0;
}
unsigned __int64 __fastcall sub_4F80(__m128i *a1, __m128i *a2)
{
  __m128i v2; // xmm0
  __m128i v3; // xmm1
  __m128i *v4; // rbx
  int v5; // edi
  unsigned int v6; // ecx
  unsigned int v7; // r10d
  __m128i v8; // xmm0
  __int32 v9; // esi
  __int32 v10; // edx
  int v11; // r11d
  unsigned __int32 v12; // eax
  unsigned __int32 v13; // r14d
  int v14; // r8d
  int v15; // edx
  int v16; // edi
  int v17; // eax
  int v18; // edi
  unsigned __int32 v19; // r12d
  int v20; // esi
  int v21; // r8d
  int v22; // esi
  int v23; // ecx
  int v24; // edi
  int v25; // edx
  int v26; // r11d
  int v27; // esi
  int v28; // ecx
  unsigned __int32 v29; // r15d
  int v30; // edx
  int v31; // r11d
  int v32; // ecx
  int v33; // eax
  int v34; // r11d
  int v35; // edx
  unsigned __int32 v36; // r13d
  int v37; // r8d
  int v38; // r10d
  int v39; // eax
  int v40; // edi
  int v41; // r10d
  int v42; // r8d
  int v43; // esi
  unsigned __int32 v44; // r11d
  int v45; // r10d
  int v46; // edi
  int v47; // ecx
  int v48; // r10d
  int v49; // esi
  int v50; // edx
  unsigned __int32 v51; // r10d
  int v52; // eax
  int v53; // r9d
  int v54; // ecx
  unsigned __int32 v55; // r10d
  int v56; // eax
  int v57; // r8d
  int v58; // edx
  int v59; // r8d
  int v60; // r9d
  int v61; // eax
  int v62; // edi
  int v63; // r9d
  int v64; // ebp
  int v65; // r8d
  int v66; // edi
  unsigned __int32 v67; // r9d
  int v68; // esi
  int v69; // ebp
  int v70; // edi
  int v71; // ecx
  int v72; // ebp
  int v73; // esi
  int v74; // edx
  int v75; // ecx
  int v76; // eax
  int v77; // edx
  unsigned int v78; // r12d
  int v79; // edi
  int v80; // eax
  int v81; // ebp
  int v82; // esi
  int v83; // r12d
  int v84; // r15d
  int v85; // ecx
  int v86; // ebp
  int v87; // r14d
  int v88; // edx
  int v89; // r15d
  int v90; // r13d
  int v91; // eax
  int v92; // r14d
  int v93; // r12d
  int v94; // ebp
  int v95; // r12d
  int v96; // r13d
  int v97; // r12d
  unsigned int v98; // ebp
  int v99; // r15d
  int v100; // r14d
  int v101; // r12d
  int v102; // r14d
  int v103; // r13d
  int v104; // r14d
  int v105; // ebp
  int v106; // r13d
  unsigned int v107; // r11d
  int v108; // r14d
  unsigned int v109; // r10d
  int v110; // r13d
  int v111; // r10d
  int v112; // r15d
  unsigned int v113; // r9d
  int v114; // r11d
  int v115; // r9d
  int v116; // ebp
  int v117; // r10d
  int v118; // r8d
  int v119; // r12d
  int v120; // r9d
  int v121; // ebp
  unsigned int v122; // r12d
  unsigned int v123; // r11d
  int v124; // r12d
  unsigned int v125; // r10d
  int v126; // ebp
  unsigned int v127; // r8d
  int v128; // r10d
  unsigned int v129; // edi
  int v130; // r11d
  int v131; // edi
  int v132; // r13d
  int v133; // esi
  int v134; // r8d
  int v135; // esi
  int v136; // ecx
  int v137; // edi
  int v138; // ecx
  int v139; // r9d
  int v140; // esi
  int v141; // edx
  int v142; // r9d
  int v143; // eax
  int v144; // ecx
  int v145; // eax
  int v146; // r9d
  int v147; // r8d
  int v148; // edx
  int v149; // r8d
  int v150; // r9d
  int v151; // edi
  int v152; // eax
  int v153; // edi
  int v154; // r9d
  int v155; // esi
  int v156; // r8d
  int v157; // esi
  int v158; // edx
  int v159; // edi
  int v160; // ecx
  int v161; // eax
  int v162; // ebp
  int v163; // r9d
  int v164; // ecx
  int v165; // esi
  int v166; // eax
  int v167; // r14d
  int v168; // r9d
  int v169; // r8d
  int v170; // r15d
  int v171; // ecx
  int v172; // eax
  int v173; // ecx
  int v174; // edi
  int v175; // edx
  int v176; // r8d
  int v177; // edx
  int v178; // edi
  int v179; // esi
  int v180; // ecx
  int v181; // edi
  int v182; // r9d
  int v183; // esi
  int v184; // edx
  int v185; // esi
  int v186; // r11d
  int v187; // r9d
  int v188; // r12d
  int v189; // edi
  int v190; // eax
  int v191; // ebp
  int v192; // ecx
  int v193; // esi
  int v194; // r12d
  int v195; // r9d
  int v196; // r13d
  int v197; // eax
  int v198; // edx
  int v199; // r9d
  int v200; // r8d
  int v201; // ecx
  int v202; // r8d
  int v203; // r13d
  int v204; // edi
  int v205; // edx
  int v206; // edi
  int v207; // r9d
  int v208; // esi
  int v209; // r8d
  int v210; // esi
  int v211; // r9d
  int v212; // eax
  int v213; // edi
  int v214; // eax
  int v215; // r9d
  int v216; // ecx
  int v217; // esi
  int v218; // ecx
  int v219; // r9d
  int v220; // edx
  int v221; // eax
  int v222; // edx
  int v223; // r9d
  int v224; // ecx
  int v225; // r9d
  int v226; // r8d
  int v227; // edx
  int v228; // r8d
  int v229; // edi
  int v230; // r9d
  int v231; // edi
  int v232; // r10d
  int v233; // r8d
  int v234; // esi
  int v235; // r15d
  int v236; // ecx
  int v237; // edi
  int v238; // ecx
  int v239; // r10d
  int v240; // esi
  int v241; // edx
  int v242; // r9d
  int v243; // r10d
  int v244; // ecx
  int v245; // r8d
  int v246; // r10d
  int v247; // edx
  int v248; // r11d
  int v249; // edi
  unsigned int v250; // ebp
  int v251; // r10d
  int v252; // esi
  unsigned int v253; // r12d
  int v254; // r11d
  int v255; // esi
  int v256; // ebp
  int v257; // ecx
  unsigned int v258; // r14d
  int v259; // r10d
  unsigned int v260; // r14d
  int v261; // r12d
  int v262; // ebp
  int v263; // edx
  int v264; // r13d
  int v265; // r12d
  int v266; // r14d
  int v267; // r15d
  int v268; // r11d
  int v269; // r12d
  int v270; // r13d
  int v271; // r10d
  int v272; // r9d
  int v273; // r15d
  int v274; // ebp
  int v275; // r9d
  int v276; // r8d
  int v277; // r12d
  int v278; // r8d
  int v279; // edi
  int v280; // ebp
  int v281; // edi
  unsigned int v282; // esi
  int v283; // ecx
  int v284; // r9d
  unsigned int v285; // edi
  int v286; // eax
  int v287; // edx
  unsigned int v288; // esi
  unsigned __int32 v290; // [rsp+4h] [rbp-E4h]
  int v291; // [rsp+4h] [rbp-E4h]
  int v292; // [rsp+4h] [rbp-E4h]
  int v293; // [rsp+4h] [rbp-E4h]
  unsigned __int32 v294; // [rsp+8h] [rbp-E0h]
  int v295; // [rsp+8h] [rbp-E0h]
  int v296; // [rsp+8h] [rbp-E0h]
  int v297; // [rsp+8h] [rbp-E0h]
  int v298; // [rsp+Ch] [rbp-DCh]
  int v299; // [rsp+Ch] [rbp-DCh]
  int v300; // [rsp+Ch] [rbp-DCh]
  unsigned __int32 v301; // [rsp+10h] [rbp-D8h]
  int v302; // [rsp+10h] [rbp-D8h]
  int v303; // [rsp+10h] [rbp-D8h]
  int v304; // [rsp+10h] [rbp-D8h]
  unsigned __int32 v305; // [rsp+14h] [rbp-D4h]
  int v306; // [rsp+14h] [rbp-D4h]
  int v307; // [rsp+14h] [rbp-D4h]
  int v308; // [rsp+14h] [rbp-D4h]
  unsigned __int32 v309; // [rsp+18h] [rbp-D0h]
  int v310; // [rsp+18h] [rbp-D0h]
  int v311; // [rsp+18h] [rbp-D0h]
  int v312; // [rsp+18h] [rbp-D0h]
  unsigned __int32 v313; // [rsp+1Ch] [rbp-CCh]
  int v314; // [rsp+1Ch] [rbp-CCh]
  int v315; // [rsp+1Ch] [rbp-CCh]
  int v316; // [rsp+1Ch] [rbp-CCh]
  int v317; // [rsp+20h] [rbp-C8h]
  int v318; // [rsp+20h] [rbp-C8h]
  int v319; // [rsp+20h] [rbp-C8h]
  int v320; // [rsp+20h] [rbp-C8h]
  unsigned __int32 v321; // [rsp+24h] [rbp-C4h]
  int v322; // [rsp+24h] [rbp-C4h]
  int v323; // [rsp+24h] [rbp-C4h]
  unsigned __int32 v324; // [rsp+28h] [rbp-C0h]
  int v325; // [rsp+28h] [rbp-C0h]
  int v326; // [rsp+28h] [rbp-C0h]
  int v327; // [rsp+2Ch] [rbp-BCh]
  int v328; // [rsp+2Ch] [rbp-BCh]
  int v329; // [rsp+30h] [rbp-B8h]
  int v330; // [rsp+30h] [rbp-B8h]
  __int32 v331; // [rsp+34h] [rbp-B4h]
  __m128i *v332; // [rsp+38h] [rbp-B0h]
  unsigned int v333; // [rsp+4Ch] [rbp-9Ch] BYREF
  unsigned int v334; // [rsp+50h] [rbp-98h] BYREF
  unsigned int v335; // [rsp+54h] [rbp-94h] BYREF
  unsigned int v336; // [rsp+58h] [rbp-90h] BYREF
  int v337; // [rsp+5Ch] [rbp-8Ch] BYREF
  __m128i v338; // [rsp+60h] [rbp-88h]
  __m128i v339; // [rsp+70h] [rbp-78h]
  __m128i v340; // [rsp+80h] [rbp-68h]
  __m128i v341; // [rsp+90h] [rbp-58h]
  unsigned __int64 v342; // [rsp+A8h] [rbp-40h]

  v2 = *a2;
  v3 = *a1;
  v342 = __readfsqword(0x28u);
  v4 = a1;
  v338 = v2;
  v5 = _mm_cvtsi128_si32(v3);
  v6 = _mm_cvtsi128_si32(a2[1]);
  v339 = a2[1];
  v290 = __builtin_bswap32(_mm_cvtsi128_si32(v2));
  v7 = _mm_cvtsi128_si32(a2[2]);
  v340 = a2[2];
  v8 = a2[3];
  v9 = v4->m128i_i32[2];
  v10 = v4->m128i_i32[3];
  v341 = v8;
  v11 = __builtin_rotateright32(v4->m128i_i32[1], 2);
  v331 = v4[1].m128i_i32[0];
  v12 = v290
      + v331
      + __builtin_rotateleft32(v5, 5)
      + 1518500249
      + (_mm_cvtsi128_si32(_mm_shuffle_epi32(v3, 255)) ^ _mm_cvtsi128_si32(_mm_shuffle_epi32(v3, 85)) & (v10 ^ v9));
  v294 = __builtin_bswap32(v338.m128i_u32[1]);
  v332 = v4;
  v13 = __builtin_bswap32(v6);
  v14 = __builtin_rotateleft32(v12, 5) + v294 + v10 + (v9 ^ (v11 ^ v9) & v5) + 1518500249;
  v15 = __builtin_rotateright32(v4->m128i_i32[0], 2);
  LODWORD(v4) = __builtin_bswap32(v338.m128i_u32[2]);
  v16 = v12 & (v15 ^ v11);
  v17 = __builtin_rotateright32(v12, 2);
  v18 = __builtin_rotateleft32(v14, 5) + v9 + v4 + 1518500249 + (v11 ^ v16);
  v19 = __builtin_bswap32(v338.m128i_u32[3]);
  v20 = v14 & (v17 ^ v15);
  v21 = __builtin_rotateright32(v14, 2);
  v22 = __builtin_rotateleft32(v18, 5) + v11 + v19 + 1518500249 + (v15 ^ v20);
  v23 = v15 + v13 + 1518500249 + (v17 ^ v18 & (v21 ^ v17));
  v24 = __builtin_rotateright32(v18, 2);
  v25 = __builtin_rotateleft32(v22, 5);
  v26 = v22 & (v24 ^ v21);
  v27 = __builtin_rotateright32(v22, 2);
  v28 = v25 + v23;
  v29 = __builtin_bswap32(v339.m128i_u32[1]);
  v30 = __builtin_rotateleft32(v28, 5) + (v21 ^ v26) + v17 + v29 + 1518500249;
  v31 = v28 & (v27 ^ v24);
  v32 = __builtin_rotateright32(v28, 2);
  v321 = __builtin_bswap32(v339.m128i_u32[2]);
  v33 = __builtin_rotateleft32(v30, 5) + (v24 ^ v31) + v21 + v321 + 1518500249;
  v34 = v30 & (v32 ^ v27);
  v35 = __builtin_rotateright32(v30, 2);
  v36 = __builtin_bswap32(v339.m128i_u32[3]);
  v37 = __builtin_rotateleft32(v33, 5) + (v27 ^ v34) + v24 + v36 + 1518500249;
  v324 = __builtin_bswap32(v7);
  v38 = v33 & (v35 ^ v32);
  v39 = __builtin_rotateright32(v33, 2);
  v305 = __builtin_bswap32(v340.m128i_u32[2]);
  v40 = __builtin_rotateleft32(v37, 5) + (v32 ^ v38) + v27 + v324 + 1518500249;
  v41 = v37 & (v39 ^ v35);
  v42 = __builtin_rotateright32(v37, 2);
  v301 = __builtin_bswap32(v340.m128i_u32[1]);
  v43 = __builtin_rotateleft32(v40, 5) + (v35 ^ v41) + v32 + v301 + 1518500249;
  v44 = __builtin_bswap32(v8.m128i_u32[1]);
  v45 = v39 ^ v40 & (v42 ^ v39);
  v46 = __builtin_rotateright32(v40, 2);
  v47 = __builtin_rotateleft32(v43, 5) + v45 + v35 + v305 + 1518500249;
  v48 = v43 & (v46 ^ v42);
  v49 = __builtin_rotateright32(v43, 2);
  v309 = __builtin_bswap32(v340.m128i_u32[3]);
  v50 = __builtin_rotateleft32(v47, 5) + (v42 ^ v48) + v39 + v309 + 1518500249;
  v51 = __builtin_bswap32(_mm_cvtsi128_si32(v8));
  v52 = v42 + v51 + 1518500249;
  v53 = v47 & (v49 ^ v46);
  v54 = __builtin_rotateright32(v47, 2);
  v313 = v51;
  v55 = __builtin_bswap32(v8.m128i_u32[2]);
  v56 = __builtin_rotateleft32(v50, 5) + (v46 ^ v53) + v52;
  v57 = (v49 ^ v50 & (v54 ^ v49)) + v46 + v44 + 1518500249;
  v58 = __builtin_rotateright32(v50, 2);
  v59 = __builtin_rotateleft32(v56, 5) + v57;
  v60 = v56 & (v58 ^ v54);
  v61 = __builtin_rotateright32(v56, 2);
  v62 = (v54 ^ v60) + v49 + v55 + 1518500249;
  v63 = v59;
  v64 = v59 & (v61 ^ v58);
  v65 = __builtin_rotateright32(v59, 2);
  v66 = __builtin_rotateleft32(v63, 5) + v62;
  v67 = __builtin_bswap32(v8.m128i_u32[3]);
  v68 = __builtin_rotateleft32(v66, 5) + (v58 ^ v64) + v54 + v67 + 1518500249;
  v69 = v66 & (v65 ^ v61);
  v70 = __builtin_rotateright32(v66, 2);
  v291 = __builtin_rotateleft32(v44 ^ v324 ^ v4 ^ v290, 1);
  v71 = __builtin_rotateleft32(v68, 5) + (v61 ^ v69) + v58 + v291 + 1518500249;
  v295 = __builtin_rotateleft32(v55 ^ v301 ^ v19 ^ v294, 1);
  v72 = v65 ^ v68 & (v70 ^ v65);
  v73 = __builtin_rotateright32(v68, 2);
  v74 = __builtin_rotateleft32(v71, 5) + v72 + v61 + v295 + 1518500249;
  v317 = __builtin_rotateleft32(v67 ^ v305 ^ v13 ^ v4, 1);
  LODWORD(v4) = v70 ^ v71 & (v73 ^ v70);
  v75 = __builtin_rotateright32(v71, 2);
  v76 = __builtin_rotateleft32(v74, 5) + v4 + v65 + v317 + 1518500249;
  LODWORD(v4) = v74 & (v75 ^ v73);
  v77 = __builtin_rotateright32(v74, 2);
  v298 = __builtin_rotateleft32(v291 ^ v309 ^ v29 ^ v19, 1);
  v78 = __builtin_rotateleft32(v76, 5) + (v73 ^ v4) + v70 + v298 + 1518500249;
  v79 = __builtin_rotateleft32(v295 ^ v313 ^ v13 ^ v321, 1);
  LODWORD(v4) = v76 ^ v77 ^ v75;
  v80 = __builtin_rotateright32(v76, 2);
  v81 = __builtin_rotateleft32(v78, 5) + v4 + v73 + v79 + 1859775393;
  v82 = __builtin_rotateleft32(v317 ^ v44 ^ v36 ^ v29, 1);
  LODWORD(v4) = v78 ^ v80 ^ v77;
  v83 = __builtin_rotateright32(v78, 2);
  v84 = __builtin_rotateleft32(v81, 5) + v4 + v75 + v82 + 1859775393;
  v85 = __builtin_rotateleft32(v298 ^ v55 ^ v324 ^ v321, 1);
  LODWORD(v4) = v81 ^ v83 ^ v80;
  v86 = __builtin_rotateright32(v81, 2);
  v87 = __builtin_rotateleft32(v84, 5) + v4 + v77 + v85 + 1859775393;
  v88 = __builtin_rotateleft32(v79 ^ v67 ^ v36 ^ v301, 1);
  LODWORD(v4) = v84 ^ v86 ^ v83;
  v89 = __builtin_rotateright32(v84, 2);
  v90 = __builtin_rotateleft32(v87, 5) + v4 + v80 + v88 + 1859775393;
  v91 = __builtin_rotateleft32(v82 ^ v291 ^ v305 ^ v324, 1);
  LODWORD(v4) = v87 ^ v89 ^ v86;
  v92 = __builtin_rotateright32(v87, 2);
  LODWORD(v4) = __builtin_rotateleft32(v90, 5) + v83 + v91 + 1859775393 + v4;
  v93 = __builtin_rotateleft32(v85 ^ v295 ^ v309 ^ v301, 1);
  v94 = v86 + v93 + 1859775393;
  v302 = v93;
  v95 = v90 ^ v92 ^ v89;
  v96 = __builtin_rotateright32(v90, 2);
  v97 = __builtin_rotateleft32(v4, 5) + v94 + v95;
  v306 = __builtin_rotateleft32(v88 ^ v317 ^ v313 ^ v305, 1);
  v98 = __builtin_rotateleft32(v97, 5) + v89 + v306 + 1859775393 + (v4 ^ v96 ^ v92);
  v99 = __builtin_rotateright32(v4, 2);
  LODWORD(v4) = __builtin_rotateleft32(v91 ^ v298 ^ v44 ^ v309, 1);
  v100 = v92 + v4 + 1859775393;
  v310 = v4;
  LODWORD(v4) = v97 ^ v99 ^ v96;
  v101 = __builtin_rotateright32(v97, 2);
  LODWORD(v4) = __builtin_rotateleft32(v98, 5) + v100 + v4;
  v102 = __builtin_rotateleft32(v302 ^ v79 ^ v55 ^ v313, 1);
  v103 = v96 + v102 + 1859775393;
  v314 = v102;
  v104 = v98 ^ v101 ^ v99;
  v105 = __builtin_rotateright32(v98, 2);
  v106 = v103 + v104 + __builtin_rotateleft32(v4, 5);
  v322 = __builtin_rotateleft32(v306 ^ v82 ^ v67 ^ v44, 1);
  v107 = (v4 ^ v105 ^ v101) + v99 + v322 + 1859775393 + __builtin_rotateleft32(v106, 5);
  LODWORD(v4) = __builtin_rotateright32(v4, 2);
  v108 = __builtin_rotateleft32(v310 ^ v85 ^ v291 ^ v55, 1);
  v109 = v106 ^ v4 ^ v105;
  v110 = __builtin_rotateright32(v106, 2);
  v111 = v109 + v101 + v108 + 1859775393 + __builtin_rotateleft32(v107, 5);
  v112 = __builtin_rotateleft32(v314 ^ v88 ^ v295 ^ v67, 1);
  v113 = v107 ^ v110 ^ v4;
  v114 = __builtin_rotateright32(v107, 2);
  v115 = v113 + v105 + v112 + 1859775393 + __builtin_rotateleft32(v111, 5);
  v329 = __builtin_rotateleft32(v322 ^ v91 ^ v317 ^ v291, 1);
  v116 = (v111 ^ v114 ^ v110) + v4 + v329 + 1859775393;
  v117 = __builtin_rotateright32(v111, 2);
  LODWORD(v4) = v116 + __builtin_rotateleft32(v115, 5);
  v118 = v79 ^ v317;
  v318 = __builtin_rotateleft32(v108 ^ v302 ^ v298 ^ v295, 1);
  v119 = (v115 ^ v117 ^ v114) + v110 + v318 + 1859775393;
  v120 = __builtin_rotateright32(v115, 2);
  v121 = v119 + __builtin_rotateleft32(v4, 5);
  v325 = __builtin_rotateleft32(v112 ^ v306 ^ v118, 1);
  v122 = (v4 ^ v120 ^ v117) + v114 + v325 + 1859775393;
  LODWORD(v4) = __builtin_rotateright32(v4, 2);
  v123 = v122 + __builtin_rotateleft32(v121, 5);
  v124 = __builtin_rotateleft32(v329 ^ v310 ^ v82 ^ v298, 1);
  v125 = (v121 ^ v4 ^ v120) + v117 + v124 + 1859775393;
  v126 = __builtin_rotateright32(v121, 2);
  v127 = v125 + __builtin_rotateleft32(v123, 5);
  v128 = __builtin_rotateleft32(v318 ^ v314 ^ v85 ^ v79, 1);
  v129 = v123 ^ v126 ^ v4;
  v130 = __builtin_rotateright32(v123, 2);
  v131 = v129 + v120 + v128 + 1859775393 + __builtin_rotateleft32(v127, 5);
  v132 = __builtin_rotateleft32(v325 ^ v322 ^ v88 ^ v82, 1);
  v133 = v127 ^ v130 ^ v126;
  v134 = __builtin_rotateright32(v127, 2);
  v135 = v133 + v4 + v132 + 1859775393 + __builtin_rotateleft32(v131, 5);
  v292 = __builtin_rotateleft32(v124 ^ v108 ^ v91 ^ v85, 1);
  v136 = v131 ^ v134 ^ v130;
  v137 = __builtin_rotateright32(v131, 2);
  v138 = v136 + v126 + v292 + 1859775393 + __builtin_rotateleft32(v135, 5);
  v139 = v135 ^ v137 ^ v134;
  v140 = __builtin_rotateright32(v135, 2);
  v327 = __builtin_rotateleft32(v128 ^ v112 ^ v302 ^ v88, 1);
  v141 = __builtin_rotateleft32(v138, 5) + v139 + v130 + v327 + 1859775393;
  v296 = __builtin_rotateleft32(v132 ^ v329 ^ v306 ^ v91, 1);
  v142 = v140 | v138;
  v143 = v138;
  v144 = __builtin_rotateright32(v138, 2);
  v145 = __builtin_rotateleft32(v141, 5) + v134 + v296 - 1894007588 + (v137 & v142 | v140 & v143);
  v299 = __builtin_rotateleft32(v292 ^ v318 ^ v310 ^ v302, 1);
  v146 = v144 | v141;
  v147 = v141;
  v148 = __builtin_rotateright32(v141, 2);
  v149 = __builtin_rotateleft32(v145, 5) + v137 + v299 - 1894007588 + (v140 & v146 | v144 & v147);
  v303 = __builtin_rotateleft32(v327 ^ v325 ^ v314 ^ v306, 1);
  v150 = v148 | v145;
  v151 = v145;
  v152 = __builtin_rotateright32(v145, 2);
  v153 = __builtin_rotateleft32(v149, 5) + v140 + v303 - 1894007588 + (v144 & v150 | v148 & v151);
  v307 = __builtin_rotateleft32(v296 ^ v124 ^ v322 ^ v310, 1);
  v154 = v152 | v149;
  v155 = v149;
  v156 = __builtin_rotateright32(v149, 2);
  v157 = __builtin_rotateleft32(v153, 5) + v144 + v307 - 1894007588 + (v148 & v154 | v152 & v155);
  v311 = __builtin_rotateleft32(v299 ^ v128 ^ v108 ^ v314, 1);
  v158 = __builtin_rotateleft32(v157, 5) + (v152 & (v156 | v153) | v156 & v153) + v148 + v311 - 1894007588;
  v159 = __builtin_rotateright32(v153, 2);
  v160 = __builtin_rotateleft32(v303 ^ v132 ^ v112 ^ v322, 1);
  v161 = v152 + v160 - 1894007588;
  v315 = v160;
  v162 = v318;
  v163 = v159 | v157;
  v164 = v157;
  v165 = __builtin_rotateright32(v157, 2);
  v166 = __builtin_rotateleft32(v158, 5) + (v156 & v163 | v159 & v164) + v161;
  v167 = __builtin_rotateleft32(v307 ^ v292 ^ v329 ^ v108, 1);
  v168 = __builtin_rotateright32(v158, 2);
  v169 = __builtin_rotateleft32(v166, 5) + (v159 & (v165 | v158) | v165 & v158) + v156 + v167 - 1894007588;
  v170 = __builtin_rotateleft32(v311 ^ v327 ^ v318 ^ v112, 1);
  v171 = (v165 & (v168 | v166) | v168 & v166) + v159 + v170 - 1894007588;
  v172 = __builtin_rotateright32(v166, 2);
  v173 = __builtin_rotateleft32(v169, 5) + v171;
  v174 = v172 | v169;
  v175 = v169;
  v176 = __builtin_rotateright32(v169, 2);
  v319 = __builtin_rotateleft32(v315 ^ v296 ^ v325 ^ v329, 1);
  v177 = __builtin_rotateleft32(v173, 5) + v165 + v319 - 1894007588 + (v168 & v174 | v172 & v175);
  v178 = v176 | v173;
  v179 = v173;
  v180 = __builtin_rotateright32(v173, 2);
  v330 = __builtin_rotateleft32(v167 ^ v299 ^ v124 ^ v162, 1);
  v181 = __builtin_rotateleft32(v177, 5) + (v172 & v178 | v176 & v179) + v168 + v330 - 1894007588;
  LODWORD(v4) = __builtin_rotateleft32(v170 ^ v303 ^ v128 ^ v325, 1);
  v182 = v180 | v177;
  v183 = v177;
  v184 = __builtin_rotateright32(v177, 2);
  v185 = __builtin_rotateleft32(v181, 5) + v172 + v4 - 1894007588 + (v176 & v182 | v180 & v183);
  v186 = __builtin_rotateleft32(v319 ^ v307 ^ v132 ^ v124, 1);
  v187 = v184 | v181;
  v188 = v181;
  v189 = __builtin_rotateright32(v181, 2);
  v190 = __builtin_rotateleft32(v185, 5) + v176 + v186 - 1894007588 + (v180 & v187 | v184 & v188);
  v191 = __builtin_rotateleft32(v330 ^ v311 ^ v292 ^ v128, 1);
  v192 = __builtin_rotateleft32(v190, 5) + (v184 & (v189 | v185) | v189 & v185) + v180 + v191 - 1894007588;
  v193 = __builtin_rotateright32(v185, 2);
  v194 = __builtin_rotateleft32(v4 ^ v315 ^ v327 ^ v132, 1);
  v195 = v193 | v190;
  v196 = v190;
  v197 = __builtin_rotateright32(v190, 2);
  v198 = __builtin_rotateleft32(v192, 5) + (v189 & v195 | v193 & v196) + v184 + v194 - 1894007588;
  v323 = __builtin_rotateleft32(v186 ^ v167 ^ v296 ^ v292, 1);
  v199 = v197 | v192;
  v200 = v192;
  v201 = __builtin_rotateright32(v192, 2);
  v202 = __builtin_rotateleft32(v198, 5) + v189 + v323 - 1894007588 + (v193 & v199 | v197 & v200);
  v203 = __builtin_rotateleft32(v191 ^ v170 ^ v299 ^ v327, 1);
  v204 = v197 & (v201 | v198) | v201 & v198;
  v205 = __builtin_rotateright32(v198, 2);
  v206 = __builtin_rotateleft32(v202, 5) + v193 + v203 - 1894007588 + v204;
  v326 = __builtin_rotateleft32(v194 ^ v319 ^ v303 ^ v296, 1);
  v207 = v205 | v202;
  v208 = v202;
  v209 = __builtin_rotateright32(v202, 2);
  v210 = __builtin_rotateleft32(v206, 5) + v197 + v326 - 1894007588 + (v201 & v207 | v205 & v208);
  v328 = __builtin_rotateleft32(v323 ^ v330 ^ v307 ^ v299, 1);
  v211 = v209 | v206;
  v212 = v206;
  v213 = __builtin_rotateright32(v206, 2);
  v214 = __builtin_rotateleft32(v210, 5) + v201 + v328 - 1894007588 + (v205 & v211 | v209 & v212);
  v293 = __builtin_rotateleft32(v203 ^ v4 ^ v311 ^ v303, 1);
  v215 = v213 | v210;
  v216 = v210;
  v217 = __builtin_rotateright32(v210, 2);
  v218 = __builtin_rotateleft32(v214, 5) + v205 + v293 - 1894007588 + (v209 & v215 | v213 & v216);
  v297 = __builtin_rotateleft32(v326 ^ v186 ^ v315 ^ v307, 1);
  v219 = v217 | v214;
  v220 = v214;
  v221 = __builtin_rotateright32(v214, 2);
  v222 = __builtin_rotateleft32(v218, 5) + v209 + v297 - 1894007588 + (v213 & v219 | v217 & v220);
  v300 = __builtin_rotateleft32(v328 ^ v191 ^ v167 ^ v311, 1);
  v223 = (v218 ^ v221 ^ v217) + v213 + v300 - 899497514;
  v224 = __builtin_rotateright32(v218, 2);
  v225 = __builtin_rotateleft32(v222, 5) + v223;
  v304 = __builtin_rotateleft32(v293 ^ v194 ^ v170 ^ v315, 1);
  v226 = (v222 ^ v224 ^ v221) + v217 + v304 - 899497514;
  v227 = __builtin_rotateright32(v222, 2);
  v228 = __builtin_rotateleft32(v225, 5) + v226;
  v308 = __builtin_rotateleft32(v297 ^ v323 ^ v167 ^ v319, 1);
  v229 = (v225 ^ v227 ^ v224) + v221 + v308 - 899497514;
  v230 = __builtin_rotateright32(v225, 2);
  v231 = __builtin_rotateleft32(v228, 5) + v229;
  v232 = v228 ^ v230 ^ v227;
  v233 = __builtin_rotateright32(v228, 2);
  v312 = __builtin_rotateleft32(v300 ^ v203 ^ v170 ^ v330, 1);
  v234 = __builtin_rotateleft32(v231, 5) + v232 + v224 + v312 - 899497514;
  v235 = __builtin_rotateleft32(v304 ^ v326 ^ v4 ^ v319, 1);
  v236 = (v231 ^ v233 ^ v230) + v227 + v235 - 899497514;
  v237 = __builtin_rotateright32(v231, 2);
  v238 = __builtin_rotateleft32(v234, 5) + v236;
  v239 = v234 ^ v237 ^ v233;
  v240 = __builtin_rotateright32(v234, 2);
  v316 = __builtin_rotateleft32(v308 ^ v328 ^ v186 ^ v330, 1);
  v241 = __builtin_rotateleft32(v238, 5) + v239 + v230 + v316 - 899497514;
  v320 = v235;
  v242 = __builtin_rotateleft32(v312 ^ v293 ^ v191 ^ v4, 1);
  v243 = v238 ^ v240 ^ v237;
  v244 = __builtin_rotateright32(v238, 2);
  LODWORD(v4) = __builtin_rotateleft32(v241, 5) + v243 + v233 + v242 - 899497514;
  v245 = __builtin_rotateleft32(v235 ^ v297 ^ v194 ^ v186, 1);
  v246 = v241 ^ v244 ^ v240;
  v247 = __builtin_rotateright32(v241, 2);
  v248 = __builtin_rotateleft32(v4, 5) + v246 + v237 + v245 - 899497514;
  v249 = __builtin_rotateleft32(v316 ^ v300 ^ v191 ^ v323, 1);
  v250 = v4 ^ v247 ^ v244;
  LODWORD(v4) = __builtin_rotateright32(v4, 2);
  v251 = __builtin_rotateleft32(v248, 5) + v250 + v240 + v249 - 899497514;
  v252 = v194;
  v253 = v248 ^ v4 ^ v247;
  v254 = __builtin_rotateright32(v248, 2);
  v255 = __builtin_rotateleft32(v242 ^ v304 ^ v203 ^ v252, 1);
  v256 = __builtin_rotateleft32(v251, 5) + v253 + v244 + v255 - 899497514;
  v257 = __builtin_rotateleft32(v245 ^ v308 ^ v326 ^ v323, 1);
  v258 = (v251 ^ v254 ^ v4) + v247 + v257 - 899497514;
  v259 = __builtin_rotateright32(v251, 2);
  v260 = __builtin_rotateleft32(v256, 5) + v258;
  v261 = v256 ^ v259 ^ v254;
  v262 = __builtin_rotateright32(v256, 2);
  v263 = __builtin_rotateleft32(v249 ^ v312 ^ v203 ^ v328, 1);
  v264 = __builtin_rotateleft32(v260, 5) + v261 + v4 + v263 - 899497514;
  v265 = v260 ^ v262 ^ v259;
  v266 = __builtin_rotateright32(v260, 2);
  LODWORD(v4) = __builtin_rotateleft32(v255 ^ v235 ^ v293 ^ v326, 1);
  v267 = __builtin_rotateleft32(v264, 5) + v265 + v254 + v4 - 899497514;
  v268 = __builtin_rotateleft32(v257 ^ v316 ^ v297 ^ v328, 1);
  v269 = __builtin_rotateleft32(v267, 5) + v259 + v268 - 899497514 + (v264 ^ v266 ^ v262);
  v270 = __builtin_rotateright32(v264, 2);
  v271 = __builtin_rotateleft32(v263 ^ v242 ^ v300 ^ v293, 1);
  v272 = v267 ^ v270 ^ v266;
  v273 = __builtin_rotateright32(v267, 2);
  v274 = __builtin_rotateleft32(v269, 5) + v272 + v262 + v271 - 899497514;
  v275 = __builtin_rotateleft32(v4 ^ v245 ^ v304 ^ v297, 1);
  v276 = v269 ^ v273 ^ v270;
  v277 = __builtin_rotateright32(v269, 2);
  LODWORD(v4) = __builtin_rotateleft32(v274, 5) + v276 + v266 + v275 - 899497514;
  v278 = __builtin_rotateleft32(v268 ^ v249 ^ v308 ^ v300, 1);
  v279 = v274 ^ v277 ^ v273;
  v280 = __builtin_rotateright32(v274, 2);
  v281 = v279 + v270 + v278 - 899497514 + __builtin_rotateleft32(v4, 5);
  v337 = __builtin_rotateright32(v4, 2);
  v282 = v273
       + __builtin_rotateleft32(v271 ^ v255 ^ v312 ^ v304, 1)
       - 899497514
       + (v4 ^ v280 ^ v277)
       + __builtin_rotateleft32(v281, 5);
  v283 = v277 + __builtin_rotateleft32(v275 ^ v308 ^ v320 ^ v257, 1) - 899497514;
  v284 = v281 ^ v337 ^ v280;
  v285 = __builtin_rotateright32(v281, 2);
  v336 = v285;
  v334 = __builtin_rotateleft32(v282, 5) + v284 + v283;
  v286 = __builtin_rotateleft32(v278 ^ v263 ^ v316 ^ v312, 1);
  v287 = v282 ^ v285 ^ v337;
  v288 = __builtin_rotateright32(v282, 2);
  v335 = v288;
  v333 = __builtin_rotateleft32(v334, 5) + v287 + v280 + v286 - 899497514;
  v8.m128i_i64[0] = _mm_unpacklo_epi32(_mm_cvtsi32_si128(v333), _mm_cvtsi32_si128(v334)).m128i_u64[0];
  v332[1].m128i_i32[0] = v331 + v337;
  v8.m128i_i64[1] = _mm_unpacklo_epi32(_mm_cvtsi32_si128(v288), _mm_cvtsi32_si128(v285)).m128i_u64[0];
  *v332 = _mm_add_epi32(v8, v3);
  __explicit_bzero_chk(&v333, 4, 4);
  __explicit_bzero_chk(&v334, 4, 4);
  __explicit_bzero_chk(&v335, 4, 4);
  __explicit_bzero_chk(&v336, 4, 4);
  __explicit_bzero_chk(&v337, 4, 4);
  return v342 - __readfsqword(0x28u);
}
void *__fastcall sub_62F0(__int64 a1, char *a2, unsigned int a3)
{
  char *v3; // r14
  unsigned int v4; // ebp
  int v5; // edx
  unsigned int v6; // eax
  int v7; // ecx
  int v8; // ecx
  int v9; // eax
  _QWORD *v10; // r13
  __int64 v13; // rbx
  __int64 v14; // r12
  int v15; // eax
  unsigned __int64 v16; // rdx
  char *v17; // r13
  char *v18; // rdi
  unsigned int v19; // r13d
  unsigned int v20; // r13d
  unsigned int v21; // ecx
  __int64 v22; // rsi
  int v23; // [rsp+Ch] [rbp-3Ch]

  v3 = a2;
  v4 = a3;
  v5 = 8 * a3;
  v6 = *(a1 + 20);
  v7 = *(a1 + 24);
  *(a1 + 20) = v6 + v5;
  v8 = v7 - (!__CFADD__(v6, v5) - 1);
  v9 = (v6 >> 3) & 0x3F;
  *(a1 + 24) = v8 + (v4 >> 29);
  v10 = (a1 + (v9 + 28));
  if ( v4 + v9 > 0x3F )
  {
    LODWORD(v13) = 64 - v9;
    v14 = (64 - v9);
    if ( (64 - v9) >= 8 )
    {
      *v10 = *a2;
      *(v10 + v13 - 8) = *&a2[v13 - 8];
      v16 = (v10 + 1) & 0xFFFFFFFFFFFFFFF8LL;
      v17 = v10 - v16;
      v18 = (a2 - v17);
      v19 = (v13 + v17) & 0xFFFFFFF8;
      if ( v19 >= 8 )
      {
        v20 = v19 & 0xFFFFFFF8;
        v21 = 0;
        do
        {
          v22 = v21;
          v21 += 8;
          *(v16 + v22) = *&v18[v22];
        }
        while ( v21 < v20 );
      }
    }
    else if ( (v13 & 4) != 0 )
    {
      *v10 = *a2;
      *(v10 + v13 - 4) = *&a2[v13 - 4];
    }
    else if ( v13 != 0 )
    {
      *v10 = *a2;
      if ( (v13 & 2) != 0 )
        *(v10 + v13 - 2) = *&a2[v13 - 2];
    }
    v10 = (a1 + 28);
    v23 = v9;
    sub_4F80(a1, (a1 + 28));
    if ( 127 - v23 < v4 )
    {
      while ( 1 )
      {
        sub_4F80(a1, &v3[v14]);
        v15 = v13;
        v13 = (v13 + 64);
        if ( v15 + 127 >= v4 )
          break;
        v14 = v13;
      }
      v4 -= v13;
      v3 += v13;
    }
    else
    {
      v3 += v13;
      v4 = v23 + v4 - 64;
    }
  }
  return memcpy(v10, v3, v4);
}
unsigned __int64 __fastcall sub_6460(__int64 a1, __int64 a2)
{
  unsigned __int64 i; // rax
  __int64 k; // rax
  char j; // [rsp+Fh] [rbp-39h] BYREF
  char v8[8]; // [rsp+10h] [rbp-38h] BYREF
  unsigned __int64 v9; // [rsp+18h] [rbp-30h]

  v9 = __readfsqword(0x28u);
  for ( i = 0; i != 8; ++i )
    v8[i] = *(a2 + 4LL * (i <= 3) + 20) >> (8 * (~i & 3));
  for ( j = 0x80; ; j = 0 )
  {
    sub_62F0(a2, &j, 1u);
    if ( (*(a2 + 20) & 0x1F8) == 0x1C0 )
      break;
  }
  sub_62F0(a2, v8, 8u);
  for ( k = 0; k != 20; ++k )
    *(a1 + k) = *(a2 + 4LL * (k >> 2)) >> (8 * (~k & 3));
  *a2 = 0;
  *(a2 + 84) = 0;
  memset(((a2 + 8) & 0xFFFFFFFFFFFFFFF8LL), 0, 8LL * ((a2 - ((a2 + 8) & 0xFFFFFFF8) + 92) >> 3));
  return v9 - __readfsqword(0x28u);
}
