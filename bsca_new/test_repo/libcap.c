__int64 sub_3580()
{
  int *v0; // rax
  int v1; // r12d
  int *v2; // rbp
  char v3; // al
  int v4; // r13d
  __int64 result; // rax
  int v6; // r14d
  int v7; // ebx

  v0 = __errno_location();
  v1 = *v0;
  v2 = v0;
  while ( 1 )
  {
    v3 = byte_B038;
    byte_B038 = 1;
    if ( v3 == 0 )
      break;
    sched_yield();
  }
  v4 = dword_B034;
  if ( dword_B034 == 0 )
  {
    v6 = 64;
    cap_set_syscall(0, 0);
    do
    {
      while ( 1 )
      {
        v7 = (v4 + v6) / 2;
        if ( cap_get_bound(v7) < 0 )
          break;
        v4 = v7 + 1;
        if ( v7 + 1 > v6 )
          goto LABEL_10;
      }
      v6 = v7 - 1;
    }
    while ( v4 <= v7 - 1 );
LABEL_10:
    if ( v4 == 0 || v4 > 64 )
      v4 = 41;
    dword_B034 = v4;
  }
  result = byte_B038;
  byte_B038 = 0;
  *v2 = v1;
  return result;
}
__int64 __fastcall cap_free(__int64 a1, __int64 a2)
{
  __int64 v3; // rdx
  unsigned int v4; // eax
  char v6; // al
  void *v7; // rax
  _BYTE *v8; // rax

  if ( a1 == 0 )
    return 0;
  v3 = a1 & 7;
  if ( (a1 & 7) != 0 )
  {
LABEL_7:
    *__errno_location() = 22;
    return 0xFFFFFFFFLL;
  }
  v4 = *(a1 - 8);
  if ( v4 == 13275564 )
  {
    v8 = *(a1 + 56);
    if ( v8 != nullptr )
    {
      LOBYTE(v3) = *v8;
      *v8 = a1 & 7;
      if ( cap_free(*(a1 + 56), a2, v3) != 0 )
        return 0xFFFFFFFFLL;
    }
    *(a1 + 56) = 0;
    if ( cap_free(*(a1 + 64), a2, v3) != 0 )
      return 0xFFFFFFFFLL;
    *(a1 + 64) = 0;
  }
  else if ( v4 > 0xCA91AC )
  {
    if ( v4 != 13276624 )
      goto LABEL_7;
  }
  else if ( v4 == 13275344 )
  {
    while ( 1 )
    {
      v6 = *a1;
      *a1 = 1;
      if ( v6 == 0 )
        break;
      sched_yield();
    }
  }
  else if ( v4 != 13275563 )
  {
    goto LABEL_7;
  }
  v7 = memset((a1 - 8), 0, *(a1 - 4));
  free(v7);
  return 0;
}
__int64 __fastcall sub_3CC0(_QWORD *a1, __uid_t a2)
{
  __int64 proc; // rax
  __int64 v5; // rbx
  unsigned int v6; // r12d
  int *v7; // r13
  int v8; // r14d
  int v10; // r14d
  int *v11; // rax
  int v12; // [rsp+4h] [rbp-34h] BYREF
  unsigned __int64 v13; // [rsp+8h] [rbp-30h]

  v13 = __readfsqword(0x28u);
  v12 = 7;
  proc = cap_get_proc();
  if ( proc == 0 )
    return -1;
  v5 = proc;
  cap_set_flag(proc, 0, 1, &v12, 1);
  sub_3B00(*a1, 8, 1);
  v6 = sub_3BD0(a1, v5);
  if ( v6 != 0 )
    goto LABEL_3;
  if ( dword_B00C == 0 )
  {
    v6 = setuid(a2);
LABEL_3:
    v7 = __errno_location();
    goto LABEL_4;
  }
  v10 = (*a1)(105, a2, 0, 0);
  v6 = v10;
  v11 = __errno_location();
  v7 = v11;
  if ( v10 < 0 )
  {
    v8 = -v10;
    v6 = 0xFFFFFFFF;
    *v11 = v8;
    goto LABEL_5;
  }
LABEL_4:
  v8 = *v7;
LABEL_5:
  sub_3B00(*a1, 8, 0);
  cap_clear_flag(v5, 0);
  sub_3BD0(a1, v5);
  cap_free(v5);
  *v7 = v8;
  return v6;
}
__int64 __fastcall sub_3E00(
        __int64 (__fastcall **a1)(__int64, __int64, __int64, _QWORD),
        __gid_t a2,
        size_t a3,
        const __gid_t *a4)
{
  __int64 proc; // rax
  _BYTE *v9; // rbx
  int v10; // ebp
  int *v11; // r13
  int v12; // r14d
  int *v14; // rax
  int v15; // r14d
  int v16; // [rsp+4h] [rbp-44h] BYREF
  unsigned __int64 v17; // [rsp+8h] [rbp-40h]

  v17 = __readfsqword(0x28u);
  v16 = 6;
  proc = cap_get_proc();
  if ( proc == 0 )
    return -1;
  v9 = proc;
  cap_set_flag(proc, 0, 1, &v16, 1);
  v10 = sub_3BD0(a1, v9);
  if ( dword_B00C != 0 )
  {
    if ( v10 == 0 )
    {
      v10 = (*a1)(106, a2, 0, 0);
      if ( v10 == 0 )
        v10 = (*a1)(116, a3, a4, 0);
    }
    v14 = __errno_location();
    v11 = v14;
    if ( v10 < 0 )
    {
      v15 = v10;
      v10 = -1;
      v12 = -v15;
      *v14 = v12;
      goto LABEL_6;
    }
  }
  else
  {
    if ( v10 == 0 )
    {
      v10 = setgid(a2);
      if ( v10 == 0 )
        v10 = setgroups(a3, a4);
    }
    v11 = __errno_location();
  }
  v12 = *v11;
LABEL_6:
  cap_clear_flag(v9, 0);
  sub_3BD0(a1, v9);
  cap_free(v9);
  *v11 = v12;
  return v10;
}
__int64 __fastcall sub_4220(__int64 (__fastcall **a1)(__int64, __int64, __int64, _QWORD), _DWORD *a2)
{
  __int64 proc; // rax
  __int64 v5; // rdi
  __m128i *v6; // r12
  __int32 v7; // edx
  __int64 v8; // rcx
  int v9; // eax
  __int32 v10; // r15d
  __int64 v11; // rsi
  int v12; // eax
  __int64 v13; // rdx
  int v14; // r15d
  int v15; // r15d
  __m128i *v16; // r13
  int v17; // edx
  int v18; // edx
  __int64 v19; // rcx
  __int64 v20; // rdi
  __int64 v21; // rsi
  int v24; // eax
  int v25; // ebx
  int v27; // eax
  unsigned int v28; // ecx
  __int64 v29; // rbx
  int v30; // r12d
  _DWORD *v31; // r14
  __int64 v32; // r15
  __int64 v33; // rbp
  int v34; // eax
  int v35; // [rsp+0h] [rbp-58h]
  unsigned int v36; // [rsp+0h] [rbp-58h]
  __int64 (__fastcall **v37)(__int64, __int64, __int64, _QWORD); // [rsp+0h] [rbp-58h]
  __m128i *v38; // [rsp+10h] [rbp-48h]

  proc = cap_get_proc();
  if ( proc == 0 )
    return -1;
  v6 = proc;
  v7 = a2[1];
  v8 = a2[5];
  v9 = ~(*(proc + 16) | *(proc + 20));
  v10 = v6[2].m128i_i32[0];
  v11 = a2[6];
  v6[1].m128i_i32[1] = v7;
  v12 = v7 & v9;
  v13 = a2[2];
  v14 = v13 & ~(v6[1].m128i_i32[3] | v10);
  v6[2].m128i_i32[0] = v13;
  v15 = v12 | v14;
  if ( __PAIR64__(v11, v8) != 0 && (v24 = cap_max_bits(v5, v11, v13, v8), v25 = v24 - 1, v24 != 0) )
  {
    while ( (a2[(v25 >> 5) + 5] & (1 << v25)) == 0 || cap_get_bound(v25) == 0 )
    {
      if ( v25-- == 0 )
        goto LABEL_3;
    }
    v16 = cap_dup(v6);
    if ( v16 != nullptr )
    {
      v15 = 1;
      goto LABEL_18;
    }
  }
  else
  {
LABEL_3:
    v16 = cap_dup(v6);
    if ( v16 != nullptr )
    {
      if ( v15 != 0 )
      {
        v15 = 0;
LABEL_18:
        v17 = cap_set_flag(v16, 0, 1, &unk_B008, 1);
        if ( v17 != 0 )
          goto LABEL_8;
      }
      v17 = sub_3BD0(a1, v16);
      if ( v17 == 0 )
      {
        v18 = sub_41A0(a1);
        if ( v18 == 0 )
        {
          v27 = cap_max_bits(v20, v21, 0, v19);
          v18 = 0;
          v28 = v27 - 1;
          if ( v27 != 0 )
          {
            v29 = v28;
            v38 = v6;
            v37 = a1;
            v30 = v15;
            v31 = a2;
            v32 = v27 - 2LL - v28;
            do
            {
              while ( 1 )
              {
                v33 = (v29 >> 5);
                if ( (v31[v33 + 3] & (1 << v29)) != 0 )
                {
                  v34 = sub_3B60(v37, 47, 2, v29, 0, 0, 0);
                  if ( v34 != 0 )
                  {
LABEL_27:
                    v6 = v38;
                    v18 = v34;
                    goto LABEL_7;
                  }
                }
                if ( v30 != 0 && (v31[v33 + 5] & (1 << v29)) != 0 )
                  break;
                if ( --v29 == v32 )
                  goto LABEL_30;
              }
              v34 = sub_3B00(*v37, 24, v29);
              if ( v34 != 0 )
                goto LABEL_27;
              --v29;
            }
            while ( v29 != v32 );
LABEL_30:
            v6 = v38;
            v18 = 0;
          }
        }
LABEL_7:
        v35 = v18;
        cap_set_proc(v6);
        v17 = v35;
      }
      goto LABEL_8;
    }
  }
  v16 = nullptr;
  v17 = -1;
LABEL_8:
  v36 = v17;
  cap_free(v16);
  cap_free(v6);
  return v36;
}
__int64 __fastcall sub_44B0(__int64 (__fastcall **a1)(__int64, __int64, __int64, _QWORD), unsigned int a2)
{
  __int64 proc; // rax
  _BYTE *v5; // rbx
  int v6; // ebp
  int v7; // ebp
  int v8; // eax
  __int64 v9; // rdx
  int i; // r13d
  __int64 v11; // rdx
  unsigned int v12; // ebp

  proc = cap_get_proc();
  if ( proc == 0 )
    return -1;
  v5 = proc;
  v6 = cap_set_flag(proc, 0, 1, &unk_B008, 1);
  v7 = sub_3BD0(a1, v5) | v6;
  if ( v7 != 0 )
    goto LABEL_13;
  if ( a2 == 3 )
  {
    if ( cap_get_ambient(0) < 0 )
    {
      v7 = sub_3B00(*a1, 28, 47);
      goto LABEL_13;
    }
    goto LABEL_7;
  }
  if ( a2 > 3 )
  {
    if ( a2 == 4 )
    {
      v7 = sub_3B00(*a1, 28, 0);
      goto LABEL_13;
    }
LABEL_17:
    v7 = -1;
    *__errno_location() = 22;
    goto LABEL_13;
  }
  if ( a2 - 1 > 1 )
    goto LABEL_17;
  cap_clear_flag(v5, 2);
  if ( cap_get_ambient(0) < 0 )
  {
    v9 = 47;
LABEL_8:
    v7 = sub_3B00(*a1, 28, v9);
    if ( a2 == 1 )
    {
      for ( i = 0; cap_get_bound(i) >= 0; ++i )
      {
        v11 = i;
        sub_3B00(*a1, 24, v11);
      }
      cap_clear_flag(v5, 1);
      sub_3B60(a1, 38, 1, 0, 0, 0, 0);
    }
    goto LABEL_13;
  }
LABEL_7:
  v8 = sub_41A0(a1);
  v9 = 239;
  v7 = v8;
  if ( v8 == 0 )
    goto LABEL_8;
LABEL_13:
  cap_clear_flag(v5, 0);
  v12 = sub_3BD0(a1, v5) | v7;
  cap_free(v5);
  return v12;
}
__int64 cap_get_mode()
{
  int secbits; // ebp
  __int64 result; // rax
  int *v2; // rax
  int v3; // ebx
  int v4; // r13d
  int *v5; // r12
  int ambient; // eax
  __int64 proc; // rbp
  char *v8; // rax
  char *v9; // r12
  char v10; // bl
  int v11; // ebx
  int bound; // eax

  secbits = cap_get_secbits();
  result = 4;
  if ( secbits != 0 )
  {
    if ( (~secbits & 0x2F) != 0 )
      return 0;
    v2 = __errno_location();
    v3 = 0;
    v4 = *v2;
    v5 = v2;
    while ( 1 )
    {
      ambient = cap_get_ambient(v3);
      if ( ambient == -1 )
        break;
      if ( ambient != 0 )
        return 0;
      ++v3;
    }
    *v5 = v4;
    if ( v3 != 0 && secbits != 239 )
      return 0;
    proc = cap_get_proc();
    v8 = cap_init();
    v9 = v8;
    if ( proc == 0 || v8 == nullptr )
    {
      cap_free(v8);
      cap_free(proc);
      return 0;
    }
    v10 = cap_compare(v8, proc);
    cap_free(v9);
    cap_free(proc);
    result = 3;
    if ( (v10 & 4) == 0 )
    {
      v11 = v10 & 3;
      if ( v11 != 0 )
      {
        return 2;
      }
      else
      {
        while ( 1 )
        {
          bound = cap_get_bound(v11);
          if ( bound == -1 )
            break;
          if ( bound != 0 )
            return 2;
          ++v11;
        }
        return 1;
      }
    }
  }
  return result;
}
__int64 __fastcall cap_launch(char *a1, __int64 a2)
{
  char v4; // al
  int v5; // eax
  char v6; // r12
  int v7; // ebp
  unsigned int v8; // r15d
  int *v9; // r14
  int v10; // eax
  unsigned int (__fastcall *v12)(__int64); // rax
  int v13; // r12d
  const char *v14; // r15
  __int64 proc; // rax
  __int64 v16; // rbp
  int v17; // ebx
  int v18; // eax
  int v19; // edx
  int v20; // r15d
  int v21; // [rsp+Ch] [rbp-5Ch]
  int buf; // [rsp+14h] [rbp-54h] BYREF
  int stat_loc; // [rsp+18h] [rbp-50h] BYREF
  int v24; // [rsp+1Ch] [rbp-4Ch] BYREF
  int pipedes[2]; // [rsp+20h] [rbp-48h] BYREF
  unsigned __int64 v26; // [rsp+28h] [rbp-40h]

  v26 = __readfsqword(0x28u);
  if ( a1 == nullptr || *(a1 - 2) != 13275564 )
  {
    *__errno_location() = 22;
    return -1;
  }
  while ( 1 )
  {
    v4 = *a1;
    *a1 = 1;
    if ( v4 == 0 )
      break;
    sched_yield();
  }
  if ( *(a1 + 1) == 0 && (*(a1 + 9) == 0 || *(a1 + 10) == 0) )
  {
    *__errno_location() = 22;
    goto LABEL_19;
  }
  v5 = pipe2(pipedes, 0x80000);
  v6 = v5;
  if ( v5 != 0 )
  {
LABEL_19:
    *a1 = 0;
    return -1;
  }
  v7 = fork();
  v8 = v7;
  v9 = __errno_location();
  buf = *v9;
  if ( v7 == 0 )
  {
    close(pipedes[0]);
    prctl(15, "cap-launcher", 0, 0, 0);
    v12 = *(a1 + 1);
    v13 = pipedes[1];
    if ( v12 == nullptr || v12(a2) == 0 )
    {
      if ( *(a1 + 9) == 0 )
        exit(0);
      if ( (*(a1 + 4) == 0 || sub_3CC0(&off_B010, *(a1 + 5)) == 0)
        && (*(a1 + 6) == 0 || sub_3E00(&off_B010, *(a1 + 7), *(a1 + 8), *(a1 + 5)) == 0)
        && (*(a1 + 12) == 0 || sub_44B0(&off_B010, *(a1 + 13)) == 0)
        && (*(a1 + 7) == 0 || sub_4220(&off_B010) == 0) )
      {
        v14 = *(a1 + 8);
        if ( v14 == nullptr )
          goto LABEL_42;
        v24 = 18;
        proc = cap_get_proc();
        v16 = proc;
        if ( proc != 0 )
        {
          cap_set_flag(proc, 0, 1, &v24, 1);
          if ( sub_3BD0(&off_B010, v16) != 0 )
            goto LABEL_33;
          if ( dword_B00C != 0 )
          {
            v18 = off_B010();
            v19 = v18;
            if ( v18 < 0 )
            {
              *v9 = -v18;
              goto LABEL_33;
            }
          }
          else
          {
            v19 = chroot(v14);
          }
          if ( v19 != 0 )
          {
LABEL_33:
            v17 = *v9;
            cap_clear_flag(v16, 0);
            sub_3BD0(&off_B010, v16);
            cap_free(v16);
            *v9 = v17;
            goto LABEL_34;
          }
          v20 = chdir("/");
          v21 = *v9;
          cap_clear_flag(v16, 0);
          sub_3BD0(&off_B010, v16);
          cap_free(v16);
          *v9 = v21;
          if ( v20 != 0 )
            goto LABEL_34;
LABEL_42:
          execve(*(a1 + 9), *(a1 + 10), *(a1 + 11));
        }
      }
    }
LABEL_34:
    stat_loc = *v9;
    while ( write(v13, &stat_loc, 4u) < 0 && *v9 == 11 )
      ;
    close(v13);
    exit(1);
  }
  *a1 = v6;
  close(pipedes[1]);
  if ( v7 >= 0 )
  {
    while ( 1 )
    {
      v10 = read(pipedes[0], &buf, 4u);
      if ( v10 == 0 )
        break;
      if ( v10 >= 0 || *v9 != 11 )
      {
        v8 = 0xFFFFFFFF;
        waitpid(v7, &stat_loc, 0);
        buf = 10;
        break;
      }
    }
  }
  close(pipedes[0]);
  *v9 = buf;
  return v8;
}
__int64 __fastcall cap_copy_ext(_BYTE *a1, _BYTE *a2, __int64 a3)
{
  char v6; // bp
  _BYTE *v7; // rax
  int *v8; // rcx
  _BYTE *v9; // r12
  int v10; // edx
  int v11; // edx

  if ( a2 == nullptr || *(a2 - 2) != 13275344 || a1 == nullptr )
  {
    *__errno_location() = 22;
    return -1;
  }
  while ( 1 )
  {
    v6 = *a2;
    *a2 = 1;
    if ( v6 == 0 )
      break;
    sched_yield();
  }
  if ( a3 <= 28 )
  {
    *__errno_location() = 22;
    *a2 = 0;
    return -1;
  }
  a1[4] = 8;
  v7 = a1 + 5;
  v8 = (a2 + 12);
  v9 = a1 + 8;
  *(v9 - 2) = 1359069840;
  do
  {
    v10 = *v8;
    ++v7;
    ++v8;
    *(v7 - 1) = v10;
    v7[2] = BYTE1(v10);
    v7[5] = BYTE2(v10);
    v7[8] = HIBYTE(v10);
    v11 = v8[2];
    v7[11] = v11;
    v7[14] = BYTE1(v11);
    v7[17] = BYTE2(v11);
    v7[20] = HIBYTE(v11);
  }
  while ( v9 != v7 );
  *a2 = 0;
  return 29;
}
char *__fastcall cap_copy_int(__int64 a1)
{
  char *v2; // rax
  char *v3; // rbp
  int v4; // r8d
  char *v5; // r10
  int i; // r11d
  char *v7; // rcx
  int v8; // edx
  int v9; // eax
  int v10; // esi

  if ( a1 == 0 || *a1 != 1359069840 )
  {
    *__errno_location() = 22;
    return nullptr;
  }
  v2 = cap_init();
  v3 = v2;
  if ( v2 == nullptr )
    return nullptr;
  v4 = *(a1 + 4);
  v5 = v2 + 24;
  for ( i = 0; i != 3; ++i )
  {
    v7 = v5 - 24;
    v8 = 0;
    do
    {
      v9 = 0;
      if ( v4 != v8 )
      {
        v9 = *(a1 + 3LL * v8 + i + 5);
        if ( v4 == v8 + 1
          || (v9 |= *(a1 + 3LL * (v8 + 1) + i + 5) << 8, v4 == v8 + 2)
          || (v10 = v8 + 3, v9 |= *(a1 + 3LL * (v8 + 2) + i + 5) << 16, v4 == v8 + 3) )
        {
          v8 = v4;
        }
        else
        {
          v8 += 4;
          v9 |= *(a1 + 3LL * v10 + i + 5) << 24;
        }
      }
      *(v7 + 3) = v9;
      v7 += 12;
    }
    while ( v5 != v7 );
    v5 += 4;
  }
  return v3;
}
__int64 __fastcall cap_set_flag(_BYTE *a1, unsigned int a2, int a3, int *a4, unsigned int a5)
{
  __int64 v6; // r14
  __int64 v7; // r13
  char v10; // al
  int *v11; // rax
  int v12; // ecx
  int v13; // r9d
  __int64 v14; // rsi
  int v15; // edx
  int v16; // ecx
  int v17; // edx

  if ( a1 != nullptr && *(a1 - 2) == 13275344 && (v6 = a3, (a3 - 1) <= 0x3E) && (v7 = a2, a2 <= 2) && a5 <= 1 )
  {
    while ( 1 )
    {
      v10 = *a1;
      *a1 = 1;
      if ( v10 == 0 )
        break;
      sched_yield();
    }
    v11 = a4;
    do
    {
      v12 = *v11;
      if ( *v11 <= 0x3F )
      {
        v13 = 1 << v12;
        v14 = v7 + 3LL * (v12 >> 5);
        v15 = *&a1[4 * v14 + 12];
        v16 = v15 & ~(1 << v12);
        v17 = v13 | v15;
        if ( a5 != 1 )
          v17 = v16;
        *&a1[4 * v14 + 12] = v17;
      }
      ++v11;
    }
    while ( v11 != &a4[v6] );
    *a1 = 0;
    return 0;
  }
  else
  {
    *__errno_location() = 22;
    return 0xFFFFFFFFLL;
  }
}
__int64 __fastcall cap_compare(char *a1, __m128i *a2)
{
  __m128i *v3; // r12
  char v4; // al
  __m128i v5; // xmm7
  __m128i v6; // xmm6
  __m128i v7; // xmm1
  __m128i v8; // xmm0
  __m128i v9; // xmm4
  __m128i v10; // xmm3
  __m128i v11; // xmm2
  __m128i v12; // xmm5
  __m128i v13; // xmm0
  unsigned int v14; // ebp

  if ( a1 == nullptr || *(a1 - 2) != 13275344 || a2 == nullptr || a2[-1].m128i_i32[2] != 13275344 )
  {
    *__errno_location() = 22;
    return -1;
  }
  v3 = cap_dup(a2);
  if ( v3 == nullptr )
    return -1;
  while ( 1 )
  {
    v4 = *a1;
    *a1 = 1;
    if ( v4 == 0 )
      break;
    sched_yield();
  }
  v5 = _mm_loadl_epi64((a1 + 12));
  v6 = _mm_loadl_epi64((&v3->m128i_u128 + 12));
  v7 = _mm_loadl_epi64((a1 + 20));
  v8 = _mm_loadl_epi64((a1 + 28));
  v9 = _mm_loadl_epi64((v3 + 20));
  v10 = _mm_loadl_epi64((v3 + 28));
  v11 = _mm_unpacklo_epi32(_mm_shuffle_epi32(v5, 225), v8);
  v8.m128i_i32[0] = v7.m128i_i32[0];
  v7.m128i_i32[0] = v5.m128i_i32[0];
  v12 = _mm_unpacklo_epi32(_mm_shuffle_epi32(v6, 225), v10);
  v10.m128i_i32[0] = v9.m128i_i32[0];
  v9.m128i_i32[0] = v6.m128i_i32[0];
  v13 = _mm_or_si128(
          _mm_or_si128(
            _mm_andnot_si128(_mm_cmpeq_epi32(v8, v10), _mm_loadl_epi64(&qword_8020)),
            _mm_andnot_si128(_mm_cmpeq_epi32(v11, v12), _mm_loadl_epi64(&qword_8028))),
          _mm_andnot_si128(_mm_cmpeq_epi32(v7, v9), _mm_loadl_epi64(&qword_8018)));
  v14 = _mm_cvtsi128_si32(_mm_or_si128(v13, _mm_srli_epi64(v13, 0x20u)));
  *a1 = 0;
  cap_free(v3);
  return v14;
}
__int64 __fastcall cap_fill_flag(_BYTE *a1, unsigned int a2, __m128i *a3, unsigned int a4)
{
  __int64 v5; // r12
  __int64 v6; // r14
  __m128i *v7; // r13
  char v8; // al

  if ( a1 == nullptr
    || *(a1 - 2) != 13275344
    || a3 == nullptr
    || a3[-1].m128i_i32[2] != 13275344
    || (v5 = a2, a2 > 2)
    || (v6 = a4, a4 > 2) )
  {
    *__errno_location() = 22;
    return 0xFFFFFFFFLL;
  }
  v7 = cap_dup(a3);
  if ( v7 == nullptr )
    return 0xFFFFFFFFLL;
  while ( 1 )
  {
    v8 = *a1;
    *a1 = 1;
    if ( v8 == 0 )
      break;
    sched_yield();
  }
  *&a1[4 * v5 + 12] = v7->m128i_i32[v6 + 3];
  *&a1[4 * v5 + 24] = v7[1].m128i_i32[v6 + 2];
  *a1 = 0;
  cap_free(v7);
  return 0;
}
_BOOL8 __fastcall cap_iab_get_vector(_BYTE *a1, int a2, int a3)
{
  _BOOL8 result; // rax
  __int64 v7; // r13
  int v8; // ebp
  char v9; // al

  if ( a1 == nullptr )
    return false;
  if ( *(a1 - 2) != 13275563 || cap_max_bits() <= a3 )
    return false;
  v7 = (a3 >> 5);
  v8 = 1 << a3;
  while ( 1 )
  {
    v9 = *a1;
    *a1 = 1;
    if ( v9 == 0 )
      break;
    sched_yield();
  }
  if ( a2 == 3 )
  {
    result = (*&a1[4 * v7 + 12] & v8) != 0;
  }
  else if ( a2 == 4 )
  {
    result = (*&a1[4 * v7 + 20] & v8) != 0;
  }
  else
  {
    result = false;
    if ( a2 == 2 )
      result = (*&a1[4 * v7 + 4] & v8) != 0;
  }
  *a1 = 0;
  return result;
}
__int64 __fastcall cap_iab_set_vector(_BYTE *a1, int a2, int a3, unsigned int a4)
{
  int v8; // r14d
  unsigned int v9; // r15d
  char v10; // dl
  _BYTE *v11; // rdx
  int v12; // eax
  int v13; // ecx
  int v14; // eax
  __int64 v16; // r15
  int v17; // edx
  int v18; // eax
  int v19; // edx
  _BYTE *v20; // rdx
  int v21; // eax
  int v22; // ecx
  int v23; // eax
  int v24; // [rsp+Ch] [rbp-3Ch]

  if ( a1 != nullptr && *(a1 - 2) == 13275563 && a4 >> 1 == 0 && cap_max_bits() > a3 )
  {
    v8 = 1 << a3;
    v9 = a3 >> 5;
    v24 = ~(1 << a3);
    while ( 1 )
    {
      v10 = *a1;
      *a1 = 1;
      if ( v10 == 0 )
        break;
      sched_yield();
    }
    switch ( a2 )
    {
      case 3:
        v20 = &a1[4 * v9];
        v21 = *(v20 + 3);
        v22 = v21 & v24;
        v23 = v8 | v21;
        if ( a4 == 0 )
          v23 = v22;
        *(v20 + 1) |= v23;
        *(v20 + 3) = v23;
        goto LABEL_14;
      case 4:
        v16 = v9 + 4LL;
        v17 = *&a1[4 * v16 + 4];
        v18 = v17 & v24;
        v19 = v8 | v17;
        if ( a4 == 0 )
          v19 = v18;
        *&a1[4 * v16 + 4] = v19;
        goto LABEL_14;
      case 2:
        v11 = &a1[4 * v9];
        v12 = *(v11 + 1);
        v13 = v12 & v24;
        v14 = v8 | v12;
        if ( a4 == 0 )
          v14 = v13;
        *(v11 + 3) &= v14;
        *(v11 + 1) = v14;
LABEL_14:
        *a1 = 0;
        return 0;
      default:
        break;
    }
    *__errno_location() = 22;
    *a1 = 0;
  }
  else
  {
    *__errno_location() = 22;
  }
  return 0xFFFFFFFFLL;
}
__int64 __fastcall cap_iab_fill(_BYTE *a1, int a2, __m128i *a3, unsigned int a4)
{
  __int64 v5; // r14
  __m128i *v7; // r13
  char v8; // al
  __int8 *v9; // rdx
  _DWORD *v10; // rax
  _DWORD *v11; // rcx
  unsigned int v12; // ebp
  int v14; // esi
  int v15; // esi
  int v16; // esi

  if ( a3 == nullptr || a3[-1].m128i_i32[2] != 13275344 || a1 == nullptr || *(a1 - 2) != 13275563 || (v5 = a4, a4 > 2) )
  {
    *__errno_location() = 22;
    return -1;
  }
  v7 = cap_dup(a3);
  if ( v7 == nullptr )
    return -1;
  while ( 1 )
  {
    v8 = *a1;
    *a1 = 1;
    if ( v8 == 0 )
      break;
    sched_yield();
  }
  v9 = &v7->m128i_i8[4 * v5];
  v10 = a1 + 4;
  v11 = a1 + 12;
  switch ( a2 )
  {
    case 3:
      do
      {
        v16 = *(v9 + 3);
        *v10++ |= v16;
        v9 += 12;
        v10[1] = v16;
      }
      while ( v10 != v11 );
      goto LABEL_17;
    case 4:
      do
      {
        v15 = *(v9 + 3);
        ++v10;
        v9 += 12;
        v10[3] = ~v15;
      }
      while ( v10 != v11 );
      goto LABEL_17;
    case 2:
      do
      {
        v14 = *(v9 + 3);
        v10[2] &= v14;
        ++v10;
        v9 += 12;
        *(v10 - 1) = v14;
      }
      while ( v11 != v10 );
LABEL_17:
      v12 = 0;
      goto LABEL_14;
    default:
      break;
  }
  v12 = 0xFFFFFFFF;
  *__errno_location() = 22;
LABEL_14:
  *a1 = 0;
  cap_free(v7);
  return v12;
}
__int64 __fastcall cap_iab_compare(char *a1, char *a2)
{
  __int64 v3; // r12
  char v4; // al
  __m128i v5; // xmm0
  unsigned int v6; // ebp

  if ( a1 == nullptr || *(a1 - 2) != 13275563 || a2 == nullptr || *(a2 - 2) != 13275563 )
  {
    *__errno_location() = 22;
    return -1;
  }
  v3 = cap_iab_dup(a2);
  if ( v3 == 0 )
    return -1;
  while ( 1 )
  {
    v4 = *a1;
    *a1 = 1;
    if ( v4 == 0 )
      break;
    sched_yield();
  }
  v5 = _mm_or_si128(
         _mm_or_si128(
           _mm_andnot_si128(
             _mm_cmpeq_epi32(_mm_loadl_epi64((v3 + 4)), _mm_loadl_epi64((a1 + 4))),
             _mm_loadl_epi64(&qword_8020)),
           _mm_andnot_si128(
             _mm_cmpeq_epi32(_mm_loadl_epi64((a1 + 12)), _mm_loadl_epi64((v3 + 12))),
             _mm_loadl_epi64(&qword_8030))),
         _mm_andnot_si128(
           _mm_cmpeq_epi32(_mm_loadl_epi64((a1 + 20)), _mm_loadl_epi64((v3 + 20))),
           _mm_loadl_epi64(&qword_8038)));
  v6 = _mm_cvtsi128_si32(_mm_or_si128(v5, _mm_srli_epi64(v5, 0x20u)));
  *a1 = 0;
  cap_free(v3);
  return v6;
}
unsigned __int64 __fastcall sub_5C80(unsigned __int8 **a1)
{
  unsigned __int8 *v1; // rbx
  const unsigned __int16 **v3; // rax
  const unsigned __int16 *v4; // rdi
  __int64 v5; // r8
  unsigned __int16 v6; // ax
  __int64 v7; // rcx
  unsigned __int8 *v8; // rdx
  __int64 v9; // rsi
  int v10; // ecx
  __int64 v11; // rax
  _BYTE *v12; // rdi
  __int64 v13; // rcx
  char v14; // r8
  char v15; // r11
  unsigned __int64 result; // rax
  _QWORD v17[5]; // [rsp+0h] [rbp-28h] BYREF

  v1 = *a1;
  v17[1] = __readfsqword(0x28u);
  v17[0] = v1;
  v3 = __ctype_b_loc();
  v4 = *v3;
  v5 = *v1;
  v6 = (*v3)[v5];
  if ( (v6 & 0x800) != 0 )
  {
    result = __isoc23_strtoul(v1, v17, 0);
    if ( result <= 0x3F )
    {
      *a1 = v17[0];
      return result;
    }
    return 0xFFFFFFFFLL;
  }
  LODWORD(v7) = v5;
  v8 = v1;
  v9 = 0;
  if ( v5 == 0 )
    return 0xFFFFFFFFLL;
  while ( (v6 & 0x400) != 0 || v7 == 95 )
  {
    v7 = *++v8;
    ++v9;
    if ( v7 == 0 )
      break;
    v6 = v4[v7];
  }
  if ( (v9 - 7) > 0xF )
    return 0xFFFFFFFFLL;
  v10 = v9;
  if ( v9 != 7 )
  {
    if ( v9 != 8 )
      v10 = v9 + byte_8520[v1[8]];
    v10 += byte_8520[v1[7]];
  }
  v11 = v10 + byte_8520[v1[4]];
  if ( v11 > 0x3A )
    return 0xFFFFFFFFLL;
  v12 = *(&unk_A680 + 2 * v11);
  if ( v12 == nullptr || ((v5 ^ *v12) & 0xDF) != 0 )
    return 0xFFFFFFFFLL;
  v13 = 0;
  while ( 1 )
  {
    v14 = byte_8620[v1[v13]];
    v15 = byte_8620[v12[v13]];
    if ( v14 == 0 )
      break;
    if ( v14 != v15 )
      return 0xFFFFFFFFLL;
    if ( ++v13 == v9 )
      goto LABEL_25;
  }
  if ( v15 != 0 )
    return 0xFFFFFFFFLL;
LABEL_25:
  if ( v12[v9] != 0 )
    return 0xFFFFFFFFLL;
  *a1 = v8;
  return *(&unk_A680 + 4 * v11 + 2);
}
__int64 __fastcall sub_5E00(int *a1, const char *a2, int a3)
{
  size_t v6; // rax
  int v7; // r14d
  int v9; // r12d
  unsigned int v10; // edi
  unsigned int v11; // eax
  __int64 v12; // rdx
  unsigned int v13; // eax
  int *i; // rdx
  int v15; // ecx
  int v16; // ebp
  __int64 v17; // r8
  char *v18; // rsi
  char *v19; // rdi
  int v20; // eax
  char *v21; // rcx
  int v22; // edx
  int v23; // eax
  int v24; // eax

  v6 = strlen(a2) >> 3;
  v7 = v6;
  if ( v6 > 2 )
    return 0;
  v9 = v6;
  v10 = cap_max_bits();
  v11 = v7 - 1;
  if ( v7 != 0 )
  {
    v12 = v11;
    v13 = 32 * v11;
    for ( i = &a1[v12]; ; --i )
    {
      v15 = -1;
      if ( v10 < v13 + 32 )
      {
        v15 = 0;
        if ( v13 < v10 )
          v15 = (1LL << (v10 & 0x1F)) - 1;
      }
      *i = v15;
      v13 -= 32;
      if ( i == a1 )
        break;
    }
  }
  if ( v7 <= 0 )
    return 0xFFFFFFFFLL;
  v16 = -a3;
  v17 = 0;
  v18 = &a2[8 * v7];
  do
  {
    v19 = v18;
    v18 -= 8;
    v20 = 0;
    v21 = v18;
    while ( 1 )
    {
      v22 = 16 * v20;
      v23 = *v21;
      if ( *v21 <= 47 )
        break;
      if ( v23 <= 57 )
      {
        v20 = v22 + v23 - 48;
        goto LABEL_14;
      }
      if ( v23 > 102 )
        break;
      if ( v23 <= 96 )
      {
        if ( (v23 - 65) > 5u )
          break;
        v20 = v22 + v23 - 55;
LABEL_14:
        if ( ++v21 == v19 )
          goto LABEL_20;
      }
      else
      {
        ++v21;
        v20 = v22 + v23 - 87;
        if ( v21 == v19 )
          goto LABEL_20;
      }
    }
    v20 = 0;
LABEL_20:
    a1[v17] &= v16 ^ v20;
    v24 = v17 + 1;
    v17 = 1;
  }
  while ( v9 > v24 );
  return 0xFFFFFFFFLL;
}
char *__fastcall cap_from_text(unsigned __int8 *a1)
{
  unsigned __int8 *v1; // r12
  char *v2; // rax
  char *v3; // r15
  int v4; // eax
  const unsigned __int16 **v6; // rax
  __int64 v7; // rdi
  unsigned __int8 *v8; // rsi
  const unsigned __int16 *v9; // rdx
  const unsigned __int16 **v10; // rbx
  __int64 v11; // rax
  unsigned __int16 v12; // cx
  unsigned __int8 *v13; // r12
  __int64 v14; // rcx
  __int64 v15; // rcx
  unsigned int v16; // edi
  unsigned int j; // edx
  int v18; // ecx
  unsigned __int8 *v19; // r14
  int v20; // eax
  unsigned __int8 *v21; // rdx
  signed __int8 v22; // cl
  char v23; // bp
  int v24; // r8d
  __int64 v25; // rsi
  char *v26; // r10
  const unsigned __int16 *v27; // rdx
  const __int32_t **v28; // rax
  unsigned __int8 *v29; // rdi
  const unsigned __int16 *v30; // rdx
  const __int32_t **v31; // rbp
  unsigned __int8 *v32; // rcx
  int v33; // eax
  const char *v34; // rsi
  int v35; // eax
  __int64 v36; // rcx
  __int64 v37; // rcx
  unsigned int v38; // eax
  unsigned int i; // edx
  int v40; // ecx
  unsigned __int8 *v41; // r9
  int v42; // r11d
  int v43; // r14d
  char *v44; // rcx
  __int64 v45; // rdi
  __int64 v46; // rsi
  __int64 v47; // rax
  __int64 v48; // rdx
  int v49; // r12d
  char *v50; // rcx
  __int64 v51; // rdx
  int v52; // r12d
  char *v53; // rdx
  int v54; // ecx
  bool v55; // zf
  char *v56; // rax
  int v57; // edx
  char *v58; // rax
  int v59; // edx
  char *v60; // rax
  int v61; // edx
  __int64 v62; // rdx
  char *v63; // rax
  int v64; // ecx
  char *v65; // rax
  int v66; // edx
  __int64 v67; // rsi
  unsigned __int16 v68; // di
  __int64 v69; // rdx
  char *v70; // rax
  int v71; // ecx
  unsigned int v72; // [rsp+4h] [rbp-64h]
  int v73; // [rsp+8h] [rbp-60h]
  unsigned __int8 *v74; // [rsp+10h] [rbp-58h]
  const unsigned __int16 *v75; // [rsp+10h] [rbp-58h]
  unsigned __int8 *v76; // [rsp+18h] [rbp-50h] BYREF
  _QWORD v77[9]; // [rsp+20h] [rbp-48h]

  v76 = a1;
  v77[1] = __readfsqword(0x28u);
  if ( a1 == nullptr )
  {
LABEL_7:
    *__errno_location() = 22;
    return nullptr;
  }
  v1 = a1;
  v2 = cap_init();
  v3 = v2;
  if ( v2 != nullptr )
  {
    v4 = *(v2 + 1);
    if ( v4 != 537333798 && v4 != 537396514 )
    {
      if ( v4 == 429392688 )
      {
        v72 = 1;
        goto LABEL_11;
      }
LABEL_6:
      cap_free(v3);
      goto LABEL_7;
    }
    v72 = 2;
LABEL_11:
    v77[0] = 0;
    v6 = __ctype_b_loc();
    v9 = *v6;
    v10 = v6;
    v11 = *v1;
    v12 = v9[v11];
    if ( (v12 & 0x2000) != 0 )
    {
      v13 = v1 + 1;
      do
      {
        v14 = *v13;
        v8 = v13++;
        LOBYTE(v11) = v14;
        v12 = v9[v14];
      }
      while ( (v12 & 0x2000) != 0 );
      v76 = v8;
    }
    if ( v11 == 0 )
      return v3;
    v15 = v12 & 8;
    if ( v15 != 0 || v11 == 95 )
    {
      v75 = v9;
      v28 = __ctype_tolower_loc();
      v29 = v76;
      v30 = v75;
      v31 = v28;
      while ( 1 )
      {
        v32 = v29;
        v33 = 97;
        v34 = "all";
        do
        {
          if ( (*v31)[*v32] != v33 )
            goto LABEL_35;
          v33 = *++v34;
          ++v32;
        }
        while ( v33 != 0 );
        v37 = *v32;
        if ( (v30[v37] & 8) != 0 || v37 == 95 )
        {
LABEL_35:
          v35 = sub_5C80(&v76);
          if ( v35 == -1 )
            goto LABEL_6;
          v19 = v76;
          *(v77 + (v35 >> 5)) |= 1 << v35;
          v20 = *v19;
          v21 = v19 + 1;
          v29 = v19 + 1;
          if ( v20 != 44 )
          {
LABEL_49:
            v73 = 1;
            goto LABEL_26;
          }
        }
        else
        {
          v19 = v29 + 3;
          v38 = cap_max_bits(v29, v34, v30, v37);
          for ( i = v72 - 1; ; i = 0 )
          {
            v40 = -1;
            if ( v38 < (32 * i == 0 ? 32 : 64) )
            {
              v40 = 0;
              if ( 32 * i < v38 )
                v40 = (1LL << (v38 & 0x1F)) - 1;
            }
            *(v77 + i) = v40;
            if ( i == 0 )
              break;
          }
          v20 = *v19;
          v21 = v19 + 1;
          v29 = v19 + 1;
          if ( v20 != 44 )
            goto LABEL_49;
        }
        v30 = *v10;
        v36 = v19[1];
        v76 = v29;
        if ( (v30[v36] & 8) == 0 && v36 != 95 )
          goto LABEL_6;
      }
    }
    if ( ((v11 - 43) & 0xFD) == 0 )
      goto LABEL_6;
    v16 = cap_max_bits(v7, v8, v9, v15);
    for ( j = v72 - 1; ; j = 0 )
    {
      v18 = -1;
      if ( v16 < (32 * j == 0 ? 32 : 64) )
      {
        v18 = 0;
        if ( 32 * j < v16 )
          v18 = (1LL << (v16 & 0x1F)) - 1;
      }
      *(v77 + j) = v18;
      if ( j == 0 )
        break;
    }
    v19 = v76;
    v73 = 0;
    v20 = *v76;
    v21 = v76 + 1;
LABEL_26:
    v76 = v21;
    v1 = v21;
    if ( v20 != 61 )
    {
      if ( ((v20 - 43) & 0xFD) != 0 )
        goto LABEL_6;
      v22 = v19[1];
      if ( v22 == 0 )
        goto LABEL_6;
      v74 = v21;
      v23 = 0;
      v24 = 0;
      v25 = v22;
      v26 = &v3[12 * v72];
      v27 = *v10;
LABEL_67:
      if ( (v27[v25] & 0x2000) != 0 )
        goto LABEL_103;
LABEL_68:
      v41 = v1 + 1;
      switch ( v22 )
      {
        case 'e':
          v24 |= 1u;
          switch ( v20 )
          {
            case '+':
              goto LABEL_88;
            case '-':
              goto LABEL_80;
            case '=':
            case 'M':
            case 'P':
              v42 = v20;
              v43 = 1;
              v23 = 1;
              goto LABEL_53;
            default:
              goto LABEL_72;
          }
        case 'i':
          v24 |= 4u;
          break;
        case 'p':
          v24 |= 2u;
          break;
        default:
          goto LABEL_6;
      }
      switch ( v20 )
      {
        case '+':
          if ( (v24 & 1) != 0 )
          {
LABEL_88:
            v42 = v20;
            v23 = 1;
            LODWORD(v45) = v72 - 1;
            v46 = (v72 - 1);
            goto LABEL_89;
          }
          v42 = v20;
          v23 = 1;
          if ( (v24 & 2) == 0 )
          {
            v24 = 4;
            LODWORD(v45) = v72 - 1;
            goto LABEL_95;
          }
          LODWORD(v45) = v72 - 1;
          break;
        case '-':
          if ( (v24 & 1) != 0 )
          {
LABEL_80:
            v42 = v20;
            v23 = 1;
            LODWORD(v45) = v72 - 1;
            v46 = (v72 - 1);
            goto LABEL_81;
          }
          v42 = v20;
          v23 = 1;
          if ( (v24 & 2) == 0 )
          {
            v24 = 4;
            LODWORD(v45) = v72 - 1;
            goto LABEL_85;
          }
          LODWORD(v45) = v72 - 1;
          goto LABEL_119;
        case '=':
        case 'M':
        case 'P':
          v42 = v20;
          v23 = 1;
          v43 = v24 & 1;
          goto LABEL_53;
        default:
LABEL_72:
          v22 = v1[1];
          if ( ((v22 - 43) & 0xFD) == 0 )
          {
            v20 = v1[1];
            goto LABEL_98;
          }
          ++v1;
          if ( v22 == 0 )
            goto LABEL_124;
          v23 = 1;
          goto LABEL_66;
      }
      goto LABEL_92;
    }
    v22 = v19[1];
    if ( ((v22 - 43) & 0xFD) == 0 )
    {
      if ( v73 == 0 )
        goto LABEL_6;
      v1 = v19 + 2;
      v55 = v22 == 43;
      v22 = v19[2];
      v76 = v19 + 2;
      if ( v55 )
      {
        if ( v22 == 0 )
          goto LABEL_6;
        v74 = v19 + 2;
        v23 = 0;
        v24 = 0;
        v27 = *v10;
        v25 = v22;
        v26 = &v3[12 * v72];
        v20 = 80;
      }
      else
      {
        if ( v22 == 0 )
          goto LABEL_6;
        v74 = v19 + 2;
        v23 = 0;
        v24 = 0;
        v27 = *v10;
        v25 = v22;
        v26 = &v3[12 * v72];
        v20 = 77;
      }
      goto LABEL_67;
    }
    v74 = v21;
    v23 = 0;
    v24 = 0;
    v26 = &v3[12 * v72];
    if ( v22 != 0 )
    {
      v27 = *v10;
      v25 = v22;
      goto LABEL_67;
    }
    while ( 1 )
    {
      v41 = v1;
      v42 = 61;
      v43 = v24 & 1;
LABEL_53:
      v44 = v26;
      LODWORD(v45) = v72 - 1;
      v46 = (v72 - 1);
      v47 = v46;
      v48 = v46;
      do
      {
        v49 = *(v77 + v48--);
        *v44 &= ~v49;
        v44 -= 12;
      }
      while ( v48 != -1 );
      v50 = v26;
      v51 = v45;
      do
      {
        v52 = *(v77 + v51--);
        *(v50 + 1) &= ~v52;
        v50 -= 12;
      }
      while ( v51 != -1 );
      v53 = v26;
      do
      {
        v54 = *(v77 + v47--);
        *(v53 + 2) &= ~v54;
        v53 -= 12;
      }
      while ( v47 != -1 );
      if ( v42 == 77 )
      {
        if ( v43 != 0 )
        {
LABEL_81:
          v56 = &v3[-v46 & 0xC];
          do
          {
            v57 = *(v77 + v46--);
            *(v56 + 3) &= ~v57;
            v56 -= 12;
          }
          while ( v46 != -1 );
        }
        if ( (v24 & 2) != 0 )
        {
LABEL_119:
          v69 = v45;
          v70 = v26;
          do
          {
            v71 = *(v77 + v69--);
            *(v70 + 1) &= ~v71;
            v70 -= 12;
          }
          while ( v69 != -1 );
        }
        if ( (v24 & 4) != 0 )
        {
LABEL_85:
          v45 = v45;
          v58 = v26;
          do
          {
            v59 = *(v77 + v45--);
            *(v58 + 2) &= ~v59;
            v58 -= 12;
          }
          while ( v45 != -1 );
        }
LABEL_63:
        v20 = *v41;
        if ( ((v20 - 43) & 0xFD) != 0 )
          goto LABEL_64;
        goto LABEL_98;
      }
      if ( v43 != 0 )
      {
LABEL_89:
        v60 = &v3[-v46 & 0xC];
        do
        {
          v61 = *(v77 + v46--);
          *(v60 + 3) |= v61;
          v60 -= 12;
        }
        while ( v46 != -1 );
        if ( (v24 & 2) == 0 )
        {
LABEL_62:
          if ( (v24 & 4) == 0 )
            goto LABEL_63;
          goto LABEL_95;
        }
      }
      else if ( (v24 & 2) == 0 )
      {
        goto LABEL_62;
      }
LABEL_92:
      v62 = v45;
      v63 = v26;
      do
      {
        v64 = *(v77 + v62--);
        *(v63 + 1) |= v64;
        v63 -= 12;
      }
      while ( v62 != -1 );
      if ( (v24 & 4) == 0 )
        goto LABEL_63;
LABEL_95:
      v45 = v45;
      v65 = v26;
      do
      {
        v66 = *(v77 + v45--);
        *(v65 + 2) |= v66;
        v65 -= 12;
      }
      while ( v45 != -1 );
      v20 = *v41;
      if ( ((v20 - 43) & 0xFD) != 0 )
      {
LABEL_64:
        if ( v20 != 0 )
        {
          v27 = *v10;
          v22 = v20;
          v1 = v41;
          v20 = v42;
LABEL_66:
          v25 = v22;
          if ( (v27[v22] & 0x2000) == 0 )
            goto LABEL_67;
          if ( v23 == 0 )
          {
            v1 = v74;
            goto LABEL_11;
          }
        }
        else
        {
          v1 = v74;
          if ( v23 == 0 )
            goto LABEL_11;
          v1 = v41;
        }
LABEL_124:
        v76 = v1;
        goto LABEL_11;
      }
LABEL_98:
      if ( v73 == 0 )
        goto LABEL_6;
      v67 = v41[1];
      v27 = *v10;
      v1 = v41 + 1;
      v68 = (*v10)[v67];
      v22 = v41[1];
      if ( (v68 & 0x400) == 0 )
        goto LABEL_6;
      if ( v67 == 0 || (v68 & 0x2000) != 0 )
        goto LABEL_124;
      v24 = 0;
      v23 = 1;
      if ( (v27[v67] & 0x2000) == 0 )
        goto LABEL_68;
LABEL_103:
      if ( v20 != 61 )
        goto LABEL_6;
    }
  }
  return nullptr;
}
void *__fastcall cap_to_text(__int64 a1, _QWORD *a2, __int64 a3, __int64 a4)
{
  unsigned int v5; // ebp
  int v6; // r8d
  int v7; // eax
  int v8; // edx
  __int64 v9; // rax
  int i; // r12d
  bool v11; // cf
  char *v12; // r8
  char *v13; // rax
  char *v14; // r9
  int v15; // eax
  __int128 *v16; // r11
  __int64 v17; // r14
  int v18; // r10d
  _BYTE *v19; // r13
  int v20; // r12d
  _BYTE *v21; // rdi
  int v22; // r8d
  int v23; // eax
  _BYTE *v24; // r9
  __int64 v25; // r15
  _BYTE *v26; // r12
  _BYTE *v27; // rdi
  _BYTE *v29; // r15
  unsigned int v30; // r13d
  const char *v31; // rax
  const char *v32; // r14
  size_t v33; // rax
  const char *v34; // r8
  char v35; // al
  __int64 v36; // r8
  const char *v37; // rdx
  const char *v38; // r9
  const char *v39; // rsi
  int v40; // eax
  int v41; // eax
  _BYTE *v42; // r13
  unsigned int j; // r12d
  const char *v44; // rax
  const char *v45; // rbp
  size_t v46; // rax
  const char *v47; // rcx
  const char *v48; // r9
  const char *v49; // r8
  int v50; // eax
  __int64 v51; // [rsp+8h] [rbp-6B0h]
  int v52; // [rsp+8h] [rbp-6B0h]
  int v53; // [rsp+8h] [rbp-6B0h]
  __int128 *v54; // [rsp+8h] [rbp-6B0h]
  __int128 *v55; // [rsp+8h] [rbp-6B0h]
  unsigned int v56; // [rsp+10h] [rbp-6A8h]
  int v57; // [rsp+10h] [rbp-6A8h]
  __int128 *v58; // [rsp+10h] [rbp-6A8h]
  __int128 *v59; // [rsp+10h] [rbp-6A8h]
  __int128 *v60; // [rsp+18h] [rbp-6A0h]
  _BYTE *src; // [rsp+28h] [rbp-690h]
  __int128 v63; // [rsp+30h] [rbp-688h] BYREF
  __int128 v64; // [rsp+40h] [rbp-678h]
  _BYTE v65[1576]; // [rsp+50h] [rbp-668h] BYREF
  unsigned __int64 v66; // [rsp+678h] [rbp-40h]

  v66 = __readfsqword(0x28u);
  if ( a1 == 0 || *(a1 - 8) != 13275344 )
  {
    *__errno_location() = 22;
    return nullptr;
  }
  v63 = 0;
  v64 = 0;
  v5 = cap_max_bits(a1, a2, a3, a4);
  if ( v5 != 0 )
  {
    v6 = 0;
    do
    {
      v7 = sub_5C40(a1, v6);
      ++*(&v63 + v7);
    }
    while ( v6 != v5 );
    v8 = HIDWORD(v64);
  }
  else
  {
    v8 = 0;
  }
  v9 = 6;
  for ( i = 7; ; v8 = *(&v63 + i) )
  {
    if ( *(&v63 + v9) >= v8 )
      i = v9;
    v11 = v9-- == 0;
    if ( v11 )
      break;
  }
  v12 = "";
  v13 = "p";
  v14 = "i";
  if ( (i & 2) == 0 )
    v13 = "";
  if ( (i & 4) == 0 )
    v14 = "";
  if ( (i & 1) != 0 )
    v12 = "e";
  v15 = __sprintf_chk(v65, 1, 1572, "=%s%s%s", v12, v14, v13);
  src = v65;
  v16 = &v63;
  v17 = 7;
  v18 = i;
  v19 = &v65[v15];
  do
  {
    v20 = v17;
    if ( v17 != v18 && *(v16 + v17) != 0 )
    {
      *v19 = 32;
      v29 = v19 + 1;
      if ( v5 == 0 )
        goto LABEL_41;
      v51 = v17;
      v30 = 0;
      v57 = v18;
      v60 = v16;
      do
      {
        while ( sub_5C40(a1, v30) != v20 )
        {
          if ( ++v30 == v5 )
            goto LABEL_40;
        }
        v31 = cap_to_name(v30);
        v32 = v31;
        if ( v31 == nullptr )
          return nullptr;
        v33 = strlen(v31);
        v34 = v32;
        if ( v33 + v29 - v65 > 0x5C0 )
        {
LABEL_79:
          cap_free(v34);
          *__errno_location() = 34;
          return nullptr;
        }
        ++v30;
        v29 += __sprintf_chk(v29, 1, -1, "%s,", v32);
        cap_free(v32);
      }
      while ( v30 != v5 );
LABEL_40:
      v17 = v51;
      v18 = v57;
      v19 = v29 - 1;
      v16 = v60;
LABEL_41:
      v35 = v17 & ~v18;
      if ( (v17 & ~v18) != 0 )
      {
        v36 = 43;
        if ( *src == 61 && src[1] == 32 )
        {
          src += 2;
          v36 = 61;
        }
        if ( (v35 & 2) != 0 )
        {
          v37 = "";
          if ( (v35 & 4) != 0 )
            v37 = "i";
          v39 = "p";
LABEL_46:
          v38 = "";
          if ( (v35 & 1) != 0 )
            v38 = "e";
        }
        else
        {
          v37 = "";
          v38 = "e";
          v39 = "";
          if ( (v35 & 4) != 0 )
          {
            v39 = "";
            v37 = "i";
            goto LABEL_46;
          }
        }
        v52 = v18;
        v58 = v16;
        v40 = __sprintf_chk(v19, 1, -1, "%c%s%s%s", v36, v38, v37, v39);
        v16 = v58;
        v18 = v52;
        v19 += v40;
      }
      if ( (v18 & ~v17) != 0 )
      {
        v59 = v16;
        v53 = v18;
        v41 = __sprintf_chk(v19, 1, -1, "-%s%s%s");
        v16 = v59;
        v18 = v53;
        v19 += v41;
      }
      if ( v19 - v65 > 1472 )
      {
LABEL_52:
        *__errno_location() = 34;
        return nullptr;
      }
    }
    v11 = v17-- == 0;
  }
  while ( !v11 );
  v21 = v19;
  v63 = 0;
  v64 = 0;
  if ( v5 <= 0x3F )
  {
    v22 = v5;
    do
    {
      v23 = sub_5C40(a1, v22);
      ++*(&v63 + v23);
    }
    while ( v22 != 64 );
    v21 = v24;
  }
  v56 = v5;
  v25 = 7;
  v26 = v21;
  while ( *(v16 + v25) == 0 )
  {
LABEL_28:
    if ( --v25 == 0 )
    {
      v27 = v26;
      goto LABEL_30;
    }
  }
  *v26 = 32;
  v42 = v26 + 1;
  if ( v56 <= 0x3F )
  {
    v54 = v16;
    for ( j = v56; j != 64; ++j )
    {
      while ( sub_5C40(a1, j) != v25 )
      {
        if ( ++j == 64 )
          goto LABEL_62;
      }
      v44 = cap_to_name(j);
      v45 = v44;
      if ( v44 == nullptr )
        return nullptr;
      v46 = strlen(v44);
      v34 = v45;
      if ( v46 + v42 - v65 > 0x5C0 )
        goto LABEL_79;
      v42 += __sprintf_chk(v42, 1, -1, "%s,", v45);
      cap_free(v45);
    }
LABEL_62:
    v16 = v54;
    v26 = v42 - 1;
  }
  if ( (v25 & 2) != 0 )
  {
    v48 = "";
    if ( (v25 & 4) != 0 )
      v48 = "i";
    v47 = "p";
LABEL_66:
    v55 = v16;
    v49 = "";
    if ( (v25 & 1) != 0 )
      v49 = "e";
    v50 = __sprintf_chk(v26, 1, -1, "+%s%s%s", v49, v48, v47);
    v16 = v55;
    v26 += v50;
    if ( v26 - v65 > 1472 )
      goto LABEL_52;
    goto LABEL_28;
  }
  if ( (v25 & 4) != 0 )
  {
    v47 = "";
    v48 = "i";
    goto LABEL_66;
  }
  __sprintf_chk(v26, 1, -1, "+%s%s%s", "e", "", "");
  v27 = v26 + 2;
  if ( v26 + 2 - v65 > 1472 )
    goto LABEL_52;
LABEL_30:
  if ( a2 != nullptr )
    *a2 = v27 - src;
  return sub_3700(src);
}
void *__fastcall cap_iab_to_text(_BYTE *a1, __int64 a2, __int64 a3, __int64 a4)
{
  int v5; // eax
  __int64 v6; // rbp
  char *v7; // rax
  char v9; // dl
  unsigned __int64 v10; // r15
  int v11; // edi
  char *v12; // rcx
  char *v13; // r12
  size_t v14; // rax
  int v15; // ecx
  _DWORD *v16; // rsi
  int v17; // r11d
  int v18; // edx
  int v19; // r10d
  int v20; // r11d
  int v21; // edx
  _QWORD v22[205]; // [rsp+0h] [rbp-668h] BYREF

  v22[197] = __readfsqword(0x28u);
  v5 = cap_max_bits(a1, a2, a3, a4);
  if ( a1 == nullptr || (v6 = v5, *(a1 - 2) != 13275563) )
  {
    v7 = v22;
    goto LABEL_4;
  }
  while ( 1 )
  {
    v9 = *a1;
    *a1 = 1;
    if ( v9 == 0 )
      break;
    sched_yield();
  }
  if ( v6 > 0 )
  {
    v10 = 0;
    v11 = 1;
    v7 = v22;
    while ( 1 )
    {
      v15 = 1 << v10;
      v16 = &a1[4 * (v10 >> 5)];
      v17 = v16[1];
      v18 = v16[3];
      v19 = v16[5];
      if ( ((1 << v10) & (v19 | v18 | v17)) == 0 )
        goto LABEL_15;
      if ( v11 == 0 )
        *v7++ = 44;
      v20 = v15 & v17;
      v21 = v15 & v18;
      if ( (v15 & v19) == 0 )
        break;
      *v7 = 33;
      v12 = v7 + 1;
      if ( v21 != 0 )
        goto LABEL_11;
      if ( v20 == 0 )
      {
        v13 = v7 + 1;
        goto LABEL_12;
      }
      v7[1] = 37;
      v13 = v7 + 2;
      if ( v10 <= 0x28 )
      {
LABEL_23:
        strcpy(v13, off_AA40[v10]);
        goto LABEL_14;
      }
LABEL_13:
      __sprintf_chk(v13, 1, -1, "%u", v10);
LABEL_14:
      v14 = strlen(v13);
      v11 = 0;
      v7 = &v13[v14];
LABEL_15:
      if ( v6 == ++v10 )
        goto LABEL_27;
    }
    if ( v21 != 0 )
    {
      v12 = v7;
LABEL_11:
      *v12 = 94;
      v13 = v12 + 1;
    }
    else
    {
      if ( v20 == 0 )
        goto LABEL_15;
      v13 = v7;
    }
LABEL_12:
    if ( v10 <= 0x28 )
      goto LABEL_23;
    goto LABEL_13;
  }
  v7 = v22;
LABEL_27:
  *a1 = 0;
LABEL_4:
  *v7 = 0;
  return sub_3700(v22);
}
__int64 __fastcall cap_iab_from_text(unsigned __int8 *a1)
{
  __int64 v1; // rax
  __int64 v2; // r12
  unsigned __int8 v3; // al
  int v4; // ebx
  unsigned __int8 *v5; // rax
  unsigned __int8 *v6; // rdx
  int v7; // eax
  unsigned int v8; // edx
  int v9; // esi
  int v10; // ecx
  __int64 v11; // rax
  int v12; // ebx
  int v13; // ecx
  unsigned __int8 *v15[4]; // [rsp+8h] [rbp-20h] BYREF

  v15[0] = a1;
  v1 = cap_iab_init();
  if ( v1 == 0 )
    return 0;
  v2 = v1;
  if ( v15[0] != nullptr )
  {
    v3 = *v15[0];
    if ( *v15[0] != 0 )
    {
      v4 = 0;
      while ( 1 )
      {
        switch ( v3 )
        {
          case '%':
            v5 = v15[0];
            v4 |= 4u;
            goto LABEL_7;
          case '^':
            v5 = v15[0];
            v4 |= 0xCu;
            goto LABEL_7;
          case '!':
            v5 = v15[0];
            v4 |= 0x10u;
            goto LABEL_7;
          default:
            break;
        }
        if ( v4 == 0 )
        {
          v13 = sub_5C80(v15);
          if ( v13 == -1 )
            goto LABEL_25;
          *(v2 + 4LL * (v13 >> 5) + 4) |= 1 << v13;
          goto LABEL_20;
        }
        v7 = sub_5C80(v15);
        if ( v7 == -1 )
          goto LABEL_25;
        v8 = v7 >> 5;
        v9 = v4 & 8;
        v10 = 1 << v7;
        if ( (v4 & 4) == 0 )
          break;
        v11 = v8;
        v12 = v4 & 0x10;
        *(v2 + 4LL * v8 + 4) |= v10;
        if ( v9 != 0 )
          goto LABEL_17;
LABEL_18:
        if ( v12 != 0 )
LABEL_19:
          *(v2 + 4 * v11 + 20) |= v10;
LABEL_20:
        v5 = v15[0];
        if ( *v15[0] == 0 )
          return v2;
        if ( *v15[0] != 44 )
        {
LABEL_25:
          cap_free(v2);
          *__errno_location() = 22;
          return 0;
        }
        v4 = 0;
LABEL_7:
        v6 = v5 + 1;
        v3 = v5[1];
        v15[0] = v6;
        if ( v3 == 0 )
          return v2;
      }
      v11 = v8;
      if ( (v4 & 8) == 0 )
        goto LABEL_19;
      v12 = v4 & 0x10;
      v11 = v8;
LABEL_17:
      *(v2 + 4 * v11 + 12) |= v10;
      goto LABEL_18;
    }
  }
  return v2;
}
int *__fastcall cap_iab_get_pid(unsigned int a1)
{
  const char *v1; // rcx
  FILE *v2; // rbp
  int v3; // r13d
  int *v4; // r12
  int *v5; // rdi
  char *filename; // [rsp+8h] [rbp-C0h] BYREF
  char s[3]; // [rsp+10h] [rbp-B8h] BYREF
  int v9; // [rsp+13h] [rbp-B5h]
  char v10; // [rsp+17h] [rbp-B1h]
  char v11[128]; // [rsp+18h] [rbp-B0h] BYREF
  unsigned __int64 v12; // [rsp+98h] [rbp-30h]

  v1 = qword_B040;
  v12 = __readfsqword(0x28u);
  if ( qword_B040 == 0 )
    v1 = "/proc";
  if ( __asprintf_chk(&filename, 1, "%s/%d/status", v1, a1) <= 0 )
    return nullptr;
  v2 = fopen64(filename, "r");
  free(filename);
  if ( v2 == nullptr )
    return nullptr;
  v3 = 0;
  v4 = cap_iab_init();
  if ( v4 == nullptr )
    goto LABEL_19;
  while ( fgets(s, 123, v2) != nullptr )
  {
    if ( *s == 24899 && s[2] == 112 )
    {
      if ( v9 == 979922505 && v10 == 9 )
      {
        v3 |= sub_5E00(v4 + 1, v11, 0) & 4;
      }
      else if ( v9 == 979660354 && v10 == 9 )
      {
        v3 |= sub_5E00(v4 + 5, v11, 1) & 0x10;
      }
      else if ( v9 == 979529025 && v10 == 9 )
      {
        v3 |= sub_5E00(v4 + 3, v11, 0) & 8;
      }
    }
  }
  if ( v3 != 28 )
  {
LABEL_19:
    v5 = v4;
    v4 = nullptr;
    cap_free(v5);
  }
  fclose(v2);
  return v4;
}
__int64 __fastcall sub_7410(_DWORD *a1, __int64 a2, int a3)
{
  int v3; // ecx
  __int64 result; // rax
  unsigned int v5; // esi
  int v6; // edx
  unsigned int v7; // r9d
  int v8; // ecx
  __int64 v9; // rdx
  _DWORD *v10; // r10
  int v11; // esi
  int v12; // r11d

  v3 = *a1;
  result = a2;
  v5 = *a1 & 0xFF000000;
  switch ( v5 )
  {
    case 0x1000000u:
      v6 = a3 - 12;
      v7 = 1;
      break;
    case 0x2000000u:
      v6 = a3 - 20;
      v7 = 2;
      break;
    case 0x3000000u:
      v6 = a3 - 24;
      v7 = 2;
      *(result + 36) = a1[5];
      break;
    default:
LABEL_14:
      cap_free(result);
      return 0;
  }
  if ( v6 != 0 )
    goto LABEL_14;
  v8 = v3 & 1;
  v9 = result + 12;
  v10 = &a1[2 * v7];
  do
  {
    v11 = a1[2];
    v12 = a1[1];
    *(v9 + 4) = *(a1 + 1);
    if ( v8 != 0 )
      *v9 = v12 | v11;
    a1 += 2;
    v9 += 12;
  }
  while ( a1 != v10 );
  if ( v7 == 1 )
  {
    *(result + 24) = 0;
    *(result + 32) = 0;
  }
  return result;
}
__int64 __fastcall sub_74E0(int *a1, char *a2, _DWORD *a3)
{
  char v6; // bp
  int v7; // eax
  int v9; // edx
  int v10; // edi
  __int64 v11; // rcx
  _DWORD *v12; // rax
  __int64 v13; // rsi
  int v14; // r8d
  int v15; // r10d
  int v16; // edx

  if ( a2 == nullptr || *(a2 - 2) != 13275344 )
  {
    *__errno_location() = 22;
    return 0xFFFFFFFFLL;
  }
  while ( 1 )
  {
    v6 = *a2;
    *a2 = 1;
    if ( v6 == 0 )
      break;
    sched_yield();
  }
  v7 = *(a2 + 1);
  if ( v7 == 537333798 || v7 == 537396514 )
  {
    *a3 = 20;
    if ( *(a2 + 9) == 0 )
    {
      v16 = *(a2 + 3);
      v10 = 0x2000000;
LABEL_25:
      v9 = *(a2 + 6) | v16;
      v11 = 2;
      goto LABEL_15;
    }
LABEL_23:
    if ( *(a2 + 1) <= 0x20080521u )
      goto LABEL_9;
    *a3 = 24;
    v10 = 50331648;
    v16 = *(a2 + 3);
    a1[5] = *(a2 + 9);
    goto LABEL_25;
  }
  if ( v7 == 429392688 )
  {
    *a3 = 12;
    if ( *(a2 + 9) == 0 )
    {
      v9 = *(a2 + 3);
      if ( *(a2 + 6) != 0 )
        goto LABEL_9;
      if ( *(a2 + 8) != 0 )
        goto LABEL_9;
      v10 = 0x1000000;
      v11 = 1;
      if ( *(a2 + 7) != 0 )
        goto LABEL_9;
LABEL_15:
      v12 = a2 + 12;
      v13 = 0;
      while ( 1 )
      {
        v14 = v12[1];
        v15 = v12[2];
        *&a1[2 * v13 + 1] = *(v12 + 1);
        if ( v9 != 0 && ((v15 | v14) & ~*v12) != 0 )
          break;
        ++v13;
        v12 += 3;
        if ( v13 == v11 )
        {
          if ( v9 != 0 )
            v10 |= 1u;
          *a1 = v10;
          *a2 = 0;
          return 0;
        }
      }
      goto LABEL_9;
    }
    goto LABEL_23;
  }
LABEL_9:
  *__errno_location() = 22;
  *a2 = 0;
  return 0xFFFFFFFFLL;
}
__int64 __fastcall cap_set_file(char *file, char *a2)
{
  int v4; // eax
  int v5; // eax
  int v6; // ebx
  unsigned int v7; // ebp
  int v8; // ebx
  int v10; // [rsp+Ch] [rbp-12Ch] BYREF
  int value[8]; // [rsp+10h] [rbp-128h] BYREF
  struct stat64 buf; // [rsp+30h] [rbp-108h] BYREF
  char path[72]; // [rsp+C0h] [rbp-78h] BYREF
  unsigned __int64 v14; // [rsp+108h] [rbp-30h]

  v14 = __readfsqword(0x28u);
  v4 = open64(file, 0x20000);
  if ( v4 >= 0 )
  {
    v8 = v4;
    v7 = cap_set_fd(v4);
    close(v8);
    return v7;
  }
  v5 = open64(file, 2228224);
  v6 = v5;
  if ( v5 < 0 )
    return -1;
  if ( fstat64(v5, &buf) != 0 )
  {
    close(v6);
    return -1;
  }
  if ( (buf.st_mode & 0xF000) != 0x8000 )
  {
    close(v6);
    *__errno_location() = 22;
    return -1;
  }
  __snprintf_chk(path, 64, 1, 64, "/proc/self/fd/%d", v6);
  if ( a2 != nullptr )
  {
    v7 = 0xFFFFFFFF;
    if ( sub_74E0(value, a2, &v10) == 0 )
      v7 = setxattr(path, "security.capability", value, v10, 0);
  }
  else
  {
    v7 = removexattr(path, "security.capability");
  }
  close(v6);
  return v7;
}
void __noreturn start()
{
  FILE *v0; // rax
  FILE *v1; // r15
  unsigned __int64 v2; // r12
  unsigned __int64 v3; // r13
  char *i; // r14
  char *v5; // rax
  char *v6; // rbx
  char *v7; // rax
  int v8; // edx
  __int64 v9; // r14
  unsigned __int64 v10; // r13
  int v11; // r15d
  const char *v12; // rdx
  const char **v13; // r12
  __int64 v14; // r15
  const char *v15; // rbx
  __int64 v16; // rdx
  __int64 v17; // rcx
  __int64 v18; // rdi
  __int64 v19; // rsi
  int v20; // ebx
  int mode; // eax
  const char *v22; // rax
  unsigned int v23; // edi
  const char *v24; // r14
  int v25; // r14d
  const char *v26; // rdx
  void **ptr; // [rsp+8h] [rbp-30h]

  v0 = fopen64("/proc/self/cmdline", "rb");
  if ( v0 != nullptr )
  {
    v1 = v0;
    v2 = 0;
    v3 = 32;
    for ( i = nullptr; ; i = v6 )
    {
      v5 = realloc(i, v3 + 1);
      v6 = v5;
      if ( v5 == nullptr )
      {
        perror("unable to parse arguments");
        fclose(v1);
        if ( i != nullptr )
          free(i);
        exit(1);
      }
      v2 += fread(&v5[v2], 1u, v3 - v2, v1);
      if ( v2 < v3 )
        break;
      v3 *= 2LL;
    }
    v6[v2] = 0;
    fclose(v1);
    v7 = &v6[v2 - 2];
    v8 = 1;
    if ( v7 >= v6 )
    {
      do
        v8 += *v7-- == 0;
      while ( v7 != v6 - 1 );
    }
    ptr = calloc(v8 + 1, 8u);
    if ( ptr == nullptr )
    {
      perror("failed to allocate memory for argv");
      free(v6);
      exit(1);
    }
    v9 = 1;
    v10 = 0;
    if ( v2 != 0 )
    {
      do
      {
        v11 = v9;
        ptr[v9++ - 1] = &v6[v10];
        v10 += strlen(&v6[v10]) + 1;
      }
      while ( v10 < v2 );
      sub_3580();
      v12 = *ptr;
      if ( *ptr == nullptr )
        v12 = "This library";
      __printf_chk(
        1,
        "%s is the shared library version: libcap-2.75.\n"
        "See the License file for distribution information.\n"
        "More information on this library is available from:\n"
        "\n"
        "    https://sites.google.com/site/fullycapable/\n",
        v12);
      if ( v11 != 1 )
      {
        v13 = (ptr + 1);
        v14 = &ptr[(v11 - 2) + 2];
        while ( 1 )
        {
          v15 = *v13;
          if ( strcmp(*v13, "--usage") == 0 || strcmp(v15, "--help") == 0 )
          {
            puts("\nusage: libcap.so [--help|--usage|--summary]");
            exit(0);
          }
          if ( strcmp(v15, "--summary") != 0 )
          {
            puts("\nusage: libcap.so [--help|--usage|--summary]");
            exit(1);
          }
          v20 = cap_max_bits(v18, v19, v16, v17);
          mode = cap_get_mode();
          v22 = cap_mode_name(mode);
          __printf_chk(1, "\nCurrent mode: %s\n", v22);
          __printf_chk(1, "Number of cap values known to: this libcap=%d, running kernel=%d\n", 41, v20);
          if ( v20 > 41 )
            break;
          if ( v20 != 41 )
          {
            __printf_chk(1, "=> Newer kernels also provide support for:");
            do
            {
              v23 = v20++;
              v24 = cap_to_name(v23);
              __printf_chk(1, " %s", v24);
              cap_free(v24);
            }
            while ( v20 != 41 );
LABEL_22:
            putchar(10);
          }
          if ( ++v13 == v14 )
            goto LABEL_24;
        }
        v25 = 41;
        __printf_chk(1, "=> Consider upgrading libcap to name:");
        do
          __printf_chk(1, " %d", v25++);
        while ( v20 != v25 );
        goto LABEL_22;
      }
LABEL_24:
      free(*ptr);
      free(ptr);
LABEL_25:
      exit(0);
    }
    sub_3580();
    v26 = *ptr;
    if ( *ptr != nullptr )
    {
LABEL_31:
      __printf_chk(
        1,
        "%s is the shared library version: libcap-2.75.\n"
        "See the License file for distribution information.\n"
        "More information on this library is available from:\n"
        "\n"
        "    https://sites.google.com/site/fullycapable/\n",
        v26);
      goto LABEL_25;
    }
  }
  else
  {
    sub_3580();
  }
  v26 = "This library";
  goto LABEL_31;
}
