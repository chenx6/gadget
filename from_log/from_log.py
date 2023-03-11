import idaapi
import idautils
import idc
import ida_ida

func_addr = 0x4011A0  # log function address, Use 'y' to declare argument's type first
# https://reverseengineering.stackexchange.com/questions/25301/getting-function-arguments-in-ida
name_idx = 2  # function name in log call position
# https://hex-rays.com/products/ida/support/ida74_idapython_no_bc695_porting_guide.shtml
if idaapi.IDA_SDK_VERSION <= 700:
    min_ea = idc.MinEA()
    max_ea = idc.MaxEA()
else:
    min_ea = ida_ida.inf_get_min_ea()
    max_ea = ida_ida.inf_get_max_ea()


def is_addr_invalid(addr):
    """Check `addr` is valid address"""
    return (
        addr == 0
        or addr == 0xFFFFFFFF
        or addr == 0xFFFFFFFFFFFFFFFF
        or addr < min_ea
        or addr > max_ea
    )


def read_c_str(addr, limit=32):
    """Read C string from `addr`"""
    if addr == 0 or addr == 0xFFFFFFFF or addr == 0xFFFFFFFFFFFFFFFF:
        return b""
    curr_addr = addr
    ret = b""
    while True:
        data = idc.get_bytes(curr_addr, 1)
        if data == b"\x00" or data == 0:
            break
        if curr_addr > addr + limit:
            break
        ret += data
        curr_addr += 1
    return ret


def search_nearest_function(addr):
    """Search function that `addr` belongs to"""
    prev_func = 0x0
    for func in idautils.Functions():
        if func > addr:
            break
        prev_func = func
    return prev_func


refs = list(idautils.CodeRefsTo(func_addr, 0))
print("[+] ref len %d" % len(refs))
for xref_addr in refs:
    # Get function argument
    args = idaapi.get_arg_addrs(xref_addr)
    if not args or len(args) <= name_idx:
        print("[-] No args")
        continue
    # Get argument address
    arg_addr = args[name_idx]
    if is_addr_invalid(arg_addr):
        print("[-] Argument address is invalid")
        continue
    # Get argument's immendiate value
    insn = idaapi.insn_t()
    length = idaapi.decode_insn(insn, arg_addr)
    func_name_addr = insn.ops[1].value
    if is_addr_invalid(func_name_addr):
        print("[-] Function name address is invalid")
        continue
    # Get function name
    func_name = read_c_str(func_name_addr)
    func = search_nearest_function(xref_addr)
    print("[+] %x %s" % (func, func_name))
    idc.set_name(func, func_name, 0)
