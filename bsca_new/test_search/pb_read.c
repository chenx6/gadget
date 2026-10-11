int __fastcall sub_80718B8(int a1, int a2, unsigned int a3)
{
  unsigned int v4; // r5
  int result; // r0
  const char *v6; // r3
  const char *v7; // r2
  unsigned int v8; // r3
  int v9; // r3
  _BYTE v10[20]; // [sp+0h] [bp-14h] BYREF

  v4 = a3;
  if ( a3 == 0 )
    return 1;
  if ( a2 != 0 || *a1 == sub_807182C )
  {
    if ( *(a1 + 8) < a3 )
    {
      v6 = *(a1 + 12);
      v7 = "end-of-stream";
LABEL_11:
      if ( v6 == nullptr )
        v6 = v7;
      *(a1 + 12) = v6;
      return 0;
    }
    if ( (*a1)(a1) == 0 )
    {
      v6 = *(a1 + 12);
      v7 = "io error";
      goto LABEL_11;
    }
    v8 = *(a1 + 8);
    if ( v8 >= v4 )
      v9 = v8 - v4;
    else
      v9 = 0;
    *(a1 + 8) = v9;
    return 1;
  }
  while ( v4 > 0x10 )
  {
    result = sub_80718B8(a1, a2: v10, a3: 16);
    if ( result == 0 )
      return result;
    v4 -= 16;
  }
  return sub_80718B8(a1, a2: v10, a3: v4);
}