int __fastcall sub_8071724(int a1, int a2)
{
  const char *v3; // r3
  const char *v4; // r2
  int result; // r0

  if ( *(a1 + 8) != 0 )
  {
    result = (*a1)(a1, a2, a3: 1);
    if ( result != 0 )
    {
      --*(a1 + 8);
      return result;
    }
    v3 = *(a1 + 12);
    v4 = "io error";
  }
  else
  {
    v3 = *(a1 + 12);
    v4 = "end-of-stream";
  }
  if ( v3 == nullptr )
    v3 = v4;
  *(a1 + 12) = v3;
  return 0;
}