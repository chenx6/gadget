unsigned __int64 __fastcall sub_8061514(int a1, int a2, _DWORD *a3)
{
  _DWORD *i; // r6
  _DWORD *j; // r6
  int v9; // r0
  _DWORD *v10; // r1
  __int64 v11; // r0
  int v12; // t1
  int v13; // r0
  __int64 v14; // r0
  __int64 v15; // d0
  int v16; // r2
  _DWORD *v17; // r1
  int v18; // r0
  __int64 v19; // [sp+10h] [bp-8h] BYREF

  if ( a2 == 0 || a3 == nullptr )
    return 0xFFF3000000000000LL;
  v19 = sub_8061080();
  sub_805E020(a1, a2: &v19);
  for ( i = a3; *i != 0; i += 4 )
    ;
  for ( j = i - 4; j >= a3; j -= 4 )
  {
    v9 = j[1];
    v10 = (a2 + v9);
    switch ( *(j + 8) )
    {
      case 1:
        goto LABEL_12;
      case 2:
        v10 = *(a2 + v9);
        if ( v10 == nullptr )
          goto LABEL_36;
LABEL_12:
        v11 = sub_8061514(a1, a2: v10, a3: j[3]);
        break;
      case 3:
        v13 = *(a2 + v9);
        goto LABEL_15;
      case 4:
        LODWORD(v11) = sub_8062A70(a1, a2: *(a2 + v9));
        break;
      case 5:
        v15 = *v10;
        goto LABEL_18;
      case 6:
        v14 = sub_8076E24(a1: *v10);
        goto LABEL_20;
      case 7:
        v16 = -1;
        goto LABEL_22;
      case 8:
        LODWORD(v11) = sub_8062AB8(a1, a2: *(a2 + v9));
        break;
      case 9:
        v17 = *(a2 + v9);
        if ( v17 != nullptr )
        {
          v16 = v17[1];
          v10 = *v17;
LABEL_26:
          LODWORD(v11) = sub_80631E0(a1, a2: v10, a3: v16, a4: 1);
        }
        else
        {
LABEL_36:
          v11 = 0xFFFF000000000000LL;
        }
        break;
      case 0xA:
        v16 = v10[1];
LABEL_22:
        v10 = *(a2 + v9);
        goto LABEL_26;
      case 0xB:
        v16 = j[3];
        goto LABEL_26;
      case 0xC:
        v13 = *(a2 + v9);
        goto LABEL_15;
      case 0xD:
        v13 = *(a2 + v9);
LABEL_15:
        LODWORD(v14) = sub_8076E00(a1: v13, a2: v10);
        goto LABEL_20;
      case 0xE:
        v18 = *(a2 + v9);
        goto LABEL_32;
      case 0xF:
        v18 = *(a2 + v9);
LABEL_32:
        LODWORD(v14) = sub_8076DE0(a1: v18, a2: v10);
LABEL_20:
        v15 = v14;
LABEL_18:
        LODWORD(v11) = sub_8062A0C(a1, a2: *&v15, a3: *(&v15 + 1));
        break;
      case 0x10:
        LODWORD(v11) = (j[3])(a1, a2: v10);
        break;
      default:
        v11 = 0xFFF3000000000000LL;
        break;
    }
    v12 = *j;
    sub_806138C(a1, a2: HIDWORD(v11), a3: v19, a4: HIDWORD(v19), a5: v12, a6: -1, a7: v11, a8: HIDWORD(v11));
  }
  sub_805E036(a1, a2: &v19);
  return v19;
}