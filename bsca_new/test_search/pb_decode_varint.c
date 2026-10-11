int __fastcall sub_8071950(int a1, _DWORD *a2)
{
  int v2; // r6
  int v5; // r5
  int v6; // r4
  int result; // r0
  const char *v8; // r3
  unsigned __int8 v9; // [sp+7h] [bp-1h] BYREF

  v9 = HIBYTE(a2);
  v2 = 0;
  v5 = 0;
  v6 = 0;
  while ( 1 )
  {
    result = sub_8071724(a1, a2: (int)&v9);
    if ( result == 0 )
      return 0;
    if ( v6 == 63 )
      break;
    v5 |= ((v9 & 0x7F) << (v6 - 32)) | ((unsigned __int8)(v9 & 0x7F) >> (32 - v6));
    v2 |= (v9 & 0x7F) << v6;
    v6 += 7;
    if ( (v9 & 0x80) == 0 )
      goto LABEL_11;
  }
  if ( v9 > 1u )
  {
    v8 = *(const char **)(a1 + 12);
    if ( v8 == nullptr )
      v8 = "varint overflow";
    *(_DWORD *)(a1 + 12) = v8;
    return 0;
  }
  v5 |= v9 << 31;
LABEL_11:
  *a2 = v2;
  a2[1] = v5;
  return result;
}