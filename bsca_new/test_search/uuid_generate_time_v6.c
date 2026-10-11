unsigned __int64 __fastcall sub_56A0(__int64 a1)
{
  _BYTE v3[2]; // [rsp+Eh] [rbp-1Ah] BYREF
  __int64 v4; // [rsp+10h] [rbp-18h] BYREF
  unsigned __int64 v5; // [rsp+18h] [rbp-10h]

  v5 = __readfsqword(0x28u);
  sub_33D0(a1: &v4 + 4, a2: &v4, a3: v3, a4: 0);
  *a1 = (HIDWORD(v4) >> 20)
      | (((HIDWORD(v4) >> 12)
        | (((HIDWORD(v4) >> 4)
          | (((v4 >> 28) | (((v4 >> 20) | (((v4 >> 12) | ((BYTE1(v4) | (v4 << 8)) << 8)) << 8)) << 8)) << 8)) << 8)) << 8);
  sub_3180(buf: (a1 + 8), nbytes: 8u);
  *(a1 + 6) = *(a1 + 6) & 0xF | 0x60;
  *(a1 + 8) = *(a1 + 8) & 0x3F | 0x80;
  return v5 - __readfsqword(0x28u);
}