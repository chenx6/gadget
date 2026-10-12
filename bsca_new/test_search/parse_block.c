int __fastcall sub_8061CE8(int a1, int a2)
{
  int v2; // r3
  bool v3; // nf
  int v6; // r5

  v2 = *(a1 + 60) + 1;
  v3 = *(a1 + 60) - 511 < 0;
  *(a1 + 60) = v2;
  if ( !(v3 ^ __OFSUB__(v2, 512) | (v2 == 512)) )
  {
    sub_805DE70(a1: *(a1 + 48), a2: 1, a3: "parser stack overflow", v2, a1, a2);
    return 1;
  }
  if ( a2 != 0 )
    sub_806535A(a1, a2: 27);
  v6 = sub_8061C9C(a1, a2: 7);
  if ( *(a1 + 36) != 7 )
  {
    sub_805DE70(a1: *(a1 + 48), a2: 1, a3: "parse error at line %d: [%.*s]", *(a1 + 12), 10, *(a1 + 44));
    return 1;
  }
  sub_8063734(a1);
  if ( a2 != 0 )
    sub_806535A(a1, a2: 28);
  return v6;
}